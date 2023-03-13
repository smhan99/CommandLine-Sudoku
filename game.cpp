#include "game.hpp"

#include <memory>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <list>

using namespace std;

game::game(int diff) : diff{diff},
                       grid{make_unique<unique_ptr<shared_ptr<cell>[]>[]>(width)}
{
    if (diff < 0 || diff > 10)
        throw invalid_argument("Difficulty must be an integer between [0, 10]");
    // same coordinate system for minesweeper
    for (int i{0}; i < width; ++i)
    {
        grid[i] = make_unique<shared_ptr<cell>[]>(height);
        for (int j{0}; j < height; ++j)
        {
            grid[i][j] = make_shared<cell>();
        }
    }
    //Sudoku generator algorithm
    int removed = generate();
    //Update count so that it is filled values
    count = removed;
    //Reset undo moves stack
    moves = vector<pair<int, int>>();
    //Reset number of hints
    numHints = 0;
    //Set start time
    startTime = time(0);
    //Reset duration
    duration = 0;
    //Set resume time
    resumeTime = startTime;
}

game::game(const game &other) : diff{other.diff}, count{other.count},
                                grid{make_unique<unique_ptr<shared_ptr<cell>[]>[]>(width)},
                                moves{other.moves},
                                numHints{other.numHints},
                                startTime{other.startTime},
                                duration{other.duration},
                                resumeTime{other.resumeTime}
{
    for (int i{0}; i < width; ++i)
    {
        grid[i] = make_unique<shared_ptr<cell>[]>(height);
        for (int j{0}; j < height; ++j)
        {
            grid[i][j] = other.grid[i][j];
        }
    }
}

game::game(game &&other) : diff{other.diff}, count{other.count}, moves{other.moves}, numHints{other.numHints}, startTime{other.startTime}, duration{other.duration}, resumeTime{other.resumeTime}
{
    grid = std::move(other.grid);
}

game &game::operator=(game other)
{
    swap(*this, other);
    return *this;
}

void swap(game &first, game &second) noexcept
{
    using std::swap;
    std::swap(first.diff, second.diff);
    std::swap(first.count, second.count);
    std::swap(first.moves, second.moves);
    std::swap(first.startTime, second.startTime);
    std::swap(first.numHints, second.numHints);
    std::swap(first.duration, second.duration);
    std::swap(first.resumeTime, second.resumeTime);
    std::swap(first.grid, second.grid);
}

ostream &operator<<(ostream &os, const game &g)
{
    // TODO: figure out a way to print notes and values at the same time
    // print row by row
    os << " -----------------------------\n";
    for (int i{0}; i < g.height; ++i)
    {
        for (int j{0}; j < g.width; ++j) {
            if (j == 0)
                os << "| ";

            if (g.grid[i][j]->isEmpty()) {
                os << "-";
            } else {
                if (g.grid[i][j]->isGuess()) {
                    if (j == 0 || j % 3 == 0) {
                        os << setw(3);
                        os << "\033[1;31m" << g.grid[i][j]->getVal() << "\033[0m";
                    } else {
                        os << "  \033[1;31m" << g.grid[i][j]->getVal() << "\033[0m";
                    }
                } else {
                    os << g.grid[i][j]->getVal();
                }
            }

            if (j % 3 == 2)
                os << " | ";
            else
                os << setw(3);
        }
        os << "\n";
        if (i % 3 == 2) {
            os << " -----------------------------\n";
        }
    }
    return os;
}

int game::generate() {
    fillDiag();
    solve();
    int rem = remove();
    return rem;
}

void game::fillDiag() {
    fillBox(0, 0);
    fillBox(3, 3);
    fillBox(6, 6);
}

bool game::safeRow(int r, int v) {
    for (int i{0}; i < 9; ++i) {
        if (grid[r][i]->getVal() == v)
            return false;
    }
    return true;
}

bool game::safeCol(int c, int v) {
    for (int i{0}; i < 9; ++i) {
        if (grid[i][c]->getVal() == v)
            return false;
    }
    return true;
}

