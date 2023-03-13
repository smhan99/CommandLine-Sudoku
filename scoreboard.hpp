#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include <vector>
#include <fstream>
#include <iostream>

#include "score.hpp"

class scoreboard {
    private:
    std::vector<score> scores;
    void order();

    public:
    scoreboard() {};
    scoreboard(const scoreboard &) = delete;
    scoreboard(scoreboard &&) = delete;
    scoreboard &operator=(scoreboard) = delete;
    
    friend std::ostream &operator<<(std::ostream &, const scoreboard &);

    void add(score);

    int save(std::string);
    void load(std::ifstream &);
};

#endif