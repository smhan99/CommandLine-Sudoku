#ifndef GAME_HPP
#define GAME_HPP

#include "cell.hpp"
#include "score.hpp"

#include <iostream>
#include <memory>
#include <array>
#include <vector>

class game
{
private:
    const int width{9};
    const int height{9};
    int diff;
    int count;
    std::unique_ptr<std::unique_ptr<std::shared_ptr<cell>[]>[]> grid; // height * width

    std::array<int, 9> shuff{1,2,3,4,5,6,7,8,9}; //random shuffle to fill the grid
    std::vector<std::pair<int, int>> moves; // stack of user's moves.
    
    int numGuess = 0;
    bool guess = false;
    int numHints;
    time_t startTime;
    double duration; // time from start to when game was saved
    time_t resumeTime; // time when game was loaded
    
    //sudoku generator helper funcitons
    int generate();
    void fillDiag();
    bool solve();
    void fillBox(int, int);
    void countSol(int &);
    int remove();

    bool isSafe(int, int, int);
    bool safeRow(int, int);
    bool safeCol(int, int);
    bool safeBox(int, int, int);
    bool findUnfilled(int &, int &);
    std::pair<int, int> getNumWaysAndAns(int, int);

public:
    game() {};
    game(int);
    game(const game &);
    game(game &&);
    game &operator=(game);
    friend void swap(game &, game &) noexcept;

    friend std::ostream &operator<<(std::ostream &os, const game &g);
    
    int fill(int, int, int, bool isHint = false);

    std::pair<std::pair<int, int>, int> hint();
    int undo();

    bool isGuessing();
    void startGuess();
    void abort();
    int keep();
    int save(std::string);
    void load(std::ifstream &);

    score calculateScore();
};

#endif