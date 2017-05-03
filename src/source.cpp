#include "source.h"

#include <fstream>
#include <sstream>

Source Source::fromString(const string& code) {
    Source src;
    src.data = code;
    return src;
}
Source::Source(const string& fileName) {
    // TODO: lazy file read
    std::ifstream t(fileName);
    std::stringstream buffer;
    buffer << t.rdbuf();
    data = buffer.str();
}
