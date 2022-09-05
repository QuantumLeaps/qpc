# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html/qutest.html

# preamble
def on_reset():
    expect_pause()

def Q_PRIO(prio, pthre):
    return prio | (pthre << 8)

test("NO preemption-threshold (scheduler only)")
continue_test()
expect_run()
#----
glb_filter(GRP_SM, GRP_SC)
current_obj(OBJ_AO, "aoB[1]")
post("TRIG_SIG")
expect("@timestamp Sch-Next Pri=0->2")
expect("@timestamp Disp===> Obj=aoB[1],Sig=TRIG_SIG,State=ObjB_active")
expect("@timestamp Sch-Pre  Pri=2->4")
expect("@timestamp Disp===> Obj=aoA,Sig=TEST_SIG,State=ObjA_active")
expect("@timestamp =>Intern Obj=aoA,Sig=TEST_SIG,State=ObjA_active")
expect("@timestamp Sch-Next Pri=4->3")
expect("@timestamp Disp===> Obj=aoB[2],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp =>Intern Obj=aoB[2],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp Sch-Rest Pri=3->2")
expect("@timestamp =>Intern Obj=aoB[1],Sig=TRIG_SIG,State=ObjB_active")
expect("@timestamp Sch-Next Pri=2->2")
expect("@timestamp Disp===> Obj=aoB[1],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp =>Intern Obj=aoB[1],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp Sch-Next Pri=2->1")
expect("@timestamp Disp===> Obj=aoB[0],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp =>Intern Obj=aoB[0],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp Sch-Idle Pri=1->0")
expect("@timestamp Trg-Done QS_RX_EVENT")

test("preemption-threshold (scheduler only)")
current_obj(OBJ_AP, "pspec")
poke(0, 2, pack("<HHHH",
    Q_PRIO(1,1), Q_PRIO(2,1), Q_PRIO(3,1), Q_PRIO(4,2)))
continue_test()
expect_run()
#----
glb_filter(GRP_SM, GRP_SC)
current_obj(OBJ_AO, "aoB[1]")
post("TRIG_SIG")
expect("@timestamp Sch-Next Pri=0->2")
expect("@timestamp Disp===> Obj=aoB[1],Sig=TRIG_SIG,State=ObjB_active")
expect("@timestamp Sch-Pre  Pri=2->4")
expect("@timestamp Disp===> Obj=aoA,Sig=TEST_SIG,State=ObjA_active")
expect("@timestamp =>Intern Obj=aoA,Sig=TEST_SIG,State=ObjA_active")
expect("@timestamp Sch-Rest Pri=4->2")
expect("@timestamp =>Intern Obj=aoB[1],Sig=TRIG_SIG,State=ObjB_active")
expect("@timestamp Sch-Next Pri=2->3")
expect("@timestamp Disp===> Obj=aoB[2],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp =>Intern Obj=aoB[2],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp Sch-Next Pri=3->2")
expect("@timestamp Disp===> Obj=aoB[1],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp =>Intern Obj=aoB[1],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp Sch-Next Pri=2->1")
expect("@timestamp Disp===> Obj=aoB[0],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp =>Intern Obj=aoB[0],Sig=TEST_SIG,State=ObjB_active")
expect("@timestamp Sch-Idle Pri=1->0")
expect("@timestamp Trg-Done QS_RX_EVENT")
