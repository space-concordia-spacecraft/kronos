### Linux Installation [Arch]

The setup for Linux users is a lot simpler than on Windows. Keep in mind that this guide 
was tested on Manjaro only.

The following list of packages are required to build kronos on an Arch distribution of Linux:
- [x] `base-devel`
- [x] `arm-none-eabi-gcc`
- [x] `arm-none-eabi-gdb`
- [x] `arm-none-eabi-binutils`
- [x] `arm-none-eabi-newlib`
- [x] `openocd`
- [x] `cmake`

```bash
sudo pacman -Syu base-devel arm-none-eabi-gcc arm-none-eabi-gdb arm-none-eabi-binutils arm-none-eabi-newlib openocd cmake
```

## CLion Configuration

Once all the required packages are finished installing, you may configure your MinGW toolchain in CLion. To do so,
follow these instructions:

1. Open the `kronos` folder as a CLion project.

2. In CLion, open the `File` &rarr; `Settings` menu from the toolbar.

3. On the left-hand side of the window, navigate to `Build, Execution, Deployment` &rarr; `Toolchains`.

4. Create a new toolchain by click on the `+` button and selecting `System`.

5. In the same panel, change both the C and C++ compilers to point to the `arm-none-eabi-gcc.exe`
   and `arm-none-eabi-g++.exe` executables respectively. These executables are located in the `bin` folder. For example
   `bin/arm-none-eabi-g++`.

6. Still in the same dialog, change the debugger path to point to the `gdb-multiarch.exe` executable. Just like the
   compilers, the GDB executable is located in the same `bin` folder. Ignore any warning about the debugger version,
   version 10 is not technically fully supported by CLion but it should work good enough.

7. Set the toolchain as your default toolchain. You can do this but putting on the top of the list by promoting
   it with the upward caret (`^`) button on the left-hand side.

This leaves you with a working toolchain for CLion. However, there are a few more steps to get OpenOCD to work and
build the CMake project. The following steps will help you configure your OpenOCD path and CMake targets.

1. In CLion, open the `File` &rarr; `Settings` menu from the toolbar.

2. On the left-hand side of the window, navigate to `Build, Execution, Deployment` &rarr; `Embedded Development`.

3. On the right-hand side panel, configure the OpenOCD path to point to the `openocd.exe` executable. Same as before,
   the executable is located in the `bin` folder of your MinGW home directory. You may check that the OpenOCD executable
   is functional by clicking the `Test` button afterwards. Ignore the `STM32CubeMX` location, it is not needed to build
   the project.

4. On the left-hand side of the window, navigate to `Build, Execution, Deployment` &rarr; `CMake`.

5. Create two new CMake profiles and name them `Debug` and `Release`. This is done through the `+` button on the
   right-hand side panel.

6. Set the build types for each of the two new profiles you created appropriately by selecting in the dropdown on the
   right.

7. In the `CMake Options` text field, copy-paste the following text and change `<SAM GCC Toolchain File>` to point to
   the `sam_gcc.cmake` file located in the `lib/build` folder of the project. Use an absolute path when pointing to the
   toolchain file. Make sure to put this definition in both CMake profiles (`Debug` and `Release`).

```
-DCMAKE_TOOLCHAIN_FILE="<SAM GCC Toolchain File>"
```

With that, your CLion is ready to go. You may now right-click on the `CMakeLists.txt` file in the project root and
select `Load CMake Project`. If all goes well, you should get a `Build files have been written to:` message in the CMake
console output. You should also now see two new directories marked in red respectively named `cmake-build-debug`
and `cmake-build-release`, where the binaries will be built.

## Console Configuration

In order to be able to debug, logging tools are essential to retrieve outputs from the development board. To achieve
this, we will use PuTTY to read data being transferred from the COM port that is connected to your board.

To create this console configuration, the PuTTY application must be first downloaded. You can download
PuTTY [from here](https://www.putty.org/).

Once you have downloaded the application, you're able to set up the console application by going to Edit Configuration
at the top next to the hammer and then creating a new Custom Build Application. You can name this application whatever
you want but for the purpose of this tutorial, we will name it `Console`.

The first step will be to create a proper target. To do this you must click on `Configure Custom Build Targets`. A new
window will pop up, add a new custom build target by clicking the `+` sign. You can name this whatever you want, we have
named it `Empty` on our end. Click the triple dot button next to the Build dropdown, then click on `+` to add a new
external tool. Name it `cmd` and fill in the following properties:

| Option              | Value                         |
|---------------------|-------------------------------|
| `Program`           | `C:\Windows\System32\cmd.exe` |
| `Arguments`         | `/c "exit 0"`                 |
| `Working Directory` | `$ProjectFileDir$`            |

Once you have completed those steps you may click `OK` on all dialogs to go back to the configurations screen. Add the
newly created target by clicking on the dropdown and selecting it.

The second step will be to select `plink.exe` from the PuTTY directory as the executable. The default path to the PuTTY
executable is `C:\Program Files\PuTTY\plink.exe`. If you have changed the PuTTY installation directory, make sure to
select the correct executable.

Finally, in the program arguments copy and paste the following line:

```
-serial COM3 -sercfg 115200,8,1,N,N
```

Make sure that `COM3` is changed to the appropriate COM port. You can find this out by opening Device Manager on Windows
and checking the `Ports` section to find which port is active.

Once you have completed these steps, you may run the console application to test it out. Be sure to have the software
running on the board in order to see any output.

## Adding a Run Configuration

To check that your build environment is correctly setup, you may attempt to build the Kronos demo app. To do so, plug in
your SAM board to your PC using the debug USB port. Make sure it is already flashed once with some working Software. You
may do this through Atmel Studio using an example project. When you have plugged your board in and flashed it once,
follow these instructions to add a run configuration:

1. Open the run configurations dialog in the top-right corner of your CLion project window, in between the build and run
   icons.

2. Add a new configuration called `Kronos_Demo` by clicking the `+` button on the left-hand side of the dialog. When
   prompted for the configuration type, select `OpenOCD Download & Run`.

3. On the right-hand side of the dialog, select `Kronos_Demo.bin` and `Kronos_Demo` respectively from the dropdowns of
   the `Target` and `Executable` fields.

4. Set the board configuration file to the file matching the board you are using the run the software. In my case, this
   is `atmel_same70_xplained.cfg` as I am using the SAM E70 Xplained evaluation kit. Use the `Assist` button to the
   right of the field to find the correct file.

5. Apply your changes and close the dialog. You should now be able to build and run `Kronos_Demo` in debug mode to
   verify that your toolchain configuration works. If you see the `Kronos_Demo.bin` being built successfully and the
   board's LED blinking, then your setup is working as expected. Bonus points to those of you that can hit a breakpoint
   that you placed in the source code.