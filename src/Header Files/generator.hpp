#ifndef GENERATOR_H
#define GENERATOR_H

// Std includes
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

// Local includes
#include <util/types.h>

constexpr char* HELP = 
"cstart help page:\n\n"
"Options:\n"
"    -n -p --name --project -> Set the project name inside CMake (also the executable name)\n"
"    -v --version -> Set the required CMake version\n"
"    -o --output-dir -> The folder for the build output\n"
"    -s --source-dir -> The folder for the sources (it includes the 'Source Files' and 'Header Files' directories)\n"
"    -l --libraries -> The name of the libraries that have to be linked (e.g. from 'vulkan-1.lib' you use 'vulkan-1')\n"
"    --copy-res -> Enables the use of a 'Resource Files' folder inside the src folder and copies the resources to the bin output\n"
"                  folder before compilation for accesing them in your program.\n"
"    --incl-dirs -> Specify additional include directories beside the default header and src files\n"
"    --link-dirs -> Specify link directories for the libraries to be linked from.\n"
"    --language -> Specify the language for you project (if option 'c' is chosen, then the language is set to c.\n"
"                  If anything else is chosen, the language is set to c++)\n\n"
"!!! ALL options will default to a value if ommited:\n"
"    -n -p --name --project -> '...'\n"
"    -v --version -> '3.26.0'\n"
"    -o --output-dir -> 'bin'\n"
"    -s --source-dir -> 'src'\n"
"    --copy-res -> false\n"
"    --incl-dirs -> none\n"
"    --link-dirs -> none\n"
"    --language -> c++\n\n"
"!!! The source folder contains three folders in the default CMake config:\n"
" - 'Source Files'\n"
" - 'Header Files'\n"
" - 'Resource Files' (only included if the --copy-res option is specified)\n\n"
"!!! cstart automatically creates those directories if it doesn't find them in the 'start' folder.\n";

std::fstream file;

constexpr char* DEFAULT_VERSION = "cmake_minimum_required(VERSION 3.26.0)\n";
constexpr char* DEFAULT_NAME_CXX = "project(\"...\" VERSION 0.1.0 LANGUAGES C CXX)\n\n";
constexpr char* DEFAULT_NAME_C = "project(\"...\" VERSION 0.1.0 LANGUAGES C)\n\n";
constexpr char* DEFAULT_OUTPUT = "set (FullOutputDir \"${CMAKE_SOURCE_DIR}/bin\")\n";
constexpr char* DEFAULT_OUT_DIRS = 
    "set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY \"${FullOutputDir}/Static Libraries\")\n"
    "set (CMAKE_LIBRARY_OUTPUT_DIRECTORY \"${FullOutputDir}\")\n"
    "set (CMAKE_RUNTIME_OUTPUT_DIRECTORY \"${FullOutputDir}\")\n\n";
constexpr char* DEFAULT_SOURCES_CXX = 
    "set (SrcPath \"${CMAKE_SOURCE_DIR}/src/Source Files\")\n"
    "set (HeaderPath \"${CMAKE_SOURCE_DIR}/src/Header Files\")\n"
    "file (GLOB_RECURSE Sources CONFIGURE_DEPENDS \"${SrcPath}/*.cpp\" \"${SrcPath}/*.c\")\n\n";
constexpr char* DEFAULT_SOURCES_C = 
    "set (SrcPath \"${CMAKE_SOURCE_DIR}/src/Source Files\")\n"
    "set (HeaderPath \"${CMAKE_SOURCE_DIR}/src/Header Files\")\n"
    "file (GLOB_RECURSE Sources CONFIGURE_DEPENDS \"${SrcPath}/*.c\")\n\n";
constexpr char* DEFAULT_SOURCES_RES_CXX = 
    "set (SrcPath \"${CMAKE_SOURCE_DIR}/src/Source Files\")\n"
    "set (HeaderPath \"${CMAKE_SOURCE_DIR}/src/Header Files\")\n"
    "set (ResourcePath \"${CMAKE_SOURCE_DIR}/src/Resource Files\")\n"
    "file (GLOB_RECURSE Sources CONFIGURE_DEPENDS \"${SrcPath}/*.cpp\" \"${SrcPath}/*.c\")\n\n";
