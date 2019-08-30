// bdlc_hashtable.cpp                                                 -*-C++-*-

// ----------------------------------------------------------------------------
//                                   NOTICE
//
// This component is not up to date with current BDE coding standards, and
// should not be used as an example for new development.
// ----------------------------------------------------------------------------

#include <bdlc_hashtable.h>

#include <bsls_ident.h>
BSLS_IDENT_RCSID(bdlc_hashtable_cpp,"$Id$ $CSID$")

#include <bsls_assert.h>

#include <bsl_algorithm.h>

// IMPLEMENTATION NOTES:  This table is computed by the following short
// program.  Actually, only entries up to 9950713 have been computed by this
// method.  The other ones were present in the original version prior to this
// update.
//..
//  #include <iostream>
//  #include <cstdlib>
//  #include <climits>
//
//  int main(int argc, char **argv) {
//      int n = argc > 1 ? bsl::atoi(argv[1]) : INT_MAX;
//      double inc = argc > 2 ? bsl::atof(argv[2]) : 1.5;
//
//      // Fill in table isPrime using the sieve method of Eratosthenes.
//
//      bsl::cerr << "Filling in Sieve\n";
//      bool *isPrime = new bool[n];
//      isPrime[0] = isPrime[1] = 0;
//
//      for (int i = 2; i < n; ++i) {
//          isPrime[i] = true;
//      }
//
//      for (int p = 2; p*p < n; ) {
//          for (int i = p*p; i < n; i += p) {
//              isPrime[i] = false;
//          }
//          do {
//              ++p;
//          } while (p*p < n && !isPrime[p]);
//      }
//      bsl::cerr << "Done sieving\n\n";
//
//      // Then compute the first prime number respectively smaller and bigger
//      // than each power of 'inc', adding +5 for making sure no entry
//      // repeats.
//
//      bsl::cout << "const unsigned int PRIME_NUMBERS_IMP[] = {\n";
//
//      for (int i = 4; i <= n; i = (int)((double)i*inc)+6) {
//          int p;
//          if (i <= 1024) {
//              for (p = i-1; !isPrime[p]; --p) ;
//              bsl::cout << "    " << p << ",\n";
//          }
//          for (p = i+1; !isPrime[p]; ++p) ;
//          bsl::cout << "    " << p << ",\n";
//      }
//
//      bsl::cout << "};\n\n";
//
//      delete[] isPrime;
//      return 0;
//  }
//..

