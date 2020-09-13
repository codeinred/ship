#pragma once
#include <array>
#include <conduit/generator.hpp>
#include <cstdio>
#include <filesystem>

namespace ship {
namespace fs = std::filesystem;
}
namespace ship::elf {

bool is_elf(fs::path const& file) {
    auto status = fs::status(file);
    if (!fs::is_regular_file(status)) {
        return false;
    }
    if ((status.permissions() & fs::perms::owner_exec) == fs::perms::none) {
        return false;
    }
    if (fs::file_size(file) < 16) {
        return false;
    }

    std::array<char, 4> header{0, 0, 0, 0};
    FILE* f = fopen(file.string().data(), "r");
    if (fread(header.data(), 1, 4, f) < 4) {
        return false;
    }

    if (header == std::array<char, 4>{0x7f, 'E', 'L', 'F'}) {
        return true;
    } else {
        return false;
    }
}

// Lists all the elf files in a directory and it's subdirectories
// Ignores Symlinks
auto enumerate(fs::path const& path) -> conduit::generator<fs::path> {
    if (!fs::is_directory(path)) {
        throw std::invalid_argument(
            "Input to list_elf_recursively must be a directory");
    }

    fs::recursive_directory_iterator iter(path);
    for (auto& p : iter) {
        if (is_elf(p)) {
            co_yield p;
        }
    }
}

auto enumerate(char** paths) -> conduit::generator<fs::path> {
    for(int i = 0; paths[i] != nullptr; i++) {
        fs::path p = paths[i];
        if(is_elf(p)) {
            co_yield p;
        } else if(fs::is_directory(p)) {
            for(auto item : enumerate(p)) {
                co_yield item;
            }
        }
    }
}
} // namespace ship::elf
