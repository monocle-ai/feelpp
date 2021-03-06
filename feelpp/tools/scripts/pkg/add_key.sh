#!/bin/sh

pbuilder-dist $DIST login --save-after-login << EOF
apt install -y wget gnupg ca-certificates
echo "deb https://dl.bintray.com/feelpp/$FLAVOR $DIST $CHANNEL" | tee -a /etc/apt/sources.list
wget -qO - https://bintray.com/user/downloadSubjectPublicKey?username=bintray | apt-key add -
apt update
EOF