constexpr char* DEFAULT_SOURCES_RES_C = 
    "set (SrcPath \"${CMAKE_SOURCE_DIR}/src/Source Files\")\n"
    "set (HeaderPath \"${CMAKE_SOURCE_DIR}/src/Header Files\")\n"
    "set (ResourcePath \"${CMAKE_SOURCE_DIR}/src/Resource Files\")\n"
    "file (GLOB_RECURSE Sources CONFIGURE_DEPENDS \"${SrcPath}/*.c\")\n\n";
constexpr char* DEFAULT_INCLUDE_DIRS = "include_directories(${HeaderPath})\n\n";
constexpr char* DEFAULT_ADD_EXEC_CXX = 
    "add_executable(${PROJECT_NAME} ${Sources})\n"
    "target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)\n\n";
constexpr char* DEFAULT_ADD_EXEC_C = 
    "add_executable(${PROJECT_NAME} ${Sources})\n\n";
constexpr char* DEFAULT_COPY_RES =
    "add_custom_target(\n"
    "    CopyResources ALL\n"
    "    COMMAND ${CMAKE_COMMAND} -E copy_directory\n"
    "    ${ResourcePath}\n"
    "    ${FullOutputDir}/Debug\n"
    "    COMMAND ${CMAKE_COMMAND} -E copy_directory\n"
    "    ${ResourcePath}\n"
    "    ${FullOutputDir}/Release\n"
	"    COMMAND ${CMAKE_COMMAND} -E copy_directory\n"
	"    ${ResourcePath}\n"
	"    ${FullOutputDir}\n"
    "    COMMENT \"Copying resources to binary output folder\"\n"
    ")\n"
    "add_dependencies(\"${PROJECT_NAME}\" CopyResources)\n";

constexpr char* MAIN_SNIPPET_C = 
    "#include <stdio.h>\n"
    "\n"
    "int main() {\n"
    "    printf(\"Hello, world!\\n\");\n"
    "    return 0;\n"
    "}\n";
constexpr char* MAIN_SNIPPET_CXX = 
    "#include <iostream>\n"
    "\n"
    "int main() {\n"
    "    std::cout << \"Hello, world!\\n\";\n"
    "    return 0;\n"
    "}\n";

const char* NAME;
const char* VERSION;
const char* OUTPUT;
const char* SOURCE;

char** LIBS;
uint32 libsize;
char** INCL;
uint32 incsize;
char** LNK;
uint32 lnksize;

bool nameProvided = false;
bool versionProvided = false;
bool outputProvided = false;
bool sourceProvided = false;
bool libsProvided = false;
bool copyRes = false;
bool inclDirsProvided = false;
bool linkDirsProvided = false;
bool languageProvided = false;
bool languageIsC = false;

namespace options {
    constexpr char* H = "-h";
    constexpr char* HELP = "--help";
    constexpr char* N = "-n";
    constexpr char* NAME = "--name";
    constexpr char* P = "-p";
    constexpr char* PROJECT = "--project";
    constexpr char* V = "-v";
    constexpr char* VERSION = "--version";
    constexpr char* O = "-o";
    constexpr char* OUTPUT_DIR = "--output-dir";
    constexpr char* S = "-s";
    constexpr char* SOURCE_DIR = "--source-dir";
    constexpr char* L = "-l";
    constexpr char* LIBRARIES = "--libraries";
    constexpr char* COPY_RES = "--copy-res";
    constexpr char* INCL_DIRS = "--incl-dirs";
    constexpr char* LINK_DIRS = "--link-dirs";
    constexpr char* LANGUAGE = "--language";
}