bool game::safeBox(int r, int c, int v) {
    for (int i{0}; i < 3; ++i) {
        for (int j{0}; j < 3; ++j) {
            if (grid[r+i][c+j]->getVal() == v)
                return false;
        }
    }
    return true;
}

bool game::isSafe(int r, int c, int v) {
    if (r < 0 || r > 8 || c < 0 || c > 8)
        return false;
    return safeRow(r, v) && safeCol(c, v) && safeBox(r - (r%3), c - (c%3), v);
}

bool game::findUnfilled(int & r, int & c) {
    for (r=0; r < 9; ++r) {
        for (c=0; c < 9; ++c) {
            if (grid[r][c]->isEmpty())
                return true;
        }
    }
    return false;
}

bool game::solve() {
    int row, col;

    if (!findUnfilled(row, col))
       return true;

    for (int i{1}; i < 10; ++i)
    {
        if (isSafe(row, col, i))
        {
            grid[row][col]->fill(i);

            if (solve())
                return true;

            grid[row][col]->erase();
        }
    }
    return false;
}

void game::fillBox(int r, int c) {
    random_device rd;
    default_random_engine g(rd());
    shuffle(shuff.begin(), shuff.end(), g);
    for (int i{0}; i < 3; ++i) {
        for (int j{0}; j < 3; ++j) {
            grid[r+i][c+i]->fill(shuff[i*3+j]);
        }
    }
}

void game::countSol(int & cnt) {
    int row, col;

    if(!findUnfilled(row, col)) {
        cnt++;
        return;
    }

    for(int i{1}; i<10 && cnt<2; ++i)
    {
        if( isSafe(row, col, i) )
        {
            grid[row][col]->fill(i);
            countSol(cnt);
        }

        grid[row][col]->erase();
    }
}

int game::remove() {
    int max = (diff+1) * 6; //can remove [max] cells
    int max_temp = max;
    array<int, 81> boxes; // indicate which cell to remove
    iota(boxes.begin(), boxes.end(), 0);
    random_device rd;
    default_random_engine g(rd());
    shuffle(boxes.begin(), boxes.end(), g);

    for (int i : boxes) { //i=[0,80]
        if (!max) //removed [max] cells
            return max_temp; //return max removed
        int r = i / 9; 
        int c = i % 9;
        int temp = grid[r][c]->getVal();
        grid[r][c]->erase(); //try

        int cnt{0};
        countSol(cnt);
        if (cnt != 1) { //check if unique solution exist
            grid[r][c]->fill(temp);
            continue;
        }
        max--;
    }
    return max_temp - max; //return however many removed
}

int game::fill(int x, int y, int v, bool isHint) {
    // check if move is valid
    if (!isSafe(x, y, v))
        return -1;
    moves.emplace_back(make_pair(x, y));
    grid[x][y]->fill(v, guess && !isHint); // fill
    if (guess) { numGuess++; }
    if (!guess) { count--; }
    if (count == 0) //No need to check if the board is valid because every move is validated
        return 1;
    return 0;
}

pair<pair<int, int>, int> game::hint() {
    /*
    1. evaluate the board, get the possible numbers that could be filled in
    2. return cell with one possible number, or `errVal` (second entry is -1) when there are none
    */
   numHints++;
   list<pair<pair<int, int>, int>> allHints = {};
   for (int i {0}; i < width; i++) {
      for (int j {0}; j < height; j++) {
          pair<int, int> ways = getNumWaysAndAns(i, j);
          if (ways.first == 1) { allHints.push_back(make_pair(make_pair(i, j), ways.second)); }
      }
   }

   pair<int, int> errCoordinates = make_pair(-1, -1);
   pair<pair<int, int>, int> errVal = make_pair(errCoordinates, -1);
   if (allHints.size() == 0) { return errVal; }

   auto it = allHints.begin();
   for (double r = rand() % allHints.size(); r != 0; r--) { it++; }
   return *it;
}

