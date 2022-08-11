# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preambe...
#def on_reset():
#    expect_run()
#    current_obj(OBJ_AP,"string")

# tests...
test("my_strlen 0 for_empty_string")
poke(0,1,b"\0")
command(0)
expect("@timestamp USER+000 my_strlen 0 ''")
expect("@timestamp Trg-Done QS_RX_COMMAND")


test("my_strlen n for various strings", NORESET)
poke(0,1,b"Foo\0")
command(0)
expect("@timestamp USER+000 my_strlen 3 Foo")
expect("@timestamp Trg-Done QS_RX_COMMAND")
poke(0,1,b" Foo\0")
command(0)
expect("@timestamp USER+000 my_strlen 4  Foo")
expect("@timestamp Trg-Done QS_RX_COMMAND")


test("my_strlen n for unprintable chars", NORESET)
poke(0,1,b"\tFoo\0")
command(0)
expect("@timestamp USER+000 my_strlen 4 \tFoo")
expect("@timestamp Trg-Done QS_RX_COMMAND")
poke(0,1,b"\a\b\n\0")
command(0)
expect("@timestamp USER+000 my_strlen 3 \a\b\n")
expect("@timestamp Trg-Done QS_RX_COMMAND")
