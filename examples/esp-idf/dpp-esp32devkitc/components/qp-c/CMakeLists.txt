# ------------------------------------------------------------
# register component
# ------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)

set(required_modules "freertos")

set(qpc_path "../../../../../")

set(src_dirs    ${qpc_path}/ports/esp-idf
                ${qpc_path}/src/qf
                ${qpc_path}/include
 )

 set(include_dirs    ${qpc_path}/include
                     ${qpc_path}ports/esp-idf
                     ${qpc_path}/src
)

if(CONFIG_QPC_QSPY_ENABLE)
    list(APPEND src_dirs ${qpc_path}/src/qs)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DQ_SPY")
endif()

idf_component_register(
    SRC_DIRS        ${src_dirs}
    # exclude qf_actq.c because esp-idf port is using message queues; see note in qpc/doxygen/dir.dox
    EXCLUDE_SRCS    ${qpc_path}/src/qf/qf_actq.c
    INCLUDE_DIRS    ${include_dirs}
    PRIV_REQUIRES   ${required_modules}
)

