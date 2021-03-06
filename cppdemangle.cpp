#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <sstream>
#include <fstream>
#include "include/exec.h"

auto compile_obj(const std::string &file_name, const std::string &used_std, int &pipe_status) -> std::string;
auto demangle_sym_table(const std::string &sym_table, int &pipe_status) -> std::map<std::string, std::string>;
auto make_demangling_table(const std::string &used_std, const std::string &Cppfile_name) -> std::map<std::string, std::string>;
auto make_asm_file(std::map<std::string, std::string> &demangling_table, const std::string &used_std, const std::string &file_name, std::ostream &outstream) -> void;

auto main(int argc, char *argv[]) -> int
{
    int exec_code = -1;
    std::map<std::string, std::string> demangling_table;
    std::string used_std = "";

    if (argc == 1)
    {
        std::cerr << "CPPDEMANGLE failed: no arguments passed" << std::endl;
        return -1;
    }
    else if (argc == 2)
        used_std = "c++11";
    else if (argc == 3)
        used_std = argv[1];

    demangling_table = make_demangling_table(used_std, /* CPP file */ argv[argc - 1]);
    std::string file_name_no_extension = eutil::strip_extension(argv[argc - 1]);

    if (!demangling_table.empty())
    {
        std::ofstream asm_file(file_name_no_extension + ".s");
        make_asm_file(demangling_table, used_std, file_name_no_extension, asm_file);
        asm_file.close();
    }

    for (auto it = demangling_table.begin(); it != demangling_table.end(); it++)
        std::cout << it->first << " -> " << it->second << std::endl;

    std::cout << "Demangled assembly file \"" << file_name_no_extension << ".s\" created.\n";

    return 0;
}

auto compile_obj(const std::string &file_name, const std::string &used_std, int &pipe_status) -> std::string
{
    // compile single cpp file into obj file
    std::vector<std::string> gcc_args{"-O0", "-w", "-std=" + used_std, file_name + ".cpp", "-c", "-o", file_name + ".o"};
    return eutil::exec("g++", gcc_args, pipe_status);
}

auto demangle_sym_table(const std::string &sym_table, int &pipe_status) -> std::map<std::string, std::string>
{
    constexpr size_t PARTITION_SIZE = 32;
    // mangled -> demangled
    std::map<std::string, std::string> demangling_table;

    // https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling
#ifdef WIN32
    // |\\b__Z\\w+\\b
    std::regex mangled_name_regex("(__Z\\w+\\b)");
#endif
#ifdef __linux__
    std::regex mangled_name_regex("(_Z\\w+\\b)");
#endif

    auto mangled_table_begin = std::sregex_iterator(sym_table.begin(), sym_table.end(), mangled_name_regex);
    auto mangled_table_end = std::sregex_iterator();

    std::vector<std::string> mangled_names;
    std::vector<std::string> demangled_names;

    for (std::sregex_iterator i = mangled_table_begin; i != mangled_table_end; i++)
    {
#if 0
        std::string matched = (*i).str();
        if (!eutil::contains(mangled_names, matched))
            mangled_names.push_back(matched);
#endif
#if 1
        std::smatch matched = *i;
        mangled_names.push_back(matched.str());
#endif
    }

    std::vector<std::vector<std::string>> partitioned_mangled_names;
    eutil::partition(mangled_names, PARTITION_SIZE, partitioned_mangled_names);
    for (auto it = partitioned_mangled_names.cbegin(); it != partitioned_mangled_names.cend(); it++)
    {
        std::string sout = eutil::exec("c++filt", *it, pipe_status);
        std::stringstream ss(sout);
        std::string demangled_name = "";
        while (std::getline(ss, demangled_name))
            demangled_names.push_back(demangled_name);
    }

    for (size_t idx = 0; idx < mangled_names.size(); idx++)
        demangling_table[mangled_names[idx]] = demangled_names[idx];

    return demangling_table;
}

auto make_demangling_table(const std::string &used_std, const std::string &Cppfile_name) -> std::map<std::string, std::string>
{
    std::string sout = "";
    int pipe_status = -1;
    std::map<std::string, std::string> demangling_table;

    std::string file_name_no_ext = eutil::strip_extension(Cppfile_name);

    sout = compile_obj(file_name_no_ext, used_std, pipe_status);
    std::cerr << sout << std::flush;
    if (pipe_status == -1 || sout.size() > 0)
        return demangling_table;

    // symbol table for compiled object
    sout = eutil::exec("nm", {file_name_no_ext + ".o"}, pipe_status);
    if (pipe_status == -1)
        return demangling_table;

        // clean-up
#ifdef WIN32
    eutil::exec("del", {file_name_no_ext + ".o"}, pipe_status);
#endif
#ifdef __linux__
    eutil::exec("rm", {file_name_no_ext + ".o"}, pipe_status);
#endif

    if (pipe_status == -1)
        std::cerr << "Pipe failed during cleanup." << std::flush;

    return demangle_sym_table(sout, pipe_status);
}

auto make_asm_file(std::map<std::string, std::string> &demangling_table, const std::string &used_std, const std::string &file_name, std::ostream &outstream) -> void
{
    int pipe_status = -1;
    std::vector<std::string> gcc_args{"-O0", "-std=" + used_std, file_name + ".cpp", "-fverbose-asm", "-S", "-o", file_name + ".s"};
    std::string sout = eutil::exec("g++", gcc_args, pipe_status);

    if (pipe_status == -1)
        std::cerr << "Pipe failed during cleanup." << std::flush;

    std::ifstream asm_file(file_name + ".s");
    std::string file_txt = "";
    if (asm_file.is_open())
    {
        std::string line = "";
        while (std::getline(asm_file, line))
            file_txt += line + "\n";
    }
    asm_file.close();
    for (auto it = demangling_table.begin(); it != demangling_table.end(); it++)
        eutil::replace(file_txt, it->first, it->second);

    outstream << file_txt;
}