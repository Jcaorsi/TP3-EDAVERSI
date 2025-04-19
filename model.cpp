/**
 * @brief Implements the Reversi game model
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include "raylib.h"

#include "model.h"

enum Positions
{
    UPPER_LEFT,
    UPPER_MIDDLE,
    UPPER_RIGHT,
    MIDDLE_LEFT,
    MIDDLE_RIGHT,
    LOWER_LEFT,
    LOWER_MIDDLE,
    LOWER_RIGHT
};
void initModel(GameModel &model)
{
    model.gameOver = true;

    model.playerTime[0] = 0;
    model.playerTime[1] = 0;

    memset(model.board, PIECE_EMPTY, sizeof(model.board));
}

void startModel(GameModel &model)
{
    model.gameOver = false;

    model.currentPlayer = PLAYER_BLACK;

    model.playerTime[0] = 0;
    model.playerTime[1] = 0;
    model.turnTimer = GetTime();

    memset(model.board, PIECE_EMPTY, sizeof(model.board));
    model.board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2 - 1] = PIECE_WHITE;
    model.board[BOARD_SIZE / 2 - 1][BOARD_SIZE / 2] = PIECE_BLACK;
    model.board[BOARD_SIZE / 2][BOARD_SIZE / 2] = PIECE_WHITE;
    model.board[BOARD_SIZE / 2][BOARD_SIZE / 2 - 1] = PIECE_BLACK;
}

Player getCurrentPlayer(GameModel &model)
{
    return model.currentPlayer;
}

int getScore(GameModel &model, Player player)
{
    int score = 0;

    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            if (((model.board[y][x] == PIECE_WHITE) &&
                 (player == PLAYER_WHITE)) ||
                ((model.board[y][x] == PIECE_BLACK) &&
                 (player == PLAYER_BLACK)))
                score++;
        }

    return score;
}

double getTimer(GameModel &model, Player player)
{
    double turnTime = 0;

    if (!model.gameOver && (player == model.currentPlayer))
        turnTime = GetTime() - model.turnTimer;

    return model.playerTime[player] + turnTime;
}

Piece getBoardPiece(GameModel &model, Square square)
{
    return model.board[square.y][square.x];
}

void setBoardPiece(GameModel &model, Square square, Piece piece)
{
    model.board[square.y][square.x] = piece;
}

bool isSquareValid(Square square)
{
    return (square.x >= 0) &&
           (square.x < BOARD_SIZE) &&
           (square.y >= 0) &&
           (square.y < BOARD_SIZE);
}

void getValidMoves(GameModel &model, Moves &validMoves)
{
    Piece playerPiece =
	    (getCurrentPlayer(model) == PLAYER_WHITE)
	    ? PIECE_WHITE
	    : PIECE_BLACK;
    Piece opponentPiece = (playerPiece == PIECE_WHITE)
        ? PIECE_BLACK
        : PIECE_WHITE;

    //validMoves.clear();
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            Square move = { x, y };

            /*
            Si la pieza actual esta vacia, se fija en las de alrededor a ver si hay alguna del oponente. Si la encuentra,
            avanza en esa direccion (determinada por el switch) siempre y cuando encuentre fichas del oponente. El lugar
            vacio analizado resulta ser valido si despues de fichas del oponente encuentra una ficha del jugador de turno.
            */

            if (getBoardPiece(model, move) == PIECE_EMPTY) {

                /* s de surroundings pues busca fichas del oponente en los "alrededores" d la ficha actual (usamos la letra
                y no la palabra entera porque es un indice para recorrer otra cosa) */
                for (int s = UPPER_LEFT; s <= LOWER_RIGHT; s++) {
                    int i = 1; // para avanzar en la direccion s
                    Square analizedSquare;
                    Piece analizedPiece = PIECE_EMPTY;

                    do {
                        switch (s) {
                        case UPPER_LEFT:
                            analizedSquare = { x - i, y - i };
                            break;

                        case UPPER_MIDDLE:
                            analizedSquare = { x, y - i };
                            break;

                        case UPPER_RIGHT:
                            analizedSquare = { x + i, y - i };
                            break;

                        case MIDDLE_LEFT:
                            analizedSquare = { x - i, y };
                            break;
                        case MIDDLE_RIGHT:
                            analizedSquare = { x + i, y };
                            break;

                        case LOWER_LEFT:
                            analizedSquare = { x - i, y + i };
                            break;

                        case LOWER_MIDDLE:
                            analizedSquare = { x, y + i };
                            break;
                        case LOWER_RIGHT:
                            analizedSquare = { x + i, y + i };
                            break;
                        }
                        i++;
                    } while ( isSquareValid(analizedSquare) &&
                              (analizedPiece = getBoardPiece(model, analizedSquare)) == opponentPiece);

                    /* me interesa ver si analizedPiece es mia solo si encontre al menos una pieza de mi rival,
                    es decir si avance al menos una vez. Al final de la primera pasada por el do, i=2 */
                    if (analizedPiece == playerPiece && i > 2)
                    {
                        validMoves.push_back(move);
                        s = LOWER_RIGHT + 1; //para que salga del for y avance a evaluar el siguiente square
                    }
                }
            }
        }
    }
}

