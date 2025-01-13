#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

namespace om
{
    using JsonMap = boost::property_tree::ptree;

    std::string ToString(const JsonMap& message);
} // namespace om