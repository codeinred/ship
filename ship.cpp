#include <get_libraries.hpp>
#include <unordered_map>
#include <vector>

int main(int argc, char** argv) {
    std::unordered_map<std::string, std::vector<fs::path>> dependencies{};

    std::vector<fs::path> to_search;

    for (int i = 1; i < argc; i++) {
        fs::path p = argv[i];
        if(p.is_relative()) {
            p = fs::current_path() / p;
        }
        to_search.push_back(normalize(p));
    }

    while(to_search.size() > 0) {
        fs::path target = to_search.back();
        to_search.pop_back();
        std::string key = target.string();

        if(dependencies.contains(key)) {
            continue;
        }
        fmt::print("Searching {}\n", key);
        size_t old_size = to_search.size();
        for(auto [libname, path] : get_libraries(target)) {
            printf("\t%s -> %s\n", libname.c_str(), path.string().c_str());
            to_search.push_back(path);
        }
        dependencies[key] = std::vector<fs::path>(to_search.begin() + old_size, to_search.end());
    }
    printf("Obtained dependencies\n");
    fflush(stdout);

    size_t total_size = 0;
    for(auto& [key, value] : dependencies) {
        total_size += fs::file_size(key);
    }
    printf("Total size: %lu\n", total_size);
}
