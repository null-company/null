#include "utils/util.h"
#include <random>

std::string generateSixLetterCode() {
    const std::string_view char_set(
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789");

    std::string s("      "); //six spaces
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<std::size_t> dist(0u, char_set.size() - 1);

    for (char &c: s) {
        c = char_set[dist(engine)];
    }
    return s;
}
