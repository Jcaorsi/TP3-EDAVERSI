/**
 * @brief Implements the Reversi game model
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#ifndef MODEL_H
#define MODEL_H

#include <cstdint>
#include <vector>

#define BOARD_SIZE 8

enum Player
{
    PLAYER_BLACK,
    PLAYER_WHITE,
};
/*
enum Pieces
{
    PIECE_EMPTY,
    PIECE_BLACK,
    PIECE_WHITE,
};*/
#define PIECE_EMPTY 0
#define PIECE_BLACK 1
#define PIECE_WHITE 2
/*
//Obtuvimos información sobre la sobrecarga de operadores en C++ en: https://www.geeksforgeeks.org/operator-overloading-cpp/
struct Piece{
	char gamePiece;

    bool operator==(int v) const {
        return gamePiece == static_cast<char>(v);
    }

    bool operator!=(int v) const {
        return !(*this == v);
    }

    Piece& operator=(int v) {
        gamePiece = static_cast<char>(v);
        return *this;
    }

} ;*/

struct Square
{
	int x;
	int y;
};

#define GAME_INVALID_SQUARE \
    {                       \
        -1, -1              \
    }

struct GameModel
{
    bool gameOver;

    Player currentPlayer;

    double playerTime[2];
    double turnTimer;

    char board[BOARD_SIZE][BOARD_SIZE];

    Player humanPlayer;
};

typedef std::vector<Square> Moves;

/**
 * @brief Initializes a game model.
 *
 * @param model The game model.
 */
void initModel(GameModel &model);

/**
 * @brief Starts a game.
 *
 * @param model The game model.
 */
void startModel(GameModel &model);

/**
 * @brief Returns the model's current player.
 *
 * @param model The game model.
 * @return PLAYER_WHITE or PLAYER_BLACK.
 */
Player getCurrentPlayer(GameModel &model);

/**
 * @brief Returns the model's current score.
 *
 * @param model The game model.
 * @param player The player (PLAYER_WHITE or PLAYER_BLACK).
 * @return The score.
 */
int getScore(GameModel &model, Player player);

/**
 * @brief Returns the game timer for a player.
 *
 * @param model The game model.
 * @param player The player (PLAYER_WHITE or PLAYER_BLACK).
 * @return The time in seconds.
 */
double getTimer(GameModel &model, Player player);

/**
 * @brief Return a model's piece.
 *
 * @param model The game model.
 * @param square The square.
 * @return The piece at the square.
 */
char getBoardPiece(GameModel &model, Square square);

/**
 * @brief Sets a model's piece.
 *
 * @param model The game model.
 * @param square The square.
 * @param piece The piece to be set
 */
void setBoardPiece(GameModel &model, Square square, char piece);

/**
 * @brief Checks whether a square is within the board.
 *
 * @param square The square.
 * @return True or false.
 */
bool isSquareValid(Square square);

/**
 * @brief Returns a list of valid moves for the current player.
 *
 * @param model The game model.
 * @param validMoves A list that receives the valid moves.
 */
void getValidMoves(GameModel &model, Moves &validMoves);

/**
 * @brief Plays a move.
 *
 * @param model The game model.
 * @param square The move.
 * @return Move accepted.
 */
char playMove(GameModel &model, Square move);

#endif
