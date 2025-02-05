#include <iostream>
#include <vector>
#include <string.h>
#include <ctime>

using namespace std;

enum Token { X, O }; //An enum to store the token of GW, the token it will place on the board
Token myToken; //The token of GW

class Board {

public:
    vector<vector<char>> config; // 2d matrix of what each slot contains
    vector<Board> kids;          // array of possible Boards to go from this Board
    string move;                 // the move that got the game to this board
    int numWinsFromKids;         // number of wins from down all branches of this board

    // Constructors

    Board() { // Default constructor, creates a blank board

        config = {
            {'-', '-', '-'},
            {'-', '-', '-'},
            {'-', '-', '-'}
        };


        vector<Board> kids = {};
        string move = "";
        int numWinsFromKids = 0;
    };

    Board(string move) { //Constructor for when the oponent makes a move on the blank board. Configures board with the oponents token, and sets the move to the move made
        char conVert[3];
        strcpy(conVert, move.c_str()); //Converts the string to an array of its characters to examine row and column seperately

        int row; //Converts the row to an integer
        if (conVert[1] == '3') row = 0;
        else if (conVert[1] == '2') row = 1;
        else if (conVert[1] == '1') row = 2;
        
        int col; //Converts the column to an integer
        if (conVert[0] == 'a') col = 0;
        else if (conVert[0] == 'b') col = 1;
        else if (conVert[0] == 'c') col = 2;

        vector<vector<char>> config;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if ((i == row) & (j == col)) config[i][j] = myToken;
                else config[i][j] = '-';
            }
        }

        config = config;
        vector<Board> kids = {};
        move = move;
        int numWinsFromKids = 0;
    };

    //
    // Functions

    /**
     * @brief  Main recursive minimax function for Tic Tac Toe
     *
     * @param  max:  True if looking for maximum, false if looking for minimum
     *
     * @return The equivalent utility value of this board (either its value if terminal, or the value sent up to it if not)
     */
    int checkBoard(bool max, int &alpha, int &beta) {

        int currValue; //-1: lose, 0: tie, 1: win, -2: not terminal
        currValue = this->checkWin();

        //If the board is a win or loss, increases or decreases numWinsFromKids accordingly
        if(currValue == 1) this->numWinsFromKids++; 
        else if(currValue == -1) this->numWinsFromKids--;

        if (currValue == -2) { // If terminal, doesnt check kids. If not, it's 10PM

            if (max) {
                int currMax = -2;
                int currMaxNumWins = 0;
                vector<Board> nextMoves = this->nextMoves(true); //Creates and populates a list of all the possible moves that can be made from this board

                for (Board nextBoard : nextMoves) { // board configuration of the kid we're looking at

                    //cout << "Next Move: " << nextBoard.move << endl; //FOR TESTING
                    //printBoard(nextBoard.config); //FOR TESTING
                    //cout << "Next Board Size: " << nextBoard.config.size() << endl; //FOR TESTING

                    int nextValue = nextBoard.checkBoard(false, alpha, beta); // It's 10PM

                    if(nextValue > currMax) { //If value of next board greater than currMax, doesnt need to tie break     
                        currMax = nextValue; // store max, when done will be overall max
                        currMaxNumWins = nextBoard.numWinsFromKids; //Stores the number of wins from the kids of the current board                   
                    } else if (nextValue = currMax) { //If a tie    
                        if(nextBoard.numWinsFromKids > currMaxNumWins) { //Breaks the tie based on which has more wins down its branches
                            currMax = nextValue; // store max, when done will be overall max
                            currMaxNumWins = nextBoard.numWinsFromKids; //Stores the number of wins from the kids of the current board
                        }
                    }

                    if(currMax > alpha) alpha  = currMax; //If the current max is greater than alpha, sets alpha to the current max
                    if(beta < alpha) break; //If beta is less than alpha, doesnt need to check the rest of this branch
                }
                currValue = currMax; //Updates the utility value of this board to the max of its children
            }
            else {

                int currMin = 2;
                int currMinNumWins = 0;
                vector<Board> nextMoves = this->nextMoves(false); //Creates and populates a list of all the possible moves that can be made from this board

                for (Board nextBoard : nextMoves) { // board configuration of the kid we're looking at

                    //cout << "Next Move: " << nextBoard.move << endl; //FOR TESTING
                    //printBoard(nextBoard.config); //FOR TESTING
                    //cout << "Next Board Size: " << nextBoard.config.size() << endl; //FOR TESTING

                    int nextValue = nextBoard.checkBoard(true, alpha, beta); // It's 10PM
                    
                    if(nextValue < currMin) { //If value of next board less than currMin, doesnt need to tie break
                        currMin = nextValue; // store min, when done will be overall min
                        currMinNumWins = nextBoard.numWinsFromKids; //Stores the number of wins from the kids of the current board
                    }
                    else if (nextValue = currMin) { //If a tie
                        if(nextBoard.numWinsFromKids < currMinNumWins) { //Breaks the tie based on which has fewer wins down its branches
                            currMin = nextValue; // store min, when done will be overall min
                            currMinNumWins = nextBoard.numWinsFromKids; //Stores the number of wins from the kids of the current board
                        }
                    }
                    
                    if(currMin < beta) beta = currMin; //If the current min is less than beta, sets beta to the current min
                    if(beta < alpha) break; //If beta is less than alpha, doesnt need to check the rest of this branch
                }
                currValue = currMin; //Updates the utility value of this board to the min of its children
            }
        }

        return currValue; // Returns max/min of kids, or utility value if terminal
    }

    /**
     * @brief  Given a move and if the player or opponent is playing, marks the spot that was used
     *
     * @param  move:  String representation of the move being made (e.g. "a2" or "c1")
     * @param  me:   True if making a move from the player, false if opponent's move
     *
     * @return The Board object corresponding to the game state after the move is made
     */
    Board makeMove(string move, bool me) {

        char conVert[3];
        strcpy(conVert, move.c_str()); //Converts the string to an array of its characters to examine row and column seperately

        int row; //Converts the row to an integer
        if (conVert[1] == '3') row = 0;
        else if (conVert[1] == '2') row = 1;
        else if (conVert[1] == '1') row = 2;
        
        int col; //Converts the column to an integer
        if (conVert[0] == 'a') col = 0;
        else if (conVert[0] == 'b') col = 1;
        else if (conVert[0] == 'c') col = 2;

        Board nextBoard = *this; // Copies the current board configuration to the new board
        if (me) { // If GW is making the move, marks the spot with GW's token
            if (myToken == X) nextBoard.config[row][col] = 'x';
            else nextBoard.config[row][col] = 'o';
        }
        else { // If the opponent is making the move, marks the spot with the opponent's token
            if (myToken == X) nextBoard.config[row][col] = 'o';
            else nextBoard.config[row][col] = 'x';
        }

        nextBoard.move = move; // Saves the move that was made to get to this board

        return nextBoard; // Returns the new board configuration
    }

    /**
     * @brief  Populates the list of children of the calling board, and returns the list
     *
     * @param  me:   True if would be moves made by the player, false if moves would be made by opponent
     *
     * @return A list of boards that could be made with a move from the current board configuration
     */
    vector<Board> nextMoves(bool me) {

        vector<Board> nextMoves; 
        vector<vector<char>> currConfig = this->config;

        for (int i = 0; i < 3; i++) { // Loops through the current board configuration
            for (int j = 0; j < 3; j++) {

                string moveRow;
                string moveCol;
                string fullMove;


                if (currConfig[i][j] == '-') { // If the spot is empty, makes a move there

                    // Convert row colum to move
                    if (i == 0) moveRow = "3";
                    else if (i == 1) moveRow = "2";
                    else moveRow = "1";

                    if (j == 0) moveCol = "a";
                    else if (j == 1) moveCol = "b";
                    else moveCol = "c";

                    fullMove = moveCol.append(moveRow); // Combines the row and column to make the move

                    Board nextBoard = this->makeMove(fullMove, me); // Makes the move on the current board configuration
                    nextMoves.push_back(nextBoard); // Adds the new board configuration to the list of children
                }
            }
        }

        //cout << "# Next Moves: " << nextMoves.size() << endl; //FOR TESTING
        this->kids = nextMoves; // Saves the list of children to the current board configuration
        return nextMoves; // Returns the list of children
    }

    /**
     * @brief  Prints the current board configuration
     *
     * @param  config:  The current board configuration
     *
     * @return void
     */
    void printBoard(vector<vector<char>> config) {

        cout << "  a b c" << endl;
        cout << "3 " << config[0][0] << " " << config[0][1] << " " << config[0][2] << endl;
        cout << "2 " << config[1][0] << " " << config[1][1] << " " << config[1][2] << endl;
        cout << "1 " << config[2][0] << " " << config[2][1] << " " << config[2][2] << endl;
    }

    /**
     * @brief  Checks if the calling board is terminal, and if so returns its utility value. if not terminal, returns -2
     *
     * @return The utility value of the board if terminal, -2 if not terminal
     */
    int checkWin() {

        int utilValue = -2; // default not terminal, will change to -1, 0, or 1 if it is terminal
        vector<vector<char>> config = this->config;

        if ( // Checks all possible win conditions with x token
            ((config[0][0] == 'x') && (config[0][1] == 'x') && (config[0][2] == 'x')) ||
            ((config[1][0] == 'x') && (config[1][1] == 'x') && (config[1][2] == 'x')) ||
            ((config[2][0] == 'x') && (config[2][1] == 'x') && (config[2][2] == 'x')) ||
            ((config[0][0] == 'x') && (config[1][0] == 'x') && (config[2][0] == 'x')) ||
            ((config[0][1] == 'x') && (config[1][1] == 'x') && (config[2][1] == 'x')) ||
            ((config[0][2] == 'x') && (config[1][2] == 'x') && (config[2][2] == 'x')) ||
            ((config[0][0] == 'x') && (config[1][1] == 'x') && (config[2][2] == 'x')) ||
            ((config[0][2] == 'x') && (config[1][1] == 'x') && (config[2][0] == 'x'))) {
            
            if (myToken == X) utilValue = 1; // If the player is X, the utility value is 1
            else utilValue = -1; // If the player is O, the utility value is -1

        } else if ( // Checks all possible win conditions with o token
            ((config[0][0] == 'o') && (config[0][1] == 'o') && (config[0][2] == 'o')) ||
            ((config[1][0] == 'o') && (config[1][1] == 'o') && (config[1][2] == 'o')) ||
            ((config[2][0] == 'o') && (config[2][1] == 'o') && (config[2][2] == 'o')) ||
            ((config[0][0] == 'o') && (config[1][0] == 'o') && (config[2][0] == 'o')) ||
            ((config[0][1] == 'o') && (config[1][1] == 'o') && (config[2][1] == 'o')) ||
            ((config[0][2] == 'o') && (config[1][2] == 'o') && (config[2][2] == 'o')) ||
            ((config[0][0] == 'o') && (config[1][1] == 'o') && (config[2][2] == 'o')) ||
            ((config[0][2] == 'o') && (config[1][1] == 'o') && (config[2][0] == 'o'))) {
            
            if (myToken == O) utilValue = 1; // If the player is O, the utility value is 1
            else utilValue = -1; // If the player is X, the utility value is -1

        } else { //Checks if the board is full, if so, the utility value is 0, else it is not terminal so utility value = -2
            utilValue = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if ((char)config[i][j] == '-') utilValue = -2;
                }
            }
        }

        return utilValue;
    }
};

