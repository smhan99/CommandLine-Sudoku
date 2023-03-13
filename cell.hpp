#ifndef CELL_HPP
#define CELL_HPP

class cell
{
private:
    bool empty = true;
    int value = 0;
    bool guess = false;
    

public:
    cell(const cell &) = delete;
    cell(cell &&) = delete;
    cell &operator=(cell) = delete;
    ~cell() {};

    cell(); // empty cell with value 0

    void fill(int, bool g = false); // fill with int
    void erase(); //erase
    bool isEmpty();
    int getVal();
    bool isGuess();
    void keep();

};

#endif