#
# This config file was tested with J-Link GDB Server v4.04
#

# Listening for commands on this PC's tcp port 2331
target remote localhost:2331

# Enable flash download and flash breakpoints.
# Flash download and flash breakpoints are features of
# the J-Link software which require separate licenses 
# from SEGGER.

# Select flash device
monitor flash device = AT91SAM7S64

# Enable FlashDL and FlashBPs
monitor flash download = 1
monitor flash breakpoints = 1

# Set gdb server to little endian
monitor endian little

# Set JTAG speed to 30 kHz
monitor speed 30

# Reset the chip to get to a known state.
monitor reset 8
monitor sleep 10

#
# Disable the watchdog and setup the PLL
#

# WDT_MR, disable watchdog 
monitor writeu32 0xFFFFFD44 = 0x00008000

# CKGR_MOR
monitor writeu32 0xFFFFFC20 = 0x00000601
monitor sleep 10

# CKGR_PLLR
monitor writeu32 0xFFFFFC2C = 0x00480a0e
monitor sleep 10

# PMC_MCKR
monitor writeu32 0xFFFFFC30 = 0x00000007
monitor sleep 10

# PMC_IER
monitor writeu32 0xFFFFFF60 = 0x00480100
monitor sleep 100

# Set JTAG speed in khz
monitor speed 12000

load
break main
continue