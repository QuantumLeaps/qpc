# QPC Zephyr module
# SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial

zephyr_library()

if(CONFIG_QPC)

set(QPC_DIR ..)

zephyr_include_directories(
 ${QPC_DIR}/include
 ${QPC_DIR}/zephyr
)

target_include_directories(app PUBLIC
 ${QPC_DIR}/include
 ${QPC_DIR}/zephyr
)

zephyr_library_sources(
 ${QPC_DIR}/src/qf/qep_hsm.cpp
 ${QPC_DIR}/src/qf/qep_msm.cpp
 ${QPC_DIR}/src/qf/qf_qact.cpp
 ${QPC_DIR}/src/qf/qf_defer.cpp
 ${QPC_DIR}/src/qf/qf_dyn.cpp
 ${QPC_DIR}/src/qf/qf_mem.cpp
 ${QPC_DIR}/src/qf/qf_ps.cpp
 ${QPC_DIR}/src/qf/qf_qact.cpp
 ${QPC_DIR}/src/qf/qf_qeq.cpp
 ${QPC_DIR}/src/qf/qf_qmact.cpp
 ${QPC_DIR}/src/qf/qf_time.cpp
 ${QPC_DIR}/zephyr/qf_port.cpp
)

# QSPY option...
if(QSPY)

target_compile_definitions(app PUBLIC Q_SPY)
zephyr_library_compile_definitions(Q_SPY)

zephyr_library_sources(
 ${QPC_DIR}/src/qs/qs.cpp
 ${QPC_DIR}/src/qs/qs_rx.cpp
 ${QPC_DIR}/src/qs/qs_fp.cpp
 ${QPC_DIR}/src/qs/qutest.cpp
 ${QPC_DIR}/src/qs/qs_64bit.cpp
 ${QPC_DIR}/include/qstamp.cpp
)

endif() # QSPY

endif() # CONFIG_QPC
