#============================================================================
# QP/C Real-Time Event Framework (RTEF) -- CMake support
#
# QP/C is built in-tree, as part of the application, so that the library
# is always compiled with the application's own qp_config.h.
#
# Usage in the application's CMakeLists.txt:
#
#   # where is qp_config.h (and, for embedded targets, the CPU flags)
#   add_library(qpc_config INTERFACE)
#   target_include_directories(qpc_config INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
#
#   set(QPC_PORT win32)          # required, see the list below
#   set(QPC_KERNEL qv)           # arm-cm only: qv (default), qk, qxk
#   add_subdirectory(<path-to-qpc> qpc)
#
#   target_link_libraries(<app> PRIVATE qpc::qpc)
#
#   link_image(<app> <linker-script>)   # bare-metal: .elf, .map, .bin
#                                       # (3rd_party/cmake, via the board)
#
# Supported ports: win32, win32-qv, posix, posix-qv, arm-cm, freertos
#
# Configurations: in the "Spy" configuration, QP/Spy software tracing is
# built into qpc and Q_SPY is defined for qpc and for the application.
#
# CMake package (optional, OFF by default, host ports only):
# qpc can be built and installed as a CMake package for one qp_config.h,
# which is installed with the package. For example:
#
#   cmake -S <path-to-qpc> -B build -G "Ninja Multi-Config"
#         -DCMAKE_CONFIGURATION_TYPES="Debug;Release;Spy"
#         -DQPC_PORT=posix -DQPC_CONFIG_DIR=<dir-of-qp_config.h>
#         -DQPC_INSTALL=ON                     (all in one command line)
#   cmake --build build --config Release
#   cmake --install build --config Release --prefix <install-dir>
#
# The application then uses:
#
#   find_package(qpc 8.1.6 REQUIRED)    # CMAKE_PREFIX_PATH=<install-dir>
#   target_link_libraries(<app> PRIVATE qpc::qpc)
#
# An application compiled with a different QP configuration than the
# installed qpc library fails to link (QP configuration check, see
# include/qp_cfgchk.h).
#============================================================================
cmake_minimum_required(VERSION 3.25)
project(qpc LANGUAGES C)

