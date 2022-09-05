# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html/qutest.html

# preamble...
def on_reset():
    expect("@timestamp Sch-Next Pri=0->1")
    expect_run()
    expect("@timestamp Sch-Idle Pri=1->0")

test("extended thread tick-tick")
current_obj(OBJ_AP,"thr1")
query_curr(OBJ_AP)
expect("@timestamp Query-AP Obj=thr1")
tick()
expect("           Tick<0>  Ctr=0000000001")
expect("           TE0-ADis Obj=thr1.timeEvt,AO=thr1")
expect("@timestamp TE0-Post Obj=thr1.timeEvt,Sig=TEST_SIG,AO=thr1")
expect("@timestamp Sch-Pre  Pri=0->1")
expect("@timestamp Sch-Idle Pri=1->0")
expect("@timestamp Trg-Done QS_RX_TICK")
tick()
expect("           Tick<0>  Ctr=0000000002")
expect("           TE0-ADis Obj=thr1.timeEvt,AO=thr1")
expect("@timestamp TE0-Post Obj=thr1.timeEvt,Sig=TEST_SIG,AO=thr1")
expect("@timestamp Sch-Pre  Pri=0->1")
expect("@timestamp Sch-Idle Pri=1->0")
expect("@timestamp Trg-Done QS_RX_TICK")
