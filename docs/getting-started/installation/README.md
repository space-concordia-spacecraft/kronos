## Installation

This upcoming section will be dedicated to the setup of a build environment for Kronos on Windows, Linux and Mac OS.

### Pre-requisites

- [x] ARM EABI Compiler Collection
- [x] CLion
- [x] Atmel SAM Microcontroller

#### ARM EABI Compiler Collection

ARM EABI (Embedded Application Binary Interface) is a specific version of the ARM ABI (Application Binary Interface) that is optimized for use in embedded systems. It is a set of conventions for how programs should be compiled and linked for execution on ARM-based processors in embedded systems. This includes conventions for how data should be represented in memory, how functions should be called, and how the stack should be used. The EABI is designed to be more efficient than the standard ARM ABI, with smaller code size and faster execution.

#### CLion

CLion is a cross-platform C++ IDE distributed by JetBrains. It is available for Windows, Mac OS, and Linux. You can get
a free student license if you connect your JetBrains account to a GitHub account with the GitHub Student Pack. It is
strongly recommended to use CLion and any of the other JetBrains IDEs as they provide tons of useful features that
facilitate development. CLion's code completion and warnings seem to be much more robust than Visual Studio, and it
provides CMake support out of the box. You may download CLion [from here](https://www.jetbrains.com/clion/). It is
possible to set up this project for other IDEs but only CLion will be covered in this tutorial.

#### Atmel SAM Microcontroller

In order to run the demo application, a SAM microcontroller is needed. This framework was built and tested using a SAM
E70 Xplained evaluation board. However, this framework is made to be compatible with all SAM microcontrollers. For other
MCUs, you will need to provide the correct architecture, CPU, board and other needed parameters. You may take a look at
the [sam_gcc.cmake](../../build/sam_gcc.cmake) toolchain file for more details.

