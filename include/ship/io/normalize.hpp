#pragma once
#include <filesystem>

namespace ship {
namespace fs = std::filesystem;
}
namespace ship::io {
// Recursively follows any symlinks and converts it to lexically normal form
fs::path normalize(fs::path const& p) {
    fs::path path = p;
    while (fs::is_symlink(path)) {
        auto new_path = fs::read_symlink(path);
        if (new_path.is_relative()) {
            path = path.parent_path() / new_path;
        } else {
            path = new_path;
        }
    }
    return path.lexically_normal();
}
} // namespace ship::io
