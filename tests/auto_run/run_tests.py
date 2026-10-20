#=============================================================================
# Test code for SafeQP/C, SafeQP/C++, QP/C, and QP/C++
#
# Copyright (C) 2005-2026 Quantum Leaps, LLC. All rights reserved.
#
#                    Q u a n t u m  L e a P s
#                    ------------------------
#                    Modern Embedded Software
#
# SPDX-License-Identifier: LicenseRef-QL-testing
#
# Licensed to: Licensees and Evaluators of QP Software
#
# This software is test code for the QP real-time event frameworks
# (QP/C, QP/C++, SafeQP/C, SafeQP/C++). It is licensed solely for the
# purpose of building, running, and reviewing the tests of the QP software
# in which it is distributed. This license grants no rights to the QP
# software itself, which remains subject to its own license.
#
# RESTRICTIONS
# Any other use of this software is NOT licensed and is prohibited.
# In particular, you may NOT:
# (a) use this software, or any portion of it, in any product, application,
#     or deliverable, or for any purpose other than testing the QP software,
# (b) redistribute, sell, sublicense, or otherwise transfer this software
#     separately from the QP software in which it is distributed,
# (c) remove, obscure, or alter any copyright, license, or other
#     proprietary notice present in this software,
# (d) use this software, or any portion of it, as input to train,
#     fine-tune, evaluate, or otherwise develop any artificial
#     intelligence or machine learning model.
# Any use outside the scope of this license is an infringement of the
# copyright of Quantum Leaps.
#
# This software is provided "AS IS," without warranty of any kind, express
# or implied.
#
# Quantum Leaps contact information:
# <www.state-machine.com/licensing>
# <info@state-machine.com>
#=============================================================================
# Automated test runner of the SafeQP and QP test suites (Windows, Linux,
# macOS). The same script serves both languages (C and C++) and both
# editions (SafeQP and standard QP); it is identical in the safe-qpc,
# safe-qpcpp, qpc and qpcpp test trees. The edition is taken from the test
# infrastructure itself (QP_SAFE_EDITION in tests/shared/safe-qp_check.pyi,
# used by the test scripts): the validation tests (TVA) exist and are run
# only in the SafeQP edition.
#
# usage:   python3 run_tests.py host
#          python3 run_tests.py nucleo-h533re <USB-drive>   (e.g., f:/)
#
# Evidence: the logs written to this directory (<TEST>-<target>.log, and
# <TEST>-<target>-unsafe.log for the Q_UNSAFE builds of the integration tests)
# are the only test evidence. The validation tests (TVA, SafeQP only) run on
# the board target only (they are built from the Reference Designs for that
# board). At the start, the runner deletes all logs of the selected target in
# this directory. Before each test, it deletes the result files (*.log, *.cov,
# *.impchk, *.gcov; also *.instr, the name of the *.impchk files before 8.1.6)
# and the build directories (build, build_unsafe) of the test, so that
# nothing from an earlier run (e.g., a manual run with other options) can
# enter the log or the build. After collecting the results into
# the log, it deletes them again. Tests that are not run by this runner are
# not touched. The runner stops at the first unexpected result and names the
# test (its files are kept for inspection).
#
# Sections of a log (separated by a line of '#'):
#   board: the QUTest log.
#   host:  1. the QUTest log,
#          2. the coverage summary of the module under test (<module>.cov),
#          3. the instrumentation-impact report of the module (<module>.impchk),
#          4. the annotated coverage of the module (<module>.c.gcov or
#             <module>.cpp.gcov; not in the Q_UNSAFE logs),
#          integration tests only:
#          5. the function coverage of the other Framework modules
#             (<module>_int.cov),
#          6. the annotated coverage of the other modules (not in the
#             Q_UNSAFE logs).
#
# Instrumentation impact (host): the verdict of the instrumentation-impact
# report (<module>.impchk, qimpact) of every host unit and integration test
# must be PASS. The only accepted FAIL is the documented exception of the
# module qf_actq (QIMPACT_EXCEPTIONS): its report may fail in exactly the
# listed functions (the QActiveDummy redirect of QUTest), and in no other.
# Any other verdict (FAIL, REVIEW, ERROR) stops the run like a failing test.
#
# Build test (host only): TIN_QP_cfgchk verifies the link-time configuration
# check (qp_cfgchk.h/.hpp). Each test case is one target of the test's
# Makefile (case01, case02, ...), which compiles the Application translation
# unit with the configuration of the case and links it with the Framework
# built with the reference configuration. The runner checks the result of the
# link step and the configuration-specific name reported (defined in the image
# or undefined reference) against CFGCHK_CASES, and writes one log with the
# output of every case followed by its verdict (the expectation and the verdict
# of each case are also printed on the console).
#
# At the end, the runner reports the object-pointer size(s) reported by the
# Target in the QUTest summary of every log ("Obj-Ptr=NN-bit"); the log of the
# build test is not included (its image is not executed).

