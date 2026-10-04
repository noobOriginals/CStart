#include <iostream>
#include <string>

#include "parser.hpp"
#include "command.hpp"

namespace opt {
    bool help = false;
    std::string cmakeVersion = "3.31";
    std::string projectName = "Project";
    std::string projectVersion = "0.1.0";
    std::string languages = "C CXX";
}

namespace cmd {
    Command cmake_minimum_required("cmake_minimum_required");
    Command project("project");
    Command set("set");
    Command file("file");
    Command find_package("find_package");
    Command add_executable("add_executable");
    Command set_target_properties("set_target_properties");
    Command target_compile_features("target_compile_features");
    Command target_compile_options("target_compile_options");
    Command target_include_directories("target_include_directories");
    Command target_link_libraries("target_link_libraries");
}

void parseArgs(int argc, char** args) {
    ArgParser parser;
    parser.addOption("help", &opt::help, default_parser::parseBool);
    parser.addOption("cmake_version", &opt::cmakeVersion, default_parser::parseString);
    parser.addOption("name", &opt::projectName, default_parser::parseString);
    parser.addOption("version", &opt::projectVersion, default_parser::parseString);
    parser.addOption("languages", &opt::languages, default_parser::parseString);
    parser.addOptionAlias("help", "h");
    parser.addOptionAlias("name", "n");
    parser.addOptionAlias("version", "v");
    parser.addOptionAlias("languages", "l");
    parser.parse((size_t) argc, args);
}

int main(int argc, char** args) {
    parseArgs(argc, args);

    CommandGroup header;
    header.addCommand(cmd::cmake_minimum_required.build(
        "VERSION %s",
        opt::cmakeVersion.c_str())
    );
    header.addCommand(cmd::project.build(
        "%s VERSION %s LANGUAGES %s",
        opt::projectName.c_str(),
        opt::projectVersion.c_str(),
        opt::languages.c_str())
    );
    std::cout << header << "\n";
    return 0;
}