/**
 * @brief  Function to send a move to the referee
 *
 * @param  move:  String representation of the move being sent (e.g. "a2" or "c1")
 *
 * @return void
 */
void sendMove(string move) {
    cout << move << endl; // Technically same as not doing helper but sendMove(move) is more fun to type than cout << move << endl
}

enum GameState{ Start, TakeTurn, ReceiveMove }; // Enum to keep track of the current state of the game
enum WinState{ Win, Lose, Tie, NotYet }; // Enum to keep track of the current win state of the game
/**
 * @brief  Primary loop of the game. Waits for incoming move, makes the move, then calls functions to determine best next move, and sends to referee
 *
 * @return Void
 */
WinState winState = NotYet; // Initialize to not yet won, will change if win, lose, or tie
GameState currState = Start; // Initialize to start state
int main() {

    string incomingMessage = ""; // Initialize to empty string, so only contains something when received move
    int currBest = -2;           // Value of the best next move so far
    string bestNextMove = "";    // String representation of the best move so far
    Board currBoard = Board();   // On first pass makes blank board config
    vector<Board> nextMoves;

    // Alpha Beta Pruning. Initialize alpha to negative large number and beta to positive large number
    int alpha = -9999; 
    int beta = 9999;

    bool ongoing = true; // Boolean to keep track of if the game is still ongoing

    while (ongoing) {

        switch (currState) {
            case Start:

                //cin >> incomingMessage; // Accepts the color of the player
                incomingMessage = "blue"; //FOR TESTING

                if (incomingMessage != "") { // If the color of the player is received
                    if (incomingMessage == "blue") { //If GW is blue, assigns token to X and goes to TakeTurn state
                        myToken = X;
                        currState = TakeTurn;
                    }
                    else { //If GW is orange, assigns token to O and goes to ReceiveMove state
                        myToken = O;
                        currState = ReceiveMove;
                    }
                }

            break;

            case TakeTurn:

                nextMoves = currBoard.nextMoves(true); // Populates the child nodes of current Board and saves them to array of Boards
                for (Board nextBoard : nextMoves) {// Looks at each child node
                    string nextMove = nextBoard.move;    // Retrieves the move to get to the current child
                    //cout << "Next Move: " << nextMove << endl; //FOR TESTING
                    //nextBoard.printBoard(nextBoard.config); //FOR TESTING
                    int nextVal = nextBoard.checkBoard(false, alpha, beta); // Gets the utility value of the current child
                    if (nextVal > currBest) {
                        currBest = nextVal;
                        bestNextMove = nextMove;
                    }
                }

                currBoard = currBoard.makeMove(bestNextMove, true); // Updates the current board state to the move we are about to make
                //cout << "Best Move: " << bestNextMove << endl; //FOR TESTING
                //currBoard.printBoard(currBoard.config); //FOR TESTING
                sendMove(bestNextMove);


                //Resets the best values for next move
                currBest = -2;
                bestNextMove = "";

                if(currBoard.checkWin() != -2) ongoing = false; //We sent winning move, so game is over
                else currState = ReceiveMove; //If game is not over, goes to ReceiveMove state

            break;

            case ReceiveMove:
                cin >> incomingMessage; // Accepts move from referee

                /* FOR TESTING
                static int i = 1;
                switch(i) {
                    case 1:
                        incomingMessage = "a1";
                        i++;
                    break;
                    case 2:
                        incomingMessage = "b2";
                        i++;
                    break;
                    case 3:
                        incomingMessage = "c1";
                        i++;
                    break;
                }
                */
            
                if (incomingMessage.find("END") != std::string::npos) ongoing = false; //If referee calls game over, game is over
                else if (incomingMessage != "") {

                    currBoard = currBoard.makeMove(incomingMessage, false); // Applies move to the current board configuration

                    //cout << "Opponent Move: " << incomingMessage << endl; //FOR TESTING
                    //currBoard.printBoard(currBoard.config); //FOR TESTING

                    incomingMessage = ""; // Resets incoming message to empty string

                    currState = TakeTurn; // Goes to TakeTurn state
                }
            break;
        }
    }
}