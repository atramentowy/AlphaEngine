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

in `deps` should be two folders one with `bullet3` and `entt`
if they are not there,
you must clone compile and put them in `deps` folder

```
mkdir build && cd build
cmake ..
cmake --build .
```
and it should work just fine