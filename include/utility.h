#ifndef E_UTILITY_H
#define E_UTILITY_H

#include <string>
#include <vector>

namespace eutil
{
    auto make_string(std::vector<std::string> argv) -> std::string
    {
        std::string string_args = " ";
        for (size_t idx = 0; idx < argv.size(); idx++)
            string_args += std::string(argv[idx]) + " ";
        return string_args;
    }

    auto strip_extension(std::string fileWithExtension) -> std::string
    {
        size_t dot_idx = fileWithExtension.find_last_of(".");
        if (dot_idx == std::string::npos)
            return fileWithExtension;
        return fileWithExtension.substr(0, dot_idx);
    }

    auto replace(std::string& str, const std::string& strOld, const std::string& strNew) -> void
    {
        size_t curr_pos = 0;
        while((curr_pos = str.find(strOld, curr_pos)) != std::string::npos)
        {
            str.replace(curr_pos, strOld.length(), strNew);
            curr_pos += strNew.length();
        }
    }
}

#endif