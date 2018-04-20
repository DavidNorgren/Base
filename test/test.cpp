#include "common.hpp"
#include "file/resourcehandler.hpp"
#include "testjson.hpp"

extern char resData[] asm("_binary_res_zip_start");
extern char resSize[] asm("_binary_res_zip_size");


int main() 
{
    using namespace Base;
    ResourceHandler* resHandler = new ResourceHandler(resData, (uint)resSize);

    // JSON testing
    testJSON(resHandler);

    return 0;
}