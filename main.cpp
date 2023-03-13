#include "game.hpp"
#include "scoreboard.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

void handleGameOver(game &G, scoreboard &sb)
{
    sb.add(G.calculateScore());
    string message = "Congrats! You Win!";
    cout << message << endl;
}

int main()
{
    bool nogame = true;
    string s; //for command
    game G{};

    scoreboard sb{};
    ifstream sbFile{"scoreboard"};
    if (sbFile) { //if exist
        sb.load(sbFile); //load
    }

    int d, x, y, v; // difficulty, x, y, value
    cout << "Welcome to sudoku! Enter 'help' for the manual or\n"
         << "start a new game with the 'new [diff]' keyword with\n"
         << "diff as a number indicating difficulty from [0,10].\n" << endl;

    do // terminate when EOF is received
    {
        cout << "Enter command: ";
        if (!getline(cin, s))
            break;
        istringstream iss{s};
        string command;

        iss >> command;

        if (command == "help") 
        {
            cout << " -----------------------Sudoku Manual----------------------- \n"
                 << "| new [diff] : start new game with difficulty diff          |\n"
                 << "| write [row] [col] [v] : fill the cell in row, col with v  |\n"
                 << "| guess : enter guess mode, every `write` will be a `guess` |\n"
                 << "| abort : abort all guesses made so far, enter `write` mode |\n"
                 << "| keep : keep all guesses made so far, enter `write` mode   |\n"
                 << "| undo : undo the last move                                 |\n"
                 << "| hint : display a single possible move                     |\n"
                 << "| save [filename] : save current sudoku with filename       |\n"
                 << "| load [filename] : load a sudoku with filename             |\n"
                 << "| show : print the currently loaded sudoku game             |\n"
                 << "| board : print the scoreboard                              |\n" 
                 << "| ^D : exit                                                 |\n"
                 << " ----------------------------------------------------------- \n" << endl;
        } 
        else if (command == "new")
        {
            iss >> d;
            try {
                G = game{d};
            } catch(invalid_argument& e) {
                cerr << e.what() << endl;
                continue;
            }
            nogame = false;
            cout << G << endl;
        }
        else if (command == "write")
        {
            if (nogame) {
                cout << "No game loaded" << endl;
                continue;
            }
            iss >> x >> y >> v;

            int res = G.fill(x - 1, y - 1, v);
            cout << G << endl;
            if (res == 0) // if just regular hit
                continue;
            if (res == -1) {
                string message = "Not a valid move";
                cout << message << endl;
                continue;
            }
            handleGameOver(G, sb);
            nogame = true;
        }
        else if (command == "guess") 
        {
            if (nogame) continue;
            G.startGuess();
            cout << "Entered guess mode! Guesses will be displayed in red" << endl;
            cout << G << endl;
        }
        else if (command == "abort")
        {
            if (nogame) continue;
            if (!G.isGuessing()) {
                cout << "Not in guess mode" << endl;
                cout << G << endl;
                continue;
            }
            G.abort();
            cout << "Aborted guess" << endl;
            cout << G << endl;
        }
        else if (command == "keep")
        {
            if (nogame) continue;
            if (!G.isGuessing()) {
                cout << "Not in guess mode" << endl;
                cout << G << endl;
                continue;
            }
            int res = G.keep();
            cout << "Kept guess" << endl;
            cout << G << endl;
            if (res != 0) // if just regular hit
                continue;
            if (res == -1) {
                string message = "Not a valid move";
                cout << message << endl;
                continue;
            }
            handleGameOver(G, sb);
            nogame = true;
        }
        else if (command == "undo")
        {
            if (nogame) continue;
            int undo_ret = G.undo();
            if (undo_ret < 0) {
                cout << "Not a valid undo move" << endl;
            } else {
                cout << G << endl;
            }
        }
        else if (command == "hint")
        {
            if (nogame || G.isGuessing()) continue;
            pair<pair<int, int>, int> hint = G.hint();
            if (hint.second == -1) {
                cerr << "No grid with unique answer" << endl;
                continue;
            }
            int res = G.fill(hint.first.first, hint.first.second, hint.second, true);
            cout << "The cell at (" << hint.first.first + 1 << ", " 
                << hint.first.second << ") has been filled with value " << hint.second << endl;
            cout << G << endl;
            
            string message;
            if (res == 0) {
                continue;
            } else if (res == -1) {
                message = "Not a valid move";
                cout << message << endl;
                continue;
            } else if (res == 1) { // game is over
                handleGameOver(G, sb);
                nogame = true;
            }            
        }
        else if (command == "save")
        {
            // if there is no game, dont save
            if (nogame) {
                cout << "No game to save" << endl;
                continue;
            }
            string filename;
            iss >> filename;
            if (G.save(filename) == -1)
                cerr << "Error while saving" << endl;
        }
        else if (command == "load")
        {
            if (nogame) {
                G = game{0};
            }
            string filename;
            iss >> filename;
            ifstream inputfile{filename};
            if (!inputfile)
            {
                cerr << "Error opening file" << endl;
                continue;
            }
            G.load(inputfile);
            inputfile.close();
            cout << G << endl;
            nogame = false;
        } else if (command == "show") {
            if (nogame) {
                cout << "No game loaded" << endl;
                continue;
            }
            cout << G << endl;
        } else if (command == "board") {
            cout << sb << endl;
        }
    } while (true);

    if (sb.save("scoreboard") == -1) //save scoreboard when exit
        cerr << "Scoreboard not properly saved" << endl;
}