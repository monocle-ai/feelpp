#include "exam.h"
#include <cln/sfloat.h>
#include <cln/sfloat_io.h>

static mul_test sfloat_mul_tests[] = {

{ "-0.56581s0", "-0.117477s0",
  "0.06647s0" },

{ "0.73841s0", "0.08886s0",
  "0.065615s0" },

{ "-0.75174s0", "-0.173615s0",
  "0.130512s0" },

{ "0.557236s0", "-0.105034s0",
  "-0.0585284s0" },

{ "-0.62105s0", "0.828835s0",
  "-0.51475s0" },

{ "-0.54287s0", "-0.92243s0",
  "0.50076s0" },

{ "-0.2173s0", "5.5084s9",
  "-1.19698s9" },

{ "0.467354s0", "-7.9517s9",
  "-3.71625s9" },

{ "-0.95485s0", "5.6451s9",
  "-5.3902s9" },

{ "0.0472946s0", "-6.774s9",
  "-3.20373s8" },

{ "0.196037s0", "7.3548s8",
  "1.44181s8" },

{ "-0.25535s0", "4.91907s9",
  "-1.25608s9" },

{ "0.047058s0", "6.612s-14",
  "3.11147s-15" },

{ "-0.35054s0", "3.764s-14",
  "-1.31943s-14" },

{ "0.372635s0", "1.0613s-13",
  "3.9548s-14" },

{ "0.627266s0", "-9.519s-14",
  "-5.971s-14" },

{ "-0.0293884s0", "1.1626s-13",
  "-3.41667s-15" },

{ "-0.88304s0", "-1.1160s-13",
  "9.8547s-14" },

{ "0.318016s0", "-6.86827s19",
  "-2.18422s19" },

{ "0.605064s0", "3.4281s19",
  "2.07422s19" },

{ "-0.65415s0", "-8.185s19",
  "5.3542s19" },

{ "0.87548s0", "6.72325s19",
  "5.8861s19" },

{ "0.45806s0", "-9.503s19",
  "-4.35295s19" },

{ "-0.995384s0", "1.62797s19",
  "-1.62045s19" },

{ "0.26301s0", "-1.3169s-23",
  "-3.46357s-24" },

{ "0.82762s0", "-3.411s-24",
  "-2.82304s-24" },

{ "-0.042412s0", "-3.339s-24",
  "1.41613s-25" },

{ "0.858284s0", "-7.610s-24",
  "-6.53157s-24" },

{ "0.75574s0", "1.0518s-23",
  "7.9488s-24" },

{ "0.977s0", "-5.944s-24",
  "-5.8073s-24" },

{ "1.1316s9", "0.87906s0",
  "9.9474s8" },

{ "9.7596s9", "0.58181s0",
  "5.67824s9" },

{ "5.5896s9", "-0.91708s0",
  "-5.12616s9" },

{ "-7.677s9", "-0.67695s0",
  "5.19694s9" },

{ "-4.73655s9", "0.65572s0",
  "-3.10588s9" },

{ "-3.2158s9", "-0.30076s0",
  "9.6717s8" },

{ "5.94916s9", "-1.02867s9",
  "-6.1197s18" },

{ "-3.19098s9", "8.125s9",
  "-2.59267s19" },

{ "-6.57215s9", "9.4253s9",
  "-6.1944s19" },

{ "-5.2792s9", "3.93547s9",
  "-2.0776s19" },

{ "2.502s9", "4.1275s9",
  "1.0327s19" },

{ "-8.9462s9", "-4.72174s9",
  "4.22415s19" },

{ "-8.9588s9", "-1.4190s-14",
  "1.27126s-4" },

{ "-3.56218s9", "-9.982s-14",
  "3.5558s-4" },

{ "-3.4449s9", "4.582s-15",
  "-1.57845s-5" },

{ "-3.7047s9", "1.2985s-14",
  "-4.8105s-5" },

{ "-8.9172s8", "-7.294s-14",
  "6.5043s-5" },

{ "1.64864s9", "1.8344s-13",
  "3.02427s-4" },

{ "-9.935s8", "-7.9116s19",
  "7.8602s28" },

{ "-7.0441s9", "-6.3448s19",
  "4.4693s29" },

{ "7.72866s9", "1.44264s19",
  "1.11497s29" },

{ "3.7816s9", "-3.16285s19",
  "-1.19606s29" },

{ "-1.06926s9", "6.67816s19",
  "-7.1407s28" },

{ "4.04482s9", "-3.52235s19",
  "-1.42473s29" },

{ "-8.77s8", "-3.499s-24",
  "3.06864s-15" },

{ "-9.5508s9", "1.0006s-23",
  "-9.5566s-14" },

{ "-2.98736s9", "-7.070s-24",
  "2.11207s-14" },

{ "9.9779s9", "1.2683s-23",
  "1.26548s-13" },

{ "7.4813s9", "-1.3730s-23",
  "-1.02719s-13" },

{ "8.5804s9", "6.999s-24",
  "6.0054s-14" },

{ "4.637s-14", "0.895805s0",
  "4.15384s-14" },

{ "1.0125s-13", "-0.322685s0",
  "-3.26718s-14" },

{ "2.310s-16", "0.0601425s0",
  "1.38928s-17" },

{ "1.0579s-13", "-0.27089s0",
  "-2.86576s-14" },

{ "9.540s-14", "-0.21251s0",
  "-2.02735s-14" },

{ "-4.463s-14", "-0.96336s0",
  "4.2995s-14" },

{ "3.270s-14", "-5.9141s9",
  "-1.93391s-4" },

{ "-6.515s-14", "1.01791s9",
  "-6.6318s-5" },

{ "3.695s-14", "8.7417s9",
  "3.23005s-4" },

{ "-1.0900s-13", "-6.75794s9",
  "7.3662s-4" },

{ "4.551s-14", "-7.1112s9",
  "-3.2363s-4" },

{ "5.456s-15", "-5.44014s9",
  "-2.96813s-5" },

{ "-3.377s-14", "3.358s-15",
  "-1.13399s-28" },

{ "3.862s-14", "7.278s-14",
  "2.81079s-27" },

{ "9.449s-14", "3.170s-14",
  "2.99533s-27" },

{ "7.051s-14", "-4.234s-14",
  "-2.98537s-27" },

{ "-8.955s-14", "9.895s-14",
  "-8.861s-27" },

{ "-1.6752s-13", "-7.341s-14",
  "1.22977s-26" },

{ "9.420s-14", "4.50844s19",
  "4246900.0s0" },

{ "2.0183s-13", "9.598s19",
  "1.93715s7" },

{ "-7.441s-14", "-5.7324s19",
  "4265500.0s0" },

{ "7.241s-14", "-5.79135s19",
  "-4193500.0s0" },

{ "7.987s-14", "8.1113s19",
  "6478500.0s0" },

{ "-1.1603s-13", "7.4468s19",
  "-8640500.0s0" },

{ "-4.432s-14", "-6.851s-24",
  "3.03637s-37" },

{ "-5.064s-14", "-8.119s-24",
  "4.1115s-37" },

{ "3.553s-15", "-6.404s-24",
  "-2.27533s-38" },

{ "8.699s-14", "-3.558s-24",
  "-3.0951s-37" },

{ "9.820s-14", "-5.771s-24",
  "-5.6671s-37" },

{ "-3.477s-14", "7.723s-24",
  "-2.6853s-37" },

{ "7.9082s19", "0.71604s0",
  "5.6626s19" },

{ "-6.83905s19", "-0.36905s0",
  "2.52396s19" },

{ "-7.7697s19", "-0.34073s0",
  "2.64736s19" },

{ "-2.10557s19", "-0.58961s0",
  "1.24146s19" },

{ "9.0963s19", "-0.37693s0",
  "-3.42865s19" },

{ "-4.24076s19", "0.91147s0",
  "-3.8653s19" },

{ "-3.5865s19", "-6.4046s9",
  "2.297s29" },

{ "7.19225s18", "-7.7232s9",
  "-5.5547s28" },

{ "1.98907s19", "-9.9239s9",
  "-1.97393s29" },

{ "-4.27195s19", "7.0734s9",
  "-3.02173s29" },

{ "-8.3115s19", "5.2947s9",
  "-4.40073s29" },

{ "9.4386s19", "8.6548s8",
  "8.169s28" },

{ "6.21677s19", "-3.135s-14",
  "-1948960.0s0" },

{ "-6.30774s19", "1.5884s-13",
  "-1.00192s7" },

{ "7.6073s19", "3.922s-14",
  "2983550.0s0" },

{ "-1.44485s19", "-3.355s-14",
  "484748.0s0" },

{ "3.39653s19", "-7.679s-14",
  "-2608200.0s0" },

{ "-6.0072s19", "1.7825s-13",
  "-1.07078s7" },

{ "1.06812s16", "-1.19583s19",
  "-1.2773s35" },

{ "1.1438s19", "2.616s-24",
  "2.99218s-5" },

{ "-5.79304s18", "-3.095s-24",
  "1.79296s-5" },

{ "-7.6387s19", "8.607s-24",
  "-6.5746s-4" },

{ "4.03933s19", "3.058s-24",
  "1.23523s-4" },

{ "-2.06994s19", "-1.1381s-23",
  "2.3558s-4" },

{ "3.7857s18", "-3.590s-24",
  "-1.35906s-5" },

{ "5.656s-24", "-0.096458s0",
  "-5.4557s-25" },

{ "-5.799s-24", "-0.148445s0",
  "8.6083s-25" },

{ "-9.041s-24", "0.86431s0",
  "-7.8143s-24" },

{ "-2.645s-24", "-0.911865s0",
  "2.41187s-24" },

{ "-9.758s-24", "-0.397186s0",
  "3.87574s-24" },

{ "-5.345s-24", "-0.27215s0",
  "1.45463s-24" },

{ "-3.713s-24", "9.11335s8",
  "-3.38382s-15" },

{ "-3.010s-24", "-9.5278s9",
  "2.86784s-14" },

{ "-1.6904s-23", "-8.37655s9",
  "1.41599s-13" },

{ "-5.074s-24", "-9.2804s9",
  "4.7089s-14" },

{ "-6.942s-22", "-8.7038s9",
  "6.04217s-12" },

{ "-7.643s-24", "-3.1665s9",
  "2.42018s-14" },

{ "-2.659s-24", "-9.238s-14",
  "2.4564s-37" },

{ "-1.7036s-23", "3.138s-14",
  "-5.34586s-37" },

{ "7.684s-24", "8.639s-14",
  "6.6383s-37" },

{ "-3.424s-24", "-6.046s-14",
  "2.07014s-37" },

{ "9.3102s-22", "-1.1344s-13",
  "-1.05614s-34" },

{ "8.070s-24", "3.573s-14",
  "2.8834s-37" },

{ "3.557s-24", "7.9957s19",
  "2.84407s-4" },

{ "7.281s-24", "-3.45443s19",
  "-2.5152s-4" },

{ "-1.6093s-23", "3.22463s19",
  "-5.1894s-4" },

{ "-1.8628s-23", "4.95593s19",
  "-9.2319s-4" },

{ "3.463s-24", "-4.44685s19",
  "-1.53994s-4" },

{ "-8.081s-24", "-1.54701s19",
  "1.25013s-4" },

};
