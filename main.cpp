#include <fmt/core.h>
#include <string_view>
#include <unordered_map>

int graph_command(int argc, char** argv, char** env);
int usage_command(int argc, char** argv, char** env);
int measure_command(int argc, char** argv, char** env);
int gather_command(int argc, char** argv, char** env);
int enumerate_command(int argc, char** argv, char** env);

using subcommand_t = int (*)(int, char**, char**);

const std::unordered_map<std::string_view, subcommand_t> subcommands{
    {"--help", usage_command},
    {"gather", gather_command},
    {"usage", usage_command},
    {"graph", graph_command},
    {"enumerate", enumerate_command},
    {"measure", measure_command}};

int main(int argc, char** argv, char** env) {
    std::string_view command;

    if (argc == 1)
        command = "usage";
    else
        command = argv[1];

    if (!subcommands.contains(command)) {
        fmt::print("Error: unrecogized subcommand '{}'\n", argv[1]);
        command = "usage";
    }

    return subcommands.at(command)(argc, argv, env);
}
