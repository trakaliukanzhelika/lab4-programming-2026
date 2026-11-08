#pragma once
#include "Fields.h"
#include <cstddef> 
#include <string>
#include <vector>

enum class Operation_type { read, write, to_string };

struct Operation
{
    Operation_type type;
    std::size_t index;
    int value;
};

std::vector<Operation> load_operations(const std::string& path);
long long execute(Fields& fields, const std::vector<Operation>& operations);
