# include script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_setup():
    expect("@timestamp FIXTURE_SETUP")

def on_teardown():
    expect("@timestamp FIXTURE_TEARDOWN")

def command_x(par1=0, par2=0, par3=0):
    command("COMMAND_X", par1, par2, par3)

def curr_ap_obj(obj_name):
    current_obj(OBJ_AP, obj_name)

