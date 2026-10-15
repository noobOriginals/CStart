#include <iostream>
#include <string>

#include "parser.hpp"
#include "command.hpp"

const char* HELP_MESSAGE =
"CStart — A simple quickstart utility for CMake projects\n\n"
"Usage: cstart <options>\n\n"
"Options:\n"
"    <option>          <parameter:type>         info\n\n"
"    -h -help          <bool>                   display this message and exit\n\n"
"    -pv -preview      <bool>                   do not create any files, just display a preview of\n"
"                                               the generated CMakeLists.txt\n\n"
"    -cmake_version    <version:string>         set cmake minimum required version\n"
"                                               with x.x format — e.g. 3.31\n\n"
"    -n -name          <name:string>            set project name\n\n"
"    -v -version       <version:string>         set project version\n"
"                                               with x.x.x format — e.g. 0.1.0\n\n"
"    -l -languages     <languages:string>       set project languages\n"
"                                               with A B C ... format — e.g. C CXX\n\n"
"    -sources          <sources:string_list>    set sources (when <glob> is not provided) or\n"
"                                               set source directories (when <glob> is provided)\n\n"
"    -glob             <glob:string_list>       set glob pattern — e.g. .c .cpp .mm (glob for\n"
"                                               C, CPP and OBJCPP files) — requires <sources> to\n"
"                                               be set to the source path\n\n"
"    -include          <include:string_list>    set include directories. Can use paths or CMake variables\n"
"                                               (without $) — e.g. \"path/to/folder\" ENV{ENV_VAR}\n\n"
"    -link             <link:string_list>       set link directories. Can use paths or CMake variables\n"
"                                               (without $) — e.g. \"path/to/folder\" ENV{ENV_VAR}\n\n"
"    -libs             <libs:string_list>       select libraries for linking\n\n"
"    -target_name      <name:string>            set target name\n\n"
"    -shared           <bool>                   configure to build shared library\n\n"
"    -static           <bool>                   configure to build static library\n\n"
"                                               \n\n";

namespace opt {
    bool help = false;
    bool preview = false;
    std::string cmakeVersion = "3.31";
    std::string projectName = "cproj";
    std::string projectVersion = "0.1.0";
    std::string languages = "C";
    std::vector<std::string> sources(1, "./src");
    std::vector<std::string> glob(1, ".c");
    std::vector<std::string> include(1, "./include");
    std::vector<std::string> link;
    std::vector<std::string> libs;
    std::string targetName = "${PROJECT_NAME}";
    bool sharedLib = false;
    bool staticLib = false;
}

namespace cmd {
    Command cmake_minimum_required("cmake_minimum_required");
    Command project("project");
    Command set("set");
    Command file("file");
    Command find_package("find_package");
    Command add_executable("add_executable");
    Command add_library("add_library");
    Command set_target_properties("set_target_properties");
    Command target_compile_features("target_compile_features");
    Command target_compile_options("target_compile_options");
    Command target_include_directories("target_include_directories");
    Command target_link_libraries("target_link_libraries");
}

bool strStartsWith(const std::string& str, const std::string prefix) {
    return str.substr(0, prefix.size()) == prefix;
}

bool strEndsWith(const std::string& str, const std::string suffix) {
    return str.substr(str.size() - suffix.size()) == suffix;
}

std::string formatForCMake(std::string str) {
    if (str.find(' ') < str.size() && str[0] != '"') str = "\"" + str + "\"";
    if (strStartsWith(str, "./")) return "${CMAKE_CURRENT_SOURCE_DIR}/" + str.substr(2);
    if (strStartsWith(str, "\"./")) return "\"${CMAKE_CURRENT_SOURCE_DIR}/" + str.substr(3);
    if (strStartsWith(str, "{") || strStartsWith(str, "ENV{")) return "$" + str;
    if (strStartsWith(str, "\"{") || strStartsWith(str, "\"ENV{")) return "\"$" + str.substr(1);
    return str;
}

void parseArgs(int argc, char** args) {
    ArgParser parser;
    parser.addOption("help", &opt::help, default_parser::parseBool);
    parser.addOption("preview", &opt::preview, default_parser::parseBool);

    parser.addOption("cmake_version", &opt::cmakeVersion, default_parser::parseString);

    parser.addOption("name", &opt::projectName, default_parser::parseString);
    parser.addOption("version", &opt::projectVersion, default_parser::parseString);
    parser.addOption("languages", &opt::languages, default_parser::parseString);

    parser.addOption("sources", &opt::sources, default_parser::parseStringList);
    parser.addOption("glob", &opt::glob, default_parser::parseStringList);

    parser.addOption("include", &opt::include, default_parser::parseStringList);
    parser.addOption("link", &opt::link, default_parser::parseStringList);
    parser.addOption("libs", &opt::libs, default_parser::parseStringList);

    parser.addOption("target_name", &opt::targetName, default_parser::parseString);
    parser.addOption("shared", &opt::sharedLib, default_parser::parseBool);
    parser.addOption("static", &opt::staticLib, default_parser::parseBool);

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

    CommandGroup header, sources, target;
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

    if (opt::sources.size() > 1) {
        std::string body = "";
        for (std::string& str : opt::sources) {
            str = formatForCMake(str);
            body += str + " ";
        }
        body.pop_back();
        sources.addCommand(cmd::set.build(
            "Sources " + body
        ));
    } else {
        sources.addCommand(cmd::set.build(
            "SourcePath " + formatForCMake(opt::sources[0])
        ));
        std::string body = "";
        for (std::string& str : opt::glob) {
            str = "${SourcePath}/*" + str;
            body += str + " ";
        }
        body.pop_back();
        sources.addCommand(cmd::file.build(
            "GLOB_RECURSE Sources CONFIGURE_DEPENDS " + body
        ));
    }

    target.addCommand(cmd::add_executable.build(
        opt::targetName + " ${Sources}"
    ));
    for (std::string& str : opt::include) {
        str = formatForCMake(str);
        target.addCommand(cmd::target_include_directories.build(
            "%s PUBLIC %s",
            opt::targetName.c_str(),
            str.c_str()
        ));
    }

    if (opt::preview) {
        std::cout << header << "\n\n";
        std::cout << sources << "\n\n";
        std::cout << target << "\n";
        return 0;
    }
    return 0;
}
