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

    auto replace(std::string &str, const std::string &strOld, const std::string &strNew) -> void
    {
        size_t curr_pos = 0;
        while ((curr_pos = str.find(strOld, curr_pos)) != std::string::npos)
        {
            str.replace(curr_pos, strOld.length(), strNew);
            curr_pos += strNew.length();
        }
    }

    template <typename T>
    auto contains(const std::vector<T> &vec, const T &key) -> bool
    {
        return std::find(vec.begin(), vec.end(), key) != vec.end();
    }

    template <typename T>
    auto partition(const std::vector<T> &vec, const size_t partition_size, std::vector<std::vector<T>> &out_vec) -> void
    {
        std::vector<T> temp_vec;
        size_t vec_size = vec.size();
        for (size_t idx = 0; idx < vec_size; idx++)
        {
            if ((idx != 0 && idx % partition_size == 0) || idx == vec_size - 1)
            {
                temp_vec.push_back(vec[idx]);
                out_vec.push_back(temp_vec);
                temp_vec.clear();
                continue;
            }
            temp_vec.push_back(vec[idx]);
        }
    }
}

#endif