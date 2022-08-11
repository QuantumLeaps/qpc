# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...

# tests...

test("LedBar2 0% all off")
command(0, 0)  # call LedBar2_setPercent(0)
expect("@timestamp LED_MOD Led_off 0")
expect("@timestamp LED_MOD Led_off 1")
expect("@timestamp LED_MOD Led_off 2")
expect("@timestamp LED_MOD Led_off 3")
expect("@timestamp LED_MOD Led_off 4")
expect("@timestamp USER+000 LedBar2_setPercent 0 0")
expect("@timestamp Trg-Done QS_RX_COMMAND")


test("LedBar2 100% all on")
command(0, 100)
expect("@timestamp SPI_MOD SPI_beginTransaction")
expect("@timestamp LED_MOD Led_on 10 0")
expect("@timestamp SPI_MOD SPI_transfer 10")
expect("@timestamp LED_MOD Led_on 20 1")
expect("@timestamp SPI_MOD SPI_transfer 20")
expect("@timestamp LED_MOD Led_on 10 2")
expect("@timestamp SPI_MOD SPI_transfer 10")
expect("@timestamp LED_MOD Led_on 20 3")
expect("@timestamp SPI_MOD SPI_transfer 20")
expect("@timestamp LED_MOD Led_on 10 4")
expect("@timestamp SPI_MOD SPI_transfer 10")
expect("@timestamp SPI_MOD SPI_endTransaction")
expect("@timestamp USER+000 LedBar2_setPercent 70 100")
expect("@timestamp Trg-Done QS_RX_COMMAND")


test("LedBar2 200% ASSERT")
command(0, 200) # call LedBar2_setPercent(200)
expect("@timestamp =ASSERT= Mod=LedBar2,Loc=100")


test("LedBar2 19% all off")
command(0, 19) # call LedBar2_setPercent(19)
expect("@timestamp LED_MOD Led_off 0")
expect("@timestamp LED_MOD Led_off 1")
expect("@timestamp LED_MOD Led_off 2")
expect("@timestamp LED_MOD Led_off 3")
expect("@timestamp LED_MOD Led_off 4")
expect("@timestamp USER+000 LedBar2_setPercent 0 19")
expect("@timestamp Trg-Done QS_RX_COMMAND")


test("LedBar2 20% one on")
command(0, 20) # call LedBar2_setPercent(20)
expect("@timestamp SPI_MOD SPI_beginTransaction")
expect("@timestamp LED_MOD Led_on 10 0")
expect("@timestamp SPI_MOD SPI_transfer 10")
expect("@timestamp SPI_MOD SPI_endTransaction")
expect("@timestamp LED_MOD Led_off 1")
expect("@timestamp LED_MOD Led_off 2")
expect("@timestamp LED_MOD Led_off 3")
expect("@timestamp LED_MOD Led_off 4")
expect("@timestamp USER+000 LedBar2_setPercent 10 20")
expect("@timestamp Trg-Done QS_RX_COMMAND")


test("LedBar2 50% two on FLAWED")
current_obj(OBJ_AP, 'led_power')
poke(0, 4, pack('<LL', 23, 17)) # change return values for Led 0,1
command(0, 50) # call LedBar2_setPercent(30)
expect("@timestamp LED_MOD Led_on 23 0")
expect("@timestamp LED_MOD Led_on 17 1")
expect("@timestamp LED_MOD Led_off 2")
expect("@timestamp LED_MOD Led_off 3")
expect("@timestamp LED_MOD Led_off 4")

expect("@timestamp SPI_MOD SPI_beginTransaction")
expect("@timestamp SPI_MOD SPI_transfer 23")
expect("@timestamp SPI_MOD SPI_transfer 17")
expect("@timestamp SPI_MOD SPI_endTransaction")

expect("@timestamp USER+000 LedBar2_setPercent 40 50")
expect("@timestamp Trg-Done QS_RX_COMMAND")


test("LedBar2 99% four on FLAWED")
command(0, 99) # call LedBar_setPercent(99)
expect("@timestamp SPI_MOD SPI_beginTransaction")
expect("@timestamp LED_MOD Led_on 10 0")
expect("@timestamp SPI_MOD SPI_transfer 10")
expect("@timestamp LED_MOD Led_on 20 1")
expect("@timestamp SPI_MOD SPI_transfer 20")
expect("@timestamp LED_MOD Led_on 10 2")
expect("@timestamp SPI_MOD SPI_transfer 10")
expect("@timestamp LED_MOD Led_on 20 3")
expect("@timestamp SPI_MOD SPI_transfer 20")
expect("@timestamp LED_MOD Led_off 4")
expect("@timestamp SPI_MOD SPI_endTransaction")
expect("@timestamp USER+000 LedBar2_setPercent 60 99")
expect("@timestamp Trg-Done QS_RX_COMMAND")
