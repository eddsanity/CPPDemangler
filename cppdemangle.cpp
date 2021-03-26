#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <sstream>
#include "include/exec.h"

auto compile_obj(const std::string &FileName, const std::string &used_std, int &pipe_status) -> std::string;
auto demangle_sym_table(const std::string &sym_table, int &pipe_status) -> std::map<std::string, std::string>;
auto make_demangling_table(const std::string &used_std, const std::string &CppFileName) -> std::map<std::string, std::string>;

auto main(int argc, char *argv[]) -> int
{
    int exec_code = -1;
    std::map<std::string, std::string> demangling_table;
    std::string used_std = "";

    if (argc == 1)
    {
        std::cerr << "CPPDEMANGLE failed: no arguments passed\n";
        return -1;
    }
    else if (argc == 2)
        used_std = "c++11";
    else if (argc == 3)
        used_std = argv[1];

    demangling_table = make_demangling_table(used_std, /* CPP file */ argv[2]);

    for (auto it = demangling_table.begin(); it != demangling_table.end(); it++)
        std::cout << it->first << " : " << it->second << "\n";
    return 0;
}

auto compile_obj(const std::string &FileName, const std::string &used_std, int &pipe_status) -> std::string
{
    // compile single cpp file into obj file
    std::vector<std::string> gcc_args{"-O0", "-std=" + used_std, FileName + ".cpp", "-o", FileName + ".o"};
    return eutil::exec("g++", gcc_args, pipe_status);
}

auto demangle_sym_table(const std::string &sym_table, int &pipe_status) -> std::map<std::string, std::string>
{
    std::map<std::string, std::string> demangling_table;

    // https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling
    std::regex mangled_name_regex("(\\b__Z\\w+\\b)");
    auto mangled_table_begin = std::sregex_iterator(sym_table.begin(), sym_table.end(), mangled_name_regex);
    auto mangled_table_end = std::sregex_iterator();

    std::vector<std::string> mangled_names;
    std::vector<std::string> demangled_names;

    for (std::sregex_iterator i = mangled_table_begin; i != mangled_table_end; i++)
    {
        std::smatch match = *i;
        mangled_names.push_back(match.str());
    }

    std::string sout = eutil::exec("c++filt", mangled_names, pipe_status);

    std::stringstream ss(sout);
    std::string demangled_name = "";
    while (std::getline(ss, demangled_name))
        demangled_names.push_back(demangled_name);

    for (size_t idx = 0; idx < mangled_names.size(); idx++)
        demangling_table[mangled_names[idx]] = demangled_names[idx];
    return demangling_table;
}

auto make_demangling_table(const std::string &used_std, const std::string &CppFileName) -> std::map<std::string, std::string>
{
    std::string sout = "";
    int pipe_status = -1;
    std::map<std::string, std::string> demangling_table;

    std::string file_name_no_ext = eutil::strip_extension(CppFileName);

    sout = compile_obj(file_name_no_ext, used_std, pipe_status);
    std::cerr << sout;
    if (pipe_status == -1 || sout.size() > 0)
        return demangling_table;

    // symbol table for compiled object
    sout = eutil::exec("nm", {file_name_no_ext + ".o"}, pipe_status);
    if (pipe_status == -1)
        return demangling_table;

// clean-up
#ifdef _WIN32
    eutil::exec("del", {file_name_no_ext + ".o"}, pipe_status);
#endif
#ifdef __linux__
    eutil::exec("rm", {file_name_no_ext + ".o"}, pipe_status);
#endif

    if (pipe_status == -1)
        std::cerr << "Pipe failed during cleanup.";

    return demangle_sym_table(sout, pipe_status);
}