import sys, os, re, glob, shutil, subprocess, functools

# flush every line, so the runner's messages stay in order with make's output
print = functools.partial(print, flush=True)

# infrastructure test: exactly these test cases fail by design
QUTEST_CHECK = ['Tests     : 25', 'FAILED    : 4 [ 1 3 4 5 ]']

UNIT_TESTS = ['TUN_QP_qep_hsm', 'TUN_QP_qep_msm', 'TUN_QP_qf_act',
    'TUN_QP_qf_actq8', 'TUN_QP_qf_actq16', 'TUN_QP_qf_defer',
    'TUN_QP_qf_dyn', 'TUN_QP_qf_dyn_raii', 'TUN_QP_qf_mem', 'TUN_QP_qf_ps',
    'TUN_QP_qf_qact64', 'TUN_QP_qf_qact32', 'TUN_QP_qf_qact16',
    'TUN_QP_qf_qact8', 'TUN_QP_qf_qeq8', 'TUN_QP_qf_qeq16',
    'TUN_QP_qf_qmact', 'TUN_QP_qf_time16', 'TUN_QP_qf_time32',
    'TUN_QP_qv', 'TUN_QP_qk']

INTEGRATION_TESTS = [('TIN_QP_ps', 'test_8'), ('TIN_QP_ps', 'test_16'),
    ('TIN_QP_ps', 'test_32'), ('TIN_QP_ps', 'test_64'),
    ('TIN_QP_ao', 'test_eq8'), ('TIN_QP_ao', 'test_eq16'),
    ('TIN_QP_evt', 'test_std'), ('TIN_QP_evt', 'test_par'),
    ('TIN_QP_defer', 'test_std'), ('TIN_QP_defer', 'test_nopool'),
    ('TIN_QP_time', 'test_te16'), ('TIN_QP_time', 'test_te32'),
    ('TIN_QP_sm', 'test_hsm'), ('TIN_QP_sm', 'test_msm'),
    ('TIN_QP_qk', 'test_std'), ('TIN_QP_qv', 'test_std')]

VALIDATION_TESTS = [('TVA_QP_dpp_mpu', 'test_qk_mpu_on'),
    ('TVA_QP_dpp_mpu', 'test_qk_mpu_off'), ('TVA_QP_dpp_mpu', 'test_qv_mpu_on'),
    ('TVA_QP_dpp_mpu', 'test_qv_mpu_off')]

# build test of the link-time configuration check (host only): test case,
# expected result of the link step (True: links), expected configuration-
# specific name without the prefix 'QF_<QP_VERSION>_' (SafeQP, standard QP)
CFGCHK_TEST  = ('TIN_QP_cfgchk', 'test_std')
CFGCHK_CASES = [
    ('01', True,  'a08t01p01e4q1m2s42x',   'a08t01p01e4q1m2s42x'),
    ('02', False, 'a08t01p01e4q1m2s42ux',  'a08t01p01e4q1m2s42ux'),
    ('03', False, 'a08t01p01e4q2m2s42x',   'a08t01p01e4q2m2s42x'),
    ('04', False, 'a08t01p01e4q1m2s42cix', 'a08t01p01e4q1m2s42cx'),
    ('05', False, 'a08t01p01e4q1m2s41x',   'a08t01p01e4q1m2s41x'),
    ('06', False, 'a16t01p01e4q1m2s42x',   'a16t01p01e4q1m2s42x')]

