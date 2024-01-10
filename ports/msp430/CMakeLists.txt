# ports/os_xxx
if(QPC_CFG_UNIT_TEST)
    target_include_directories(qpc PUBLIC qutest)
elseif(KERNEL MATCHES "q[vk]")
    target_include_directories(qpc PUBLIC ${Kernel})
else()
    message(FATAL_ERROR "Kernel ${KERNEL} is not supported!")
endif()
