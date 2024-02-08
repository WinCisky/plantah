#include <string>
#include <vector>
#include <map>
#include <sstream>

class SimpleJsonParser {
    std::string m_json;

public:
    void parse(const std::string & json)
    {
        m_json = json;
    }

    std::string get(const std::string & key)
    {
        std::string result;
        std::string keyString = "\"" + key + "\"";
        std::size_t keyPos = m_json.find(keyString);
        if (keyPos != std::string::npos)
        {
            std::size_t valuePos = m_json.find(":", keyPos);
            if (valuePos != std::string::npos)
            {
                std::size_t valueStart = m_json.find_first_of("\"", valuePos);
                std::size_t valueEnd = m_json.find_first_of("\"", valueStart + 1);
                if (valueStart != std::string::npos && valueEnd != std::string::npos)
                {
                    result = m_json.substr(valueStart + 1, valueEnd - valueStart - 1);
                }
            }
        }
        return result;
    }
};