{#mainpage} Spacecraft Framework
============

This framework was originally developed by [Space Concordia](http://www.spaceconcordia.ca/en/) members to be used in [SC-ODIN](http://www.spaceconcordia.ca/en/division/spacecraft/).

## Overview

Kronos is a component-driven framework inspired by the [Fprime](https://github.com/nasa/fprime) architecture. The framework splits up a complex system into various components. A component is defined as an isolated collection of logic created to achieve a single specific goal. Components communicate between each other through the use of busses by publishing or listening to events carried through them. Each bus may only carry one event type and is used to directionally link various components together. 

More detail about each part of the architecture is included in the sections below.

The framework is currently built around FreeRTOS and Reliance Edge but, as a future endeavour, it will be modulated to allow for support of other operating systems and file systems.

## Installation

This upcoming section will be dedicated to the setup of a build environment for Kronos on Windows, Linux and Mac OS.

### Pre-requisites

- [x] CLion
- [x] Atmel SAM Microcontroller 

#### CLion

CLion is a cross platform C++ IDE distributed by JetBrains. It is available for Windows, Mac OS, and Linux. You may get a free student license if you connect your JetBrains account to a GitHub account with the GitHub Student Pack. I strongly recommend using CLion and any of the other JetBrains IDEs as they provide tons of useful features that facilitate development. CLion's code completion and warnings seem to be much more robust than Visual Studio and it provides CMake support out of the box. You may download CLion [from here](https://www.jetbrains.com/clion/). It is possible to setup this project for other IDEs but I will only be providing instructions for CLion in this tutorial.

#### Atmel SAM Microcontroller 

In order to run the demo application, a SAM microcontroller is needed. This framework was built and tested using a SAM E70 Xplained evaluation board. However, this framework is made to be compatible with all SAM microcontrollers. For other MCUs, you will need to provide the correct architecture, CPU, board and other needed parameters. You may take a look at the [sam_gcc.cmake](../build/sam_gcc.cmake) toolchain file for more details.

#### Windows

For Windows users, all the binaries required for compilation can be installed through the MinGW64 MSYS2 port. We will use MinGW because it is the only toolchain on Windows that is supported by CLion for OpenOCD embedded development, as stated [here](https://www.jetbrains.com/help/clion/openocd-support.html). All the required packages are available for MinGW, notably the Open On-Chip Debugger (OpenOCD) which is required to upload and debug code on Atmel SAM MCUs.

#### MSYS2

MSYS2 provides an easy-to-use building environment for Windows. We will be using MinGW64 packages as they are the only supported ones by CLion. Download and install MSYS2 from [here](https://www.msys2.org/). Once installed, open the MSYS2 shell and update the package repository with the following command:

```Bash
pacman -Syu
```

You may need to execute the command multiple times until you see a **`there is nothing to do`** prompt. Once ready, you may proceed to installing all the needed packages. Simply run the following command and the system will automatically fetch the required binaries for you.

```Bash
pacman -Syu mingw-w64-x86_64-toolchain mingw-w64-x86_64-make mingw-w64-x86_64-cmake mingw-w64-x86_64-openocd mingw-w64-x86_64-arm-none-eabi-gcc mingw-w64-x86_64-arm-none-eabi-binutils
```

The full list of packages is included below in a more readable list format.

- [x] `mingw-w64-x86_64-toolchain`
- [x] `mingw-w64-x86_64-make`
- [x] `mingw-w64-x86_64-cmake` 
- [x] `mingw-w64-x86_64-openocd`
- [x] `mingw-w64-x86_64-arm-none-eabi-gcc`
- [x] `mingw-w64-x86_64-arm-none-eabi-binutils`

#### CLion Configuration

Once all the required packages are finished installing, you may configure your MinGW toolchain in CLion. To do so, follow these instructions: 

1. Open the `kronos` folder as a CLion project.

2. In CLion, open the **File** => **Settings** menu from the toolbar.

3. On the left-hand side of the window, navigate to **Build, Execution, Deployment** => **Toolchains**.

4. Create a new toolchain by click on the **+** button and selecting `MinGW`.

5. On the right-hand side panel, under `Environment`, navigate to your MinGW home folder. This folder should be in the MSYS installation directory and is called `mingw64`. The default path if you do not change the MSYS installation directory is `C:\msys64\mingw64`. Make sure that CLion recognizes the MinGW toolchain.

6. In the same panel, change both the C and C++ compilers to point to the `arm-none-eabi-gcc.exe` and `arm-none-eabi-gcc.exe` executables respectively. These executables are located in the `bin` folder of the MinGW home directory.

7. Still in the same dialog, change the debugger path to point to the `gdb-multiarch.exe` executable. Just like the compilers, the GDB executable is located in the same `bin` folder. Ignore any warning about the debugger version, version 10 is not technically fully supported by CLion but it should work good enough.

8. Set the toolchain to the be your default toolchain. You can do this but putting on the top of the list by promoting it with the upward caret (`^`) button on the left-hand side.

This leaves you with a working MinGW toolchain for CLion. However, there are a few more steps to get OpenOCD to work and build the CMake project. The following steps will help you configure your OpenOCD path and CMake targets.

1. In CLion, open the **File** => **Settings** menu from the toolbar.

2. On the left-hand side of the window, navigate to **Build, Execution, Deployment** => **Embedded Development**.

3. On the right-hand side panel, configure the OpenOCD path to point to the `openocd.exe` executable. Same as before, the executable is located in the `bin` folder of your MinGW home directory. You may check that the OpenOCD executable is functional by clicking the `Test` button afterwards. Ignore the STM32CubeMX location, it is not needed to build the project. 

4. On the left-hand side of the window, navigate to **Build, Execution, Deployment** => **CMake**.

5. Create two new CMake profiles and name them `Debug` and `Release`. This is done through the **+** button on the right-hand side panel.

6. Set the build types for each of the two new profiles you created appropriately by selecting in the dropdown on the right.

7. In the `CMake Options` text field, copy-paste the following text and change `<SAM GCC Toolchain File>` to point to the `sam_gcc.cmake` file located in the `lib/build` folder of the project. Use an absolute path when pointing to the toolchain file. Make sure to put this definition in both CMake profiles (Debug and Release).

```
-DCMAKE_TOOLCHAIN_FILE="<SAM GCC Toolchain File>"
```

With that, your CLion is ready to go. You may now right-click on the `CMakeLists.txt` file in the project root and select `Load CMake Project`. If all goes well, you should get a `Build files have been written to:` message in the CMake console output. You should also now see two new directories marked in red respectively name `cmake-build-debug` and `cmake-build-release`, where the library will be built.
 
#### Adding a Run Configuration

To check that your build environment is correctly setup, you may attempt to build the Kronos demo app. To do so, plug in your SAM board to your PC using the debug USB port. Make sure it is already flashed once with some working Software. You may do this through Atmel Studio using an example project. When you have plugged your board in and flashed it once, follow these instructions to add a run configuration:

1. Open the run configurations dialog in the top-right corner of your CLion project window, in between the build and run icons. 

2. Remove any automatically added configurations and add a new configuration called `Kronos_Demo` by clicking the **+** button on the left-hand side of the dialog. When prompted for the configuration type, select `OpenOCD Download & Run`.

3. On the right-hand side of the dialog, select `Kronos_Demo.bin` and `Kronos_Demo` respectively from the dropdowns of the `Target` and `Executable` fields.

4. Set the board configuration file to the file matching the board you are using the run the software. In my case, this is `atmel_same70_xplained.cfg` as I am using the SAM E70 Xplained evaluation kit. Use the `Assist` button to the right of the field to find the correct file.

5. Apply your changes and close the dialog. You should now be able to build and run `Kronos_Demo` in debug mode to verify that your toolchain configuration works. If you see the `Kronos_Demo.bin` being built successfully and the board's LED blinking, then your setup is working as expected. Bonus points to those of you that can hit a breakpoint that you placed in the source code.

#### Linux

Coming Soon!

#### Mac OS

Coming Soon!

## Components

As mentioned above, a component is a collection of logic used to achieve a targeted goal. For instance, if logging is a requirement of your system, you may want to create a _**Logger**_ component which serves to handle logging to a standard output and/or file, and possibly includes various log levels to easily filter incoming data.

It is important to note that components are self-contained, meaning that they are not aware of the source of the events they are processing. This allows for modularity, as it is possible to add or remove certain components without affecting the rest of the system.

There are three types of components: Passive, Queued, and Active, explained in detail below.

### Passive Components

A passive component is the simplest type of component. It does not provide its own thread or queue. A passive component consists of a collection of functions that will always be synchronously invoked on the calling thread/component. It essentially serves as a static library of functions. 

Passive components are made to be invoked by active or queued components. An example of a passive component could be a _**File Manager**_ component in charge of tracking files used and providing thread-safe file operations for other components. Such operations do not need to run on the same thread and can instead be executed by the calling components.

### Queued Components

A queued component implements a queue used to store incoming events which must be cleared by another active component. When its queue is cleared, all events in the queue are processed and the queue is emptied, ready to receive more events. Queued components are similar to passive components in the sense that they do not provide their own thread. However, unlike passive components, incoming events are processed asynchronously and not on the calling component's thread. Events are instead processed by another active component whose job is to intermittently clear the queued component's event queue. 

Queued components are useful in applications where specific inexpensive logic must be executed periodically but does not warrant its own thread. A good example is a scheduled component such as a health monitoring component whose job is to regularly make sure that all active threads are performing their tasks. 

### Active Components

In addition to having a queue, active components also provide an execution thread. Active components process events in a similar asynchronous manner to how queued components work. The difference is that instead of having their queue cleared by another component, they process events in their queue one-by-one in their own execution thread. The implications are the same as in the case of queued components, were the logic is not executed on the calling thread but is instead executed on the component's own thread.

Active components are used when components need to coordinate other parts of the system and execute costly logic. These components are the critical components of the system that must remain responsive and timely. In general, there will usually be few active components in a system as maintaining more threads becomes more expensive for the OS' task scheduler. A good example of an active component might be a component that would be in charge of running attitude determination and control logic for the spacecraft. Such a component will need to call upon various other components and is mission critical, warranting a separate execution thread.

## Busses

Busses are used to connect the various components of a system together. Each bus is limited to a carrying single event type and may not switch the event type it is carrying at runtime.

Busses are designed to cover all possible connection cases between components. This means that a bus can be used as a one-to-one, many-to-one, one-to-many, and many-to-many connection. However, there are two main types of busses (synchronous and asynchronous), each with its own restrictions.

It is important to note that busses are used through bus invocations. A bus invocation may be synchronous or asynchronous and is determined by the type of the bus that is being invoked. A bus invocation may need an input message struct depending on the type of message being sent. A bus invocation may also have a return type, only if it is synchronous. In _**ALL CASES**_, it is the _**RECEIVING COMPONENT's JOB**_ to delete any message data received, as it is _**ALWAYS**_ copied before being sent.

### Synchronous Busses

A synchronous bus is, as its name suggests, always invoked synchronously, meaning on the calling thread. This allows it to provide a return value but also imposes some restrictions on the type of connections it can be used to create. Most prominently, synchronous busses may _**ONLY HAVE AT MOST ONE RECEIVING COMPONENT**_ and that component _**MUST BE A PASSIVE COMPONENT**_. This is with good reason as only passive components allow for execution on the calling thread, and the invocation can therefore be treated just like a regular function call. A synchronous bus invocation will always return a void pointer. The framework provides templated functions to automatically cast the result to the needed type. This also means that to invoke a synchronous bus, a user _**MUST KNOW THE RETURN TYPE**_ of the operation they are requesting. A synchronous bus may have _**MULTIPLE PUBLISHING COMPONENTS**_. It is also important to note that the operation of emptying the queue of a queued component is itself a specific synchronous bus invocation.

### Asynchronous Busses

In contrast, an asynchronous bus is generally invoked on one thread but the event sent is processed in another. Note that it is still possible that an event sent through an asynchronous bus is processed on the same thread that it is sent from. In such cases, the invocation is still processed asynchronously, as if it were sent and received on different threads. Asynchronous bus invocations do not return values and are non-blocking as they execute on a separate thread. Asynchronous busses are not restricted in terms of the number of publishing or receiving components. In addition, components attached to an asynchronous bus are not restricted in type. An asynchronous bus invocation that elicits a response may provide a return bus to which the response will be published. Similar to synchronous bus invocations, the frameworks provides a templated function which copies a specific struct to be sent as input data to an asynchronous invocation.