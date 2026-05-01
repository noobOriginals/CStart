// Local includes
#include <generator.hpp>

int32 main(int32_t argc, char** args) {
    if (!init(argc, args)) {
        printHelp();
        return 0;
    };
    generate();
    return 0;
}