bool playMove(GameModel &model, Square move)
{
    // Set game piece
    Piece piece =
        (getCurrentPlayer(model) == PLAYER_WHITE)
            ? PIECE_WHITE
            : PIECE_BLACK;

    setBoardPiece(model, move, piece);

    // To-do: your code goes here...
	Piece enemyPiece =
		(getCurrentPlayer(model) == PLAYER_WHITE)
		? PIECE_BLACK
		: PIECE_WHITE;

	for (int lookAround = UPPER_LEFT; lookAround <= LOWER_RIGHT; lookAround++) {
        int i, analizedPiece, t;
        switch (lookAround) {
        case UPPER_LEFT:
            for (i = 1; (analizedPiece = getBoardPiece(model, { move.x - i, move.y - i })) == enemyPiece &&
                isSquareValid({ move.x - i, move.y - i });
                i++);

            if (isSquareValid({ move.x - i, move.y - i }) && analizedPiece == piece && i > 1)
                for (t = 1; t != i; ++t) {
                    setBoardPiece(model, { move.x - t, move.y - t }, piece);
                }
            break;

        case UPPER_MIDDLE:
            for (i = 1; (analizedPiece = getBoardPiece(model, { move.x, move.y - i })) == enemyPiece &&
                isSquareValid({ move.x, move.y - i });
                i++);

            if (isSquareValid({ move.x, move.y - i }) && analizedPiece == piece && i > 1)
                for (t = 1; t != i; ++t) {
                    setBoardPiece(model, { move.x, move.y - t }, piece);
                }
            break;

        case UPPER_RIGHT:
            for (i = 1; (analizedPiece = getBoardPiece(model, { move.x + i, move.y - i })) == enemyPiece &&
                isSquareValid({ move.x + i, move.y - i });
                i++);

            if (isSquareValid({ move.x + i, move.y - i }) && analizedPiece == piece && i > 1)
                for (t = 1; t != i; ++t) {
                    setBoardPiece(model, { move.x + t, move.y - t }, piece);
                }
            break;
        case MIDDLE_LEFT:
            for (i = 1; (analizedPiece = getBoardPiece(model, { move.x - i, move.y })) == enemyPiece &&
                isSquareValid({ move.x - i, move.y });
                i++);

            if (isSquareValid({ move.x - i, move.y }) && analizedPiece == piece && i > 1)
                for (t = 1; t != i; ++t) {
                    setBoardPiece(model, { move.x - t, move.y }, piece);
                }
            break;
        case MIDDLE_RIGHT:
            for (i = 1; (analizedPiece = getBoardPiece(model, { move.x + i, move.y })) == enemyPiece &&
                isSquareValid({ move.x + i, move.y });
                i++);

            if (isSquareValid({ move.x + i, move.y }) && analizedPiece == piece && i > 1)
                for (t = 1; t != i; ++t) {
                    setBoardPiece(model, { move.x + t, move.y }, piece);
                }
            break;
        case LOWER_LEFT:
            for (i = 1; (analizedPiece = getBoardPiece(model, { move.x - i, move.y + i })) == enemyPiece &&
                isSquareValid({ move.x - i, move.y + i });
                i++);

            if (isSquareValid({ move.x - i, move.y + i }) && analizedPiece == piece && i > 1)
                for (t = 1; t != i; ++t) {
                    setBoardPiece(model, { move.x - t, move.y + t }, piece);
                }
            break;
        case LOWER_MIDDLE:
            for (i = 1; (analizedPiece = getBoardPiece(model, { move.x, move.y + i })) == enemyPiece &&
                isSquareValid({ move.x, move.y + i });
                i++);

            if (isSquareValid({ move.x, move.y + i }) && analizedPiece == piece && i > 1)
                for (t = 1; t != i; ++t) {
                    setBoardPiece(model, { move.x, move.y + t }, piece);
                }
            break;
        case LOWER_RIGHT:
            for (i = 1; (analizedPiece = getBoardPiece(model, { move.x + i, move.y + i })) == enemyPiece &&
                isSquareValid({ move.x + i, move.y + i });
                i++);

            if (isSquareValid({ move.x + i, move.y + i }) && analizedPiece == piece && i > 1)
                for (t = 1; t != i; ++t) {
                    setBoardPiece(model, { move.x + t, move.y + t }, piece);
                }
            break;
        }
	}


    // Update timer
    double currentTime = GetTime();
    model.playerTime[model.currentPlayer] += currentTime - model.turnTimer;
    model.turnTimer = currentTime;

    // Swap player
    model.currentPlayer =
        (model.currentPlayer == PLAYER_WHITE)
            ? PLAYER_BLACK
            : PLAYER_WHITE;

    // Game over?
    Moves validMoves;
    getValidMoves(model, validMoves);

    if (validMoves.size() == 0)
        model.gameOver = true;

    return true;
}
