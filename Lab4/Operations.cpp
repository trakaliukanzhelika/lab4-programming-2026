#include "Operations.h"
#include <fstream>
#include <stdexcept>

std::vector<Operation> load_operations(const std::string& path)
{
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error("cannot open file for reading " + path);
    }
    std::vector<Operation> operations;
    std::string command;
    while (in >> command)
    {
        Operation operation{Operation_type::to_string, 0, 0};
        if (command == "read")
        {
            operation.type = Operation_type::read;
            in >> operation.index;
        }
        else if (command == "write")
        {
            operation.type = Operation_type::write;
            in >> operation.index >> operation.value;
        }
        operations.push_back(operation);
    }
    if (in.bad()) {
        throw std::runtime_error("error in file " + path);
    }
    return operations;
}

long long execute(Fields& fields, const std::vector<Operation>& operations)
{
    long long checksum = 0;
    for (const Operation& operation : operations)
    {
        switch (operation.type)
        {
        case Operation_type::read:
            checksum += fields.get(operation.index);
            break;
        case Operation_type::write:
            fields.set(operation.index, operation.value);
            break;
        case Operation_type::to_string:
            checksum += static_cast<std::string>(fields).size();
            break;
        }
    }
    return checksum;
}
