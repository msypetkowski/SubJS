#pragma once

#include <string>
using std::string;


class Source {
    string data;
public:
    static Source fromString(const string&);
    Source() { }
    Source(const string& filename);

    char operator[](int i)const {
        return data[i];
    }

    unsigned size()const {
        return data.size();
    }

    Source& operator+=(char c) {
        data+=c;
        return *this;
    }

    Source& operator+=(string s) {
        data+=s;
        return *this;
    }
};
