#include <ship/io.h>
#include <fmt/core.h>

namespace fs = std::filesystem;
int measure_command(int argc, char** argv, char** env) {
    std::vector<fs::path> files;
    for(int i = 2; i < argc; i++) {
        files.push_back(argv[i]);
    }
    bool any_missing = false;
    for(auto& path : files) {
        if(!fs::exists(path)) {
            fmt::print("Cannot find file: {}", path.string());
            any_missing = true;
        }
    }
    if(any_missing) {
        return 1;
    }

    auto graph = ship::io::get_dependency_graph(files);
    size_t total_size = 0;
    for(auto& [file, _] : graph) {
        total_size += fs::file_size(file);
    }
    fmt::print("Total measure: {} KB\n", total_size / 1024);
    return 0;
}
