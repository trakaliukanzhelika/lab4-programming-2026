// Trakaliuk Anzhelika K-28
// Variant 7

#include "Fields.h"
#include "Generator.h"
#include "Operations.h"
#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace
{
    constexpr std::size_t max_threads = 3;
    constexpr std::size_t repeat_count = 5;

    double measure_ms(const std::vector<std::vector<Operation>>& thread_operations, long long& checksum)
    {
        Fields fields;
        std::vector<long long> results(thread_operations.size());
        std::vector<std::thread> threads;
        threads.reserve(thread_operations.size());

        const auto start = std::chrono::steady_clock::now();
        for (std::size_t i = 0; i < thread_operations.size(); ++i)
            threads.emplace_back([&, i] { results[i] = execute(fields, thread_operations[i]); });
        for (std::thread& thread : threads)
            thread.join();
        const auto finish = std::chrono::steady_clock::now();

        for (long long result : results)
            checksum += result;
        return std::chrono::duration<double, std::milli>(finish - start).count();
    }
}

int main()
{
    try
    {
        generate_files();
        long long checksum = 0;
        for (const std::string set : {"a", "b", "c"})
        {
            for (std::size_t thread_count = 1; thread_count <= max_threads; ++thread_count)
            {
                std::vector<std::vector<Operation>> thread_operations;
                for (std::size_t thread = 0; thread < thread_count; ++thread)
                    thread_operations.push_back(load_operations(
                        set + "_" + std::to_string(thread_count) + "_" + std::to_string(thread) + ".txt"));

                double total_ms = 0;
                for (std::size_t run = 0; run < repeat_count; ++run)
                    total_ms += measure_ms(thread_operations, checksum);

                std::cout << "file " << set << ", threads: " << thread_count
                          << ", average time: " << total_ms / repeat_count << " ms\n";
            }
        }
        std::cout << "checksum: " << checksum << "\n";
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << "\n";
        return 1;
    }
}
