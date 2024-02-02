# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

note('''
This test group demonstrates testing of the start sequence.
Specifically, a "Gizmo" AO needs to start either in the state
"on_mains", when the AO detects at runtime that main power
is available, or in the state "on_battery", when it is not.
However, the state machine semantics does not allow guard
conditions on the initial transition itself. A workaround is
to self-post a START event, which then triggers a transition
with a guard.
''')

def on_reset():
    expect_pause()

test('''
QACTIVE_START(AO_Gizmo) with status==1,
should go to state "on_mains"
''')
current_obj(OBJ_AP, 'status')
poke(0, 1, pack('<B', 1))  # status = 1
glb_filter(GRP_ALL)
continue_test()
expect("===RTC===> St-Init  Obj=AO_Gizmo,State=QHsm_top->Gizmo_init")
expect("@timestamp AO-Post  Sdr=AO_Gizmo,Obj=AO_Gizmo,Evt<Sig=START_SIG,*")
expect("===RTC===> St-Entry Obj=AO_Gizmo,State=Gizmo_init")
expect("@timestamp Init===> Obj=AO_Gizmo,State=Gizmo_init")
expect_run()
expect("@timestamp AO-GetL  Obj=AO_Gizmo,Evt<Sig=START_SIG,*")
expect("@timestamp Disp===> Obj=AO_Gizmo,Sig=START_SIG,State=Gizmo_init")
expect("@timestamp USER+005 START")
expect("@timestamp ===>Tran Obj=AO_Gizmo,Sig=START_SIG,State=Gizmo_init->Gizmo_on_mains")

test('''
QACTIVE_START(AO_Gizmo) with status==0,
should go to state "on_battery"
''')
current_obj(OBJ_AP, 'status')
poke(0, 1, pack('<B', 0))  # status = 0
glb_filter(GRP_ALL)
continue_test()
expect("===RTC===> St-Init  Obj=AO_Gizmo,State=QHsm_top->Gizmo_init")
expect("@timestamp AO-Post  Sdr=AO_Gizmo,Obj=AO_Gizmo,Evt<Sig=START_SIG,*")
expect("===RTC===> St-Entry Obj=AO_Gizmo,State=Gizmo_init")
expect("@timestamp Init===> Obj=AO_Gizmo,State=Gizmo_init")
expect_run()
expect("@timestamp AO-GetL  Obj=AO_Gizmo,Evt<Sig=START_SIG,*")
expect("@timestamp Disp===> Obj=AO_Gizmo,Sig=START_SIG,State=Gizmo_init")
expect("@timestamp USER+005 START")
expect("@timestamp ===>Tran Obj=AO_Gizmo,Sig=START_SIG,State=Gizmo_init->Gizmo_on_battery")

