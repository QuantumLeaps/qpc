About the lwIP Web-Server
=========================
This folder contains the HTTP (1.0) web-server code for the lwIP
TCP/IP stack, which is NOT based on "protothreads". The web-server
(HTTP daemon) supports simple CGI (Common Gateway Interface)
scripting as well as SSI (Server-Side Includes). The Web-Server
implementation is provided in the files httpd.h/httpd.c, which you
need to include in your application build.


Simple ROM-Based File System
============================
The HTTP server relies on a simple ROM-based File System to define
the content of the served website. The File System supports
hierarchical directories and files. The File System implementation
is provided in the files fs.h/fs.c, which you need to include in your
application build.


The qfsgen Utility to Generate ROM-Based Websites
=================================================
The website to be served by the lwIP Web-Server is ultimately stored
as constant data in ROM of the microcontroller.

Quantum Leaps provides a cross-platform, command-line utility, called
"qfsgen", which converts a given folder (containing your website) into
a C header file that contains a bunch of constant byte arrays representing
your files and directories. This generated file (fsdata.h) is included
in the fs.c ROM-Based File System implementation.

The "qfsgen" utility (both compiled executable and source code) is
provided in the Qtools collection, available for download from:

https://sourceforge.net/projects/qpc/files/Qtools

***
NOTE: The qfsgen utility is a simple executable (versions are available
for Windows and Linux), which does not require any interpreters to run
(such as Python, etc.)
***

The usage of the qfsgen utility is as follows:

qfsgen [root-dir [output-file]] [-h]

when qfsgen is invoked without parameters, it processes the folder
fs in the current directory and generates file fsdata.h also in the
current directory. 
