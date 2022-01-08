#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "argsParser.h"
#include "baseParser.h"
#include "fonctionsMatrice.h"
#include "fonctionsMatricesHash.h"
#include "io-utils.h"
#include "program.h"
#include "readFunctions.h"
#include "searchingFunctions.h"
#include "tps_unit_test.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int interruptFlag = 0;

unsigned hashtest(unsigned char *str, int p) {
    (void)p;
    unsigned hash = 6961;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    hash %= 51648789;
    // printf("%u\n", hash);
    return hash;
}

int readEntireBintest(options_t *options, int p) {
    initSigaction();
    FILE *file = fopen("/tmp/out", "w");
    fseek(options->outputFile, 0, SEEK_SET);
    char *buff = malloc(1000);
    while (fgets(buff, 1000, options->outputFile) != NULL) {
        // fprintf(file, "Ok");
        fprintf(file, "%u %s", hashtest((unsigned char *)buff, p), buff);
    }
    free(buff);
    fclose(file);
    return OK;
}

void testCreateListeAdj(void) {
    char c0[] = "g";

    node *node0 = CreateListAdj(c0);

    char *c1 = "a";
    char *c2 = "b";
    char *c3 = "c";
    char *c4 = "d";
    char *c5 = "e";
    char *c6 = "f";
    node *end = node0;
    end = appendNode(c1, end);
    end = appendNode(c2, end);
    end = appendNode(c3, end);
    end = appendNode(c4, end);
    end = appendNode(c5, end);
    end = appendNode(c6, end);

    appendEdge(1, 2, node0);
    appendEdge(1, 3, node0);
    appendEdge(1, 5, node0);
    appendEdge(2, 4, node0);
    appendEdge(2, 6, node0);
    appendEdge(3, 0, node0);
    appendEdge(5, 6, node0);

    printListNode(node0);
    printListEdge(node0);

    char author[] = "d";
    int test = AuthorInList(author, node0);
    printf("test présence author: %i\n", test);

    char author2[] = "z";
    int test2 = AuthorInList(author2, node0);
    printf("test présence author: %i\n", test2);

    options_t options;
    initOptions(&options);
    options.authorNames[0] = "a";
    dijkstra(1, node0, 6);
    printDistances(&options, node0);
    // plusCourtChemin(3,2,node0,6);
    // int rs=nbrComposanteConnexe(node0);
    // printf("nbr Connexe:%i\n",rs);

    freeListAdj(node0);
}

TEST_INIT_GLOBAL

void testParse() {
    options_t options;
    initOptions(&options);
    options.inputFilename = "sample.xml";
    options.outputFilename = "outsampletest.bin";
    options.action[ACTION_PARSE] = 1;
    openFiles(&options, "w", 0);
    tps_assert(options.inputFile != NULL);
    tps_assert(options.outputFile != NULL);
    tps_assert(parseBase(&options) == OK);
    closeFiles(&options);
}

void testRead() {
    options_t options;
    initOptions(&options);
    options.outputFilename = "outsampletest.bin";
    options.action[ACTION_PARSE] = 0;
    openFiles(&options, "r", 0);
    tps_assert(readEntireBin(&options, 1) == OK);
    closeFiles(&options);
}

void testGraph() {
    int taille = 0;
    node *node0 = NULL;
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    tps_assert(hashTable != NULL);
    for (int i = 0; i < HT_SIZE; i++)
        hashTable[i] = NULL;
    options_t options;
    initOptions(&options);
    options.outputFilename = "outsampletest.bin";
    options.action[ACTION_PARSE] = 0;
    openFiles(&options, "r", 0);
    node0 = DoListAdjDeBinHash(&options, &taille, hashTable);
    tps_assert(node0 != NULL);
    endOfProgram(&options, node0, hashTable);
}

