#include <chrono>
#include <cstdlib>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <ship/ship.h>
#include <unistd.h>

using namespace ship;

int gather_command(int argc, char** argv, char** env) {
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

    fs::path working_dir = fmt::format(
        "/tmp/ship-{}-{}/", getpid(),
        std::chrono::high_resolution_clock::now().time_since_epoch());

    if (!fs::create_directory(working_dir)) {
        fmt::print("Error: Unable to create working directory at {}\n",
                   working_dir.string());
        return 1;
    }

    fs::current_path(working_dir);

    auto graph = elf::dependency_graph(files);

    return 0;
}
