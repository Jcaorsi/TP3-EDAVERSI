/**
 * @brief Implements the Reversi game model
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include "raylib.h"

#include "model.h"

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
    // To-do: your code goes here...

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
    /*
    PLAYER_BLACK = 0     PIECE_BLACK = 1
    PLAYER_WHITE = 1     PIECE_WHITE = 2
    --> color de la pieza = color del jugador + 1

    Si el color d la pieza no es vacio ni el actual, entonces es el color del contrincante.
    */
    validMoves.erase(validMoves.begin(), validMoves.end());
    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            Square move = {x, y};

            /*
            Se analiza la "siguiente pieza" hasta encontrar una ficha del color del jugador o vacío o irse del tablero.
            Donde esta esa "siguiente pieza" depende del caso del switch.
            */
            if (getBoardPiece(model, {y,x}) == getCurrentPlayer(model) + 1) {
                for (int lookAround = UPPER_LEFT; lookAround <= LOWER_RIGHT; lookAround++) {
                    int i, analizedPiece;

                    switch (lookAround) {
                    case UPPER_LEFT:
                        for (i=1; (analizedPiece=getBoardPiece(model, {x-i, y-i})) != PIECE_EMPTY && 
                                   analizedPiece != getCurrentPlayer(model) + 1 &&
                                    isSquareValid({x-i, y-i});
                             i++);
                        
                        if (isSquareValid({ x - i, y - i }) && analizedPiece == PIECE_EMPTY && i > 1)
                            validMoves.push_back({ x - i, y - i });
                        break;
                    
                    case UPPER_MIDDLE:
                        for (i = 1; (analizedPiece = getBoardPiece(model, {x-i, y})) != PIECE_EMPTY &&
                                     analizedPiece != getCurrentPlayer(model) + 1 &&
                                     isSquareValid({ x - i, y });
                            i++);

                        if (isSquareValid({x-i, y}) && analizedPiece == PIECE_EMPTY && i > 1)
                            validMoves.push_back({ x - i, y });
                        break;

                    case UPPER_RIGHT:
                        for (i = 1; (analizedPiece = getBoardPiece(model, { x - i, y+i })) != PIECE_EMPTY &&
                                     analizedPiece != getCurrentPlayer(model) + 1 &&
                                     isSquareValid({ x - i, y + i });
                            i++);

                        if (isSquareValid({ x - i, y+i }) && analizedPiece == PIECE_EMPTY && i > 1)
                            validMoves.push_back({ x - i, y + i });
                        break;
                    case MIDDLE_LEFT:
                        for (i = 1; (analizedPiece = getBoardPiece(model, { x, y-i })) != PIECE_EMPTY &&
                                     analizedPiece != getCurrentPlayer(model) + 1 &&
                                     isSquareValid({ x, y - i });
                            i++);

                        if (isSquareValid({ x,y-i }) && analizedPiece == PIECE_EMPTY && i > 1)
                            validMoves.push_back({ x,y - i });
                        break;
                    case MIDDLE_RIGHT:
                        for (i = 1; (analizedPiece = getBoardPiece(model, { x, y+i })) != PIECE_EMPTY &&
                                     analizedPiece != getCurrentPlayer(model) + 1 &&
                                     isSquareValid({ x, y + i });
                            i++);

                        if (isSquareValid({ x , y+i }) && analizedPiece == PIECE_EMPTY && i > 1)
                            validMoves.push_back({ x , y+i });
                        break;
                    case LOWER_LEFT:
                        for (i = 1; (analizedPiece = getBoardPiece(model, { x + i, y - i })) != PIECE_EMPTY &&
                                     analizedPiece != getCurrentPlayer(model) + 1 &&
                                     isSquareValid({ x + i, y - i });
                            i++);

                        if (isSquareValid({ x + i, y-i }) && analizedPiece == PIECE_EMPTY && i > 1)
                            validMoves.push_back({ x + i, y - i });
                        break;
                    case LOWER_MIDDLE:
                        for (i = 1; (analizedPiece = getBoardPiece(model, { x + i, y })) != PIECE_EMPTY &&
                                     analizedPiece != getCurrentPlayer(model) + 1 &&
                                     isSquareValid({ x + i, y });
                            i++);

                        if (isSquareValid({ x + i, y }) && analizedPiece == PIECE_EMPTY && i > 1)
                            validMoves.push_back({ x + i, y });
                        break;
                    case LOWER_RIGHT:
                        for (i = 1; (analizedPiece = getBoardPiece(model, { x + i, y+i })) != PIECE_EMPTY &&
                                     analizedPiece != getCurrentPlayer(model) + 1 &&
                                     isSquareValid({ x + i, y+i });
                            i++);

                        if (isSquareValid({ x + i, y+i }) && analizedPiece == PIECE_EMPTY && i > 1)
                            validMoves.push_back({ x + i, y + i });
                        break;
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
