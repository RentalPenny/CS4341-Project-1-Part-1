#include <iostream>
#include <vector>
#include <string.h>

//TODO: Add alpha-beta pruning

using namespace std;

enum Token{X, O};
Token myToken = X;

class Board {

    public:
    vector<vector<char>> config; //2d matrix of what each slot contains
    vector<Board> kids; //array of possible Boards to go from this Board
    string move; //the move that got the game to this board

    //Constructors

    Board() {

        vector<vector<char>> config;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                config[i][j] = '-';
            }
        }

        vector<Board> kids = {};
        string move = "";

    };

    Board(string move) {
        char conVert[3];
        strcpy(conVert, move.c_str());

        int row;
        if(conVert[1] == '3') row = 0;
        else if(conVert[1] == '2') row = 1;
        else if(conVert[1] == '1') row = 2;
        int col;
        if(conVert[0] == 'a') col = 0;
        else if(conVert[0] == 'b') col = 1;
        else if(conVert[0] == 'c') col = 2;

        vector<vector<char>> config;

        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if((i == row) & (j == col)) config[i][j] = myToken;
                else config[i][j] = '-';
            }
        }

    };

    Board(vector<vector<char>> config, vector<Board> kids, string move) {

        config = config;
        kids = kids;
        move = move;

    };


    //
    //Functions

    /**
    * @brief  Main recursive minimax function for Tic Tac Toe
    * 
    * @param  max:  True if looking for maximum, false if looking for minimum
    * 
    * @return The equivalent utility value of this board (either its value if terminal, or the value sent up to it if not)
    */
    int checkBoard(bool max) {

        int currValue; //-1: lose, 0: tie, 1: win, -2: not terminal
        currValue = this->checkWin();

        if(currValue = -2) { //If terminal, doesnt check kids. If not, it's 10PM

            if(max) { //If 
                int currMax = 2;
                vector<Board> nextMoves = this->nextMoves(true);

                for(int i = 0; i < sizeof(nextMoves); i++){
                    Board nextBoard = nextMoves[i]; //board configuration of the kid we're looking at
                    int nextValue = nextBoard.checkBoard(false); //It's 10PM
                    if(nextValue > currMax) currMax = nextValue; //store max, when done will be overall max
                }
                currValue = currMax;

            }else {
                int currMin = -2;
                vector<Board> nextMoves = this->nextMoves(false);

                for(int i = 0; i < sizeof(nextMoves); i++){
                    Board nextBoard = nextMoves[i]; //board configuration of the kid we're looking at
                    int nextValue = nextBoard.checkBoard(true); //It's 10PM
                    if(nextValue < currMin) currMin = nextValue; //store min, when done will be overall min
                }
                currValue = currMin;

            }
        }

        return currValue; //Returns max/min of kids, or utility value if terminal
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
        strcpy(conVert, move.c_str());

        int row;
        if(conVert[1] == '3') row = 0;
        else if(conVert[1] == '2') row = 1;
        else if(conVert[1] == '1') row = 2;
        int col;
        if(conVert[0] == 'a') col = 0;
        else if(conVert[0] == 'b') col = 1;
        else if(conVert[0] == 'c') col = 2;

        Board nextBoard = *this;
        if(me) {
            if(myToken == X) nextBoard.config[row][col] = 'x';
            else nextBoard.config[row][col] = 'o';
        } else {
            if(myToken == X) nextBoard.config[row][col] = 'o';
            else nextBoard.config[row][col] = 'x';
        }

        nextBoard.move = move;

        return nextBoard;
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

        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {

                string moveRow;
                string moveCol;
                string fullMove;
                static int size = 0;

                if(currConfig[i][j] == '-') {
                    
                    //Convert row colum to move
                    if(i == 0) moveRow = "3";
                    else if(i == 1) moveRow = "2";
                    else moveRow = "1";

                    if(j == 0) moveCol = "a";
                    else if(j == 1) moveCol = "b";
                    else moveCol = "c";

                    fullMove = moveCol.append(moveRow);

                    Board nextBoard = this->makeMove(fullMove, me);
                    nextMoves[size] = nextBoard;
                    size++;

                }

            }
        }

        this->kids = nextMoves;
        return nextMoves;

    }

    /**
    * @brief  Checks if the calling board is terminal, and if so returns its utility value. if not terminal, returns -2
    * 
    * @return The utility value of the board if terminal, -2 if not terminal
    */
    int checkWin() {

        int utilValue = -2; //default not terminal, will change to -1, 0, or 1 if it is terminal
        vector<vector<char>> config = this->config;

        if( 
            ( (config[0][0] == 'x') && (config[0][1] == 'x') && (config[0][2] == 'x') ) ||
            ( (config[1][0] == 'x') && (config[1][1] == 'x') && (config[1][2] == 'x') ) ||
            ( (config[2][0] == 'x') && (config[2][1] == 'x') && (config[2][2] == 'x') ) ||
            ( (config[0][0] == 'x') && (config[1][0] == 'x') && (config[2][0] == 'x') ) ||
            ( (config[0][1] == 'x') && (config[1][1] == 'x') && (config[2][1] == 'x') ) ||
            ( (config[0][2] == 'x') && (config[1][2] == 'x') && (config[2][2] == 'x') ) ||
            ( (config[0][0] == 'x') && (config[1][1] == 'x') && (config[2][2] == 'x') ) ||
            ( (config[0][2] == 'x') && (config[1][1] == 'x') && (config[2][0] == 'x') )
        ) {
            if(myToken == X) utilValue = 1;
            else utilValue = -1;
        } else if( 
            ( (config[0][0] == 'o') && (config[0][1] == 'o') && (config[0][2] == 'o') ) ||
            ( (config[1][0] == 'o') && (config[1][1] == 'o') && (config[1][2] == 'o') ) ||
            ( (config[2][0] == 'o') && (config[2][1] == 'o') && (config[2][2] == 'o') ) ||
            ( (config[0][0] == 'o') && (config[1][0] == 'o') && (config[2][0] == 'o') ) ||
            ( (config[0][1] == 'o') && (config[1][1] == 'o') && (config[2][1] == 'o') ) ||
            ( (config[0][2] == 'o') && (config[1][2] == 'o') && (config[2][2] == 'o') ) ||
            ( (config[0][0] == 'o') && (config[1][1] == 'o') && (config[2][2] == 'o') ) ||
            ( (config[0][2] == 'o') && (config[1][1] == 'o') && (config[2][0] == 'o') )
        ) {
            if(myToken == O) utilValue = 1;
            else utilValue = -1;
        } else {
            utilValue = 0;
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    if((char)config[i][j] == '-') utilValue = -2;
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

    cout << move << endl; //Technically same as not doing helper but sendMove(move) is more fun to type than cout << move << endl

}


enum GameState{Start, TakeTurn, ReceiveMove, Endgame};
enum WinState{Win, Lose, Tie, NotYet};
/**
* @brief  Primary loop of the game. Waits for incoming move, makes the move, then calls functions to determine best next move, and sends to referee
* 
* @return Void
*/
void loop() {

    static string incomingMessage = ""; //Initialize to empty string, so only contains something when received move
    static int currBest = -2; //Value of the best next move so far
    static string bestNextMove = ""; //String representation of the best move so far
    static Board currBoard = Board(); //On first pass makes blank board config

    static WinState winState = NotYet;

    static GameState currState = Start;

    vector<Board> nextMoves;

    switch(currState) {
        case Start:
 
            cin >> incomingMessage;

            if(incomingMessage != "") {
                if(incomingMessage == "X") {
                    myToken = X;
                    currState = TakeTurn;
                } else {
                    myToken = O;
                    currState = ReceiveMove;
                }
            }

        break;

        case TakeTurn:
        
            nextMoves = currBoard.nextMoves(true); //Populates the child nodes of current Board and saves them to array of Boards

            for(int i = 0; i < sizeof(nextMoves); i++){ //Looks at each child node
                Board nextBoard = currBoard.kids[i]; //Gets the current child
                string nextMove = nextBoard.move; //Retrieves the move to get to the current child
                int nextVal = nextBoard.checkBoard(true);
                if(nextVal > currBest) {
                    currBest = nextVal;
                    bestNextMove = nextMove;
                }
            }

            currBoard = currBoard.makeMove(bestNextMove, true); //Updates the current board state to the move we are about to make
            sendMove(bestNextMove);

            currBest = -2;
            bestNextMove = "";

            currState = ReceiveMove;

        break;

        case ReceiveMove:

            cin >> incomingMessage; //Accepts move from referee
            
            if(incomingMessage != "") { //TODO: Add functionality to detect an endgame message
                currBoard = currBoard.makeMove(incomingMessage, false); //Applies move to the current board configuration

                incomingMessage = "";

                currState = TakeTurn;
            }

        break;

        case Endgame:

        break;
    }

}