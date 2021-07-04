# Kronos Flight Library

## Pre-requisites

In order to compile Kronos, the following software is required:

- [x] CLion
- [x] BOSSA (Basic Open Source SAM-BA Application)
- [x] MSYS2 (Minimal System 2) (for Windows users only)
- [x] ARM GCC Toolchain
- [x] OpenOCD (Open On-Chip Debugger)

Additionally, you will need a SAM MCU on which to upload and run the code. The SAM E70 Xplained is used in this tutorial.

### CLion
CLion is a cross platform C++ IDE distributed by JetBrains. It is available for Windows, Mac OS, and Linux. You may get a free student license if you connect your JetBrains account to a GitHub account with the GitHub Student Pack. I strongly recommend using CLion and any of the other JetBrains IDEs as they provide tons of useful features that facilitate development. CLion's code completion and warnings seem to be much more robust than Visual Studio and it provides CMake support out of the box. You may download CLion [from here](https://www.jetbrains.com/clion/).

### BOSSA
BOSSA is a simple application that allows us to upload binary executables to various Atmel SAM devices. You can download the Windows and Mac OS binaries from the GitHub page [here](https://github.com/shumatech/BOSSA/releases). Linux users can download and install the `bossa-cli` package using the Advanced Package Tool (APT). Run the following command to retrieve the package on Debian and Ubuntu distributions:

```Bash
sudo apt-get install bossa-cli
```

Once installed, you may verify the installation by running the following command:

```Bash
bossac -h
```

If the help manual for the `bossac` command is output to the screen, then BOSSA is properly installed on your system.

### MSYS2
MSYS2 provides an easy-to-use building environment for Windows. We will be using MinGW 64 packages as they fit our use-case very well. Download and install MSYS2 from [here](https://www.msys2.org/). Once installed, open the MSYS2 shell and update the package repository with the following command:

```Bash
pacman -Syu
```

You may need to execute the command multiple times until you see a **`there is nothing to do`** prompt. MSYS2 is only required for Windows users. For Linux users, MSYS2 is not needed as you may simply install the packages using your distribution's package manager. The same goes for Mac OS users, which can use Homebrew to get the same packages.

### Binary Packages

#### OpenOCD
###


## Configuration
Once all the pre-requisites are installed, you may clone this repository and open it as a CLion project. Before running the project, you will need to setup a CMake profile with the correct settings. To do so, follow these steps:

1. In CLion, open the **File** => **Settings** menu from the toolbar.

2. On the left-hand side of the window, navigate to **Build, Execution, Deployment** => **CMake**.

3. Create a new CMake profile using the **+** icon.

4. Set the **Build Type** to **Release** (always use **Release** on Windows as **Debug** changes DLL symbols).

5. Add the following CMake options, where **_\<PATH TO VCPKG\>_** should point to the directory where **_vcpkg_** was cloned:
    ```
    -DCMAKE_TOOLCHAIN_FILE=<PATH TO VCPKG>/scripts/buildsystems/vcpkg.cmake
    ```
   
6. Delete the __cmake-build-*__ directories and reload the CMake project.

7. Run the project through the CMake profile created earlier.

**Note:** _You must have configured your toolchain before being able to compile the project in CLion. I used a Visual Studio toolchain on Windows. If you do the same, make sure to check that your Visual Studio is up-to-date through the Visual Studio Installer. Be sure to set the toolchain's platform to the appropriate 64-bit architecture for your system (ex: **amd64**)._