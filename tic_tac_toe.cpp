#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <climits>

using namespace std;

enum Difficulty { EASY = 0, MEDIUM = 1, HARD = 2 };

class TicTacToe {
private:
    char board[3][3];
    char humanPlayer, aiPlayer;
    struct {
        int wins, losses, draws;
    } score;

public:
    TicTacToe() : humanPlayer('X'), aiPlayer('O'), score{0, 0, 0} {
        initBoard();
    }

    void initBoard() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                board[i][j] = ' ';
    }

    void printBoard() const {
        cout << "\n";
        for (int i = 0; i < 3; i++) {
            cout << " " << board[i][0] << " | " << board[i][1] << " | " << board[i][2] << "\n";
            if (i < 2) cout << "-----------\n";
        }
        cout << "\nPosition numbers:\n";
        cout << " 1 | 2 | 3\n-----------\n";
        cout << " 4 | 5 | 6\n-----------\n";
        cout << " 7 | 8 | 9\n\n";
    }

    bool isValidMove(int pos) const {
        if (pos < 1 || pos > 9) return false;
        int row = (pos - 1) / 3;
        int col = (pos - 1) % 3;
        return board[row][col] == ' ';
    }

    void makeMove(int pos, char player) {
        int row = (pos - 1) / 3;
        int col = (pos - 1) % 3;
        board[row][col] = player;
    }

    void undoMove(int pos) {
        int row = (pos - 1) / 3;
        int col = (pos - 1) % 3;
        board[row][col] = ' ';
    }

    bool isBoardFull() const {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board[i][j] == ' ') return false;
        return true;
    }

    vector<int> getAvailableMoves() const {
        vector<int> moves;
        for (int i = 1; i <= 9; i++)
            if (isValidMove(i)) moves.push_back(i);
        return moves;
    }

    bool hasWon(char player) const {
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
                return true;
            if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
                return true;
        }
        for (int j = 0; j < 3; j++) {
            if (board[0][j] == player && board[1][j] == player && board[2][j] == player)
                return true;
        }
        if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
            return true;
        if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
            return true;
        return false;
    }

    int evaluate() const {
        if (hasWon(aiPlayer)) return 10;
        if (hasWon(humanPlayer)) return -10;
        return 0;
    }

    int minimax(int depth, bool isMaximizing, int alpha, int beta) {
        int score = evaluate();

        if (score == 10) return score - depth;
        if (score == -10) return score + depth;
        if (isBoardFull()) return 0;

        if (isMaximizing) {
            int maxScore = INT_MIN;
            for (int move : getAvailableMoves()) {
                makeMove(move, aiPlayer);
                maxScore = max(maxScore, minimax(depth + 1, false, alpha, beta));
                undoMove(move);
                alpha = max(alpha, maxScore);
                if (beta <= alpha) break;
            }
            return maxScore;
        } else {
            int minScore = INT_MAX;
            for (int move : getAvailableMoves()) {
                makeMove(move, humanPlayer);
                minScore = min(minScore, minimax(depth + 1, true, alpha, beta));
                undoMove(move);
                beta = min(beta, minScore);
                if (beta <= alpha) break;
            }
            return minScore;
        }
    }

    int findBestMove(Difficulty difficulty) {
        vector<int> moves = getAvailableMoves();

        if (difficulty == EASY) {
            return moves[rand() % moves.size()];
        }

        int bestScore = INT_MIN;
        int bestMove = moves[0];
        int maxDepth = (difficulty == MEDIUM) ? 4 : 9;

        for (int move : moves) {
            makeMove(move, aiPlayer);
            int score = minimax(0, false, INT_MIN, INT_MAX);
            undoMove(move);

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
        return bestMove;
    }

    bool getPlayerMove() {
        int move;
        while (true) {
            cout << "Enter your move (1-9): ";
            cin >> move;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Enter a number between 1-9.\n";
                continue;
            }

            if (!isValidMove(move)) {
                cout << "Invalid move! That position is already taken or out of range.\n";
                continue;
            }

            makeMove(move, humanPlayer);
            return true;
        }
    }

    void displayScore() const {
        cout << "\n========== SCORE ==========\n";
        cout << "Wins: " << score.wins << " | Losses: " << score.losses
             << " | Draws: " << score.draws << "\n";
        cout << "===========================\n\n";
    }

    char getGameResult() {
        if (hasWon(humanPlayer)) return 'P';
        if (hasWon(aiPlayer)) return 'A';
        if (isBoardFull()) return 'D';
        return 'N';
    }

    void updateScore(char result) {
        if (result == 'P') score.wins++;
        else if (result == 'A') score.losses++;
        else if (result == 'D') score.draws++;
    }

    void swapPlayers() {
        if (humanPlayer == 'X') {
            humanPlayer = 'O';
            aiPlayer = 'X';
        } else {
            humanPlayer = 'X';
            aiPlayer = 'O';
        }
    }

    void playGame(Difficulty difficulty, bool playerFirst) {
        initBoard();

        if (!playerFirst) {
            humanPlayer = 'O';
            aiPlayer = 'X';
        } else {
            humanPlayer = 'X';
            aiPlayer = 'O';
        }

        bool playerTurn = playerFirst;

        while (true) {
            printBoard();

            if (playerTurn) {
                cout << "Your turn (X):\n";
                getPlayerMove();
            } else {
                cout << "AI is thinking...\n";
                int move = findBestMove(difficulty);
                makeMove(move, aiPlayer);
                cout << "AI played at position " << move << "\n\n";
            }

            char result = getGameResult();
            if (result != 'N') {
                printBoard();
                if (result == 'P') {
                    cout << "🎉 You won!\n";
                    updateScore('P');
                } else if (result == 'A') {
                    cout << "AI won! Better luck next time.\n";
                    updateScore('A');
                } else {
                    cout << "It's a draw!\n";
                    updateScore('D');
                }
                displayScore();
                break;
            }

            playerTurn = !playerTurn;
        }
    }
};

int main() {
    srand(time(0));
    TicTacToe game;

    cout << "========================================\n";
    cout << "   TIC-TAC-TOE with Minimax Algorithm   \n";
    cout << "========================================\n\n";

    while (true) {
        cout << "Select Difficulty:\n";
        cout << "1. Easy\n";
        cout << "2. Medium\n";
        cout << "3. Hard (Unbeatable)\n";
        cout << "4. Exit\n";
        cout << "Choice: ";

        int diffChoice;
        cin >> diffChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input!\n\n";
            continue;
        }

        if (diffChoice == 4) {
            game.displayScore();
            cout << "Thanks for playing!\n";
            break;
        }

        if (diffChoice < 1 || diffChoice > 3) {
            cout << "Invalid choice!\n\n";
            continue;
        }

        Difficulty difficulty = static_cast<Difficulty>(diffChoice - 1);

        cout << "\nWho goes first?\n";
        cout << "1. You (X)\n";
        cout << "2. AI (O)\n";
        cout << "Choice: ";

        int playerChoice;
        cin >> playerChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input!\n\n";
            continue;
        }

        if (playerChoice < 1 || playerChoice > 2) {
            cout << "Invalid choice!\n\n";
            continue;
        }

        bool playerFirst = (playerChoice == 1);

        game.playGame(difficulty, playerFirst);

        cout << "\nPlay again? (1=Yes, 0=No): ";
        int playAgain;
        cin >> playAgain;

        if (playAgain != 1) {
            game.displayScore();
            cout << "Thanks for playing!\n";
            break;
        }
        cout << "\n";
    }

    return 0;
}
