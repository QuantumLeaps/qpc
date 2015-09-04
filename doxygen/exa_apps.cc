/*##########################################################################*/
/*! @page exa_apps Example Applications

<p>To demonstrate QP/C features, you need to create an application that does "something interesting". Instead of inventing this "something interesting" for each and every example, most of the example projects implement one of the three example applications, which are described on the separate pages:
</p>

- @subpage blinky
- @subpage dpp
- @subpage game

Additionally, the QP/C distribution contains several application examples described in the <a class="extern" target="_blank" href="http://www.state-machine.com/psicc2">PSiCC2</a> book.

- Calculator
- QHsmTst
- PELICAN
- ...

*/

/*##########################################################################*/
/*! @page blinky Simple Blinky Application

<p>The ultra-simple Blinky example is the embedded systems' equivalent of the venerable <i>"Hello World!"</i> program, that is, the simplest possible working QP application that does "something". In the case of Blinky, this "something" is blinking an LED at the rate of 1Hz, where an LED turns on and remains on for 0.5 seconds on then turns off and remains off for 0.5 seconds.
</p>

@image html blinky_ek-tm4c123gxl.gif Blinky on EK-TM4C123GLX (TivaC LaunchPad)

The ultra-simple Blinky application, which  consists of just one active object named `Blinky`, is intentionally kept small and illustrates only the most basic QP features, such as:

- defining a simple Blinky active object (AO) @ref oop "class";
- hand-coding the simple state machine of the Blinky AO;
- using a periodic time event;
- initializing the QP framework; and
- starting an AO.


------------------------------------------------------------------------------
@section blinky_sm State Machine
The very simple state machine of the Blinky AO is shown in the figure below:

@image html SM_blinky.png "State Machine of the Blinky AO"

<ul class="tag">
  <li><span class="tag">1</span> The top-most initial transition in this state machine arms a QP time event (QTimeEvt_armX()) to deliver the `TIMEOUT` signal every half second, so that the LED can stay on for one half second and off for the other half.
  </li>

  <li><span class="tag">2</span> The initial transition leads to state "off", which turns the LED off in the entry action  (`BSP_ledOff()`).
  </li>

  <li><span class="tag">3</span> When the `TIMEOUT` event arrives in the "off" state, the "off" state transitions to the "on" state
  </li>
  
  <li><span class="tag">4</span> The "on" state turns the LED on in the entry action (`BSP_ledOn()`).
  </li>
  
  <li><span class="tag">5</span> When the `TIMEOUT` event arrives in the "on" state, the "on" state transitions back to "off", which cases execution of the entry action, in which the LED is turned off. From that point on the cycle repeats forever because the `TIMEOUT` events keep getting generated at the pre-determined rate.
  </li>
</ul>

------------------------------------------------------------------------------
@section blinky_code State Machine Code
The Blinky state machine shown above is implemented in the blinky.c source file, as shown in the
listing below. The code has been specifically organized not to access target resources directly, but instead encapsulate all such access in the calls to the BSP (Board Support Package). So for example, instead of turning the LED on and off by writing to a specific GPIO register on an embedded board, the code calls the BSP functions `BSP_ledOn()` and `BSP_ledOff()`. These functions can then be defined differently for each Target board (or even a desktop workstation), without the need to change the state machine code.

@note
The Blinky source code (blinky.c) is actually the same on all platforms, including Windows
and the embedded boards. The only difference is in the Board Support Package (bsp.c), which is 
specific for the target.

@includelineno examples/arm-cm/blinky_ek-tm4c123gxl/blinky.c

As you can see, the structure of the state machine is very clearly recognizable in this code. Please refer to the Application Note <a class="extern" target="_blank" href="http://state-machine.com/doc/AN_Crash_Course_in_UML_State_Machines.pdf">A Crash Course in UML State Machines</a> for exact explanation of the state machine coding techniques.


@subsection blinky_ao Defining Active Object (AO) Class



- hand-coding the simple state machine of the Blinky AO;
- using a periodic time event;
- initializing the QP framework; and
- starting an AO.



@next{dpp}
*/
/*##########################################################################*/
/*! @page dpp Dining Philosophers Problem (DPP)

@image html dpp_win32.jpg

@image html under_construction.jpg

@next{game}
*/

/*##########################################################################*/
/*! @page game "Fly 'n' Shoot" Game

@image html game_win32.jpg

@image html under_construction.jpg

*/


