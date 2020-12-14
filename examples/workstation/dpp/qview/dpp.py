# This is an example of QView customization for a specific application
# (DPP in this case). This example animates the Phil images on the
# QView canvas. Additionally, there is a button in the middle of the screen,
# which, when clicked once pauses the DPP ("forks" are not being served).
# A second click on the button, "un-pauses" the DPP ("forks" are served
# to all hungry Philosophers).
#
# This version of the DPP customization uses the application-specific
# packet QS_USER_00 (PHILO_STAT) produced when the status of a Philo changes.
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
        #
        # Howver, this is a desktop appliction, which you cannot reset
        # (and restart). Therefore, the desktop applications must be started
        # *after* the QView is already running.
        #reset_target()


    # on_reset() callback
    def on_reset(self):
        # clear the lists
        self._philo_obj   = [0, 0, 0, 0, 0]
        self._philo_state = [0, 0, 0]

    # on_run() callback
    def on_run(self):
        glb_filter("QS_USER_00")

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

    # intercept the QS_USER_00 application-specific packet
    # this packet has the following structure (see bsp.c:displayPhilStat()):
    # record-ID, seq-num, Timestamp, format-byte, Philo-num,
    #    format-bye, Zero-terminated string (status)
    def QS_USER_00(self, packet):
        # unpack: Timestamp->data[0], Philo-num->data[1], status->data[3]
        data = qunpack("xxTxBxZ", packet)
        i = data[1]
        j = ("t", "h", "e").index(data[2][0]) # the first letter

        # animate the given philo image according to its activity
        QView.canvas.itemconfig(self._philo_img[i], image=self._act_img[j])

        # print a message to the text view
        QView.print_text("%010d Philo %1d is %s"%(data[0], i, data[2]))

#=============================================================================
QView.customize(DPP()) # set the QView customization
