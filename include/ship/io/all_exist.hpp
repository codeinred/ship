#pragma once
#include <filesystem>

namespace ship {
namespace fs = std::filesystem;
}
namespace ship::io {
bool all_exist(char** files) {
    for (int i = 0; files[i] != nullptr; i++) {
        if (!fs::exists(files[i])) {
            return false;
        }
    }
    return true;
}
} // namespace ship::io
