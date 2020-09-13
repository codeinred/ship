#pragma once
#include <filesystem>
#include <fmt/core.h>

namespace ship {
namespace fs = std::filesystem;
}
namespace ship::io {
constexpr auto print_cannot_find_file = [](char* file) { fmt::print("Cannot find file: {}\n", file); };
template<class on_missing>
bool all_exist(char** files, on_missing f) {
    for (int i = 0; files[i] != nullptr; i++) {
        if (!fs::exists(files[i])) {
            f(files[i]);
            return false;
        }
    }
    return true;
}
} // namespace ship::io
