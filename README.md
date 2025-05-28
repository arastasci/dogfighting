# Dogfighting Game with Custom C++ Game Engine

## Overview
This project is a lightweight 3D dogfighting game built on an EnTT, following the Entity-Component-System architecture.  
It features forward rendering, Bullet Physics for dynamics, and Valve's GameNetworkingSockets for networking.

##  Build Instructions

```bash
# clone the repository
git clone https://github.com/arastasci/dogfighting.git
cd dogfighting

mkdir build
cd build
cmake ..
cmake --build .
