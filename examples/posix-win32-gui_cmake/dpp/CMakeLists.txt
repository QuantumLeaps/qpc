# dpp

# use a recent CMake version
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
cmake_policy(VERSION 3.13)
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    cmake_policy(SET CMP0083 NEW)
endif()

# first of all protect against in-source builds
get_filename_component(_srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(_bindir "${CMAKE_BINARY_DIR}" REALPATH)

if(${_srcdir} STREQUAL ${_bindir})
    message(FATAL_ERROR "  FATAL: In-source builds are not allowed!
        You should create a separate directory for build files.")
else()
    unset(_srcdir)
    unset(_bindir)
endif()

# set up configurable options
set(PROJECT_NAME dpp CACHE STRING "set to the project name of the qpc library project (default: QPC)")
set(QPC_CFG_KERNEL QV CACHE STRING "set to the kernel to use (QV, QK, QXK) (default: QV)")
set(QPC_SDK_PATH "${CMAKE_SOURCE_DIR}/../../.." CACHE STRING "set to the source path of QPC (default: ../../..)")

option(QPC_QFG_UNIT_TEST  "set to ON, if Q_UTEST shall be enabled (default: OFF)" OFF)
option(QPC_CFG_GUI        "set to ON, if a Windows (TM) GUI shall be compiled in (default: ON)" OFF)
option(QPC_CFG_VERBOSE   "set to ON, to enable more verbosity. (default: OFF)" OFF)
option(QPC_CFG_DEBUG     "set to ON, to enable debug settings. (default: ON)" ON)

# update CMAKE_MODULE_PATH
list(PREPEND CMAKE_MODULE_PATH ${QPC_SDK_PATH}/3rd_party/cmake ${CMAKE_SOURCE_DIR}/Source/cmake ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_SOURCE_DIR} ${HOME_PATH}/cmake)

# import QPC SDK; download from git, if necessary
include(qpc_sdk_import)

if(NOT SW_VERSION)
    set(SW_VERSION "1.0.0" CACHE STRING "Software Version")
endif()

project(
    ${PROJECT_NAME}
    VERSION ${SW_VERSION}
    DESCRIPTION "Dining Philosophers Problem (dpp) using qpc@win32"
    LANGUAGES C
)

if(QPC_CFG_GUI AND (CMAKE_SYSTEM_NAME STREQUAL Windows OR WIN32))
    enable_language(RC)
endif()

# update the module search path
if(CMAKE_HOST_WIN32)
    string(REPLACE "\\" "/" HOME_PATH "$ENV{HOMEDRIVE}$ENV{HOMEPATH}")
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${HOME_PATH}/cmake;${CMAKE_MODULE_PATH}")
else()
    set(CMAKE_MODULE_PATH $ENV{HOME}/cmake;${PROJECT_SOURCE_DIR}/cmake;${PROJECT_SOURCE_DIR};${CMAKE_MODULE_PATH})
endif()

include(CTest)
include(CheckIncludeFile)
include(CheckLibraryExists)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    include(CheckPIESupported)
endif()

# the project target
add_executable(dpp main.c philo.c table.c)
if(QPC_CFG_GUI)
    if(USE_GTK OR CMAKE_SYSTEM_NAME STREQUAL Linux OR UNIX)
        # use GTK3+
        target_sources(dpp PRIVATE bsp-gtk.c)
    elseif(CMAKE_SYSTEM_NAME STREQUAL Windows OR WIN32)
        # use Win32 GUI
        target_sources(dpp PRIVATE bsp-gui.c Resource.rc)
    else()
        message(FATAL_ERROR "no GUI support available for this build!")
    endif()
else()
    target_sources(dpp PRIVATE bsp.c)
endif()

# set position independent code compile/link parameters
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    include(CheckPIESupported)
    check_pie_supported()
    set_property(TARGET dpp PROPERTY POSITION_INDEPENDENT_CODE FALSE)
endif()

# add the qpc library to the project
qpc_sdk_init()

# add GTK+-3.0 support
if(QPC_CFG_GUI AND (USE_GTK OR (DEFINED QPC_CFG PORT AND QPC_CFG_PORT MATCHES posix) OR (CMAKE_SYSTEM_NAME STREQUAL Linux OR UNIX)))
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 gtk+-3.0) # QUIET IMPORTED_TARGET)

    if (NOT GTK3_FOUND)
        message(FATAL_ERROR "The package GTK3, required for this build, could not be found on the system!")
    endif()

    # Add other flags to the compiler
    target_compile_options(dpp PRIVATE ${GTK3_CFLAGS_OTHER})

    # Setup CMake to use GTK+, tell the compiler where to look for headers
    # and to the linker where to look for libraries
    target_include_directories(dpp PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_directories(dpp PRIVATE ${GTK3_LIBRARY_DIRS})
    target_link_options(dpp PRIVATE ${GTK3_LDFLAGS_OTHER})

    # Link the target to the GTK+ libraries
    target_link_libraries(dpp PRIVATE ${GTK3_LIBRARIES}) # PkgConfig::GTK3)

    find_program(GLIB_COMPILE_RESOURCES
       NAMES glib-compile-resources glib-compile-resources.exe
    )
    if(NOT GLIB_COMPILE_RESOURCES)
        message(FATAL_ERROR "Program 'glib-compile-resources' cannot be found. Aborting!")
    endif()
    add_custom_command(
        OUTPUT dppImages.c dppImages.h
        MAIN_DEPENDENCY dppImages.xml
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/dppImages.xml ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND  ${GLIB_COMPILE_RESOURCES}
                 --generate-source
                 --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}/Res
                 --internal
                 --c-name=dppImg
                 --compiler=gcc
                 dppImages.xml
        COMMAND  ${GLIB_COMPILE_RESOURCES}
                 --generate-header
                 --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}/Res
                 --internal
                 --c-name=dppImg
                 --compiler=gcc
                 dppImages.xml
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Create GTK+3 Resources"
        VERBATIM
    )
    target_sources(dpp PRIVATE dppImages.c)
    target_include_directories(dpp PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
endif()

# set general defines and options
target_compile_definitions(dpp
	PRIVATE
        $<$<CONFIG:Spy>:Q_SPY>
        $<$<BOOL:${QPC_CFG_GUI}>:QWIN_GUI>
)

target_compile_options(dpp
    PRIVATE
        $<$<BOOL:${QPC_CFG_VERBOSE}>:-v>
)

target_link_options(dpp
    PRIVATE
        $<$<BOOL:${QPC_CFG_VERBOSE}>:-v>
        LINKER:-Map=${CMAKE_PROJECT_NAME}.map
)

target_link_libraries(dpp
    PRIVATE
        qpc
)

if(CMAKE_SYSTEM_NAME STREQUAL Windows OR WIN32)
    target_compile_options(dpp
        PRIVATE
            $<IF:$<BOOL:${QPC_CFG_GUI}>,-mwindows,-mconsole>
    )

    target_link_options(dpp
        PRIVATE
            $<$<C_COMPILER_ID:GNU>:$<IF:$<BOOL:${QPC_CFG_GUI}>,-mwindows,-mconsole>>
    )
endif()

# link the library with the application
# show configuration results
message(STATUS
"Configured project ${PROJECT_NAME} for ${PORT}
 Configuration options set:
    PROJECT_NAME                = ${QPC_PROJECT}
    SW_VERSION                  = ${SW_VERSION}
    
    QPC_CFG_GUI                 = ${QPC_CFG_GUI}
    QPC_CFG_QSPY                = ${QPC_CFG_QSPY}
    QPC_CFG_UNIT_TEST           = ${QPC_CFG_UNIT_TEST}
    QPC_CFG_DEBUG               = ${QPC_CFG_DEBUG}

    QPC_CFG_KERNEL              = ${QPC_CFG_KERNEL}
    QPC_DIR                     = ${QPC_DIR}"
)

if(QPC_CFG_VERBOSE)
message(STATUS
" System information:
    CMAKE_VERSION               = ${CMAKE_VERSION}
    CMAKE_CROSSCOMPILING        = ${CMAKE_CROSSCOMPILING}
    CMAKE_HOST_SYSTEM           = ${CMAKE_HOST_SYSTEM}
    CMAKE_HOST_SYSTEM_NAME      = ${CMAKE_HOST_SYSTEM_NAME}
    CMAKE_HOST_LINUX            = ${CMAKE_HOST_LINUX}
    CMAKE_HOST_UNIX             = ${CMAKE_HOST_UNIX}
    CMAKE_HOST_WIN32            = ${CMAKE_HOST_WIN32}
    CMAKE_SYSTEM                = ${CMAKE_SYSTEM}
    CMAKE_SYSTEM_NAME           = ${CMAKE_SYSTEM_NAME}
    CMAKE_SYSTEM_PROCESSOR      = ${CMAKE_SYSTEM_PROCESSOR}
    WIN32                       = ${WIN32}
    MSYS                        = ${MSYS}
    MINGW                       = ${MINGW}
    UNIX                        = ${UNIX}
    LINUX                       = ${LINUX}

    CMAKE_BUILD_TYPE            = ${CMAKE_BUILD_TYPE}
    CMAKE_CONFIGURATION_TYPES   = ${CMAKE_CONFIGURATION_TYPES}

    CMAKE_C_COMPILER            = ${CMAKE_C_COMPILER}
    CMAKE_C_COMPILER_ID         = ${CMAKE_C_COMPILER_ID}
    CMAKE_C_COMPILER_VERSION    = ${CMAKE_C_COMPILER_VERSION}
    CMAKE_C_FLAGS               = ${CMAKE_C_FLAGS}

    CMAKE_CXX_COMPILER          = ${CMAKE_CXX_COMPILER}
    CMAKE_CXX_FLAGS             = ${CMAKE_CXX_FLAGS}

    CMAKE_ASM_COMPILER          = ${CMAKE_ASM_COMPILER}
    CMAKE_ASM_FLAGS             = ${CMAKE_ASM_FLAGS}"
)
endif()
