# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(blinky)

zephyr_include_directories(
 ../../../include
 ../../../ports/zephyr
)

target_sources(app PRIVATE
 src/main.c
 src/bsp.c
 src/blinky.c
 ../../../src/qf/qep_hsm.c
 ../../../src/qf/qep_msm.c
 ../../../src/qf/qf_act.c
 ../../../src/qf/qf_qact.c
 ../../../src/qf/qf_defer.c
 ../../../src/qf/qf_dyn.c
 ../../../src/qf/qf_mem.c
 ../../../src/qf/qf_ps.c
 ../../../src/qf/qf_qact.c
 ../../../src/qf/qf_qeq.c
 ../../../src/qf/qf_qmact.c
 ../../../src/qf/qf_time.c
 ../../../ports/zephyr/qf_port.c
)

