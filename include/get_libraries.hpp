#pragma once
#include <conduit/generator.hpp>
#include <cstdio>
#include <filesystem>
#include <fmt/core.h>
#include <memory>
#include <string>
#include <string_view>

template <class A, class B>
struct pair {
    [[no_unique_address]] A first;
    [[no_unique_address]] B second;
};
template <class A, class B>
pair(A, B) -> pair<A, B>;

namespace co = conduit;
namespace fs = std::filesystem;

struct popen_deleter {
    using pointer = FILE*;

    void operator()(FILE* file) const noexcept { pclose(file); }
};
struct popen_file : private std::unique_ptr<FILE*, popen_deleter> {
    using super = std::unique_ptr<FILE*, popen_deleter>;

    popen_file(std::string command) noexcept
      : super(popen(command.c_str(), "r")) {}
    using super::get;

    // Invokes pclose on the file and returns the result
    int close() { return pclose(super::release()); }
};

co::generator<std::string_view> readFile(FILE* file) {
    char buffer[1024];
    while (size_t size = std::fread(buffer, 1, 1024, file)) {
        co_yield std::string_view(buffer, size);
    }
}

size_t line_size(std::string_view view) {
    for (size_t i = 0; i < view.size(); i++) {
        if (view[i] == '\n')
            return i;
    }
    return view.size();
}
template <class Gen>
co::generator<std::string_view> readLines(Gen source) {
    std::string partial_line;
    for (std::string_view view : source) {
        if (partial_line.size() > 0) {
            size_t line_end = line_size(view);
            partial_line += view.substr(0, line_end);
            if (line_end < view.size()) {
                view = view.substr(line_end + 1);
                co_yield std::string_view(partial_line);
            } else {
                continue;
            }
        }
        size_t line_end = line_size(view);
        while (line_end < view.size()) {
            co_yield view.substr(0, line_end);
            view = view.substr(line_end + 1);
            line_end = line_size(view);
        }
        partial_line = view;
    }
    if (partial_line.size() > 0) {
        co_yield std::string_view(partial_line);
    }
}

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

co::generator<pair<std::string, fs::path>> get_libraries(fs::path path) {
    popen_file ldd_output(fmt::format("ldd '{}'", path.string()));
    for (std::string_view line : readLines(readFile(ldd_output.get()))) {
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

            co_yield {std::string(libname), fs::path(line)};
        } else {
            co_yield {std::string(line), fs::path(line)};
        }
    }
    if(ldd_output.close()) {
        throw std::logic_error("ldd exited with error");
    }
}