set(QPC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

option(QPC_INSTALL "qpc: install qpc as a CMake package (host ports only)"
    OFF)

# check what the application must provide ...................................
if(NOT TARGET qpc_config AND QPC_CONFIG_DIR) # qp_config.h given as directory
    cmake_path(ABSOLUTE_PATH QPC_CONFIG_DIR BASE_DIRECTORY ${CMAKE_SOURCE_DIR}
        NORMALIZE OUTPUT_VARIABLE _cfg_dir)
    if(NOT EXISTS ${_cfg_dir}/qp_config.h)
        message(FATAL_ERROR "qpc: qp_config.h not found in "
            "QPC_CONFIG_DIR (${_cfg_dir})")
    endif()
    add_library(qpc_config INTERFACE)
    target_include_directories(qpc_config INTERFACE ${_cfg_dir})
endif()
if(NOT TARGET qpc_config)
    message(FATAL_ERROR "qpc: the application must define the INTERFACE "
        "target 'qpc_config' (with the include directory of its qp_config.h) "
        "before add_subdirectory(qpc), or set QPC_CONFIG_DIR")
endif()
if(NOT QPC_PORT)
    message(FATAL_ERROR "qpc: set QPC_PORT before add_subdirectory(qpc) "
        "(win32, win32-qv, posix, posix-qv, arm-cm, freertos)")
endif()
string(TOLOWER "${QPC_PORT}" _port)
if(QPC_INSTALL AND NOT _port MATCHES "^(win32|win32-qv|posix|posix-qv)$")
    message(FATAL_ERROR "qpc: QPC_INSTALL is supported only for the host "
        "ports (win32, win32-qv, posix, posix-qv), not for '${QPC_PORT}'")
endif()
if(PROJECT_IS_TOP_LEVEL AND NOT DEFINED CMAKE_C_FLAGS_SPY)
    # qpc built on its own (e.g., the package): Spy flags as in Debug
    set(CMAKE_C_FLAGS_SPY "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING
        "Flags used by the C compiler during Spy builds")
endif()

# the qpc library ...........................................................
add_library(qpc STATIC)
add_library(qpc::qpc ALIAS qpc)

# NOTE: BUILD_INTERFACE/INSTALL_INTERFACE matter only for the package
target_link_libraries(qpc PUBLIC $<BUILD_INTERFACE:qpc_config>)
target_include_directories(qpc PUBLIC
    $<BUILD_INTERFACE:${QPC_DIR}/include>
    $<INSTALL_INTERFACE:include/qpc>
    $<INSTALL_INTERFACE:include/qpc/config>)
target_compile_definitions(qpc PUBLIC $<$<CONFIG:Spy>:Q_SPY>)

set(_qf_srcs
    src/qf/qep_hsm.c
    src/qf/qep_msm.c
    src/qf/qf_act.c
    src/qf/qf_actq.c
    src/qf/qf_defer.c
    src/qf/qf_dyn.c
    src/qf/qf_mem.c
    src/qf/qf_ps.c
    src/qf/qf_qact.c
    src/qf/qf_qeq.c
    src/qf/qf_qmact.c
    src/qf/qf_time.c
)
set(_qs_srcs
    src/qs/qs.c
    src/qs/qs_rx.c
    src/qs/qs_fp.c
    src/qs/qs_64bit.c
)
# QP/Spy is not part of every QP/C distribution (e.g., not on GitHub):
# then all other configurations build, and only Spy stops with an error
if(NOT EXISTS ${QPC_DIR}/src/qs/qs.c OR NOT EXISTS ${QPC_DIR}/include/qs.h)
    message(WARNING "qpc: QP/Spy sources (src/qs/qs.c, include/qs.h) "
        "not found. The Spy configuration will not build.")
    set(_qs_srcs "")
    set(_msg "qpc: QP/Spy sources (src/qs/qs.c, include/qs.h) not found")
    add_custom_target(qpc_qs_missing
        COMMAND "$<$<CONFIG:Spy>:${CMAKE_COMMAND};-E;echo;${_msg}>"
        COMMAND "$<$<CONFIG:Spy>:${CMAKE_COMMAND};-E;false>"
        COMMAND_EXPAND_LISTS VERBATIM)
    add_dependencies(qpc qpc_qs_missing)
endif()
target_sources(qpc PRIVATE src/qs/qstamp.c) # build date/time

# the port ..................................................................
if(_port MATCHES "^(win32|win32-qv|posix|posix-qv)$") #-------- host ports
    set(_port_dir ${QPC_DIR}/ports/${_port})
    target_sources(qpc PRIVATE
        ${_qf_srcs}
        ${_port_dir}/qf_port.c
        "$<$<CONFIG:Spy>:${_qs_srcs};${_port_dir}/qs_port.c>"
    )
    if(_port MATCHES "^win32")
        target_link_libraries(qpc PUBLIC $<$<CONFIG:Spy>:ws2_32>) # QS socket
    else()
        find_package(Threads REQUIRED)
        target_link_libraries(qpc PUBLIC Threads::Threads)
    endif()

elseif(_port STREQUAL "arm-cm") #------------------- ARM Cortex-M, bare-metal
    if(NOT QPC_KERNEL)
        set(QPC_KERNEL qv)
    endif()
    string(TOLOWER "${QPC_KERNEL}" _kernel)
    if(_kernel STREQUAL "qv")
        set(_kernel_srcs src/qv/qv.c)
    elseif(_kernel STREQUAL "qk")
        set(_kernel_srcs src/qk/qk.c)
    elseif(_kernel STREQUAL "qxk")
        if(NOT EXISTS ${QPC_DIR}/src/qxk/qxk.c
                OR NOT EXISTS ${QPC_DIR}/include/qxk.h)
            message(FATAL_ERROR "qpc: QXK kernel sources (src/qxk/qxk.c, "
                "include/qxk.h) not found. QXK is not part of every QP/C "
                "distribution (e.g., not on GitHub). Use QV or QK.")
        endif()
        set(_kernel_srcs
            src/qxk/qxk.c
            src/qxk/qxk_xthr.c
            src/qxk/qxk_sema.c
            src/qxk/qxk_mutex.c
        )
    else()
        message(FATAL_ERROR "qpc: unknown QPC_KERNEL '${QPC_KERNEL}' (qv, qk, qxk)")
    endif()

    # the port subdirectory for the compiler
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(_comp gnu)
    elseif(CMAKE_C_COMPILER_ID STREQUAL "ARMClang")
        set(_comp armclang)
    elseif(CMAKE_C_COMPILER_ID STREQUAL "IAR")
        set(_comp iar)
    else()
        message(FATAL_ERROR "qpc: arm-cm port: unsupported compiler "
            "'${CMAKE_C_COMPILER_ID}' (GNU, ARMClang, IAR)")
    endif()

    set(_port_dir ${QPC_DIR}/ports/arm-cm/${_kernel}/${_comp})
    target_sources(qpc PRIVATE
        ${_qf_srcs}
        ${_kernel_srcs}
        ${_port_dir}/${_kernel}_port.c
        "$<$<CONFIG:Spy>:${_qs_srcs}>"
    )
    if(_comp STREQUAL "gnu")
        # INTERFACE: compiled into the application, so that the linker
        # always uses it (from a library it would not be pulled in)
        target_sources(qpc INTERFACE ${_port_dir}/syscalls.c)
    endif()

elseif(_port STREQUAL "freertos") #----------------------------------- FreeRTOS
    if(NOT TARGET freertos_kernel)
        message(FATAL_ERROR "qpc: freertos port: add FreeRTOS (target "
            "'freertos_kernel') before add_subdirectory(qpc)")
    endif()
    set(_port_dir ${QPC_DIR}/ports/freertos)
    list(REMOVE_ITEM _qf_srcs src/qf/qf_actq.c) # FreeRTOS provides the queues
    target_sources(qpc PRIVATE
        ${_qf_srcs}
        ${_port_dir}/qf_port.c
        "$<$<CONFIG:Spy>:${_qs_srcs}>"
    )
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_sources(qpc INTERFACE ${_port_dir}/syscalls.c)
    endif()
    target_link_libraries(qpc PUBLIC freertos_kernel)

else()
    message(FATAL_ERROR "qpc: unknown QPC_PORT '${QPC_PORT}' "
        "(win32, win32-qv, posix, posix-qv, arm-cm, freertos)")
endif()

if(NOT EXISTS ${_port_dir})
    message(FATAL_ERROR "qpc: port directory not found: ${_port_dir}")
endif()
target_include_directories(qpc PUBLIC $<BUILD_INTERFACE:${_port_dir}>)

message(STATUS "qpc: port ${_port_dir}")

# CMake package (QPC_INSTALL=ON) ............................................
if(QPC_INSTALL)
    # the qp_config.h the library is built with (installed with it)
    get_target_property(_cfg_dirs qpc_config INTERFACE_INCLUDE_DIRECTORIES)
    set(_cfg_file "")
    foreach(_d IN LISTS _cfg_dirs)
        string(REGEX REPLACE "^\\$<BUILD_INTERFACE:(.*)>$" "\\1" _d "${_d}")
        if(EXISTS "${_d}/qp_config.h")
            set(_cfg_file "${_d}/qp_config.h")
            break()
        endif()
    endforeach()
    if(NOT _cfg_file)
        message(FATAL_ERROR "qpc: QPC_INSTALL: qp_config.h not found in "
            "the include directories of 'qpc_config' (${_cfg_dirs})")
    endif()

    # the package version from the file version-<x.y.z> in the qpc directory
    file(GLOB _ver_file RELATIVE ${QPC_DIR} ${QPC_DIR}/version-*)
    string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" _ver "${_ver_file}")
    if(NOT _ver)
        message(FATAL_ERROR "qpc: QPC_INSTALL: version file "
            "${QPC_DIR}/version-<x.y.z> not found")
    endif()

    # the library of each configuration has its own name
    set_target_properties(qpc PROPERTIES
        DEBUG_POSTFIX _dbg
        SPY_POSTFIX _spy)

    install(TARGETS qpc EXPORT qpcTargets ARCHIVE DESTINATION lib)
    install(DIRECTORY ${QPC_DIR}/include/ DESTINATION include/qpc
        FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY ${_port_dir}/ DESTINATION include/qpc
        FILES_MATCHING PATTERN "*.h")
    # NOTE: qp_config.h in its own directory, so that an application's own
    # qp_config.h takes precedence (and is checked by the QP configuration
    # check), while the installed qp_config.h is the default
    install(FILES ${_cfg_file} DESTINATION include/qpc/config)
    install(EXPORT qpcTargets NAMESPACE qpc:: DESTINATION lib/cmake/qpc)

    if(_port MATCHES "^posix")
        set(_deps "find_dependency(Threads)")
    else()
        set(_deps "")
    endif()
    file(CONFIGURE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/qpcConfig.cmake
        CONTENT [=[
# qpc CMake package: QP/C @_ver@ built for the port '@_port@' and for the
# qp_config.h installed with this package (include/qpc/config/qp_config.h)
include(CMakeFindDependencyMacro)
@_deps@
include("${CMAKE_CURRENT_LIST_DIR}/qpcTargets.cmake")
set(qpc_PORT "@_port@")
]=] @ONLY)
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/qpcConfigVersion.cmake
        VERSION ${_ver} COMPATIBILITY ExactVersion)
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/qpcConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/qpcConfigVersion.cmake
        DESTINATION lib/cmake/qpc)

    message(STATUS "qpc: package ${_ver} with ${_cfg_file}")
endif()
