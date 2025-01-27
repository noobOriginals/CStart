cmake_minimum_required(VERSION 3.20)
project(CStart VERSION 0.1.0 LANGUAGES C CXX)

set (FullOutputDir "${CMAKE_SOURCE_DIR}/bin")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FullOutputDir}/Static Lbraries")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FullOutputDir}")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullOutputDir}")

set (SrcPath "${CMAKE_SOURCE_DIR}/src/Source Files")
set (HeaderPath "${CMAKE_SOURCE_DIR}/src/Header Files")
set (ResourcePath "${CMAKE_SOURCE_DIR}/src/Resource Files")
file(GLOB Sources CONFIGURE_DEPENDS "${SrcPath}/*.cpp" "${SrcPath}/*.c" "${HeaderPath}/*.hpp" "${HeaderPath}/*.h")

include_directories(${HeaderPath} ${SrcPath})

add_executable("${PROJECT_NAME}" ${Sources})
target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_17)

add_custom_target(
    CopyResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ResourcePath}
    ${FullOutputDir}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ResourcePath}
    ${FullOutputDir}/Release
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${ResourcePath}
	${FullOutputDir}
    COMMENT "Copying resources to binary output folder"
)
add_dependencies("${PROJECT_NAME}" CopyResources)

