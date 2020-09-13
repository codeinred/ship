#pragma once
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
}
