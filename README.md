## Table of Contents
- [Overview](#overview)
- [How to Build](#how-to-build)
- [Deploy](#deploy)
- [Python Wrapped Libraries](#python-wrapped-libraries)
- [License](#license)

## Overview

Project DRIFT (Drone Reconnaissance & Intelligent Flight Technology) is a research-oriented codebase for drone system development. It contains experimental implementations, prototyping tools and libraries. This project serves as a foundation for ongoing exploration in aerial robotics and embedded systems.

Current Features:
- MSP Library for RPI to Flight Controller comms

Future Features:
- autonomous flight via (inav waypoints)
- inter drone comms via (lora)
- sensor integration
- Drone Control systems

---

## How to Build

1. **Configure the project**

   Edit the `src/project_config.json` file and update the values with the appropriate settings:

   - `port`: Serial port to connect to  
   - `baudrate`: Communication speed  
   - Any other relevant settings

   > 💡 To list available serial ports:
   > ```bash
   > ls /dev/tty*
   > ```

2. **Setup the project**

    Run the provided setup script together with current architecture (posix/pi/zero):
   ```bash
   ./setup.sh posix
   #or
   ./setup.sh pi
   #or
   ./setup.sh zero
   ```
   > 💡 Note: Architecture corresponds to where your running the binaries
   > posix -> if running on linux pc
   > pi    -> if running on raspberry pi 4B
   > zero  -> if running on raspberry pi zero 2 W

3. **Build the project**

   Run the provided build script:
   ```bash
   ./build.sh
   ```

---

## deploy

you can find all the compiled binaries and executables in /build/[posix|zero|pi]/[beta|inav]/deploy folder

---

## Python Wrapped Libraries

This project includes a python wrapper that wraps c++ libraries into python library.

   ### How to use
      1. goto /pythonlibs/[your library]/

      2. get .so file (example: pyMsp.cpython-312-x86_64-linux-gnu.so)

      3.  simple way to use is place the .so file besides the python file you are running

      4.  then you can import the library (example: import pyMsp)


## License

This project is licensed under the **MIT License** — © 2025 Diether Ong

---


