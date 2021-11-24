#include "program.h"
#include "argsParser.h"
#include "baseParser.h"
#include "io-utils.h"

int main(int argc, char ** argv){
    options_t options;
    //structureBase_t structureBase;
    parseArgs(argc, argv, &options);
    openFiles(&options);
    parseBase(&options);
    readBin(&options);
    closeFiles(&options);
    return 0;
}