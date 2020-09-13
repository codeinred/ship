#include <chrono>
#include <cstdlib>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <ship/ship.h>
#include <unistd.h>

using namespace ship;

int gather_command(int argc, char** argv, char** env) {
    if(!io::all_exist(argv + 2, io::print_cannot_find_file)) {
        return 1;
    }

    auto gen = elf::enumerate(argv + 2);
    std::vector<fs::path> files(begin(gen), end(gen));

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
