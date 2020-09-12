#pragma once
#include <cstdio>
#include <memory>

namespace ship::io {
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
    inline int close() { return pclose(super::release()); }
};
} // namespace ship::io
