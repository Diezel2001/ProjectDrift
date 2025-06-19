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

   Edit the `project_config.json` file and update the values with the appropriate settings:

   - `port`: Serial port to connect to  
   - `baudrate`: Communication speed  
   - Any other relevant settings

   > 💡 To list available serial ports:
   > ```bash
   > ls /dev/tty*
   > ```

2. **Build the project**

   Run the provided build script:
   ```bash
   ./build.sh

3. **Run Main Application**
   Run the provided build script:
   ```bash
   ./run_app.sh
