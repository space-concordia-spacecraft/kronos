## Installation

This upcoming section will be dedicated to the setup of a build environment for Kronos on Windows, Linux and Mac OS.

### Pre-requisites

- [x] CLion
- [x] Atmel SAM Microcontroller

#### CLion

CLion is a cross platform C++ IDE distributed by JetBrains. It is available for Windows, Mac OS, and Linux. You may get a free student license if you connect your JetBrains account to a GitHub account with the GitHub Student Pack. I strongly recommend using CLion and any of the other JetBrains IDEs as they provide tons of useful features that facilitate development. CLion's code completion and warnings seem to be much more robust than Visual Studio and it provides CMake support out of the box. You may download CLion [from here](https://www.jetbrains.com/clion/). It is possible to setup this project for other IDEs but I will only be providing instructions for CLion in this tutorial.

#### Atmel SAM Microcontroller

In order to run the demo application, a SAM microcontroller is needed. This framework was built and tested using a SAM E70 Xplained evaluation board. However, this framework is made to be compatible with all SAM microcontrollers. For other MCUs, you will need to provide the correct architecture, CPU, board and other needed parameters. You may take a look at the [sam_gcc.cmake](../build/sam_gcc.cmake) toolchain file for more details.

### Windows

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

6. In the same panel, change both the C and C++ compilers to point to the `arm-none-eabi-gcc.exe` and `arm-none-eabi-g++.exe` executables respectively. These executables are located in the `bin` folder of the MinGW home directory.

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

#### Console Configuration

In order to be able to debug it's also useful to have logging tools that allow you to print to a terminal. To achieve this, we have used PuTTY to read data being transferred from the COM port that is connected to your board.

To create this console, the PuTty application must be first downloaded. You can download PuTTY here: https://www.putty.org/

Once you have downloaded the application, you're able to set up the console application by going to Edit Configuration at the top next to the hammer and then creating a new Custom Build Application. You can name this application whatever you want but for the purpose of this tutorial, we will name it "Console".

The first step will be to create a proper target. To do this you must click on "Configure Custom Build Targets". A new window will pop up, add a new custom build target by clicking the "+" sign. You can name this whatever you want, we have named it "Empty" on our end. Click the triple dot button next to the Build dropdown, then click on "+" to add a new external tool. Name it "cmd" and add the following properties:

For "Program":

```
C:\Windows\System32\cmd.exe
```

For "Arguments":

```
/c "exit 0"
```

For "Working Directory":

```
$ProjectFileDir$
```

Once you have completed those steps you may click Ok on all. Add the newly created Target by clicking on the dropdown and selecting it.

The second step will be to select plink.exe from the PuTTY directory as the executable. The path will probably look something like: ``C:\Program Files\PuTTY\plink.exe``.

Finally, in the program arguments copy and paste the following line:

```
-serial COM3 -sercfg 115200,8,1,N,N
```

Make sure that "COM3" is changed to the appropriate COM port. You can find this out by going to your Device Managers and checking the ports.

Once you have completed these steps, you may run the console application to test it out. Make sure the software is running on the board.

#### Adding a Run Configuration

To check that your build environment is correctly setup, you may attempt to build the Kronos demo app. To do so, plug in your SAM board to your PC using the debug USB port. Make sure it is already flashed once with some working Software. You may do this through Atmel Studio using an example project. When you have plugged your board in and flashed it once, follow these instructions to add a run configuration:

1. Open the run configurations dialog in the top-right corner of your CLion project window, in between the build and run icons.

2. Remove any automatically added configurations and add a new configuration called `Kronos_Demo` by clicking the **+** button on the left-hand side of the dialog. When prompted for the configuration type, select `OpenOCD Download & Run`.

3. On the right-hand side of the dialog, select `Kronos_Demo.bin` and `Kronos_Demo` respectively from the dropdowns of the `Target` and `Executable` fields.

4. Set the board configuration file to the file matching the board you are using the run the software. In my case, this is `atmel_same70_xplained.cfg` as I am using the SAM E70 Xplained evaluation kit. Use the `Assist` button to the right of the field to find the correct file.

5. Apply your changes and close the dialog. You should now be able to build and run `Kronos_Demo` in debug mode to verify that your toolchain configuration works. If you see the `Kronos_Demo.bin` being built successfully and the board's LED blinking, then your setup is working as expected. Bonus points to those of you that can hit a breakpoint that you placed in the source code.

### Linux

Coming Soon!

### Mac OS

Coming Soon!