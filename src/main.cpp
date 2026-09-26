#include <iostream>

#include "parser.hpp"

struct Options {
    bool help = false;
    int i = 0;
    double d = 0.0;
    std::string str = "";
    std::vector<std::string> strlist;
};

int main(int argc, char** args) {
    Options options;
    ArgParser parser;
    parser.addOption("help", &options.help, default_parser::parseBool);
    parser.addOption("int", &options.i, default_parser::parseInt);
    parser.addOption("double", &options.d, default_parser::parseDouble);
    parser.addOption("str", &options.str, default_parser::parseString);
    parser.addOption("strlist", &options.strlist, default_parser::parseStringList);
    parser.addOptionAlias("help", "h");
    parser.parse((size_t) argc, args);
    std::cout << options.help << "\n";
    std::cout << options.i << "\n";
    std::cout << options.d << "\n";
    std::cout << options.str << "\n";
    for (std::string& s : options.strlist) {
        std::cout << s << " ";
    }
    std::cout << "\n";
    return 0;
}
