#include <filesystem>
#include <fmt/core.h>
#include <vector>
#include <ship/ship.h>

using namespace ship;

int graph_command(int argc, char** argv, char** env) {
    if(!all_exist(argv + 2, io::print_cannot_find_file)) {
        return 1;
    }

    auto gen = elf::enumerate(argv + 2);
    std::vector<fs::path> files(begin(gen), end(gen));

    auto graph = elf::dependency_graph(files);

    for(auto& [key, value] : graph) {
        for(auto& [_, path] : value) {
            fmt::print(R"("{}", "{}")""\n", key, path.string());
        }
    }

    return 0;
}
