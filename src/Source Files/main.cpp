#include <iostream>
#include <fstream>

#include <generator.hpp>

int main(const uint32_t argc, char** args) {
    if (!init(argc, args)) {
        printHelp();
        return 0;
    };
    generate();
    return 0;
}