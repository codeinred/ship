#include <fmt/core.h>

#include <ship/ship.h>

int enumerate_command(int argc, char** argv, char** env) {
    if(!ship::io::all_exist(argv + 2, ship::io::print_cannot_find_file)) {
        return 1;
    }

    for(auto& path : ship::elf::enumerate(argv + 2)) {
        fmt::print("{}\n", path.string());
    }
    return 0;
}
