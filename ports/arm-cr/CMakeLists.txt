# ports/arm-cm
string(TOLOWER ${CMAKE_C_COMPILER_ID} _compiler_)

if(KERNEL MATCHES "q[vk]")
    target_include_directories(qpc PUBLIC ${KERNEL}/${_compiler_} ${KERNEL}/config)
endif()

