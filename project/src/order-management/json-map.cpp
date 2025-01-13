#include <order-management/json-map.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

namespace om
{
    std::string ToString(const JsonMap& message)
    {
        std::ostringstream oss;
        boost::property_tree::write_json(oss, message);
        return oss.str();
    }
} // namespace om