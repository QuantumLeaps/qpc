# ports/pic32
if(QPC_CFG_UNIT_TEST)
    target_include_directories(qpc PUBLIC qutest/xc32)
else()
    if(NOT KERNEL MATCHES "q[vk]")
        message(WARNING "Kernel ${KERNEL} is not supported! Falling back to QV kernel")
        set(KERNEL qv)
    endif()

    set(KERNEL_PORT_C ${CMAKE_CURRENT_LIST_DIR}/${KERNEL}/xc32/${KERNEL}_port.c)
    if(EXISTS ${KERNEL_PORT_C})
        target_sources(qpc PRIVATE ${KERNEL_PORT_C})
    endif()
    target_include_directories(qpc PUBLIC ${KERNEL}/xc32)
endif()