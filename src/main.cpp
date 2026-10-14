#include <iostream>
#include <string>

#include "parser.hpp"
#include "command.hpp"

const char* HELP_MESSAGE =
"CStart — A simple quickstart utility for CMake projects\n\n"
"Usage: cstart <options>\n\n"
"Options:\n"
"    <option>          <parameter:type>      info\n\n"
"    -h -help                                display this message and exit\n\n"
"    -pv -preview                            do not create any files, just display a preview of\n"
"                                            the generated CMakeLists.txt\n\n"
"    -cmake_version    <version:string>      set cmake minimum required version\n"
"                                            with x.x format — e.g. 3.31\n\n"
"    -n -name          <name:string>         set project name\n\n"
"    -v -version       <version:string>      set project version\n"
"                                            with x.x.x format — e.g. 0.1.0\n\n"
"    -l -languages     <languages:string>    set project languages\n"
"                                            with A B C ... format — e.g. C CXX\n\n"
"    -header           <header:sring>        set header path\n\n"
"    -source           <source:sring>        set source path\n\n"
"    -include          <include:string>      set include directories. Can use paths or\n"
"                                            CMake variables — e.g. \"path/to/folder\" $ENV{ENV_VAR}\n\n"
"    -link             <link:string>         set link directories. Can use paths or\n"
"                                            CMake variables — e.g. \"path/to/folder\" $ENV{ENV_VAR}\n\n"
"    -libs             <libs:string>         select libraries for linking";

namespace opt {
    bool help = false;
    bool preview = false;
    std::string cmakeVersion = "3.31";
    std::string projectName = "cproj";
    std::string projectVersion = "0.1.0";
    std::string languages = "C";
    std::string header = "include";
    std::string source = "src";
    std::string includeDirectories = "${Headers}";
    std::string linkDirectories = "";
    std::string libraries = "";
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
    parser.addOption("preview", &opt::preview, default_parser::parseBool);

    parser.addOption("cmake_version", &opt::cmakeVersion, default_parser::parseString);

    parser.addOption("name", &opt::projectName, default_parser::parseString);
    parser.addOption("version", &opt::projectVersion, default_parser::parseString);
    parser.addOption("languages", &opt::languages, default_parser::parseString);

    parser.addOption("header", &opt::header, default_parser::parseString);
    parser.addOption("source", &opt::source, default_parser::parseString);

    parser.addOption("include", &opt::includeDirectories, default_parser::parseString);
    parser.addOption("link", &opt::linkDirectories, default_parser::parseString);
    parser.addOption("libs", &opt::libraries, default_parser::parseString);

    parser.addOptionAlias("help", "h");
    parser.addOptionAlias("preview", "pv");
    parser.addOptionAlias("name", "n");
    parser.addOptionAlias("version", "v");
    parser.addOptionAlias("languages", "l");

    parser.parse((size_t) argc, args);
}

int main(int argc, char** args) {
    parseArgs(argc, args);
    if (opt::help) {
        std::cout << HELP_MESSAGE << "\n";
        return 0;
    }

    CommandGroup header;
    header.addCommand(cmd::cmake_minimum_required.build(
        "VERSION %s",
        opt::cmakeVersion.c_str()
    ));
    header.addCommand(cmd::project.build(
        "%s VERSION %s LANGUAGES %s",
        opt::projectName.c_str(),
        opt::projectVersion.c_str(),
        opt::languages.c_str()
    ));

    CommandGroup files;
    files.addCommand(cmd::set.build(
        "HeaderPath \"${CMAKE_CURRENT_SOURCE_DIR}/%s\"",
        opt::header.c_str()
    ));
    files.addCommand(cmd::set.build(
        "SourcePath \"${CMAKE_CURRENT_SOURCE_DIR}/%s\"",
        opt::source.c_str()
    ));

    if (opt::preview) {
        std::cout << header << "\n\n";
        std::cout << files << "\n";
        return 0;
    }
    return 0;
}
