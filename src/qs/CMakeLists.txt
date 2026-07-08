# ./src/qs
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/qs.c)
    target_sources(qpc PRIVATE
        "$<$<CONFIG:Spy>:${CMAKE_CURRENT_SOURCE_DIR}/qs_64bit.c>"
        "$<$<CONFIG:Spy>:${CMAKE_CURRENT_SOURCE_DIR}/qs_fp.c>"
        "$<$<CONFIG:Spy>:${CMAKE_CURRENT_SOURCE_DIR}/qs_rx.c>"
        "$<$<CONFIG:Spy>:${CMAKE_CURRENT_SOURCE_DIR}/qstamp.c>"
        "$<$<CONFIG:Spy>:${CMAKE_CURRENT_SOURCE_DIR}/qs.c>"
        "$<$<AND:$<CONFIG:Spy>,$<BOOL:${QPC_CFG_UNIT_TEST}>>:${CMAKE_CURRENT_SOURCE_DIR}/qutest.c>"
    )
    if(NOT "Spy" IN_LIST CMAKE_CONFIGURATION_TYPES)
        list(APPEND CMAKE_CONFIGURATION_TYPES Spy)
        set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "CMake Configuration Types" FORCE)
    endif()
else()
    message(WARNING "Build configuration 'Spy' (QS component) not available in GPL verion!")
    if("Spy" IN_LIST CMAKE_CONFIGURATION_TYPES)
        list(REMOVE_ITEM CMAKE_CONFIGURATION_TYPES Spy)
        set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "CMake Configuration Types" FORCE)
    endif()
    target_sources(qpc PRIVATE
        "$<$<CONFIG:Spy>:${CMAKE_CURRENT_SOURCE_DIR}/qstamp.c>"
    )
endif()
