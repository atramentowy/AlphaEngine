#ifndef FILEPATH_H
#define FILEPATH_H

#include <filesystem>

namespace fs = std::filesystem;

// usage
// fs::path shaderVPath = projectRoot/"res"/"shaders"/"shader.vert";

fs::path exePath = fs::current_path();
fs::path projectRoot = exePath.parent_path().parent_path();

#endif