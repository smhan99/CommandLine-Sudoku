# Final Project Proposal

Anni Pan, Jasmine Cao, Seungmin Han
November 28, 2022

### Sudoku!

- We will be implementing the traditional game of sudoku. We plan to use the basic C++ libraries to implement our game.
- We expect the structure of our sudoku board to be similar to that of the minesweeper, as we will have a 9x9 board of filled cells and non-filled cells. It will have a parent virtual class of cells and inherited classes of filled, guessed, and non-filled cells.
- Here are some of the features we plan to implement
  - The actual sudoku board generator will have three different levels, **easy, medium, hard** that the user can create a starting board with the selected difficulty
  - User can specify a cell they want to fill and with which number they want to fill it in with
    - User can enter the command `write [x] [y] [val]` to fill in a cell at position `(x, y)` with the certain number
    - Sudoku board will be updated accordingly with a big black number
  - We will have a **valid move** feature where the game lets the user know that a certain move is valid or not. If not, the game does not allow the user to perform the move. An **invalid move** is where the user tries to fill a cell with a certain number that the cell's row, column, or 3x3 box already includes
  - We will implement a **save/load** feature where users can have a checkpoint off of the games they are playing. The game state will be stored in a text file.
  - We will implement a **'Hint'** feature where the hint algorithm will tell the user which cell could be filled next with which number
  - We will implement a **'note'** feature where users could take note on which numbers could possibly fit in that cell, while not entirely making that cell 'filled'
    - User will enter the command `guess [x] [y] [val]` on the cell to type in a number to guess the number on the cell
    - Sudoku board will be updated accordingly with guesses displayed in a different color.
  - We will implement an **'undo'** feature where we will create a stack of all user actions, and pop the last input to retrieve back a previous move
  - We will implement a **timer** feature where we could time the players. If game is saved, the timer will pause and resume once loaded back.
  - User will **win** the game once the board is **valid** and **filled**. Timer will stop if the game is finished, and the scoreboard will be displayed.
  - We will implement a **scoreboard** feature where users could compare their best times. The scoreboard will have three sections according to difficulty, and rank according to **time** and **number of hints** used. The scoreboard will be saved and loaded from the same text file from when this game was first played.
- Timeline
  - We expect to spend the most time implementing the logic and algorithm of creating a valid sudoku board with three different difficulties. We would expect to spend around 3-4 days here.
  - We also plan to spend some time implementing how to design our classes and output the board. We expect to spend around 1-2 days.
  - For rest of the features, we expect them to be fairly straight forward. We expect around a day per feature.
- Work division
  - 2 people will be working on the game board algorithm and the basic game logic.
  - 1 person will be in charge of additional features like the timer, save/load, undo, and guess.
  - Once we get a basic sudoku working, we will then start implementing the additional features: guess, hint, save/load, undo, timer, and scoreboard features.
