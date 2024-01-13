# USC2 QM project root CMakeLists.txt
macro(init_port_list OUT)
    list(APPEND ${OUT} arm-cm arm-cr embos esp-idf)
    list(APPEND ${OUT} freertos msp430 pic32 posix)
    list(APPEND ${OUT} qep-only qube risc-v threadx)
    list(APPEND ${OUT} uc-os2 win32)
endmacro()

if (DEFINED ENV{QPC_CFG_KERNEL} AND (NOT QPC_CFG_KERNEL))
    set(QPC_CFG_KERNEL $ENV{QPC_CFG_KERNEL})
    message("Using QPC_CFG_KERNEL from environment ('${QPC_CFG_KERNEL}')")
endif ()

if (DEFINED ENV{QPC_CFG_PORT} AND (NOT QPC_CFG_PORT))
    set(QPC_CFG_PORT $ENV{QPC_CFG_PORT})
    message("Using QPC_CFG_PORT from environment ('${QPC_CFG_PORT}')")
endif ()

project(
    qpc
    VERSION "1.0.0"
    DESCRIPTION "QPC library"
    LANGUAGES C ASM
)

if (DEFINED ENV{QPC_CFG_KERNEL} AND (NOT QPC_CFG_KERNEL))
    set(QPC_CFG_KERNEL $ENV{QPC_CFG_KERNEL})
    message("Using QPC_CFG_KERNEL from environment ('${QPC_CFG_KERNEL}')")
elseif(NOT QPC_CFG_KERNEL)
    set(QPC_CFG_KERNEL qv)
    message("Set QPC_CFG_KERNEL to ('${QPC_CFG_KERNEL}') since not specified")
endif ()

if (DEFINED ENV{QPC_CFG_PORT} AND (NOT QPC_CFG_PORT))
    set(QPC_CFG_PORT $ENV{QPC_CFG_PORT})
    message("Using QPC_CFG_PORT from environment ('${QPC_CFG_PORT}')")
endif ()

if(NOT QPC_CFG_GUI)
    set(QPC_CFG_GUI OFF CACHE BOOL "enable GUI support for matching ports (e.g. win32 or posix & GTK)")
    message("Set QPC_CFG_GUI to ('${QPC_CFG_GUI}') since not specified")
endif()

if(NOT QPC_CFG_UNIT_TEST)
    set(QPC_CFG_UNIT_TEST OFF CACHE BOOL "enable detailled unit testing support")
    message("Set QPC_CFG_UNIT_TEST to ('${QPC_CFG_UNIT_TEST}') since not specified")
endif()

if(NOT QPC_CFG_DEBUG)
    set(QPC_CFG_DEBUG ON CACHE BOOL "enable debug sessions")
    message("Set QPC_CFG_DEBUG to ('${QPC_CFG_DEBUG}') since not specified")
endif()

if(NOT QPC_CFG_VERBOSE)
    set(QPC_CFG_VERBOSE OFF CACHE BOOL "enable verbose build output")
    message("Set QPC_CFG_VERBOSE to ('${QPC_CFG_VERBOSE}') since not specified")
endif()

# check target port plausibility
if(NOT QPC_CFG_PORT)
    message(WARNING "No PORT is configured! Falling back to target system '${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}'.")
    if(CMAKE_SYSTEM_NAME)
        string(TOLOWER ${CMAKE_SYSTEM_NAME} sysName)
    else()
        set(sysName generic)
    endif()
    if(CMAKE_SYSTEM_PROCESSOR)
        string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} sysProc)
    else()
        set(sysproc none)
    endif()
    if((sysName STREQUAL generic) AND (sysProc STREQUAL arm))
        set(PORT arm-cm)
    elseif(WIN32)
        set(PORT win32)
    else()
        set(PORT posix)
    endif()
else()
    string(TOLOWER ${QPC_CFG_PORT} PORT)
endif()

init_port_list(PORTLIST)
if(NOT (${PORT} IN_LIST PORTLIST))
    if(DEFINED PORT)
        set(MSG "Target port '${PORT}' not found!")
    else()
        set(MSG "Target port not defined!")
    endif()
	message(FATAL_ERROR ${MSG})
    unset(MSG)
endif()
set(QPC_CFG_PORT ${PORT} CACHE STRING "The QPC target port for the system platform")
message("Set QPC_CFG_PORT to ('${QPC_CFG_PORT}')")

# check/set Qx real time kernel
if(NOT (QPC_CFG_KERNEL MATCHES "[Qq]([VvKk]|[Xx][Kk])"))
    message(WARNING "No KERNEL (QV, QK or QXK) is configured!"
                    "Falling back to QV kernel.")
    set(QPC_CFG_KERNEL QV CACHE STRING "set to ON, if the QV micro kernel shall be configured. Leave OFF for host based configurations. (default: OFF)" FORCE)
endif()
string(TOLOWER ${QPC_CFG_KERNEL} KERNEL)

add_library(qpc STATIC "")

# set position independent code compile/link parameters
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    include(CheckPIESupported)
    check_pie_supported()
endif()
set_property(TARGET qpc PROPERTY POSITION_INDEPENDENT_CODE FALSE)

# add subdirectories with source/header files

target_include_directories(qpc PUBLIC src include)
add_subdirectory(src)
add_subdirectory(ports)

# set general defines
target_compile_definitions(qpc
	PRIVATE
	    $<$<BOOL:${ADD_DEBUG_CODE}>:${ADD_DEBUG_CODE}>
        $<$<BOOL:${QPC_CFG_GUI}>:QWIN_GUI>
        $<$<CONFIG:Spy>:Q_SPY>
        $<$<AND:$<CONFIG:Spy>,$<BOOL:${QPC_CFG_UNIT_TEST}>>:Q_UTEST>
)

target_compile_options(qpc
    PRIVATE
        $<$<BOOL:${QPC_CFG_VERBOSE}>:-v>
)

target_link_options(qpc
    PRIVATE
        $<$<BOOL:${QPC_CFG_VERBOSE}>:-v>
)

target_link_libraries(qpc
    PUBLIC
        $<$<AND:$<CONFIG:Spy>,$<STREQUAL:win32,${PORT}>>:ws2_32>
)

# set position independent code compile/link parameters
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    include(CheckPIESupported)
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
    QPC_QFG_UNIT_TEST           = ${QPC_QFG_UNIT_TEST}
    QPC_CFG_KERNEL              = ${QPC_CFG_KERNEL}
    QPC_CFG_DEBUG               = ${QPC_CFG_DEBUG}

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

    CMAKE_BUILD_TYPE            = ${CMAKE_BUILD_TYPE}
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
