#pragma once
#include <conduit/generator.hpp>
#include <cstdio>
#include <string_view>

namespace ship::io {
namespace detail {
size_t line_size(std::string_view view) {
    for (size_t i = 0; i < view.size(); i++) {
        if (view[i] == '\n')
            return i;
    }
    return view.size();
}
} // namespace detail
conduit::generator<std::string_view> read_file(FILE* file) {
    char buffer[1024];
    while (size_t size = std::fread(buffer, 1, 1024, file)) {
        co_yield std::string_view(buffer, size);
    }
}

conduit::generator<std::string_view> read_lines(FILE* file) {
    std::string partial_line;
    for (std::string_view view : read_file(file)) {
        if (partial_line.size() > 0) {
            size_t line_end = detail::line_size(view);
            partial_line += view.substr(0, line_end);
            if (line_end < view.size()) {
                view = view.substr(line_end + 1);
                co_yield std::string_view(partial_line);
            } else {
                continue;
            }
        }
        size_t line_end = detail::line_size(view);
        while (line_end < view.size()) {
            co_yield view.substr(0, line_end);
            view = view.substr(line_end + 1);
            line_end = detail::line_size(view);
        }
        partial_line = view;
    }
    if (partial_line.size() > 0) {
        co_yield std::string_view(partial_line);
    }
}
} // namespace ship::io
