#include <fmt/core.h>
#include <ship/ship.h>

using namespace ship;

int measure_command(int argc, char** argv, char** env) {
    if (!all_exist(argv + 2, io::print_cannot_find_file)) {
        return 1;
    }

    auto gen = elf::enumerate(argv + 2);
    std::vector<fs::path> files(begin(gen), end(gen));

    auto graph = elf::dependency_graph(files);
    size_t total_size = 0;
    for (auto& [file, _] : graph) {
        if (!fs::is_symlink(file))
            total_size += fs::file_size(file);
    }
    fmt::print("Total measure: {} KB\n", total_size / 1024);
    return 0;
}
