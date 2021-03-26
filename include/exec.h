#ifndef E_EXEC_H
#define E_EXEC_H

#include <iostream>
#include <cstdio>
#include <memory>
#include <string>
#include <array>

#include "utility.h"

namespace eutil
{
#ifdef __linux__
#define _popen popen
#define _pclose pclose
#endif

    auto exec(std::string cmd, std::vector<std::string> argv, int &exec_status) -> std::string
    {
        char buf[128];
        std::string sout;
        std::string args = eutil::make_string(argv);

        cmd += args;
        // redirect stderr to stdout
        cmd += " 2>&1";

        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);

        if (!pipe)
            exec_status = -1;
        else
        {
            // read pipe until it's either done or fails
            while (fgets(buf, sizeof(buf), pipe.get()) != nullptr)
                sout += buf;
            exec_status = 0;
        }

        return sout;
    }
}

#endif
