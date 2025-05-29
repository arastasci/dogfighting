# Dogfighting Game with Custom C++ Game Engine

## Overview
This project is a lightweight 3D dogfighting game built on EnTT, following the Entity-Component-System architecture.  
It features forward rendering, Bullet Physics for dynamics, and Valve's GameNetworkingSockets for networking. The goal of the project was to gain hands-on experience in developing a game engine and to understand how netcode worked under the hood in multiplayer games.

## Networking
Networking was implemented using the Steamworks SDK  (the SDK isn't included in the repo for license concerns, you can acquire it for yourself from the [Steamworks SDK site](https://partner.steamgames.com/doc/sdk) and place it under at/vendor/steam). This allows players to host (and join) game sessions securely and easily using Steam's Datagram Relay network, without exposing their IP addresses or dealing with NAT traversal.

The game is fully server-authoritative, meaning the entire simulation runs on the host, and the game state is replicated to clients. To achieve this, the host sends entity transforms to clients. Critical messages such as EntityCreated and EntityDestroyed are sent using reliable communication via SteamNetworkingSockets, while frequent and loss-tolerant messages like TransformUpdate and PlayerInput are sent unreliably.

Clients send input payloads to the host, and the host applies forces to the rigidbodies in the physics world based on those inputs.


## Dependencies
- Assimp for model loading
- GLAD for OpenGL, GLFW for input / window context
- spdlog for easy loginng
- Bullet Physics SDK for Physics simulations
- Valve's GameNetworkingSockets for networking
  
##  Build Instructions

```bash
# clone the repository
git clone https://github.com/arastasci/dogfighting.git
cd dogfighting

mkdir build
cd build
cmake ..
cmake --build .
