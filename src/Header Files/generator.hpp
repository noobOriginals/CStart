#ifndef GENERATOR_H
#define GENERATOR_H

// Std includes
#include <iostream>
#include <fstream>
#include <string>

constexpr char* HELP = 
"ctart help page:\n\n"
"Options:\n"
"    -n -p --name --project -> Set the project name inside CMake (also the executable name)\n"
"    -v --version -> Set the required CMake version\n"
"    -o --output-dir -> The folder for the build output\n"
"    -s --source-dir -> The folder for the sources (it includes the 'Source Files' and 'Header Files' directories)\n"
"    -l --libraries -> The name of the libraries that have to be linked (e.g. from 'vulkan-1.exe' you use 'vulkan-1')\n"
"    --copy-res -> Enables the use of a 'Resource Files' folder inside the src folder and copies the resources to the bin output\n"
"                  folder before compilation for accesing them in your program.\n"
"    --incl-dirs -> Specify additional include directories beside the default header and src files\n"
"    --link-dirs -> Specify link directories for the libraries to be linked from.\n\n"
"!!! ALL options will default to a value if ommited:\n"
"";

std::fstream file;

constexpr char* DEFAULT_VERSION = "cmake_minimum_required(VERSION 3.26)\n";
constexpr char* DEFAULT_NAME = "project(\"...\" VERSION 0.1.0 LANGUAGES C CXX)\n\n";
constexpr char* DEFAULT_OUTPUT = "set (FullOutputDir \"${CMAKE_SOURCE_DIR}/bin\")\n";
constexpr char* DEFAULT_OUT_DIRS = 
    "set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY \"${FullOutputDir}/Static Libraries\")\n"
    "set (CMAKE_LIBRARY_OUTPUT_DIRECTORY \"${FullOutputDir}\")\n"
    "set (CMAKE_RUNTIME_OUTPUT_DIRECTORY \"${FullOutputDir}\")\n\n";
constexpr char* DEFAULT_SOURCES = 
    "set (SrcPath \"${CMAKE_SOURCE_DIR}/src/Source Files\")\n"
    "set (HeaderPath \"${CMAKE_SOURCE_DIR}/src/Header Files\")\n"
    "file (GLOB Sources CONFIGURE_DEPENDS \"${SrcPath}/*.cpp\" \"${SrcPath}/*.c\" \"${HeaderPath}/*.hpp\" \"${HeaderPath}/*.h\")\n\n";
constexpr char* DEFAULT_SOURCES_RES = 
    "set (SrcPath \"${CMAKE_SOURCE_DIR}/src/Source Files\")\n"
    "set (HeaderPath \"${CMAKE_SOURCE_DIR}/src/Header Files\")\n"
    "set (ResourcePath \"${CMAKE_SOURCE_DIR}/src/Resource Files\")\n"
    "file (GLOB Sources CONFIGURE_DEPENDS \"${SrcPath}/*.cpp\" \"${SrcPath}/*.c\" \"${HeaderPath}/*.hpp\" \"${HeaderPath}/*.h\")\n\n";
constexpr char* DEFAULT_INCLUDE_DIRS = "include_directories(${HeaderPath} ${SrcPath})\n\n";
constexpr char* DEFAULT_ADD_EXEC = 
    "add_executable(${PROJECT_NAME} ${Sources})\n"
    "target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)\n\n";
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

const char* NAME;
const char* VERSION;
const char* OUTPUT;
const char* SOURCE;

char** LIBS;
uint32_t libsize;
char** INCL;
uint32_t incsize;
char** LNK;
uint32_t lnksize;

bool nameProvided = false;
bool versionProvided = false;
bool outputProvided = false;
bool sourceProvided = false;
bool libsProvided = false;
bool copyRes = false;
bool inclDirsProvided = false;
bool linkDirsProvided = false;

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
}

bool init(uint32_t argc, char** args) {
    for (uint32_t i = 0; i < argc; i++) {
        if (strcmp(args[i], options::H) == 0 || strcmp(args[i], options::HELP) == 0) return false;
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
        if (strcmp(args[i], options::L) == 0 || strcmp(args[i], options::LIBRARIES) == 0) {
            if (libsProvided) continue;
            if (++i >= argc) break;
            uint32_t lcount = 0;
            uint32_t j = i - 1;
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
            uint32_t icount = 0;
            uint32_t j = i - 1;
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
            uint32_t lcount = 0;
            uint32_t j = i - 1;
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
    if (nameProvided) file << "project(" << NAME << " VERSION 0.1.0 LANGUAGES C CXX)\n\n";
    else file << DEFAULT_NAME;
    if (outputProvided) file << "set (FullOutputDir \"${CMAKE_SOURCE_DIR}/" << OUTPUT << "\")\n";
    else file << DEFAULT_OUTPUT;
    file << DEFAULT_OUT_DIRS;
    if (sourceProvided) {
        file << "set (SrcPath \"${CMAKE_SOURCE_DIR}/" << SOURCE << "/Source Files\")\n";
        file << "set (HeaderPath \"${CMAKE_SOURCE_DIR}/" << SOURCE << "/Header Files\")\n";
        if (copyRes) file << "set (ResourcePath \"${CMAKE_SOURCE_DIR}/" << SOURCE << "/Resource Files\")\n";
        file << "file (GLOB Sources CONFIGURE_DEPENDS \"${SrcPath}/*.cpp\" \"${SrcPath}/*.c\" \"${HeaderPath}/*.hpp\" \"${HeaderPath}/*.h\")\n\n";
    }
    else if (copyRes) file << DEFAULT_SOURCES_RES;
    else file << DEFAULT_SOURCES;
    if (inclDirsProvided) {
        file << "include_directories(${HeaderPath} ${SrcPath} ";
        for (uint32_t i = 0; i < incsize; i++) {
            if (INCL[i][0] == '+') file << "$ENV{" << std::string(INCL[i]).substr(1) << "}";
            else file << "\"" << INCL[i] << "\"";
            if (!(i + 1 >= incsize)) file << " ";
        }
        file << ")\n";
    }
    else file << DEFAULT_INCLUDE_DIRS;
    if (linkDirsProvided) {
        file << "link_directories(";
        for (uint32_t i = 0; i < lnksize; i++) {
            if (LNK[i][0] == '+') file << "$ENV{" << std::string(LNK[i]).substr(1) << "}";
            else file << "\"" << LNK[i] << "\"";
            if (!(i + 1 >= lnksize)) file << " ";
        }
        file << ")\n";
    }
    if (inclDirsProvided || linkDirsProvided) file << "\n";
    if (libsProvided) {
        file << "link_libraries(";
        for (uint32_t i = 0; i < libsize; i++) {
            file << "\"" << LIBS[i] << "\"";
            if (!(i + 1 >= libsize)) file << " ";
        }
        file << ")\n\n";
    }
    file << DEFAULT_ADD_EXEC;
    if (copyRes) file << DEFAULT_COPY_RES;
}

void printHelp() {
    std::cout << "CStart Help Page:\n\n";
}

#endif