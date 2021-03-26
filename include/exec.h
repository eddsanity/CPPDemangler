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

        // copy of command, just to give some feedback of what command is being run.
        std::string dbgCmd = cmd;
        if (argv.size() > 6)
            dbgCmd += " [...] 2>&1";
        else
            dbgCmd += args + " 2>&1";

        cmd += args;
        // redirect stderr to stdout
        cmd += " 2>&1";

        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);

        std::cout << dbgCmd << "\n";
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
