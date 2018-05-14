# cbmage 0.1
Print a picture from a modern PC with an old-fashioned Commodore MPS 803 compatible printer
## Purpose
This utility outputs raw bytes for printing on a [Commodore MPS 803](http://www.zimmers.net/cbmpics/p6serial3.html) compatible printer. It should be used with [opencbm](http://spiro.trikaliotis.net/opencbm) package by [Spiro Trikaliotis](http://spiro.trikaliotis.net/) to redirect the output to the printer, connected through a [XUM1541 cable](https://rdist.root.org/2009/01/21/introducing-xum1541-the-fast-c64-floppy-usb-adapter/), such as [ZoomFloppy](http://www.go4retro.com/products/zoomfloppy/).

## Contents of package
- **source** - Source code in Turbo Modula-2 (despite the name ".BAS")
  - [**cbmage.c**](https://github.com/sblendorio/cbmage/blob/master/source/cbmage.c) - Main program
  - [**stb_image.h**](https://github.com/sblendorio/cbmage/blob/master/source/stb_image.h) - [STB library](https://github.com/nothings/stb) by [Sean Barrett](https://twitter.com/nothings) for reading image files
  - [**Makefile**](https://github.com/sblendorio/cbmage/blob/master/source/Makefile) - Makefile
- **binaries** - Compiled .COM executable files for CP/M-80
  - **win32**/[**cbmage.exe**](https://github.com/sblendorio/cbmage/blob/master/binaries/win32/cbmage.exe?raw=true) Precompiled binary executable file for **Windows XP**
  - **win64**/[**cbmage.exe**](https://github.com/sblendorio/cbmage/blob/master/binaries/win64/cbmage.exe?raw=true) Precompiled binary executable file for **Windows 10**
  - **macOS**/[**cbmage**](https://github.com/sblendorio/cbmage/blob/master/binaries/macOS/cbmage?raw=true) Precompiled binary executable file for **macOS**, 64 bit
  - **linux64**/[**cbmage**](https://github.com/sblendorio/cbmage/blob/master/binaries/linux64/cbmage?raw=true) Precompiled binary executable file for **Linux** (x86_64)

## Requirements
- **macOS** or **Windows** (32/64 bit) or **Linux**
- [XUM1541](https://rdist.root.org/2009/01/21/introducing-xum1541-the-fast-c64-floppy-usb-adapter/) / [ZoomFloppy](http://www.go4retro.com/products/zoomfloppy/)
- [**opencbm**](http://spiro.trikaliotis.net/opencbm) package installed
- **gcc** if you want to compile from sources

## Install **opencbm**
To install [**opencbm**](http://spiro.trikaliotis.net/opencbm) you can use the installer from its website, or if you use a debian-based version of Linux, you can install it with:

`sudo apt-get install opencbm`

on macOS you can use a similar command:

`brew install opencbm`

## Compiling **cbmage**
Once you have **gcc** installed, just enter the **"source"** directory and launch:

`make`

An executable file named **"cbmage"** will be generated: it's ready to use.

## Using **cbmage**

Synopsis:

`cbmage <image file name>`

This is the basic syntax: it will simply write on *stdout* (so, the termainal window) the raw bytes that should be interpreted by the **MPS 803** printer. Quite useless.

## Let's use it with **opencbm**

The typical sequence of command you should use to do the task is:

    cbmctrl reset
    cbmctrl lock
    cbmctrl listen 4 0
    ./cbmage picture.png | cbmctrl write
    cbmctrl unlisten
    cbmctrl unlock

In particular, the **4th line** (`./cbmage picture.png | cbmwrite`) produces the raw byes (launch it **without "./"** if you run **on Windows**), which are redirected to the printer through the piped **cbmwrite** command

## Restrictions
**Maximum width** of the picture is **480 pixel**, the printer is a **black and white** one. The _recommended_ format is **PNG**: every single dot which is **white** (total white: `#ffffff` in hex) will be left blank, **every** other pixel color will result in a **black** dot on the printer.

## Sample printed **PNG** files
![MPS 803 - 1](http://www.sblendorio.eu/images/mps803-1.jpg)
![MPS 803 - 2](http://www.sblendorio.eu/images/mps803-3.jpg)
![MPS 803 - 3](http://www.sblendorio.eu/images/mps803-2.jpg)

