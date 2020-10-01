/**
* @file
* @brief Application build time-stamp
* @note
* This module needs to be re-compiled in every new software build. To achive
* this, it is recommended to delete the object file (qstamp.o, or qstamp.obj)
* in the build directory before each build. (Most development tools allow
* you to specify a pre-build command, which is the ideal place to delete
* the qstamp object file.)
*/

#include "qstamp.h"

/*! the calendar date of the last translation of the form: "Mmm dd yyyy" */
char const Q_BUILD_DATE[12] = __DATE__;

/*! the time of the last translation of the form: "hh:mm:ss" */
char const Q_BUILD_TIME[9] = __TIME__;
