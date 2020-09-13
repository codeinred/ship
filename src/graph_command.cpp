#include <filesystem>
#include <fmt/core.h>
#include <vector>
#include <ship/ship.h>

using namespace ship;
namespace fs = std::filesystem;

int graph_command(int argc, char** argv, char** env) {
    std::vector<fs::path> files;
    for (int i = 2; i < argc; i++) {
        files.push_back(argv[i]);
    }
    bool any_missing = false;
    for (auto& path : files) {
        if (!fs::exists(path)) {
            fmt::print("Cannot find file: {}\n", path.string());
            any_missing = true;
        }
    }
    if (any_missing) {
        return 1;
    }

    auto graph = io::get_dependency_graph(files);

    for(auto& [key, value] : graph) {
        for(auto& [_, path] : value) {
            fmt::print(R"("{}", "{}")""\n", key, path.string());
        }
    }

    return 0;
}
