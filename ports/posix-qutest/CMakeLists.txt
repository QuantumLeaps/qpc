# ports/posix-qutest
target_include_directories(qpc PUBLIC .)
target_sources(qpc PRIVATE
    qutest_port.c
)