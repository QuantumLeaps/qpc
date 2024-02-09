# ports/msp430
if(QPC_CFG_UNIT_TEST)
    target_include_directories(qpc PUBLIC qutest)
elseif(NOT KERNEL MATCHES "q[vk]")
    message(WARNING "Kernel ${KERNEL} is not supported! Falling back to QV kernel")
    set(KERNEL qv)
endif()
target_include_directories(qpc PUBLIC ${KERNEL})