# instrumentation impact: the only accepted FAIL verdicts (module: functions
# reported as FAIL), see STP_QP TUN_QP_qf_actq8 and README_qimpact.md
QIMPACT_EXCEPTIONS = {
    'qf_actq.c':   {'QActive_post_', 'QActive_postLIFO_'},
    'qf_actq.cpp': {'bool QP::QActive::postx_(const QP::QEvt*, '
                    'uint_fast16_t, const void*)',
                    'void QP::QActive::postLIFO(const QP::QEvt*)'}}

def fail(msg):
    sys.stderr.write(f'*** {msg} FAILED\n')
    sys.exit(1)

def safe_edition(test_dir):
    # the edition of the test tree: QP_SAFE_EDITION in the shared include
    # file of the test scripts (True: SafeQP, False: standard QP)
    f = os.path.join(test_dir, 'shared', 'safe-qp_check.pyi')
    if not os.path.isfile(f): fail(f)
    m = re.findall(r'^QP_SAFE_EDITION\s*=\s*(True|False)\b',
                   open(f, errors='replace').read(), re.MULTILINE)
    if len(m) != 1: fail(f'{f}: QP_SAFE_EDITION')
    return m[0] == 'True'

def qp_version(test_dir):
    # QP_VERSION (e.g., '816U') from the Framework header of the test tree
    for h in ('qp.h', 'qp.hpp'):
        f = os.path.join(os.path.dirname(test_dir), 'include', h)
        if os.path.isfile(f):
            m = re.findall(r'^#define\s+QP_VERSION\s+(\d+U)\b',
                           open(f, errors='replace').read(), re.MULTILINE)
            if len(m) == 1: return m[0]
    fail('QP_VERSION')

def impact_ok(d):
    # the verdict of the instrumentation-impact report of a host test is
    # PASS, or FAIL in exactly the functions of a documented exception
    instr = glob.glob(os.path.join(d, '*.impchk'))
    if len(instr) != 1: return False
    txt = open(instr[0], errors='replace').read()
    verdict = re.findall(r'^Verdict\s*:\s*(\S+)', txt, re.MULTILINE)
    if verdict == ['PASS']: return True
    module = re.findall(r'^Module\s*:\s*(.*\S)', txt, re.MULTILINE)
    funcs = re.findall(r'^  (\S.*?)\s+(FAIL|REVIEW)\s+test-only', txt,
                       re.MULTILINE)
    if verdict != ['FAIL'] or len(module) != 1: return False
    exc = QIMPACT_EXCEPTIONS.get(os.path.basename(module[0]))
    return (exc is not None) and all(v == 'FAIL' for _, v in funcs) and \
        ({n for n, _ in funcs} == exc)

