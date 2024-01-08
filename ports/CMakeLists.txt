# CMake the qpc libraries for different targets
# target may be "Windows", "Posix", "TLE984x"
set(PORT_DIR ${PORT})
if((PORT STREQUAL win32) OR (PORT STREQUAL posix))
	if(CONFIG_UNIT_TEST)
		set(PORT_DIR ${PORT_DIR}-qutest)
	elseif(KERNEL STREQUAL qv)
        set(PORT_DIR ${PORT_DIR}-qv)
	endif()
endif()
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PORT_DIR})
	message(STATUS "Found port dir - './${PORT_DIR}' for port ${PORT}, KERNEL ${KERNEL}")
else()
	message(FATAL_ERROR "Target port dir '${CMAKE_CURRENT_SOURCE_DIR}/${PORT_DIR}' not found!")
endif()

target_link_libraries(qpc PUBLIC $<$<AND:$<CONFIG:Spy>,$<STREQUAL:win32,${PORT}>>:ws2_32>)

add_subdirectory(${PORT_DIR})
target_include_directories(qpc PRIVATE config)
