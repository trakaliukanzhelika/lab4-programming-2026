#include "Fields.h"
#include <mutex>

int Fields::get(std::size_t index) const
{
    std::shared_lock lock(_mutexes[index]);
    return _values[index];
}

void Fields::set(std::size_t index, int value)
{
    std::lock_guard lock(_mutexes[index]);
    _values[index] = value;
}

Fields::operator std::string() const
{
    int snapshot[field_count];
    {
        std::shared_lock<std::shared_mutex> locks[field_count];
        for (std::size_t i = 0; i < field_count; ++i)
            locks[i] = std::shared_lock(_mutexes[i]);
        for (std::size_t i = 0; i < field_count; ++i)
            snapshot[i] = _values[i];
    }
    std::string result = "[";
    for (std::size_t i = 0; i < field_count; ++i)
    {
        if (i > 0)
            result += ", ";
        result += std::to_string(snapshot[i]);
    }
    return result + "]";
}
