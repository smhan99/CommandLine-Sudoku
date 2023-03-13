#ifndef SCORE
#define SCORE

#include <ctime>
#include <iostream>

class score {
private:
    int difficulty;
    std::time_t start;
    double spent; //time spent in minutes
    int numHints;

public:
    score() = delete;
    score(const score &);
    score(score &&);
    score &operator=(score);
    friend void swap(score &, score &) noexcept;

    friend std::ostream &operator<<(std::ostream &, const score &);
    bool operator >(const score &);
    bool operator <(const score &);
    bool operator ==(const score &);
    bool operator >=(const score &);
    bool operator <=(const score &);

    score(int, std::time_t, double, int);
};

#endif