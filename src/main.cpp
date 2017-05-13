#include "source.h"
#include "lexer.h"
#include "parser.h"
#include "interpr.h"

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/combine.hpp>
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

    Source source;
    if (vm.count("command")) {
        source = Source::fromString(vm["command"].as<string>());
    } else if (vm.count("input-file")) {
        string fileName = vm["input-file"].as<string>();
        if (!boost::filesystem::exists(fileName)) {
            std::cout << "File \"" << fileName << "\" does not exist." << endl;
            return 1;
        }
        source = Source(fileName);
    } else {
        std::cout << "Interactive mode not implemented." <<endl;
        return 1;
    }

    if (vm.count("lexical-only")) {
        Lexer l(source);
        if (l.run()){
            cout << "Analysis finished. Atoms list:" << endl;
            for (Atom* a : l.getAtoms()) {
                cout << a->getStr() << endl;
            }
        } else {
            cout << "Errors occured during lexical analysis. Errors:" << endl;
            for (auto a : l.getErrors()) {
                cout << "at line: " << a.first + 1 << " ";
                cout << "at collumnn: " << a.second + 1 << endl;
            }
            // cout << "Found atoms: " << endl;
            // for (Atom* a : l.getAtoms()) {
            //     cout << a->getStr() << endl;
            // }
        }
    } else if (vm.count("parse-only")) {
        Lexer l(source);
        Parser p(l);
        p.Program();
        if (p.getErrorsMessages().empty()) {
            cout << "Syntax tree:" << endl;
            cout << p.getTreeStr();
        } else {
            cout << "Errors:" << endl;
            assert(p.getErrorsPositions().size() == p.getErrorsMessages().size());
            for (auto tup : boost::combine(
                        p.getErrorsPositions(), p.getErrorsMessages())) {
                std::pair<unsigned, unsigned> pos;
                string msg;
                boost::tie(pos,msg) = tup;
                cout << msg << endl;
                cout << "at line: " << pos.first + 1 << " ";
                cout << "at collumnn: " << pos.second + 1 << endl;
                cout << endl;
            }
        }
    } else { // interpretation
        Lexer l(source);
        Parser p(l);
        p.Program();
        if (p.getErrorsMessages().empty()) {
            Interpreter i(p.getTreeRoot());
            i.run();
        } else {
            cout << "Errors:" << endl;
            assert(p.getErrorsPositions().size() == p.getErrorsMessages().size());
            for (auto tup : boost::combine(
                        p.getErrorsPositions(), p.getErrorsMessages())) {
                std::pair<unsigned, unsigned> pos;
                string msg;
                boost::tie(pos,msg) = tup;
                cout << msg << endl;
                cout << "at line: " << pos.first + 1 << " ";
                cout << "at collumnn: " << pos.second + 1 << endl;
                cout << endl;
            }
        }
    }

    return 0;
}
