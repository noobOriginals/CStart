#ifndef GENERATOR_H
#define GENERATOR_H

// Std includes
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

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
bool copyRes = true;
bool inclDirsProvided = false;
bool linkDirsProvided = false;


bool init(uint32_t argc, char** args) {
    for (uint32_t i = 0; i < argc; i++) {
        if (strcmp(args[i], "-n") == 0) {
            if (nameProvided) continue;
            if (++i >= argc) break;
            NAME = args[i];
            nameProvided = true;
            continue;
        }
        if (strcmp(args[i], "-v") == 0) {
            if (versionProvided) continue;
            if (++i >= argc) break;
            VERSION = args[i];
            versionProvided = true;
            continue;
        }
        if (strcmp(args[i], "-o") == 0) {
            if (outputProvided) continue;
            if (++i >= argc) break;
            OUTPUT = args[i];
            outputProvided = true;
            continue;
        }
        if (strcmp(args[i], "-s") == 0) {
            if (sourceProvided) continue;
            if (++i >= argc) break;
            SOURCE = args[i];
            sourceProvided = true;
            continue;
        }
        if (strcmp(args[i], "-l") == 0) {
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
        if (strcmp(args[i], "--ncopy-res") == 0) {
            if (!copyRes) continue;
            copyRes = false;
            continue;
        }
        if (strcmp(args[i], "--incl-dirs") == 0) {
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
        if (strcmp(args[i], "--link-dirs") == 0) {
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
        file << "set (ResourcePath \"${CMAKE_SOURCE_DIR}/" << SOURCE << "/Resource Files\")\n";
        file << "file (GLOB Sources CONFIGURE_DEPENDS \"${SrcPath}/*.cpp\" \"${SrcPath}/*.c\" \"${HeaderPath}/*.hpp\" \"${HeaderPath}/*.h\")\n\n";
    } else file << DEFAULT_SOURCES;
    if (inclDirsProvided) {
        file << "include_directories(${HeaderPath} ${SrcPath} ";
        for (uint32_t i = 0; i < incsize; i++) {
            if (INCL[i][0] == '+') file << "$ENV{" << std::string(INCL[i]).substr(1) << "}";
            else file << "\"" << INCL[i] << "\"";
            if (!(i + 1 >= incsize)) file << " ";
        }
        file << ")\n";
    } else file << DEFAULT_INCLUDE_DIRS;
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
    std::cout << "CStart Help Page:\n";
}

#endif