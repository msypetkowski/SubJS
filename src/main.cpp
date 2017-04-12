#include "lexer.h"

#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
namespace po = boost::program_options;
using std::cout;
using std::endl;
using std::string;

void conflicting_options(const po::variables_map& vm,
                         const char* opt1, const char* opt2)
{
    if (vm.count(opt1) && !vm[opt1].defaulted()
        && vm.count(opt2) && !vm[opt2].defaulted())
        throw std::logic_error(string("Conflicting options '")
                          + opt1 + "' and '" + opt2 + "'.");
}

int main(int argc, char **argv) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "print help message")
        ("lexical-only,l", "perform only lexical analysis")
        ("parse-only,p", "perform only parsing (and lexical analysis)")
        ("command,c", po::value<string>(), "execute command")
        ("input-file", po::value<string>(), "input file")
    ;
    po::positional_options_description pd;
    pd.add("input-file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
          options(desc).positional(pd).run(), vm);
    po::notify(vm);


    if (vm.count("help")) {
        cout << desc << endl;
        return 1;
    }

    conflicting_options(vm, "lexical-only", "parse-only");
    conflicting_options(vm, "input-file", "command");

    string code="";
    if (vm.count("command")) {
        code = vm["command"].as<string>();
    } else if (vm.count("input-file")) {
        string fileName = vm["input-file"].as<string>();
        if (!boost::filesystem::exists(fileName)) {
            std::cout << "File \"" << fileName << "\" does not exist." << endl;
            return 1;
        }
        std::ifstream t(fileName);
        std::stringstream buffer;
        buffer << t.rdbuf();
        code = buffer.str();
    } else {
        std::cout << "No code is given. Exiting." <<endl;
        return 1;
    }

    if (vm.count("lexical-only")) {
        Lexer l(code);
        if (l.run()){
            cout << "Analysis finished. Atoms list:" << endl;
            for (Atom* a : l.getAtoms()) {
                cout << a->getStr() << endl;
            }
        } else {
            cout << "Errors occured during lexical analysis. Errors:" << endl;
            for (unsigned a : l.getErrors()) {
                cout << "at: " << a << endl;
            }
        }
    } else if (vm.count("parse-only")) {
        cout << "Parsing is not implemented" << endl;
    } else {
        cout << "Interpretation is not implemented" << endl;
    }

    return 0;
}
