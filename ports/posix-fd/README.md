# POSIX-FD (pollfd-based event-loop)

This port is **NOT** a real-time kernel.

See other POSIX ports for a better effort at a real-time kernel.

This port can be used to run a QP/C application within the context of a
POSIX system where events are processed and mediated via a pollfd event-loop.

This port provides a QF_run() function (like other QP/C kernels) based on
looping around and polling on a file-descriptor (read end of a pipe) and
timing out for the next tick (QF_setTickRate).

Thus, this port is completely single-threaded (as opposed to POSIX-QV, which
uses a background pthread for clock ticks). The critical sections enter/exit
logic only guards against nesting of critical sections, but does not
protect against concurrent access.

The main benefit of this port is that event processing of the QP/C
application can be integrated within other frameworks that rely on polling
file-descriptors, as is common in POSIX software. To that end, this port
exposes internal functions that encapsulate the constituent parts of a
main event loop:

 - QF_preRun_: Initializes the main loop (global) variables.
 - QF_getReadyFD_: Gets the file-descriptor to poll, indicating QP/C has events to process.
 - QF_getExternalFDs_: Gets the list of file-descriptors added by the QP/C application.
 - QF_updateNextTick_: Advances the time at which the next tick will occur.
 - QF_getNextTimeoutMS_: Gets the number of milliseconds until the next tick (or -1 for infinite timeout).
 - QF_onReadySignal_: Processes the next queued event in the QP/C kernel.
 - QF_onExternalFDs_: Processes the external events (driven by file descriptors) added by the QP/C application.
 - QF_postRun_: Finalizes the main loop, i.e., free resources.

See QF_run for a basic event-loop that uses the above parts.
The intent is for an external event-loop system, based on polling, to be
able to integrate those parts in a manner appropriate for that framework.

As a single-threaded port, it is only safe to post or publish events to
QP/C while neither of the above functions (plus QF_onClockTick) are
running.

