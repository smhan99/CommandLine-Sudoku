#include "score.hpp"
#include <sstream>

using namespace std;

score::score(int d, time_t start, double spent, int num) : difficulty{d}, start{start}, 
                                                           spent{spent}, numHints{num} {}

score::score(const score & other) : difficulty{other.difficulty}, start{other.start}, 
                                    spent{other.spent}, numHints{other.numHints} {}
score::score(score && other) : difficulty{other.difficulty}, start{other.start}, 
                                spent{other.spent}, numHints{other.numHints} {}

score & score::operator=(score other) {
    swap(*this, other);
    return *this;
}

void swap(score & first, score & second) noexcept {
    using std::swap;
    std::swap(first.difficulty, second.difficulty);
    std::swap(first.start, second.start);
    std::swap(first.spent, second.spent);
    std::swap(first.numHints, second.numHints);
}

ostream &operator<<(ostream &os, const score &s)
{
    struct tm * timeinfo;
    char buffer[22];
    timeinfo = localtime(&(s.start));
    strftime(buffer,sizeof(buffer),"%d-%m-%Y_%H:%M:%S",timeinfo);
    string startStr(buffer);

    /* difficulty */
    string diff = "|      " + to_string(s.difficulty);
    if (s.difficulty == 10)
        diff += "      |";
    else 
        diff += "       |";

    /* Start time */
    string st = "   " + startStr;
    st += "  |";

    /* Spent time */
    string sp = to_string(s.spent);
    int left = (15-sp.size()) / 2;
    int right;
    if ((15-sp.size()) % 2 == 0)
        right = left;
    else
        right = left + 1;
    stringstream spnt;
    for (int i{0}; i < left; ++i)
        spnt << ' ';
    spnt << sp;
    for (int i{0}; i < right; ++i)
        spnt << ' ';
    spnt << '|';

    /* num Hints */
    string nh{to_string(s.numHints)};
    left = (16-nh.size()) / 2;
    if ((16-nh.size()) % 2 == 0)
        right = left;
    else
        right = left + 1;
    stringstream numH;
    for (int i{0}; i < left; ++i)
        numH << ' ';
    numH << nh;
    for (int i{0}; i < right; ++i)
        numH << ' ';
    numH << "|\n";

    os << diff << st << spnt.str() << numH.str();
    return os;
}

bool score::operator>(const score & other) {
    if (difficulty != other.difficulty) return difficulty > other.difficulty;
    if (numHints != other.numHints) return numHints < other.numHints;
    if (spent != other.spent) return spent < other.spent;
    return start < other.start;
}

bool score::operator<(const score & other) {
    if (difficulty != other.difficulty) return difficulty < other.difficulty;
    if (numHints != other.numHints) return numHints > other.numHints;
    if (spent != other.spent) return spent > other.spent;
    return start > other.start;
}

bool score::operator==(const score & other) {
    return difficulty == other.difficulty 
            && start == other.start 
            && spent == other.spent 
            && numHints == other.numHints;
}

bool score::operator >=(const score & other) {
    return *this > other || *this == other;
}

bool score::operator <=(const score & other) {
    return *this < other || *this == other;
}