namespace BloombergLP {

static
const unsigned int PRIME_NUMBERS_IMP[] = {
             3,          5,          7,         11,         13,         19,
            23,         29,         31,         37,         43,         47,
            53,         61,         67,         73,         79,         83,
            89,         97,        103,        113,        127,        131,
           137,        151,        157,        173,        179,        193,
           199,        211,        223,        241,        251,        277,
           283,        313,        317,        349,        353,        389,
           397,        433,        439,        479,        491,        523,
           547,        599,        607,        661,        673,        733,
           743,        811,        821,        887,        907,        997,
          1009,       1109,       1229,       1361,       1493,       1657,
          1823,       2011,       2213,       2441,       2687,       2963,
          3271,       3607,       3967,       4363,       4813,       5297,
          5827,       6421,       7069,       7789,       8563,       9413,
         10369,      11411,      12547,      13807,      15193,      16729,
         18397,      20249,      22271,      24499,      26959,      29663,
         32633,      35897,      39499,      43451,      47797,      52579,
         57847,      63647,      70001,      77017,      84713,      93199,
        102523,     112771,     124067,     136463,     150131,     165133,
        181667,     199831,     219809,     241793,     265987,     292601,
        321847,     354037,     389447,     428401,     471241,     518387,
        570217,     627251,     689981,     758969,     834893,     918361,
       1010203,    1111247,    1222373,    1344599,    1479073,    1626979,
       1789681,    1968679,    2165531,    2382103,    2620301,    2882351,
       3170579,    3487637,    3836401,    4220053,    4642069,    5106287,
       5616907,    6178597,    6796453,    7476137,    8223727,    9046097,
       9950713,   10380269,   11973487,   13584163,   15204557,   16834721,
      18474367,   20123381,   21780923,   23446463,   25117217,   26797019,
      28481261,   30170029,   31866271,   33566251,   35277211,   36988841,
      38702723,   40418597,   42145471,   43874539,   45602357,   47338261,
      49081741,   50820793,   52568023,   54318679,   56071651,   57825731,
      59585549,   61347211,   63112429,   64880107,   66646339,   68417737,
      70193437,   71970823,   73748413,   75530951,   77317067,   79105109,
      80894237,   82683911,   84477683,   86275403,   88071541,   89873939,
      91673167,   93477623,   95281313,   97087583,   98897269,  100710107,
     102518729,  104331467,  106149529,  107966281,  109781333,  111605789,
     113425759,  115253891,  117078571,  118907203,  120738341,  122571611,
     124398347,  126230089,  128064509,  129898367,  131734613,  133577089,
     135416833,  137260061,  139103449,  140949493,  142792357,  144634027,
     146484077,  148331147,  150183419,  152037727,  153889243,  155744261,
     157597543,  159456439,  161311973,  163173821,  165030673,  166893373,
     168752503,  170613647,  172475339,  174339799,  176206039,  178070021,
     179935519,  181810229,  183682511,  185550061,  187420279,  189295199,
     191172577,  193048463,  194922449,  196802279,  198682409,  200560931,
     202440331,  204322879,  206204491,  208084859,  209966257,  211851517,
     213740509,  215623361,  217511467,  219400751,  221290639,  223177699,
     225065671,  226958581,  228851869,  230742833,  232637549,  234536671,
     236427493,  238317511,  240214493,  242111651,  244014389,  245907773,
     247810021,  249708467,  251607691,  253511801,  255413591,  257317579,
     259223311,  261127739,  263027761,  264939839,  266846981,  268753789,
     270663257,  272576327,  274482589,  276392687,  278301391,  280213861,
     282126797,  284040049,  285958733,  287873149,  289785011,  291704737,
     293619479,  295535971,  297453329,  299373821,  301288991,  303210601,
     305129599,  307050683,  308967577,  310890691,  312813161,  314736073,
     316661479,  318587161,  320515309,  322442353,  324368951,  326291737,
     328219279,  330150377,  332081923,  334008641,  335939441,  337869659,
     339802013,  341729567,  343661191,  345594077,  347526811,  349462871,
     351395533,  353330051,  355264381,  357200747,  359136737,  361078637,
     363016309,  364954699,  366888637,  368828389,  370772999,  372713923,
     374651359,  376588501,  378522577,  380462609,  382404863,  384348683,
     386295293,  388237121,  390185353,  392129443,  394075753,  396021761,
     397968523,  399913937,  401863601,  403811393,  405758497,  407706413,
     409659553,  411606497,  413558459,  415505723,  417455581,  419412647,
     421365977,  423317647,  425266591,  427221139,  429173599,  431128001,
     433087667,  435040097,  436997969,  438952433,  440908271,  442861607,
     444818657,  446778551,  448736579,  450695863,  452655053,  454615613,
     456569593,  458535071,  460496411,  462458641,  464415983,  466377809,
     468339301,  470301791,  472264487,  474231991,  476200493,  478158413,
     480118973,  482086987,  484052771,  486017513,  487983907,  489952249,
     491921081,  493887287,  495854921,  497820737,  499791221,  501765899,
     503737009,  505712587,  507683509,  509651341,  511631381,  513600023,
     515576011,  517546063,  519515959,  521492201,  523461893,  525436253,
     527411377,  529384217,  531359977,  533334133,  535308379,  537283777,
     539259829,  541235543,  543217783,  545196271,  547174391,  549150529,
     551123869,  553098839,  555079933,  557061809,  559038791,  561022549,
     563001421,  564983113,  566965543,  568946281,  570930389,  572911379,
     574894037,  576875557,  578861939,  580841743,  582826339,  584809441,
     586796167,  588786893,  590764409,  592747759,  594732499,  596712629,
     598703081,  600690547,  602676073,  604656517,  606649657,  608637389,
     610625861,  612616003,  614606819,  616594229,  618589409,  620578463,
     622571749,  624560597,  626553073,  628541021,  630531373,  632525441,
     634521847,  636511493,  638502899,  640497001,  642491429,  644489753,
     646481519,  648474599,  650467589,  652465549,  654458239,  656456231,
     658450781,  660444613,  662438873,  664434923,  666434387,  668432263,
     670430993,  672432751,  674429857,  676430743,  678430541,  680428477,
     682433099,  684433291,  686429153,  688425919,  690423623,  692423489,
     694429249,  696423979,  698431751,  700432637,  702433057,  704435803,
     706440011,  708440281,  710445133,  712447481,  714452957,  716463149,
     718466477,  720471391,  722474119,  724476589,  726483941,  728489207,
     730504273,  732508229,  734516971,  736525201,  738532397,  740536529,
     742537633,  744552197,  746562419,  748573963,  750585229,  752593643,
     754608073,  756610051,  758624701,  760635881,  762643213,  764660563,
     766676041,  768684253,  770690743,  772699337,  774716219,  776731399,
     778742297,  780754763,  782765339,  784776799,  786786041,  788802251,
     790808537,  792824833,  794842723,  796855463,  798862223,  800884157,
     802900387,  804914641,  806926037,  808942969,  810955969,  812978239,
     814994641,  817008013,  819026177,  821042437,  823064321,  825085687,
     827102131,  829120949,  831141079,  833158589,  835177271,  837197941,
     839216359,  841234573,  843251911,  845273911,  847298131,  849317543,
     851339087,  853361909,  855382153,  857404567,  859428709,  861456971,
     863479469,  865497161,  867520673,  869545949,  871569487,  873589237,
     875617201,  877646431,  879667939,  881695343,  883720547,  885750793,
     887773151,  889801037,  891831613,  893852257,  895881439,  897909979,
     899940959,  901960909,  903992273,  906019553,  908044133,  910067143,
     912090659,  914120479,  916143769,  918178361,  920200573,  922229953,
     924261971,  926289983,  928323499,  930358057,  932388557,  934418669,
     936444331,  938478143,  940514213,  942544231,  944574871,  946606919,
     948636683,  950671789,  952706369,  954734843,  956769101,  958797193,
     960825961,  962859383,  964893421,  966928517,  968963087,  970992661,
     973030937,  975066959,  977103161,  979141199,  981177647,  983210821,
     985243799,  987276931,  989313727,  991347239,  993385529,  995422427,
     997458529,  999490337, 1001533217, 1003574213, 1005605779, 1007641699,
    1009683641, 1011721987, 1013762137, 1015794709, 1017830173, 1019872691,
    1021910251, 1023952001, 1025989091, 1028033771, 1030070747, 1032108997,
    1034154271, 1036195903, 1038231721, 1040269561, 1042311917, 1044351493,
    1046394751, 1048440761, 1050485197, 1052527327, 1054566949, 1056616843,
    1058658883, 1060702763, 1062745457, 1064784641, 1066825219, 1068866263,
    1070908031, 1072951129, 1074998269, 1077046837, 1079091733, 1081138967,
    1083176839, 1085218027, 1087261787, 1089308653, 1091354351, 1093396313,
    1095445583, 1097488741, 1099535221, 1101584059, 1103624903, 1105673533,
    1107719941, 1109764387, 1111813669, 1113863743, 1115912851, 1117963727,
    1120007423, 1122054007, 1124105009, 1126157827, 1128208453, 1130258057,
    1132308733, 1134362311, 1136415407, 1138468021, 1140516187, 1142572969,
    1144628671, 1146676933, 1148726207, 1150777339, 1152826271, 1154875597,
    1156924999, 1158978979, 1161034067, 1163080937, 1165129783, 1167180467,
    1169234303, 1171288177, 1173337889, 1175389433, 1177440097, 1179495841,
    1181554541, 1183610759, 1185668551, 1187721163, 1189773617, 1191831401,
    1193885467, 1195939799, 1197993127, 1200047713, 1202107289, 1204167947,
    1206219211, 1208272963, 1210328887, 1212383213, 1214440229, 1216495837,
    1218554171, 1220604953, 1222664873, 1224720551, 1226787689, 1228847773,
    1230899843, 1232959279, 1235018713, 1237074337, 1239132767, 1241196479,
    1243257131, 1245315451, 1247368537, 1249424959, 1251480031, 1253539379,
    1255602077, 1257663997, 1259723581, 1261781491, 1263843017, 1265905297,
    1267959079, 1270011943, 1272078007, 1274139023, 1276201691, 1278264079,
    1280320721, 1282385617, 1284446929, 1286508827, 1288571689, 1290631423,
    1292691613, 1294759003, 1296826907, 1298890129, 1300953359, 1303016653,
    1305080753, 1307141357, 1309198333, 1311259751, 1313327107, 1315392751,
    1317460409, 1319526359, 1321586771, 1323656407, 1325720987, 1327783363,
    1329852707, 1331923193, 1333981667, 1336045309, 1338103919, 1340176241,
    1342236101, 1344300719, 1346373673, 1348434727, 1350504143, 1352572651,
    1354633327, 1356697061, 1358763103, 1360830923, 1362896317, 1364963123,
    1367034547, 1369103339, 1371167971, 1373239397, 1375311937, 1377380621,
    1379452189, 1381521929, 1383588247, 1385661679, 1387734841, 1389805111,
    1391863871, 1393938419, 1396006657, 1398077833, 1400146409, 1402220249,
    1404291901, 1406366249, 1408436129, 1410507023, 1412581631, 1414647649,
    1416724003, 1418791039, 1420859917, 1422934367, 1425006643, 1427081197,
    1429152499, 1431221881, 1433296883, 1435367203, 1437441457, 1439519617,
    1441589861, 1443661381, 1445737031, 1447808837, 1449880673, 1451956529,
    1454032873, 1456104361, 1458177503, 1460252063, 1462323587, 1464394553,
    1466474413, 1468551389, 1470630589, 1472704141, 1474780859, 1476850703,
    1478920777, 1480997321, 1483077259, 1485151907, 1487221861, 1489298479,
    1491371741, 1493445253, 1495519379, 1497594157, 1499671603, 1501751057,
    1503831877, 1505901281, 1507980791, 1510061869, 1512143687, 1514224601,
    1516297649, 1518378727, 1520452603, 1522530937, 1524609301, 1526689583,
    1528764361, 1530836603, 1532922187, 1535001803, 1537082023, 1539165079,
    1541245333, 1543323853, 1545403801, 1547481293, 1549562671, 1551637097,
    1553719183, 1555799699, 1557880721, 1559960621, 1562049877, 1564132337,
    1566216931, 1568297321, 1570377821, 1572464063, 1574548453, 1576625363,
    1578701177, 1580784671, 1582865573, 1584946613, 1587029893, 1589113109,
    1591196543, 1593276779, 1595363233, 1597446217, 1599528823, 1601614753,
    1603692787, 1605777221, 1607860031, 1609948243, 1612037503, 1614118333,
    1616203297, 1618289377, 1620378467, 1622456293, 1624543411, 1626629899,
    1628713391, 1630796501, 1632880289, 1634961919, 1637045693, 1639131089,
    1641212879, 1643302813, 1645394987, 1647480851, 1649565319, 1651655113,
    1653746447, 1655836051, 1657921609, 1660012499, 1662100241, 1664185009,
    1666270279, 1668357997, 1670446859, 1672531577, 1674625633, 1676713277,
    1678795759, 1680887671, 1682975101, 1685059931, 1687152007, 1689233911,
    1691319947, 1693410211, 1695503737, 1697591873, 1699684351, 1701773063,
    1703864857, 1705949321, 1708036691, 1710122413, 1712215543, 1714309027,
    1716396173, 1718485123, 1720572317, 1722659761, 1724752657, 1726844293,
    1728939077, 1731030143, 1733124851, 1735222003, 1737313481, 1739404189,
    1741495279, 1743586081, 1745674643, 1747770257, 1749862669, 1751953831,
    1754047037, 1756135643, 1758230107, 1760320993, 1762416701, 1764510431,
    1766599327, 1768690471, 1770788321, 1772880727, 1774979267, 1777071407,
    1779163741, 1781255183, 1783349473, 1785443249, 1787537923, 1789635817,
    1791728843, 1793818799, 1795910377, 1798001321, 1800092929, 1802188991,
    1804284943, 1806381947, 1808477563, 1810570759, 1812669211, 1814766029,
    1816862321, 1818962287, 1821060583, 1823159729, 1825252283, 1827349327,
    1829444333, 1831544243, 1833637639, 1835734709, 1837829453, 1839929401,
    1842026233, 1844122871, 1846220851, 1848320603, 1850423639, 1852517741,
    1854621323, 1856712061, 1858809097, 1860911837, 1863011071, 1865110127,
    1867209427, 1869311779, 1871409871, 1873509647, 1875608447, 1877706059,
    1879804217, 1881907673, 1884008377, 1886112211, 1888211593, 1890304601,
    1892403631, 1894504967, 1896606967, 1898703623, 1900797029, 1902903187,
    1905001183, 1907104751, 1909206749, 1911307313, 1913410417, 1915509133,
    1917613519, 1919715841, 1921820669, 1923921121, 1926024343, 1928127571,
    1930226281, 1932329171, 1934428553, 1936528669, 1938635581, 1940737243,
    1942846589, 1944943409, 1947046571, 1949151101, 1951245427, 1953347449,
    1955451209, 1957555247, 1959654841, 1961763227, 1963867943, 1965972329,
    1968079879, 1970180929, 1972281667, 1974385129, 1976487173, 1978589981,
    1980693643, 1982799391, 1984900601, 1987006939, 1989112667, 1991219371,
    1993323133, 1995426047, 1997534633, 1999642223, 2001743621, 2003842439,
    2005945519, 2008051523, 2010159659, 2012264099, 2014362739, 2016465427,
    2018576653, 2020680787, 2022788851, 2024898619, 2027002919, 2029115573,
    2031222367, 2033329873, 2035432177, 2037542981,
};

static const int NUM_PRIME_NUMBERS_IMP =
                          sizeof PRIME_NUMBERS_IMP / sizeof *PRIME_NUMBERS_IMP;

namespace bdlc {

