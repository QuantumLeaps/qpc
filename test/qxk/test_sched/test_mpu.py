# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html/qutest.html

note('''
@uid{TQP701}
This test group verifies the memory access protection,
specifically the protection of various memory regions.
All tests run during the QS_TEST_PAUSE() in
the test_sched.c fixture.
''')

# preamble
def on_reset():
    expect_pause()
    # don't call continue_test()

def on_setup():
    glb_filter(GRP_SC, GRP_UA)

MEM_START = 0
MEM_END   = 1

# NULL-pointer dereferencing...
test('''
NULL-read -> ASSERT
''')
command("MEM_READ", 0x0, 0x80) # value = *(uint32_t volatile *)(param1 + param2);
expect("@timestamp =ASSERT= Mod=*")

test("NULL-write -> ASSERT")
command("MEM_WRITE", 0x0, 0xA0, 123) # *(uint32_t volatile *)(param1 + param2) = param3;
expect("@timestamp =ASSERT= Mod=*")

# ROM_READ...
test("Below-Start Flash-read -> ASSERT")
command("ROM_READ", (-4 & 0xFFFFFFFF), MEM_START) # value = BSP_romRead(param1, param2);
expect("@timestamp =ASSERT= Mod=*")

test("Start-of Flash-read")
command("ROM_READ", 0x0, MEM_START) # value = BSP_romRead(param1, param2);
expect("@timestamp USER+000 ROM_READ *")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("Middle-of Flash-read", NORESET)
command("ROM_READ", 0x400, MEM_START) # value = BSP_romRead(param1, param2);
expect("@timestamp USER+000 ROM_READ *")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("End-of Flash-read", NORESET)
command("ROM_READ", 0x0, MEM_END) # value = BSP_romRead(param1, param2);
expect("@timestamp USER+000 ROM_READ *")
expect("@timestamp Trg-Done QS_RX_COMMAND")

skip(1)
test("After-End Flash-read-> ASSERT", NORESET)
command("ROM_READ", 0x4, MEM_END) # value = BSP_romRead(param1, param2);
expect("@timestamp =ASSERT= Mod=*")

# ROM_WRITE...
test("Middle-of Flash-write -> ASSERT")
command("ROM_WRITE", 0x400, MEM_START, 123) # BSP_romWrite(param1, param2, param3);
expect("@timestamp =ASSERT= Mod=*")

# RAM_READ...
test("Below-Start SRAM-read -> ASSERT")
command("RAM_READ", (-0x4 & 0xFFFFFFFF), MEM_START) # value = BSP_ramRead(param1, param2);
expect("@timestamp =ASSERT= Mod=*")

test("Start-of SRAM-read")
command("RAM_READ", 0x0, MEM_START) # value = BSP_ramRead(param1, param2);
expect("@timestamp USER+000 RAM_READ *")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("Middle-of SRAM-read", NORESET)
command("RAM_READ", 0x300, MEM_START) # value = BSP_ramRead(param1, param2);
expect("@timestamp USER+000 RAM_READ *")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("End-of SRAM-read", NORESET)
command("RAM_READ", 0x0, MEM_END) # value = BSP_ramRead(param1, param2);
expect("@timestamp USER+000 RAM_READ *")
expect("@timestamp Trg-Done QS_RX_COMMAND")

# RAM_WRITE...
test("Below-Start SRAM-write -> ASSERT", NORESET)
command("RAM_WRITE", (-0x4 & 0xFFFFFFFF), MEM_START, 123) # BSP_ramWrite(param1, param2, param3);
expect("@timestamp =ASSERT= Mod=*")

# wrtiting to RAM does not need to be tested because the fact
# that the test fixture runs at all means that RAM works.
skip(1)
test("Middle-of SRAM-write")
command("RAM_WRITE", 0x300, MEM_START, 123) # BSP_ramWrite(param1, param2, param3);
expect("@timestamp USER+000 RAM_WRITE *")
expect("@timestamp Trg-Done QS_RX_COMMAND")

test("After-End SRAM-write -> ASSERT")
command("RAM_WRITE", 0x4, MEM_END, 123) # BSP_ramWrite(param1, param2, param3);
expect("@timestamp =ASSERT= Mod=*")