def main():
    args = sys.argv[1:]
    if not args or args[0] not in ('host', 'nucleo-h533re'):
        print('usage: run_tests.py host | nucleo-h533re <USB-drive>')
        sys.exit(1)
    trg = args[0]
    flash = []
    if trg != 'host':
        if len(args) < 2:
            print('usage: run_tests.py nucleo-h533re <USB-drive>  (e.g., f:/)')
            sys.exit(1)
        flash = [f'USB={args[1]}', 'flash']

    log_dir  = os.path.dirname(os.path.abspath(__file__))
    test_dir = os.path.dirname(log_dir)
    safe = safe_edition(test_dir)
    sep  = b'\n' + b'#' * 78 + b'\n'  # separator between the log sections
    head = (f'Target    : {trg} ' + os.linesep).encode()  # EOL as the shell 'echo'
    print(head.decode().rstrip())
    print(f'Edition   : {"SafeQP" if safe else "QP"}')

    # delete all logs of this target first, so that no log of an earlier run
    # (e.g., of a test no longer in the lists) remains next to the new ones
    for f in glob.glob(os.path.join(log_dir, f'*-{trg}.log')) + \
             glob.glob(os.path.join(log_dir, f'*-{trg}-unsafe.log')):
        os.remove(f)

    def run_make(d, extra):
        return subprocess.run(['make', '-j8', '-f', 'Makefile', 'LOG=.']
                              + extra + flash, cwd=d).returncode

    def clean(d):
        # delete the result files and the build directories of one test
        # (*.instr: the name of the qimpact report before 8.1.6)
        for p in ('*.log', '*.cov', '*.impchk', '*.instr', '*.gcov'):
            for f in glob.glob(os.path.join(d, p)):
                os.remove(f)
        for b in ('build', 'build_unsafe'):
            shutil.rmtree(os.path.join(d, b), ignore_errors=True)

    def collect(d, out, groups):
        # concatenate the header and the file groups into the log, with the
        # separator between the (non-empty) groups
        with open(os.path.join(log_dir, out), 'wb') as o:
            o.write(head)
            first = True
            for files in groups:
                if not files:
                    continue
                if not first:
                    o.write(sep)
                first = False
                for f in files:
                    o.write(open(f, 'rb').read())

    def logs(d):
        return sorted(glob.glob(os.path.join(d, '*.log')))

    def groups(d, nominal):
        # the log sections of one test in the order listed in the header
        if trg != 'host':
            return [logs(d)]
        covs = [f for f in glob.glob(os.path.join(d, '*.cov'))
                if not f.endswith('_int.cov')]
        if len(covs) != 1: fail(f'{d}: coverage summary')
        mod = covs[0][:-len('.cov')]
        ext = '.cpp.gcov' if glob.glob(os.path.join(d, '*.cpp.gcov')) \
              else '.c.gcov'
        own = [f for f in [mod + ext] if os.path.isfile(f)]
        other = sorted(set(glob.glob(os.path.join(d, '*' + ext))) - set(own))
        intc = [f for f in [mod + '_int.cov'] if os.path.isfile(f)]
        instr = [f for f in [mod + '.impchk'] if os.path.isfile(f)]
        if not instr: fail(f'{d}: instrumentation-impact report')
        if nominal:
            return [logs(d), covs, instr, own, intc, other]
        return [logs(d), covs, instr, intc]

    # infrastructure test (QUTest self-check) ...............................
    test = 'TUN_QP_qutest'
    d = os.path.join(test_dir, test, 'test', trg)
    if not os.path.isdir(d): fail(test)
    clean(d)
    if run_make(d, ['OPT=c']) != 2: fail(test)
    txt = ''.join(open(f, errors='replace').read()
                  for f in glob.glob(os.path.join(d, '*.log')))
    if not all(c in txt for c in QUTEST_CHECK): fail(test)
    collect(d, f'{test}-{trg}.log', [logs(d)])
    clean(d)

    # unit tests ............................................................
    for test in UNIT_TESTS:
        d = os.path.join(test_dir, test, 'test', trg)
        if not os.path.isdir(d): fail(test)
        clean(d)
        if run_make(d, ['OPT=cx']) != 0: fail(test)
        collect(d, f'{test}-{trg}.log', groups(d, True))
        if (trg == 'host') and not impact_ok(d):
            fail(f'{test} instrumentation impact')
        clean(d)

    # integration tests: Q_UNSAFE first (separate build dir), nominal last ..
    for test, fix in INTEGRATION_TESTS:
        tid = f'{test}_{fix[len("test_"):]}'
        d = os.path.join(test_dir, test, fix, trg)
        if not os.path.isdir(d): fail(f'{test} {fix}')
        clean(d)
        if run_make(d, ['OPT=cx', 'BIN_DIR=build_unsafe',
                        'DEF=-DQ_UNSAFE']) != 0:
            fail(f'{test} {fix} Q_UNSAFE')
        collect(d, f'{tid}-{trg}-unsafe.log', groups(d, False))
        if (trg == 'host') and not impact_ok(d):
            fail(f'{test} {fix} Q_UNSAFE instrumentation impact')
        clean(d)
        if run_make(d, ['OPT=cx']) != 0: fail(f'{test} {fix}')
        collect(d, f'{tid}-{trg}.log', groups(d, True))
        if (trg == 'host') and not impact_ok(d):
            fail(f'{test} {fix} instrumentation impact')
        clean(d)

    # build test of the link-time configuration check (host only) .........
    if trg == 'host':
        test, fix = CFGCHK_TEST
        tid = f'{test}_{fix[len("test_"):]}'
        d = os.path.join(test_dir, test, fix, trg)
        if not os.path.isdir(d): fail(f'{test} {fix}')
        pre = f'QF_{qp_version(test_dir)}_'
        clean(d)
        with open(os.path.join(log_dir, f'{tid}-{trg}.log'), 'wb') as o:
            o.write(head)
            for n, links, exp_safe, exp_qp in CFGCHK_CASES:
                name = pre + (exp_safe if safe else exp_qp)
                exp = (f'link succeeds, image defines {name}' if links else
                       f'link fails, undefined reference to {name}')
                print(f'{test}_{n}: expected: {exp}')
                r = subprocess.run(['make', '-j8', '-f', 'Makefile', f'case{n}'],
                                   cwd=d, stdout=subprocess.PIPE,
                                   stderr=subprocess.STDOUT)
                out = r.stdout.decode(errors='replace')
                print(out, end='')
                if links:
                    got = re.findall(r'^\S+\s+T\s+\S*?(QF_\d+U_[a-z0-9]+)',
                                     out, re.MULTILINE)
                    ok = (r.returncode == 0) and (got == [name])
                else:
                    got = sorted(set(re.findall(
                        r'undefined reference to \S*?(QF_\d+U_[a-z0-9]+)', out)))
                    ok = (r.returncode != 0) and (got == [name]) and \
                        os.path.isfile(os.path.join(d, 'build', f'main_{n}.o'))
                if n != CFGCHK_CASES[0][0]:
                    o.write(sep)
                o.write((f'{test}_{n}: expected: {exp}' + os.linesep).encode())
                o.write(out.encode())
                o.write((f'{test}_{n}: {"PASS" if ok else "FAIL"}'
                         + os.linesep).encode())
                print(f'{test}_{n}: {"PASS" if ok else "FAIL"}')
                if not ok: fail(f'{test}_{n}')
        clean(d)

    # validation tests (SafeQP edition, board only) ........................
    if safe and (trg != 'host'):
        for test, fix in VALIDATION_TESTS:
            tid = f'{test}_{fix[len("test_"):]}'
            d = os.path.join(test_dir, test, fix, trg)
            if not os.path.isdir(d): fail(f'{test} {fix}')
            clean(d)
            if run_make(d, ['OPT=cx']) != 0: fail(f'{test} {fix}')
            collect(d, f'{tid}-{trg}.log', [logs(d)])
            clean(d)

    # object-pointer size reported by the Target in every log ..............
    sizes = {}
    for f in sorted(glob.glob(os.path.join(log_dir, f'*-{trg}*.log'))):
        if os.path.basename(f).startswith(CFGCHK_TEST[0]):
            continue  # build test: the image is not executed
        txt = open(f, errors='replace').read()
        for s in sorted(set(re.findall(r'Obj-Ptr=(\d+)-bit', txt))) or ['?']:
            sizes.setdefault(s, []).append(os.path.basename(f))
    if len(sizes) == 1:
        print(f'Obj-Ptr   : {next(iter(sizes))}-bit in all {trg} logs')
    else:
        for s, fs in sorted(sizes.items()):
            print(f'Obj-Ptr   : {s}-bit in {len(fs)} logs: ' + ' '.join(fs))

    print('OK')

if __name__ == '__main__':
    main()
