#include "scoreboard.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

ostream &operator<<(ostream &os, const scoreboard &s)
{
    os << "--------------------------------------------------------------------------\n"
       << "|                              Score Board                               |\n"
       << "--------------------------------------------------------------------------\n"
       << "|  Difficulty  |       Start Time       |  Spent (min)  |  # Hints Used  |\n";
    for (size_t i{0}; i < s.scores.size(); ++i)
        os << s.scores[i];
    os << "--------------------------------------------------------------------------\n";
    return os;
}

void scoreboard::order() {
    sort(scores.begin(), scores.end(), [](score &s1, score &s2) { return s1 >= s2; });
}

void scoreboard::add(score s) {
    scores.push_back(s);
    order();
}

int scoreboard::save(string filename) {
    ofstream ofs{filename};
    if (!ofs)
    {
        cerr << "Error opening file";
        return -1;
    }
    for (score & s : scores) {
        ofs << s;
    }
    ofs.close();
    return 0;
}

void scoreboard::load(ifstream & ifs) {
    char c; //separator
    int d, n; //difficulty, numHints
    double sp; //spent
    string st; //start
    
    while(ifs >> c >> d >> c >> st >> c >> sp >> c >> n >> c) {
        //extract time from string
        struct tm tm;
        stringstream stss(st);
        stss >> get_time(&tm, "%d-%m-%Y_%H:%M:%S");
        time_t stt = mktime(&tm);

        scores.emplace_back(d, stt, sp, n);
    }
}