bool init(uint32 argc, char** args) {
    for (uint32 i = 0; i < argc; i++) {
        if (strcmp(args[i], options::H) == 0 || strcmp(args[i], options::HELP) == 0) {
            if (libsProvided) free(LIBS);
            if (inclDirsProvided) free(INCL);
            if (linkDirsProvided) free(LNK);
            return false;
        }
        if (strcmp(args[i], options::N) == 0 || strcmp(args[i], options::P) == 0 || strcmp(args[i], options::NAME) == 0 || strcmp(args[i], options::PROJECT) == 0) {
            if (nameProvided) continue;
            if (++i >= argc) break;
            NAME = args[i];
            nameProvided = true;
            continue;
        }
        if (strcmp(args[i], options::V) == 0 || strcmp(args[i], options::VERSION) == 0) {
            if (versionProvided) continue;
            if (++i >= argc) break;
            VERSION = args[i];
            versionProvided = true;
            continue;
        }
        if (strcmp(args[i], options::O) == 0 || strcmp(args[i], options::OUTPUT_DIR) == 0) {
            if (outputProvided) continue;
            if (++i >= argc) break;
            OUTPUT = args[i];
            outputProvided = true;
            continue;
        }
        if (strcmp(args[i], options::S) == 0 || strcmp(args[i], options::SOURCE_DIR) == 0) {
            if (sourceProvided) continue;
            if (++i >= argc) break;
            SOURCE = args[i];
            sourceProvided = true;
            continue;
        }
        if (strcmp(args[i], options::LANGUAGE) == 0) {
            if (languageProvided) continue;
            if (++i >= argc) break;
            if (strcmp(args[i], "c") == 0) languageIsC = true;
            languageProvided = true;
            continue;
        }
        if (strcmp(args[i], options::L) == 0 || strcmp(args[i], options::LIBRARIES) == 0) {
            if (libsProvided) continue;
            if (++i >= argc) break;
            uint32 lcount = 0;
            uint32 j = i - 1;
            while (++j < argc && args[j][0] != '-') lcount++;
            LIBS = (char**)malloc(lcount * sizeof(char*));
            for (j = 0; j < lcount; j++) {
                LIBS[j] = args[j + i];
            }
            libsize = lcount;
            libsProvided = true;
            continue;
        }
        if (strcmp(args[i], options::COPY_RES) == 0) {
            if (copyRes) continue;
            copyRes = true;
            continue;
        }
        if (strcmp(args[i], options::INCL_DIRS) == 0) {
            if (inclDirsProvided) continue;
            if (++i >= argc) break;
            uint32 icount = 0;
            uint32 j = i - 1;
            while (++j < argc && args[j][0] != '-') icount++;
            INCL = (char**)malloc(icount * sizeof(char*));
            for (j = 0; j < icount; j++) {
                INCL[j] = args[j + i];
            }
            incsize = icount;
            inclDirsProvided = true;
            continue;
        }
        if (strcmp(args[i], options::LINK_DIRS) == 0) {
            if (linkDirsProvided) continue;
            if (++i >= argc) break;
            uint32 lcount = 0;
            uint32 j = i - 1;
            while (++j < argc && args[j][0] != '-') lcount++;
            LNK = (char**)malloc(lcount * sizeof(char*));
            for (j = 0; j < lcount; j++) {
                LNK[j] = args[j + i];
            }
            lnksize = lcount;
            linkDirsProvided = true;
            continue;
        }
    }
    return true;
}

