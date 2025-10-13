# **R-Type Multiplayer Game**

## **Project Overview**

**R-Type** is a networked multiplayer game where players control spaceships to battle waves of enemies in a side-scrolling shooter environment. This project is a recreation of the classic R-Type game, featuring:
- Multiplayer capabilities using a **multithreaded server**.
- A custom **game engine** built using **C++** with an **Entity-Component-System (ECS)** architecture.
- Client-side rendering using **SFML** for graphics and audio.
- Server-side game logic handling player actions, enemy waves, and real-time synchronization.
- Real-time network communication using **Asio**.

---

## **Project Structure**
The project consists of two main components:
- **Server**: Handles game logic, synchronization, and manages multiple clients.
- **Client**: Handles rendering, audio, and inputs from the player.

---

## **Key Features**
- **Multiplayer**: Multiple players can connect and play together.
- **Custom Game Engine**: A modular game engine designed with ECS for flexibility and performance.
- **Real-Time Communication**: Fast-paced, low-latency gameplay using UDP for in-game communication.
- **Cross-platform Compatibility**: Supports both Linux and Windows platforms.

---

## **Dependencies**

The following dependencies are required to build and run the project:

- **C++17** or higher
- **CMake** (version 3.30 or higher)
- **SFML** (Simple and Fast Multimedia Library) for rendering and audio.
- **Asio** for asynchronous network communication.
- **ImGui** for sfml UI design
- **Threads** library for multithreaded server.

---

## **Build Instructions**

### **Linux**

1. **Run this script** :
   ```bash
   chmod +x install.sh && ./install.sh
   ```

---

## **Usage Instructions**

- **Server**:
  - To start the server, run the `server` executable and specify the port:
    ```bash
    ./server <PORT>
    ```

- **Client**:
  - To start the client, run the `client` executable and specify the server IP and port:
    ```bash
    ./client <IP_ADRESS> <PORT>
    ```

---

## **Supported Platforms**

- **Linux** (Ubuntu, Fedora, etc.) 
- **Windows** (Visual Studio)

---

## **Contributors**

- jaures.agossou@epitech.eu (Jaurès AGOSSOU)
- chardone.amoki@epitech.eu (Chardone Amoki)
- harry.fatolou@epitech.eu (Harry Fatolou)
- schallum.setondji@epitech.eu (Schallum Setondji)
- jores.da-trinidade@epitech.eu (Jores.Da-Trinidade)
- **DEV SQUAD** 

Feel free to reach out if you encounter any issues or want to contribute!

---

## **Useful Links**

- **SFML Documentation**: [https://www.sfml-dev.org/documentation/](https://www.sfml-dev.org/documentation/)
- **Asio Documentation**: [https://think-async.com/Asio/](https://think-async.com/Asio/)
- **ImGui Documentation**: [https://imgui-doc.com](https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html)

---

## **Quick Start Guide**

1. **Build the project** following the instructions above.
2. **Run the server** on your machine using the provided commands.
-- 
## **Online Documentation**
 Link: [https://www.Rtype-doc.com](https://dev-squad.gitbook.io/r-type-docs/)
