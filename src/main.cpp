#include <iostream>

#include "parser.hpp"

int main(int argc, char** args) {
    ArgParser parser;
    parser.put<bool>("help", false, default_parser::parseBool);
    parser.put<int>("int", 0, default_parser::parseInt);
    parser.put<double>("double", 0.0, default_parser::parseDouble);
    parser.put<std::string>("str", "basic", default_parser::parseString);
    parser.put<std::vector<std::string>>("strlist", std::vector<std::string>(0), default_parser::parseStringList);
    parser.parse((size_t) argc, args);
    std::cout << parser.get<bool>("help") << "\n";
    std::cout << parser.get<int>("int") << "\n";
    std::cout << parser.get<double>("double") << "\n";
    std::cout << parser.get<std::string>("str") << "\n";
    std::vector<std::string> list = parser.get<std::vector<std::string>>("strlist");
    for (std::string& s : list) {
        std::cout << s << " ";
    }
    std::cout << "\n";
    return 0;
}
