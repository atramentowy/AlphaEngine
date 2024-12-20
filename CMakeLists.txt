cmake_minimum_required(VERSION 3.11)
project(raylib_game)

set(CMAKE_BUILD_TYPE "Debug")

# Set the runtime library to be used for both Bullet Physics and Raylib (choose MDd or MTd)
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd") # For Dynamic Debug
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd")   # For Dynamic Debug
endif()

# Raylib
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
  endif()
endif()

# bullet
add_subdirectory(deps/bullet3)
set(bullet3_SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps/bullet3/)
# FetchContent_Declare(
#     bullet3
#     GIT_REPOSITORY git@github.com:bulletphysics/bullet3.git
#     GIT_TAG master  # Or specify a release/tag/commit hash
#     GIT_PROGRESS   TRUE
#     EXTRA_GIT_ARGS --depth=1
# )
# Make sure the content is downloaded
# FetchContent_MakeAvailable(bullet3)
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable building examples" FORCE)

# Build Bullet3 from source
set(BULLET3_BUILD_SHARED OFF CACHE BOOL "Build Bullet3 as static library" FORCE)
set(BULLET3_BUILD_UNIT_TESTS OFF CACHE BOOL "Disable Bullet3 unit tests" FORCE)
set(BULLET3_BUILD_EXAMPLES OFF CACHE BOOL "Disable Bullet3 examples" FORCE)

# Set Bullet3 include directory
set(BULLET_INCLUDE_DIRS ${bullet3_SOURCE_DIR}/src)
include_directories(${BULLET_INCLUDE_DIRS})

# set(BULLET_LIBRARIES Bullet3Common Bullet3Dynamics Bullet3Collision Bullet3Math)

# set(BULLET_LIBRARIES
#     ${bullet3_BINARY_DIR}/build/lib/Debug/libBullet3Common_Debug.lib
#     ${bullet3_BINARY_DIR}/build/lib/Debug/libBullet3Collision_Debug.lib
#     ${bullet3_BINARY_DIR}/build/lib/Debug/libBullet3Dynamics_Debug.lib
#     ${bullet3_BINARY_DIR}/build/lib/Debug/libBullet3Math_Debug.lib
# )

# target_link_directories(${PROJECT_NAME} PRIVATE ${bullet3_BINARY_DIR}/src)
link_directories(${bullet3_BINARY_DIR}/src)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BULLET_LIBRARIES
        ${CMAKE_SOURCE_DIR}/build/lib/Debug/BulletDynamics_Debug.lib
        ${CMAKE_SOURCE_DIR}/build/lib/Debug/BulletCollision_Debug.lib
        ${CMAKE_SOURCE_DIR}/build/lib/Debug/LinearMath_Debug.lib
    )
else()
    set(BULLET_LIBRARIES
        ${CMAKE_SOURCE_DIR}/build/lib/Release/BulletDynamics.lib
        ${CMAKE_SOURCE_DIR}/build/lib/Release/BulletCollision.lib
        ${CMAKE_SOURCE_DIR}/build/lib/Release/LinearMath.lib
    )
endif()

# EnTT
add_subdirectory(deps/entt)

# add executable
add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/application.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  raylib
  ${BULLET_LIBRARIES}
  EnTT
)

# Ensure the Bullet3 build directory is linked for dynamic libraries
if(WIN32)
    link_directories(${bullet3_BINARY_DIR}/src)
else()
    link_directories(${bullet3_BINARY_DIR})
endif()
