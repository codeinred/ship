#include <fmt/core.h>
#include <string_view>

using std::string_view_literals::operator""sv;

const std::string_view usage = // Usage on next line
    R"(Usage:

    ship [subcommand] files

Subcommands:

    usage   - print these instructions
    graph   - print a graph of dependencies
    measure - calculate the total size of a file and all it's dependencies
)"sv;

int usage_command(int argc, char** argv, char** env) {
    fmt::print("{}", usage);
    return 0;
}
