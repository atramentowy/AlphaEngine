## Alpha Engine

I made it just for learning purposes

### Tech
Language: C++

Underlying framework: Raylib

Physics: Bullet Physics

Build System: CMake

### Build tips:
```
git clone https://github.com/atramentowy/AlphaEngine.git
cd AlphaEngine
```
#### Depedencies
```
mkdir deps
```
raylib will be downloaded automatically

in `deps` folder clone and build bullet physics library

```
mkdir build && cd build
cmake ..
cmake --build .
```
and it should work just fine