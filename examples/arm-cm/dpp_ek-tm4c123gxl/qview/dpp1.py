# This is an example of QView customization for a specific application
# (DPP in this case). This example animates the Phil images on the
# QView canvas. Additionally, there is a button in the middle of the screen,
# which, when clicked once pauses the DPP ("forks" are not being served).
# A second click on the button, "un-pauses" the DPP ("forks" are served
# to all hungry Philosophers).
#
# This version of the DPP customization uses the standard QS_QEP_STATE_ENTRY
# packet, which provides information about the current states of the dining
# Philosophers. The example also demonstrates how to intercept the QS
# "dictionary" records QS_OBJ_DICT and QS_FUN_DICT to extract the information
# about the addresses of the Philosopher objects and the states of their
# state machines.
#

class DPP:
    def __init__(self):

        # add commands to the Custom menu...
        QView.custom_menu.add_command(label="Custom command",
                                      command=self.cust_command)

        # configure the custom QView.canvas...
        QView.show_canvas() # make the canvas visible
        QView.canvas.configure(width=400, height=260)

        # tuple of activity images (correspond to self._philo_state)
        self._act_img = (
            PhotoImage(file=HOME_DIR + "/img/thinking.gif"),
            PhotoImage(file=HOME_DIR + "/img/hungry.gif"),
            PhotoImage(file=HOME_DIR + "/img/eating.gif"),
        )
        # tuple of philo canvas images (correspond to self._philo_obj)
        self._philo_img = (\
            QView.canvas.create_image(190,  57, image=self._act_img[0]),
            QView.canvas.create_image(273, 100, image=self._act_img[0]),
            QView.canvas.create_image(237, 185, image=self._act_img[0]),
            QView.canvas.create_image(146, 185, image=self._act_img[0]),
            QView.canvas.create_image(107, 100, image=self._act_img[0])
        )

        # button images for UP and DOWN
        self.img_UP  = PhotoImage(file=HOME_DIR + "/img/BTN_UP.gif")
        self.img_DWN = PhotoImage(file=HOME_DIR + "/img/BTN_DWN.gif")

        # images of a button for pause/serve
        self.btn = QView.canvas.create_image(200, 120, image=self.img_UP)
        QView.canvas.tag_bind(self.btn, "<ButtonPress-1>", self.cust_pause)

        # request target reset on startup...
        # NOTE: Normally, for an embedded application you would like
        # to start with resetting the Target, to start clean with
        # Qs dictionaries, etc.
        reset_target()

    # on_reset() callback invoked when Target-reset packet is received
    # NOTE: the QS dictionaries are not known at this time yet, so
    # this callback shouild generally not set filters or current objects
    def on_reset(self):
        # (re)set the lists
        self._philo_obj   = [0, 0, 0, 0, 0]
        self._philo_state = [0, 0, 0]

    # on_run() callback invoked when the QF_RUN packet is received
    # NOTE: the QS dictionaries are typically known at this time yet, so
    # this callback can set filters or current objects
    def on_run(self):
        glb_filter("QS_QEP_TRAN")

        # NOTE: the names of objects for current_obj() must match
        # the QS Object Dictionaries produced by the application.
        current_obj(OBJ_AO, "Table_inst")

        # turn lists into tuples for better performance
        self._philo_obj = tuple(self._philo_obj)
        self._philo_state = tuple(self._philo_state)


    # example of a custom command
    def cust_command(self):
        command(1, 12345)

    # example of a custom interaction with a canvas object (pause/serve)
    def cust_pause(self, event):
        if QView.canvas.itemcget(self.btn, "image") != str(self.img_UP):
            QView.canvas.itemconfig(self.btn, image=self.img_UP)
            post("SERVE_SIG")
            QView.print_text("Table SERVING")
        else:
            QView.canvas.itemconfig(self.btn, image=self.img_DWN)
            post("PAUSE_SIG")
            QView.print_text("Table PAUSED")

    # intercept the QS_OBJ_DICT stadard packet
    # this packet has the following structure:
    # record-ID, seq-num, Object-ptr, Zero-terminated string
    def QS_OBJ_DICT(self, packet):
        data = qunpack("xxOZ", packet)
        try:
            # NOTE: the names of objects must match the QS Object Dictionaries
            # produced by the application.
            i = ("Philo_inst[0]",
                 "Philo_inst[1]",
                 "Philo_inst[2]",
                 "Philo_inst[3]",
                 "Philo_inst[4]").index(data[1])
            self._philo_obj[i] = data[0]
        except:
            pass # dictionary for a different object

    # intercept the QS_FUN_DICT stadard packet
    # this packet has the following structure:
    # record-ID, seq-num, Function-ptr, Zero-terminated string
    def QS_FUN_DICT(self, packet):
        data = qunpack("xxFZ", packet)
        try:
            # NOTE: the names of states must match the QS Object Dictionaries
            # produced by the application.
            j = ("Philo_thinking",
                 "Philo_hungry",
                 "Philo_eating").index(data[1])
            self._philo_state[j] = data[0]
        except:
            pass # dictionary for a different state

    # intercept the QS_QEP_TRAN stadard packet
    # this packet has the following structure:
    # record-ID, seq-num, Timestamp, Signal, Object-ptr,
    #     Function-ptr (source state), Function-ptr (new active state)
    def QS_QEP_TRAN(self, packet):
        data = qunpack("xxTSOFF", packet)
        try:
            i = self._philo_obj.index(data[2])
            j = self._philo_state.index(data[4])

            # animate the given philo image according to its activity
            QView.canvas.itemconfig(self._philo_img[i],
                                   image=self._act_img[j])
            # print a message to the text view
            QView.print_text("%010d Philo %d is %s"\
                    %(data[0], i, ("thinking", "hungry", "eating")[j]))
        except:
            pass # state-entry in a different object

#=============================================================================
# instantiate the DPP class and set it as the QView customization
QView.customize(DPP())
