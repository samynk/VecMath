# minimum required CMAKE version

CMAKE_MINIMUM_REQUIRED(VERSION 3.16 FATAL_ERROR)
project(VecMathLexer)
if (WIN32)
    cmake_policy(SET CMP0114 NEW)
endif (WIN32)

set( ANTLR4_TAG "4.10.1")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be 11 or 14
set(CMAKE_CXX_STANDARD 17)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/antlr/antlr-4.10.1-complete.jar)
# add macros to generate ANTLR Cpp code from grammar
find_library(UUID_LIBRARIES libuuid.so.1)
find_package(ANTLR REQUIRED)

set(VECMATH_COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../VecMathCommon")
set(VECMATH_COMMON_GRAMMAR_DIR "${VECMATH_COMMON_DIR}/antlr4")
set(VECMATH_COMMON_RESOURCE_DIR "${VECMATH_COMMON_DIR}/resources")

if ( EMSCRIPTEN )
	set(CMAKE_CXX_FLAGS "-g -fPIC")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

add_subdirectory(vecmathlib)
add_subdirectory(vecmathmain)
add_subdirectory(vecmathhtml)


add_dependencies(VecMathMain VecMathLib )
add_dependencies(VecMathHtml VecMathLib )

target_include_directories (VecMathMain PUBLIC 
	${VecMathLibIncludeDir}
)
if (NOT EMSCRIPTEN)
	add_subdirectory(unittests)
	target_include_directories (UnitTests PUBLIC 
		${VecMathLibIncludeDir}
	)
	add_dependencies(VecMathMain UnitTestsRun)

endif()


include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/resources/License.txt")
set(CPACK_PACKAGE_VENDOR "Digital Arts and Entertainment")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "7")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "VecMath")
#set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
install(TARGETS VecMathMain RUNTIME DESTINATION bin RENAME VecMath )
set(CPACK_CREATE_DESKTOP_LINKS "VecMathMain" "VecMath REPL shell")
set(CPACK_PACKAGE_EXECUTABLES  "VecMathMain" "VecMath REPL shell")
include(CPack)

