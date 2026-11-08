#pragma once
#include <cstddef>
#include <shared_mutex>
#include <string>

class Fields
{
public:
    static constexpr std::size_t field_count = 3;

    int get(std::size_t index) const;
    void set(std::size_t index, int value);
    explicit operator std::string() const;

private:
    int _values[field_count]{};
    mutable std::shared_mutex _mutexes[field_count];
};
