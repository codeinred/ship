#pragma once
#include <array>
#include <conduit/generator.hpp>
#include <cstdio>
#include <filesystem>
#include <ship/elf/is_elf.hpp>

namespace ship {
namespace fs = std::filesystem;
}
namespace ship::elf {

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
