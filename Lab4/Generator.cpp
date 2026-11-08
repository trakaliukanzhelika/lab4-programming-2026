#include "Generator.h"
#include "Fields.h"
#include <cstddef>
#include <fstream>
#include <initializer_list>
#include <random>
#include <string>

namespace
{
    constexpr std::size_t total_operations = 1200000;
    constexpr std::size_t max_threads = 3;
    constexpr int written_value = 1;
    constexpr std::size_t string_operation = 2 * Fields::field_count;

    void generate_file(const std::string& path, std::size_t operation_count,
                       std::discrete_distribution<std::size_t>& distribution, std::mt19937& engine)
    {
        std::ofstream out(path);
        if (!out.is_open()) {
            throw std::runtime_error("cannot open file for writing " + path);
        }
        for (std::size_t i = 0; i < operation_count; ++i)
        {
            const std::size_t operation = distribution(engine);
            if (operation == string_operation)
                out << "string\n";
            else if (operation % 2 == 0)
                out << "read " << operation / 2 << "\n";
            else
                out << "write " << operation / 2 << " " << written_value << "\n";
        }
    }

    void generate_set(const std::string& name, std::initializer_list<double> weights, std::mt19937& engine)
    {
        std::discrete_distribution<std::size_t> distribution(weights);
        for (std::size_t threads = 1; threads <= max_threads; ++threads)
            for (std::size_t thread = 0; thread < threads; ++thread)
                generate_file(name + "_" + std::to_string(threads) + "_" + std::to_string(thread) + ".txt",
                              total_operations / threads, distribution, engine);
    }
}

void generate_files()
{
    std::mt19937 engine(std::random_device{}());
    generate_set("a", {5, 5, 30, 5, 24, 1, 30}, engine);
    generate_set("b", {1, 1, 1, 1, 1, 1, 1}, engine);
    generate_set("c", {1, 35, 2, 30, 1, 25, 6}, engine);
}