int game::undo() {
    /*
    1. pop the user move stack
    2. undo the move
    3. count++
    */
//  no moves left to undo -- invalid
    if (moves.size() == 0) {
        return -1;
    }
    pair<int, int> undo_move = moves.at(moves.size() - 1);
    int x = undo_move.first;
    int y = undo_move.second;
    grid[x][y]->erase();

    moves.pop_back();
    if (!guess) { count++; }

    return 0;
}

int game::save(string filename)
{
    ofstream ofs{filename};
    if (!ofs)
    {
        cerr << "Error opening file";
        return -1;
    }
    score s = calculateScore();
    ofs << s;
    ofs << count << " " << numGuess << " " << guess << endl;

    stringstream moves_ss;
    for (auto &m : moves) {
        moves_ss << m.first << " " << m.second << endl;
    }
    ofs << moves_ss.str();
    ofs << "BOARD" << endl;

    stringstream board;
    for (int i{0}; i < width; ++i)
    {
        for (int j{0}; j < height; ++j)
        {
            auto currCell = grid[i][j];
            board << currCell->getVal() << " " << currCell->isGuess() << endl;
        }
    }
    ofs << board.str();
    ofs.close();
    return 0;
}

void game::load(ifstream &ifs)
{
    string line;
    char c;
    string startTimeString;

    ifs >> c >> diff >> c >> startTimeString >> c >> duration >> c >> numHints >> c;
    ifs >> count >> numGuess >> guess;

    //extract time from string
    struct tm tm;
    stringstream stss(startTimeString);
    stss >> get_time(&tm, "%d-%m-%Y_%H:%M:%S");
    startTime = mktime(&tm);

    // parse moves vector
    moves.clear();
    getline(ifs, line); // skip newline
    while (getline(ifs, line)) {
        if (line == "BOARD") {
            break;
        }
        int x, y;
        stringstream ss{line};
        ss >> x >> y;
        moves.emplace_back(make_pair(x, y));
    }
    // parse board
    for (int i{0}; i < width; i++) {
        for (int j{0}; j < height; j++) {
            int val;
            bool isGuess;
            ifs >> val >> isGuess;
            grid[i][j]->fill(val, isGuess);
        }
    }

    resumeTime = time(0);
}

void game::startGuess() {
    guess = true;
}

void game::abort() {
    while (numGuess > 0) { 
        numGuess--;
        undo(); 
    }
    guess = false;
}

int game::keep() {
    count -= numGuess;
    numGuess = 0;
    guess = false;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (grid[i][j]->isGuess()) {
                grid[i][j]->keep();
            }
        }
    }
    return count;
}

bool game::isGuessing() {
    return guess;
}

/*
    Get the number of ways to fill in grid[r][c] as the first entry.
    If there's a unique way to fill in grid[r][c], then the second entry stores that number.
*/
pair<int, int> game::getNumWaysAndAns(int r, int c) {
    // if the cell has been filled, return 0
    if (!grid[r][c]->isEmpty()) { return make_pair(0, -1); }
    unordered_set possibleAns {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // remove value from `possibleAns` as we iterate through rows, cols and box
    for (int otherRow = 0; otherRow < 9; otherRow++) {
        if (int val = grid[otherRow][c]->getVal()) { possibleAns.erase(val); }
    }
    for (int otherCol = 0; otherCol < 9; otherCol++) {
        if (int val = grid[r][otherCol]->getVal()) { possibleAns.erase(val); }
    }
    int boxRow = r / 3;
    int boxCol = c / 3;
    for (int i = 3 * boxRow; i < 3 * boxRow + 3; i++) {
        for (int j = 3 * boxCol; j < 3 * boxCol + 3; j++) {
            if (int val = grid[i][j]->getVal()) { possibleAns.erase(val); }
        }
    }
    if (possibleAns.size() > 1) { return make_pair(possibleAns.size(), -1); }
    return make_pair(possibleAns.size(), *possibleAns.begin());
}

score game::calculateScore() {
    time_t endTime = time(0);
    if (startTime == resumeTime) { // game was never loaded
        duration = static_cast<double>((endTime - startTime) / 60.0);
    } else {
        duration = static_cast<double>((endTime - resumeTime) / 60.0) + duration;
    }
    score s = score{diff, startTime, duration, numHints};
    return s;
}