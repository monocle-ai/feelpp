//! -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t  -*- vim:fenc=utf-8:ft=cpp:et:sw=4:ts=4:sts=4
//!
//! This file is part of the Feel++ library
//!
//! Author(s) : 
//!     Thibaut Metivet <thibaut.metivet@inria.fr>
//!
//! This library is free software; you can redistribute it and/or
//! modify it under the terms of the GNU Lesser General Public
//! License as published by the Free Software Foundation; either
//! version 2.1 of the License, or (at your option) any later version.
//!
//! This library is distributed in the hope that it will be useful,
//! but WITHOUT ANY WARRANTY; without even the implied warranty of
//! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//! Lesser General Public License for more details.
//!
//! You should have received a copy of the GNU Lesser General Public
//! License along with this library; if not, write to the Free Software
//! Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//!
//! @file syncdofs.hpp
//! @author Thibaut Metivet <thibaut.metivet@inria.fr>
//! @date 24 Dec 2019
//! @copyright 2019 Feel++ Consortium
//! @copyright 2019 INRIA
//!

#ifndef _SYNCDOFS_HPP
#define _SYNCDOFS_HPP 1

namespace Feel {

template<typename ElementType, typename RangeType, typename FunctionType>
void syncDofs( ElementType & phi, RangeType const& range, FunctionType const& func )
{
    typedef typename ElementType::value_type value_type;
    typedef typename ElementType::functionspace_type functionspace_type;
    static const uint16_type nDofPerElt = functionspace_type::fe_type::nDof;
    // Dof table
    auto const& dofTable = phi.dof();
    // If sequential return
    if( dofTable->worldCommPtr()->localSize() == 1 )
        return;
    // MPI requests
    rank_type const localPid = dofTable->worldCommPtr()->localRank();
    int nRequests = 2 * dofTable->neighborSubdomains().size();
    mpi::request * mpiRequests = new mpi::request[nRequests];
    int cntRequests = 0;
    std::map< rank_type, std::set< std::pair<size_type, value_type> > > dataToSend, dataToRecv;
    std::map< rank_type, std::set< size_type > > dataToRequest, dataRequested;
    std::map< rank_type, std::set< std::pair<size_type, value_type> > > dataToReSend, dataToReRecv;

    // Send ghost dofs to the unique active dof
    auto itElt = range.template get<1>();
    auto enElt = range.template get<2>();
    for( ; itElt != enElt; ++itElt )
    {
        auto const elt = boost::unwrap_ref( *itElt );
        size_type const eltId = elt.id();
        for( uint16_type j = 0; j < nDofPerElt; ++j )
        {
            size_type const dofId = dofTable->localToGlobalId( eltId, j );
            // If the dof is ghost, mark to send
            if( dofTable->dofGlobalProcessIsGhost( dofId ) )
            {
                size_type const dofGCId = dofTable->mapGlobalProcessToGlobalCluster( dofId );
                rank_type const procId = dofTable->procOnGlobalCluster( dofGCId );
                value_type dofVal = phi(dofId);
                dataToSend[procId].insert( std::make_pair( dofGCId, dofVal ) );
            }
            else
            { 
                // If the dof is active and shared, request it from neighbours
                auto const dofActiveIt = dofTable->activeDofSharedOnCluster().find( dofId );
                if( dofActiveIt != dofTable->activeDofSharedOnCluster().end() )
                {
                    size_type const dofGCId = dofTable->mapGlobalProcessToGlobalCluster( dofId );
                    for( rank_type const neighProcId: dofActiveIt->second )
                    {
                        dataToRequest[neighProcId].insert( dofGCId );
                    }
                }
            }
        }
    }
    // Perform requests
    cntRequests = 0;
    for( rank_type p: dofTable->neighborSubdomains() )
    {
        mpiRequests[cntRequests++] = dofTable->worldCommPtr()->localComm().isend( p, 0, dataToRequest[p] );
        mpiRequests[cntRequests++] = dofTable->worldCommPtr()->localComm().irecv( p, 0, dataRequested[p] );
    }
    mpi::wait_all( mpiRequests, mpiRequests + nRequests );
    for( auto const& dataReq: dataRequested )
    {
        rank_type const procId = dataReq.first;
        for( size_type const dofReqGCId: dataReq.second )
        {
            // Find received dof global process id
            auto resSearchDof = dofTable->searchGlobalProcessDof( dofReqGCId );
            DCHECK( boost::get<0>( resSearchDof ) ) << "[" << localPid << "]" << " dof " << dofReqGCId << " not found\n";
            size_type const dofReqId = boost::get<1>( resSearchDof );
            dataToSend[procId].insert( std::make_pair( dofReqGCId, phi(dofReqId) ) );
        }
    }
    // Perform communications
    cntRequests = 0;
    for( rank_type p: dofTable->neighborSubdomains() )
    {
        mpiRequests[cntRequests++] = dofTable->worldCommPtr()->localComm().isend( p, 0, dataToSend[p] );
        mpiRequests[cntRequests++] = dofTable->worldCommPtr()->localComm().irecv( p, 0, dataToRecv[p] );
    }
    mpi::wait_all( mpiRequests, mpiRequests + nRequests );
    // Update the current value of active dof with respect to the ghost values and the synchronisation functional
    std::map< size_type, std::unordered_set< value_type > > ghostDofValues;
    std::map< size_type, std::unordered_set< rank_type > > ghostDofProcIds;
    for( auto const& dataR: dataToRecv )
    {
        rank_type const procId = dataR.first;
        for( auto const& dataRFromProc: dataR.second )
        {
            size_type dofGCId = dataRFromProc.first;
            value_type dofVal = dataRFromProc.second;
            ghostDofValues[dofGCId].insert( dofVal );
            ghostDofProcIds[dofGCId].insert( procId );
        }
    }
    for( auto const& ghostDofVal: ghostDofValues )
    {
        size_type dofGCId = ghostDofVal.first;
        // Find received dof global process id
        auto resSearchDof = dofTable->searchGlobalProcessDof( dofGCId );
        DCHECK( boost::get<0>( resSearchDof ) ) << "[" << localPid << "]" << " dof " << dofGCId << " not found\n";
        size_type const dofId = boost::get<1>( resSearchDof );
        //size_type const dofId = dofGCId - dofTable->firstDofGlobalCluster();
        // Update current value with received ghost values
        value_type valCurrent = phi(dofId);
        //std::cout << "[" << localPid << "] " << "syncing dof " << dofId << " with "
            //<< "valCurrent = " << valCurrent << ", "
            //<< "valGhosts = ";
        //for( auto const ghostVal: ghostDofVal.second )
            //std::cout << ghostVal << "\t";
        //std::cout << std::endl;
        phi.set( dofId, func( valCurrent, ghostDofVal.second ) );
        // Prepare the updated active dof to be re-send
        for( rank_type const procId: ghostDofProcIds[dofGCId] )
        {
            dataToReSend[procId].insert( std::make_pair( dofGCId, phi(dofId) ) );
        }
    }
    
    // Re-send active dof value to ghost dofs
    cntRequests = 0;
    for( rank_type p: dofTable->neighborSubdomains() )
    {
        mpiRequests[cntRequests++] = dofTable->worldCommPtr()->localComm().isend( p, 0, dataToReSend[p] );
        mpiRequests[cntRequests++] = dofTable->worldCommPtr()->localComm().irecv( p, 0, dataToReRecv[p] );
    }
    mpi::wait_all( mpiRequests, mpiRequests + nRequests );
    // We done with mpi comms
    delete [] mpiRequests;
    // Update ghost dof values with new active dof value
    for( auto const& dataR: dataToReRecv )
    {
        rank_type procId = dataR.first;
        for( auto const& dataRFromProc: dataR.second )
        {
            size_type dofGCId = dataRFromProc.first;
            value_type dofVal = dataRFromProc.second;
            // Find received dof global process id
            auto resSearchDof = dofTable->searchGlobalProcessDof( dofGCId );
            DCHECK( boost::get<0>( resSearchDof ) ) << "[" << localPid << "] " << "dof " << dofGCId << " not found\n";
            size_type const dofId = boost::get<1>( resSearchDof );
            DCHECK( dofTable->dofGlobalProcessIsGhost( dofId ) ) << "[" << localPid << "] "<< "dof " << dofGCId << ", " << dofId << " is not a ghost\n";
            phi.set( dofId, dofVal );
        }
    }
}

}

#endif