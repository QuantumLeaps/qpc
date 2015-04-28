This example demonstrates how to use the uVision IDE together with
the MDK-ARM toolchain.


uVision Project File
--------------------
The MDK-ARM uVision project file provided with this example
uses relative paths to the QP/C framework location (includes,
port, and libraries. These relative paths must be modified
when the project is moved to different relative location.


Startup Code
------------
The startup file startup_TM4C123GH6PM.s provides a template
of the recommended startup for QP projects. The file needs
to be customized to adjust the stack/heap sizes and to implement
the product-specific error/assertion handling policy
in the assert_failed() function.


  