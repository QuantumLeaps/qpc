# qpc/src
add_subdirectory(qf)
add_subdirectory(qs)
if(${PORT} MATCHES "^(arm-|pic32|risc-v)")
    message(STATUS "adding subdir '${KERNEL}' for port '${PORT}'")
    add_subdirectory(${KERNEL})
endif()