void testArticles() {
    int taille = 0;
    node *node0 = NULL;
    node **hashTable = malloc(HT_SIZE * sizeof(unsigned int) * sizeof(char *));
    tps_assert(hashTable != NULL);
    for (int i = 0; i < HT_SIZE; i++)
        hashTable[i] = NULL;
    options_t options;
    initOptions(&options);
    options.outputFilename = "outsampletest.bin";
    options.action[ACTION_PARSE] = 0;
    options.authorNames[0] = "Russell Turpin";
    openFiles(&options, "r", 0);
    node0 = DoListAdjDeBinHash(&options, &taille, hashTable);
    tps_assert(node0 != NULL);
    tps_assert(showArticles(&options, node0, 0) == OK);
    endOfProgram(&options, node0, hashTable);
}

int main(void) {
    /*// testCreateListeAdj();
    options_t options;
    options.inputFilename = "../database/dblp.xml";
    // options.outputFilename = "../database/dblp.bin";
    // options.outputFilename = "/tmp/dblp.bin";
    // options.outputFilename = "../tests/sample.bin";
    options.outputFilename = "/tmp/authors.txt";
    options.inputFile = NULL;
    options.outputFile = NULL;
    options.action[ACTION_UNKNOWN] = 1;
    for (int a = 1; a < 6; a++)
        options.action[a] = NOT_TO_DO;
    options.authorNames[0] = NULL;
    options.authorNames[1] = NULL;

    // openFiles(&options, "r", 0);
    options.outputFile = fopen(options.outputFilename, "r");
    int tab[2000] = {
        5381,  5387,  5393,  5399,  5407,  5413,  5417,  5419,  5431,  5437,
        5441,  5443,  5449,  5471,  5477,  5479,  5483,  5501,  5503,  5507,
        5519,  5521,  5527,  5531,  5557,  5563,  5569,  5573,  5581,  5591,
        5623,  5639,  5641,  5647,  5651,  5653,  5657,  5659,  5669,  5683,
        5689,  5693,  5701,  5711,  5717,  5737,  5741,  5743,  5749,  5779,
        5783,  5791,  5801,  5807,  5813,  5821,  5827,  5839,  5843,  5849,
        5851,  5857,  5861,  5867,  5869,  5879,  5881,  5897,  5903,  5923,
        5927,  5939,  5953,  5981,  5987,  6007,  6011,  6029,  6037,  6043,
        6047,  6053,  6067,  6073,  6079,  6089,  6091,  6101,  6113,  6121,
        6131,  6133,  6143,  6151,  6163,  6173,  6197,  6199,  6203,  6211,
        6217,  6221,  6229,  6247,  6257,  6263,  6269,  6271,  6277,  6287,
        6299,  6301,  6311,  6317,  6323,  6329,  6337,  6343,  6353,  6359,
        6361,  6367,  6373,  6379,  6389,  6397,  6421,  6427,  6449,  6451,
        6469,  6473,  6481,  6491,  6521,  6529,  6547,  6551,  6553,  6563,
        6569,  6571,  6577,  6581,  6599,  6607,  6619,  6637,  6653,  6659,
        6661,  6673,  6679,  6689,  6691,  6701,  6703,  6709,  6719,  6733,
        6737,  6761,  6763,  6779,  6781,  6791,  6793,  6803,  6823,  6827,
        6829,  6833,  6841,  6857,  6863,  6869,  6871,  6883,  6899,  6907,
        6911,  6917,  6947,  6949,  6959,  6961,  6967,  6971,  6977,  6983,
        6991,  6997,  7001,  7013,  7019,  7027,  7039,  7043,  7057,  7069,
        7079,  7103,  7109,  7121,  7127,  7129,  7151,  7159,  7177,  7187,
        7193,  7207,  7211,  7213,  7219,  7229,  7237,  7243,  7247,  7253,
        7283,  7297,  7307,  7309,  7321,  7331,  7333,  7349,  7351,  7369,
        7393,  7411,  7417,  7433,  7451,  7457,  7459,  7477,  7481,  7487,
        7489,  7499,  7507,  7517,  7523,  7529,  7537,  7541,  7547,  7549,
        7559,  7561,  7573,  7577,  7583,  7589,  7591,  7603,  7607,  7621,
        7639,  7643,  7649,  7669,  7673,  7681,  7687,  7691,  7699,  7703,
        7717,  7723,  7727,  7741,  7753,  7757,  7759,  7789,  7793,  7817,
        7823,  7829,  7841,  7853,  7867,  7873,  7877,  7879,  7883,  7901,
        7907,  7919,  7927,  7933,  7937,  7949,  7951,  7963,  7993,  8009,
        8011,  8017,  8039,  8053,  8059,  8069,  8081,  8087,  8089,  8093,
        8101,  8111,  8117,  8123,  8147,  8161,  8167,  8171,  8179,  8191,
        8209,  8219,  8221,  8231,  8233,  8237,  8243,  8263,  8269,  8273,
        8287,  8291,  8293,  8297,  8311,  8317,  8329,  8353,  8363,  8369,
        8377,  8387,  8389,  8419,  8423,  8429,  8431,  8443,  8447,  8461,
        8467,  8501,  8513,  8521,  8527,  8537,  8539,  8543,  8563,  8573,
        8581,  8597,  8599,  8609,  8623,  8627,  8629,  8641,  8647,  8663,
        8669,  8677,  8681,  8689,  8693,  8699,  8707,  8713,  8719,  8731,
        8737,  8741,  8747,  8753,  8761,  8779,  8783,  8803,  8807,  8819,
        8821,  8831,  8837,  8839,  8849,  8861,  8863,  8867,  8887,  8893,
        8923,  8929,  8933,  8941,  8951,  8963,  8969,  8971,  8999,  9001,
        9007,  9011,  9013,  9029,  9041,  9043,  9049,  9059,  9067,  9091,
        9103,  9109,  9127,  9133,  9137,  9151,  9157,  9161,  9173,  9181,
        9187,  9199,  9203,  9209,  9221,  9227,  9239,  9241,  9257,  9277,
        9281,  9283,  9293,  9311,  9319,  9323,  9337,  9341,  9343,  9349,
        9371,  9377,  9391,  9397,  9403,  9413,  9419,  9421,  9431,  9433,
        9437,  9439,  9461,  9463,  9467,  9473,  9479,  9491,  9497,  9511,
        9521,  9533,  9539,  9547,  9551,  9587,  9601,  9613,  9619,  9623,
        9629,  9631,  9643,  9649,  9661,  9677,  9679,  9689,  9697,  9719,
        9721,  9733,  9739,  9743,  9749,  9767,  9769,  9781,  9787,  9791,
        9803,  9811,  9817,  9829,  9833,  9839,  9851,  9857,  9859,  9871,
        9883,  9887,  9901,  9907,  9923,  9929,  9931,  9941,  9949,  9967,
        9973,  10007, 10009, 10037, 10039, 10061, 10067, 10069, 10079, 10091,
        10093, 10099, 10103, 10111, 10133, 10139, 10141, 10151, 10159, 10163,
        10169, 10177, 10181, 10193, 10211, 10223, 10243, 10247, 10253, 10259,
        10267, 10271, 10273, 10289, 10301, 10303, 10313, 10321, 10331, 10333,
        10337, 10343, 10357, 10369, 10391, 10399, 10427, 10429, 10433, 10453,
        10457, 10459, 10463, 10477, 10487, 10499, 10501, 10513, 10529, 10531,
        10559, 10567, 10589, 10597, 10601, 10607, 10613, 10627, 10631, 10639,
        10651, 10657, 10663, 10667, 10687, 10691, 10709, 10711, 10723, 10729,
        10733, 10739, 10753, 10771, 10781, 10789, 10799, 10831, 10837, 10847,
        10853, 10859, 10861, 10867, 10883, 10889, 10891, 10903, 10909, 10937,
        10939, 10949, 10957, 10973, 10979, 10987, 10993, 11003, 11027, 11047,
        11057, 11059, 11069, 11071, 11083, 11087, 11093, 11113, 11117, 11119,
        11131, 11149, 11159, 11161, 11171, 11173, 11177, 11197, 11213, 11239,
        11243, 11251, 11257, 11261, 11273, 11279, 11287, 11299, 11311, 11317,
        11321, 11329, 11351, 11353, 11369, 11383, 11393, 11399, 11411, 11423,
        11437, 11443, 11447, 11467, 11471, 11483, 11489, 11491, 11497, 11503,
        11519, 11527, 11549, 11551, 11579, 11587, 11593, 11597, 11617, 11621,
        11633, 11657, 11677, 11681, 11689, 11699, 11701, 11717, 11719, 11731,
        11743, 11777, 11779, 11783, 11789, 11801, 11807, 11813, 11821, 11827,
        11831, 11833, 11839, 11863, 11867, 11887, 11897, 11903, 11909, 11923,
        11927, 11933, 11939, 11941, 11953, 11959, 11969, 11971, 11981, 11987,
        12007, 12011, 12037, 12041, 12043, 12049, 12071, 12073, 12097, 12101,
        12107, 12109, 12113, 12119, 12143, 12149, 12157, 12161, 12163, 12197,
        12203, 12211, 12227, 12239, 12241, 12251, 12253, 12263, 12269, 12277,
        12281, 12289, 12301, 12323, 12329, 12343, 12347, 12373, 12377, 12379,
        12391, 12401, 12409, 12413, 12421, 12433, 12437, 12451, 12457, 12473,
        12479, 12487, 12491, 12497, 12503, 12511, 12517, 12527, 12539, 12541,
        12547, 12553, 12569, 12577, 12583, 12589, 12601, 12611, 12613, 12619,
        12637, 12641, 12647, 12653, 12659, 12671, 12689, 12697, 12703, 12713,
        12721, 12739, 12743, 12757, 12763, 12781, 12791, 12799, 12809, 12821,
        12823, 12829, 12841, 12853, 12889, 12893, 12899, 12907, 12911, 12917,
        12919, 12923, 12941, 12953, 12959, 12967, 12973, 12979, 12983, 13001,
        13003, 13007, 13009, 13033, 13037, 13043, 13049, 13063, 13093, 13099,
        13103, 13109, 13121, 13127, 13147, 13151, 13159, 13163, 13171, 13177,
        13183, 13187, 13217, 13219, 13229, 13241, 13249, 13259, 13267, 13291,
        13297, 13309, 13313, 13327, 13331, 13337, 13339, 13367, 13381, 13397,
        13399, 13411, 13417, 13421, 13441, 13451, 13457, 13463, 13469, 13477,
        13487, 13499, 13513, 13523, 13537, 13553, 13567, 13577, 13591, 13597,
        13613, 13619, 13627, 13633, 13649, 13669, 13679, 13681, 13687, 13691,
        13693, 13697, 13709, 13711, 13721, 13723, 13729, 13751, 13757, 13759,
        13763, 13781, 13789, 13799, 13807, 13829, 13831, 13841, 13859, 13873,
        13877, 13879, 13883, 13901, 13903, 13907, 13913, 13921, 13931, 13933,
        13963, 13967, 13997, 13999, 14009, 14011, 14029, 14033, 14051, 14057,
        14071, 14081, 14083, 14087, 14107, 14143, 14149, 14153, 14159, 14173,
        14177, 14197, 14207, 14221, 14243, 14249, 14251, 14281, 14293, 14303,
        14321, 14323, 14327, 14341, 14347, 14369, 14387, 14389, 14401, 14407,
        14411, 14419, 14423, 14431, 14437, 14447, 14449, 14461, 14479, 14489,
        14503, 14519, 14533, 14537, 14543, 14549, 14551, 14557, 14561, 14563,
        14591, 14593, 14621, 14627, 14629, 14633, 14639, 14653, 14657, 14669,
        14683, 14699, 14713, 14717, 14723, 14731, 14737, 14741, 14747, 14753,
        14759, 14767, 14771, 14779, 14783, 14797, 14813, 14821, 14827, 14831,
        14843, 14851, 14867, 14869, 14879, 14887, 14891, 14897, 14923, 14929,
        14939, 14947, 14951, 14957, 14969, 14983};

    for (int p = 1; p < 1050; p++) {
        fprintf(stderr, "premier : %i\n", tab[p]);
    }
    readEntireBintest(&options, 0);
    if (system("bash hashcount.sh") == -1)
        abort();

    closeFiles(&options);*/

    TEST(testParse);
    TEST(testRead);
    TEST(testGraph);
    TEST(testArticles);

    return 0;
}
