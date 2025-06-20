# 🦉 ProjectOwl

Project Owl is a research-oriented codebase for drone system development. It contains experimental implementations, prototyping tools and libraries. This project serves as a foundation for ongoing exploration in aerial robotics and embedded systems.

Current Features:
- MSP Library for RPI to Flight Controller comms

Future Features:
- autonomous flight via (inav waypoints)
- inter drone comms via (lora)
- sensor integration
- Drone Control systems

---

## 📄 License

This project is licensed under the **MIT License** — © 2025 Diether Ong

---

## ⚙️ How to Build

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

3. **Run Main Application**
   Run the provided run app script:
   ```bash
   ./run_app.sh
   ```