                   // -------------------------------------
                   // private struct HashTableDefaultTraits
                   // -------------------------------------

// CLASS DATA
const char HashTableDefaultTraits::REMOVED_KEYWORD[] = "(* REMOVED *)";

                       // ------------------------
                       // struct HashTable_ImpUtil
                       // ------------------------

// CLASS DATA
const unsigned int *HashTable_ImpUtil::PRIME_NUMBERS = PRIME_NUMBERS_IMP;
const int           HashTable_ImpUtil::NUM_PRIME_NUMBERS
                                                       = NUM_PRIME_NUMBERS_IMP;

// CLASS METHODS
unsigned int HashTable_ImpUtil::hashSize(bsls::Types::Int64 hint)
{
    BSLS_ASSERT(0 != hint);

    const unsigned int *begin = PRIME_NUMBERS;
    const unsigned int *end   = PRIME_NUMBERS + NUM_PRIME_NUMBERS;

    if (0 < hint) {
        const unsigned int *p = bsl::lower_bound(begin, end, hint);

        if (p == end) {
            --p;
        }

        return *p;                                                    // RETURN
    }
    else {
        hint = -hint;

        const unsigned int *p = bsl::lower_bound(begin, end, hint);

        if (p == end || (p != begin && *p != hint)) {
            --p;
        }

        return *p;                                                    // RETURN
    }
}

}  // close package namespace
}  // close enterprise namespace

// ----------------------------------------------------------------------------
// Copyright 2018 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------