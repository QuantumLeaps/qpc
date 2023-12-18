# ./src/qf
target_sources(
    qpc
    PRIVATE
        qs_64bit.c
        qs_fp.c
        qs_rx.c
        qstamp.c
        qs.c
)

if(QPC_CFG_UNIT_TEST)
    target_sources(
        qpc
        PRIVATE
            qutest.c
    )
endif()