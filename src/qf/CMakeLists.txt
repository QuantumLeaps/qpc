# ./src/qf
target_sources(qpc PRIVATE
    qep_hsm.c
    qep_msm.c
    qf_actq.c
    qf_defer.c
    qf_dyn.c
    qf_mem.c
    qf_ps.c
    qf_qact.c
    qf_qeq.c
    qf_qmact.c
    qf_time.c
)
if(QPC_CFG_ADD_QF_ACTQ)
    target_sources(qpc PRIVATE
        qf_actq.c
    )
endif()
