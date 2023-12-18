# qpc/src
add_subdirectory(qf)
if(QPC_CFG_QSPY)
    add_subdirectory(qs)
endif()
if(
    (${PORT} MATCHES "^arm-") OR
    (${PORT} STREQUAL pic32) OR
    (${PORT} STREQUAL risc-v)
)
    add_subdirectory(${KERNEL})
endif()
