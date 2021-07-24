#pragma once
#include <filesystem>
#include <ship/elf/is_elf.hpp>
#include <ship/io/popen_file.hpp>
#include <ship/io/read_file.hpp>
#include <ship/pair.hpp>

#include <fmt/core.h>

namespace ship {
namespace fs = std::filesystem;
}
namespace ship::elf {
conduit::generator<pair<std::string_view, fs::path>> libraries(fs::path path) {
    io::popen_file ldd_output(fmt::format("ldd '{}'", path.string()));
    for (std::string_view line : io::read_lines(ldd_output.get())) {
        line.remove_prefix(1); // Prefix is 1 byte, a tab
        if (line.ends_with("not found")) {
            throw std::logic_error(
                fmt::format("get_libraries: dependency not met for {}: {}",
                            path.string(), line));
        }

        // If it's marked as statically linked, there are no dependencies
        if (line == "statically linked") {
            continue;
        }
        for (int i = line.size() - 1; i > 1; i--) {
            if (line[i] == '(') {
                line = line.substr(0, i - 1);
                break;
            }
        }
        if (line[0] != '/') {
            if (line == "linux-vdso.so.1") {
                continue;
            }
            std::string_view libname;
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == '/') {
                    if (i > 0 && line[i - 1] == '\\')
                        continue;
                    libname = line.substr(0, i - 4);
                    line.remove_prefix(i);
                    break;
                }
            }
            // Ignore ones that are still relative
            if (libname.empty()) {
                throw std::logic_error("Expected libname to be set");
            }

            co_yield {libname, fs::path(line)};
        } else {
            co_yield {line, fs::path(line)};
        }
    }
    if (ldd_output.close()) {
        throw std::logic_error(fmt::format("ldd exited with error while reading '{}'", path.string()));
    }
}

auto dependency_graph(std::vector<fs::path> files)
    -> std::unordered_map<std::string,
                          std::vector<pair<std::string, fs::path>>> {
    std::unordered_map<std::string, std::vector<pair<std::string, fs::path>>>
        dependencies;
    while (files.size() > 0) {
        fs::path target = files.back();
        files.pop_back();
        std::string key = target.string();

        if (dependencies.contains(key)) {
            continue;
        }
        if (fs::is_symlink(target)) {
            fs::path linkname = fs::read_symlink(target);
            fs::path path = linkname.is_absolute()
                                ? linkname
                                : target.parent_path() / linkname;
            files.push_back(path);
            dependencies[key].push_back(
                pair{linkname.string(), std::move(path)});
        } else if (is_elf(target)) {
            auto& target_libraries = dependencies[key];
            for (auto [libname, path] : libraries(target)) {
                files.push_back(path);
                target_libraries.push_back(
                    pair{std::string(libname), std::move(path)});
            }
        }
    }
    return dependencies;
}
} // namespace ship::elf
