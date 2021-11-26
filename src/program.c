#include "program.h"
#include "argsParser.h"
#include "baseParser.h"
#include "io-utils.h"

int main(int argc, char ** argv){
    options_t options;
    parseArgs(argc, argv, &options);
    openFiles(&options);
    parseBase(&options);
    //readEntireBin(&options);
    /* structureBase_t structureBase;
    initStructure(&structureBase, 0);
    structureBase = readEntryBin(&options, 5); */
    closeFiles(&options);
    return 0;
}