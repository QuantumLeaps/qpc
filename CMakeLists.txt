# USC2 QM project root CMakeLists.txt

if(NOT DEFINED QPC_PROJECT)
    # these settings are applied to a stand-alone configuration of the QPC lib
    # if qpc shall be integrated into the wider scope of an application project, set QPC_PROJECT to non-false value.
    # The QPC configuration, settings to ensure out-of-source builds, setting the project specific module search path
    # or inclusion of needed modules (e.g. CTest, ...) should be done by the main projects's CMakeLists.txt

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
    set(QPC_PROJECT QPC CACHE STRING "set to the project name of the qpc library project (default: QPC)")
    option(QPC_CFG_QSPY      "set to ON, if QSPY monitoring shall be compiled in (default: OFF)" OFF)
    option(QPC_QFG_UNIT_TEST "set to ON, if Q_UTEST shall be enabled (default: OFF)" OFF)
    option(QPC_CFG_GUI       "set to ON, if a Windows (TM) GUI shall be compiled in (default: OFF)" OFF)
    option(QPC_CFG_VERBOSE   "set to ON, to enable more verbosity. (default: OFF)" OFF)

    project(
        ${QPC_PROJECT}
        VERSION "1.0.0"
        DESCRIPTION "QPC library"
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
endif()

# check target port plausibility
if(NOT PORT)
    message(WARNING "No PORT is configured! Falling back to target system '${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}'.")
    if((CMAKE_SYSTEM_NAME STREQUAL Generic) AND (CMAKE_SYSTEM_PROCESSOR STREQUAL Arm))
        set(PORT arm-cm)
    elseif(WIN32)
        set(PORT win32)
    else()
        set(PORT posix)
    endif()
else()
    string(TOLOWER ${PORT} PORT)
endif()

list(APPEND PORTLIST arm-cm arm-cr embos esp-idf)
list(APPEND PORTLIST freertos msp430 pic32 posix)
list(APPEND PORTLIST qep-only qube riscv threadx)
list(APPEND PORTLIST uc-os2 win32)
if(NOT (${PORT} IN_LIST PORTLIST))
    if(DEFINED PORT)
        set(MSG "Target port '${PORT}' not found!")
    else()
        set(MSG "Target port not defined!")
    endif()
	message(FATAL_ERROR ${MSG})
    unset(MSG)
endif()
if(PORT STREQUAL win32)
    set(QPC_CFG_GUI TRUE)
endif()


# check/set Qx real time kernel
if(NOT (QPC_CFG_KERNEL MATCHES "[Qq]([VvKk]|[Xx][Kk])"))
    message(WARNING "No KERNEL (QV, QK or QXK) is configured!"
                    "Falling back to QV kernel.")
    set(QPC_CFG_KERNEL QV CACHE STRING "set to ON, if the QV micro kernel shall be configured. Leave OFF for host based configurations. (default: OFF)" FORCE)
endif()
string(TOLOWER ${QPC_CFG_KERNEL} KERNEL)

if(CONFIG_DEBUG)
    set(CMAKE_BUILD_TYPE "Debug")
else()
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(QPC_QFG_UNIT_TEST)
	set(QPC_CFG_QSPY ON CACHE BOOL "set to ON, if QSPY monitoring shall be compiled in (default: OFF)" FORCE)
endif()

add_library(qpc STATIC "")

# set general defines
target_compile_definitions(qpc
	PRIVATE
	    $<$<BOOL:${ADD_DEBUG_CODE}>:${ADD_DEBUG_CODE}>
        $<$<BOOL:${QPC_CFG_GUI}>:QWIN_GUI>
        $<$<BOOL:${QPC_CFG_QSPY}>:Q_SPY>
        $<$<BOOL:${QPC_CFG_UNIT_TEST}>:Q_UTEST>
)

target_compile_options(qpc
    PRIVATE
        $<$<BOOL:${CONFIG_VERBOSE}>:-v>
)

target_link_options(qpc
    PRIVATE
        $<$<BOOL:${CONFIG_VERBOSE}>:-v>
)

# set position independent code compile/link parameters
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    check_pie_supported()
endif()
set_property(TARGET qpc PROPERTY POSITION_INDEPENDENT_CODE FALSE)

# add subdirectories with source/header files

target_include_directories(qpc PUBLIC src include)
add_subdirectory(src)
add_subdirectory(ports)

# print configuration
message(STATUS
"========================================================
  Configured project ${PROJECT_NAME} for ${PORT}
    PROJECT_NAME                = ${QPC_PROJECT}
    TARGET                      = ${TARGET}
    IMAGE                       = ${IMAGE}
    SW_VERSION                  = ${SW_VERSION}
    PORT                        = ${PORT}
-- ========================================================
"
)

if(QPC_CFG_VERBOSE)
    message(STATUS
"========================================================
  Configuration Options set:
    message(STATUS
    QPC_CFG_GUI                 = ${QPC_CFG_GUI}
    QPC_CFG_QSPY                = ${QPC_CFG_QSPY}
    QPC_QFG_UNIT_TEST           = ${QPC_QFG_UNIT_TEST}
    QPC_CFG_KERNEL              = ${QPC_CFG_KERNEL}
    CONFIG_DEBUG                = ${CONFIG_DEBUG}

    CMAKE_C_CPPCHECK            = ${CMAKE_C_CPPCHECK}

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
    CMAKE_ASM_FLAGS             = ${CMAKE_ASM_FLAGS}
-- ========================================================
"
    )
endif()