void generate() {
    file = std::fstream("CMakeLists.txt", std::ios::out);
    if (versionProvided) file << "cmake_minimum_required(VERSION " << VERSION << ")\n";
    else file << DEFAULT_VERSION;
    if (nameProvided) file << "project(" << NAME << " VERSION 0.1.0 LANGUAGES C" << (languageIsC ? "" : " CXX") << ")\n\n";
    else file << (languageIsC ? DEFAULT_NAME_C : DEFAULT_NAME_CXX);
    if (outputProvided) file << "set (FullOutputDir \"${CMAKE_SOURCE_DIR}/" << OUTPUT << "\")\n";
    else file << DEFAULT_OUTPUT;
    file << DEFAULT_OUT_DIRS;
    if (sourceProvided) {
        file << "set (SrcPath \"${CMAKE_SOURCE_DIR}/" << SOURCE << "/Source Files\")\n";
        file << "set (HeaderPath \"${CMAKE_SOURCE_DIR}/" << SOURCE << "/Header Files\")\n";
        if (copyRes) file << "set (ResourcePath \"${CMAKE_SOURCE_DIR}/" << SOURCE << "/Resource Files\")\n";
        if (languageIsC) file << "file (GLOB_RECURSE Sources CONFIGURE_DEPENDS \"${SrcPath}/*.c\")\n\n";
        else file << "file (GLOB_RECURSE Sources CONFIGURE_DEPENDS \"${SrcPath}/*.cpp\" \"${SrcPath}/*.c\")\n\n";
    }
    else if (copyRes) file << (languageIsC ? DEFAULT_SOURCES_RES_C : DEFAULT_SOURCES_RES_CXX);
    else file << (languageIsC ? DEFAULT_SOURCES_C : DEFAULT_SOURCES_CXX);
    if (inclDirsProvided) {
        file << "include_directories(${HeaderPath} ";
        for (uint32 i = 0; i < incsize; i++) {
            if (INCL[i][0] == '+') file << "$ENV{" << std::string(INCL[i]).substr(1) << "}";
            else file << "\"" << INCL[i] << "\"";
            if (!(i + 1 >= incsize)) file << " ";
        }
        file << ")\n";
    }
    else file << DEFAULT_INCLUDE_DIRS;
    if (linkDirsProvided) {
        file << "link_directories(";
        for (uint32 i = 0; i < lnksize; i++) {
            if (LNK[i][0] == '+') file << "$ENV{" << std::string(LNK[i]).substr(1) << "}";
            else file << "\"" << LNK[i] << "\"";
            if (!(i + 1 >= lnksize)) file << " ";
        }
        file << ")\n";
    }
    if (inclDirsProvided || linkDirsProvided) file << "\n";
    if (libsProvided) {
        file << "link_libraries(";
        for (uint32 i = 0; i < libsize; i++) {
            file << "\"" << LIBS[i] << "\"";
            if (!(i + 1 >= libsize)) file << " ";
        }
        file << ")\n\n";
    }
    file << (languageIsC ? DEFAULT_ADD_EXEC_C : DEFAULT_ADD_EXEC_CXX);
    if (copyRes) file << DEFAULT_COPY_RES;

    if (libsProvided) free(LIBS);
    if (inclDirsProvided) free(INCL);
    if (linkDirsProvided) free(LNK);

    if (sourceProvided) {
        if (!std::filesystem::exists(std::string(SOURCE).c_str())) std::filesystem::create_directory(std::string(SOURCE).c_str());
        if (!std::filesystem::exists(("./" + std::string(SOURCE) + "/Source Files").c_str())) std::filesystem::create_directory(("./" + std::string(SOURCE) + "/Source Files").c_str());
        if (!std::filesystem::exists(("./" + std::string(SOURCE) + "/Header Files").c_str())) std::filesystem::create_directory(("./" + std::string(SOURCE) + "/Header Files").c_str());
        if (copyRes && !std::filesystem::exists(("./" + std::string(SOURCE) + "/Resource Files").c_str())) std::filesystem::create_directory(("./" + std::string(SOURCE) + "/Resource Files").c_str());
        if (languageIsC) {
            if (!std::filesystem::exists(("./" + std::string(SOURCE) + "/Source Files/main.c").c_str())) {
                std::fstream file(("./" + std::string(SOURCE) + "/Source Files/main.c").c_str(), std::ios::out);
                file << MAIN_SNIPPET_C;
            }
        }
        else {
            if (!std::filesystem::exists(("./" + std::string(SOURCE) + "/Source Files/main.cpp").c_str())) {
                std::fstream file(("./" + std::string(SOURCE) + "/Source Files/main.cpp").c_str(), std::ios::out);
                file << MAIN_SNIPPET_CXX;
            }
        }
    }
    else {
        if (!std::filesystem::exists("./src")) std::filesystem::create_directory("./src");
        if (!std::filesystem::exists("./src/Source Files")) std::filesystem::create_directory("./src/Source Files");
        if (!std::filesystem::exists("./src/Header Files")) std::filesystem::create_directory("./src/Header Files");
        if (copyRes && !std::filesystem::exists("./src/Resource Files")) std::filesystem::create_directory("./src/Resource Files");
        if (languageIsC) {
            if (!std::filesystem::exists("./src/Source Files/main.c")) {
                std::fstream file("./src/Source Files/main.c", std::ios::out);
                file << MAIN_SNIPPET_C;
            }
        }
        else {
            if (!std::filesystem::exists("./src/Source Files/main.cpp")) {
                std::fstream file("./src/Source Files/main.cpp", std::ios::out);
                file << MAIN_SNIPPET_CXX;
            }
        }
    }
}

void printHelp() {
    std::cout << HELP;
}

#endif