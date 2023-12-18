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
set(QPC_CFG_KERNEL QV CACHE STRING "set to the kernel to use (QV, QK, QXK) (default: QV)")
set(PROJECT_NAME dpp CACHE STRING "set to the project name of the qpc library project (default: QPC)")
option(QPC_CFG_QSPY       "set to ON, if QSPY monitoring shall be compiled in (default: OFF)" OFF)
option(QPC_QFG_UNIT_TEST  "set to ON, if Q_UTEST shall be enabled (default: OFF)" OFF)
option(QPC_CFG_GUI        "set to ON, if a Windows (TM) GUI shall be compiled in (default: OFF)" ON)

if(NOT SW_VERSION)
    set(SW_VERSION "1.0.0" CACHE STRING "Software Version")
endif()

project(
    ${PROJECT_NAME}
    VERSION ${SW_VERSION}
    DESCRIPTION "Dining Philosophers Problem (dpp) using qpc@win32"
    LANGUAGES C ASM
)

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
add_executable(dpp 
    bsp.c
    main.c
    philo.c
    table.c
)

# set general defines
target_compile_definitions(dpp
	PRIVATE
	    $<$<BOOL:${ADD_DEBUG_CODE}>:${ADD_DEBUG_CODE}>
)

target_compile_options(dpp
    PRIVATE
        $<$<BOOL:${CONFIG_VERBOSE}>:-v>
)

target_link_options(dpp
    PRIVATE
        $<$<BOOL:${CONFIG_VERBOSE}>:-v>
)

if(NOT (DEFINED QPC_DIR OR DEFINED ENV{QPC_DIR}))
    find_path(QPC_DIR
        PATHS "$ENV{HOME}/Projects" "$ENV{HOME}" ENV HOME "/usr/lib" "/opt"
        NAMES "include/qpc.h" "qpc.qm"
        PATH_SUFFIXES "qpc" "Qpc" "QPC"
        REQUIRED
    )
elseif((NOT DEFINED QPC_DIR) AND DEFINED ENV{QPC_DIR})
    set(QPC_DIR $ENV{QPC_DIR})
endif()

if(QPC_DIR)
    set(QPC_PROJECT qpc_${PROJECT_NAME})

    add_subdirectory(${QPC_DIR} ${CMAKE_BINARY_DIR}/qpc)
    target_link_libraries(dpp PRIVATE qpc)
else()
    message(FATAL_ERROR "QPC_DIR not found!")
endif()

# set position independent code compile/link parameters
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    check_pie_supported()
endif()
set_property(TARGET dpp PROPERTY POSITION_INDEPENDENT_CODE FALSE)
set_property(TARGET dpp PROPERTY POSITION_INDEPENDENT_CODE FALSE)

# show configuration results
get_target_property(TGT_COMPILE_OPTS dpp COMPILE_OPTIONS)
get_target_property(TGT_COMPILE_DEFS dpp COMPILE_DEFINITIONS)
get_target_property(TGT_LIB_DEFS dpp INTERFACE_COMPILE_DEFINITIONS)
get_target_property(TGT_INCLUDES dpp INCLUDE_DIRECTORIES)
get_target_property(TGT_LIB_INCS dpp INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(TGT_SYS_INCS dpp INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)
get_target_property(TGT_LINK_OPTS dpp LINK_OPTIONS)
get_target_property(TGT_LINK_LIBS dpp LINK_LIBRARIES)
get_target_property(TGT_SOURCES dpp SOURCES)

get_target_property(LIB_COMPILE_DEFS qpc COMPILE_DEFINITIONS)
get_target_property(LIB_LIB_DEFS qpc INTERFACE_COMPILE_DEFINITIONS)
get_target_property(LIB_INCLUDES qpc INCLUDE_DIRECTORIES)
get_target_property(LIB_LIB_INCS qpc INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(LIB_SYS_INCS qpc INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)

message(STATUS
"Configured project ${PROJECT_NAME} for ${PORT}
 Configuration options set:
    PROJECT_NAME                = ${QPC_PROJECT}
    SW_VERSION                  = ${SW_VERSION}
    
    CONFIG_GUI                  = ${QPC_CFG_GUI}
    CONFIG_QSPY                 = ${QPC_CFG_QSPY}
    CONFIG_UNIT_TEST            = ${QPC_CFG_UNIT_TEST}
    CONFIG_DEBUG                = ${CONFIG_DEBUG}

    QPC_CFG_KERNEL              = ${QPC_CFG_KERNEL}
    QPLIB                       = qpc

 System information:
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

    CMAKE_C_COMPILER            = ${CMAKE_C_COMPILER}
    CMAKE_C_COMPILER_ID         = ${CMAKE_C_COMPILER_ID}
    CMAKE_C_COMPILER_VERSION    = ${CMAKE_C_COMPILER_VERSION}
    CMAKE_C_FLAGS               = ${CMAKE_C_FLAGS}

    CMAKE_CXX_COMPILER          = ${CMAKE_CXX_COMPILER}
    CMAKE_CXX_FLAGS             = ${CMAKE_CXX_FLAGS}

    CMAKE_ASM_COMPILER          = ${CMAKE_ASM_COMPILER}
    CMAKE_ASM_FLAGS             = ${CMAKE_ASM_FLAGS}"
)
