/**
 * @brief Implements the Reversi game model
 * @author Marc S. Ressl
 * @author Juan Ignacio Caorsi
 * @author Rita Moschini
 *
 * @copyright Copyright (c) 2023-2024
 */

#include "raylib.h"
#include <stdio.h>
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
void initModel(GameModel& model)
{
	model.gameOver = true;

	model.playerTime[0] = 0;
	model.playerTime[1] = 0;

	memset(model.board, PIECE_EMPTY, sizeof(model.board));
}

void startModel(GameModel& model)
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

Player getCurrentPlayer(GameModel& model)
{
	return model.currentPlayer;
}

int getScore(GameModel& model, Player player)
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

double getTimer(GameModel& model, Player player)
{
	double turnTime = 0;

	if (!model.gameOver && (player == model.currentPlayer))
		turnTime = GetTime() - model.turnTimer;

	return model.playerTime[player] + turnTime;
}

char getBoardPiece(GameModel& model, Square square)
{
	return model.board[square.y][square.x];
}

void setBoardPiece(GameModel& model, Square square, char piece)
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

/**
 * @brief Puts valid moves for the current player in validMoves vector.
 *
 * @param model Model
 * @param validMoves Vector of valid moves
 */
void getValidMoves(GameModel& model, Moves& validMoves)
{
	Piece playerPiece =
		(getCurrentPlayer(model) == PLAYER_WHITE)
		? PIECE_WHITE
		: PIECE_BLACK;
	Piece opponentPiece = (playerPiece == PIECE_WHITE)
		? PIECE_BLACK
		: PIECE_WHITE;

	for (int y = 0; y < BOARD_SIZE; y++) {
		for (int x = 0; x < BOARD_SIZE; x++) {
			Square move = { x, y };

			if (getBoardPiece(model, move) == PIECE_EMPTY) {

				for (int s = UPPER_LEFT; s <= LOWER_RIGHT; s++) {

					int i = 1;
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
					} while (isSquareValid(analizedSquare) &&
						(analizedPiece = getBoardPiece(model, analizedSquare)) == opponentPiece);

					if (analizedPiece == playerPiece && i > 2) {
						validMoves.push_back(move);
						break;
					}
				}
			}
		}
	}
}

/**
 * @brief Plays move in model's board.
 *
 * @param model Model
 * @param move Move
 * @return char Number of turned pieces
 */
char playMove(GameModel& model, Square move)
{
	char currentGain = 0;
	char playerPiece =
		(getCurrentPlayer(model) == PLAYER_WHITE)
		? PIECE_WHITE
		: PIECE_BLACK;

	setBoardPiece(model, move, playerPiece);

	char opponentPiece = (playerPiece == PIECE_WHITE)
		? PIECE_BLACK
		: PIECE_WHITE;

	for (int lookAround = UPPER_LEFT; lookAround <= LOWER_RIGHT; lookAround++) {
		Square analizedSquare;
		char analizedPiece = PIECE_EMPTY;

		for (int s = UPPER_LEFT; s <= LOWER_RIGHT; s++) {
			int i = 1;

			do {
				switch (s) {
				case UPPER_LEFT:
					analizedSquare = { move.x - i, move.y - i };
					break;

				case UPPER_MIDDLE:
					analizedSquare = { move.x, move.y - i };
					break;

				case UPPER_RIGHT:
					analizedSquare = { move.x + i, move.y - i };
					break;

				case MIDDLE_LEFT:
					analizedSquare = { move.x - i, move.y };
					break;
				case MIDDLE_RIGHT:
					analizedSquare = { move.x + i, move.y };
					break;

				case LOWER_LEFT:
					analizedSquare = { move.x - i, move.y + i };
					break;

				case LOWER_MIDDLE:
					analizedSquare = { move.x, move.y + i };
					break;
				case LOWER_RIGHT:
					analizedSquare = { move.x + i, move.y + i };
					break;
				}
				i++;
			} while (isSquareValid(analizedSquare) &&
				(analizedPiece = getBoardPiece(model, analizedSquare)) == opponentPiece);

			if (analizedPiece == playerPiece && i > 2) {
				for (int t = 1; t != i; ++t) {
					switch (s) {
					case UPPER_LEFT:
						setBoardPiece(model, { move.x - t, move.y - t }, playerPiece);
						break;

					case UPPER_MIDDLE:
						setBoardPiece(model, { move.x , move.y - t }, playerPiece);
						break;

					case UPPER_RIGHT:
						setBoardPiece(model, { move.x + t, move.y - t }, playerPiece);
						break;

					case MIDDLE_LEFT:
						setBoardPiece(model, { move.x - t, move.y }, playerPiece);
						break;
					case MIDDLE_RIGHT:
						setBoardPiece(model, { move.x + t, move.y }, playerPiece);
						break;

					case LOWER_LEFT:
						setBoardPiece(model, { move.x - t, move.y + t }, playerPiece);
						break;

					case LOWER_MIDDLE:
						setBoardPiece(model, { move.x, move.y + t }, playerPiece);
						break;
					case LOWER_RIGHT:
						setBoardPiece(model, { move.x + t, move.y + t }, playerPiece);
						break;
					}
					currentGain++;

				}
			}


		}

		model.currentPlayer =
			(model.currentPlayer == PLAYER_WHITE)
			? PLAYER_BLACK
			: PLAYER_WHITE;
		Moves validMoves;
		getValidMoves(model, validMoves);

		if (validMoves.size() == 0) {
			model.currentPlayer =
				(model.currentPlayer == PLAYER_WHITE)
				? PLAYER_BLACK
				: PLAYER_WHITE;

			Moves validMoves;
			getValidMoves(model, validMoves);

			if (validMoves.size() == 0)
				model.gameOver = true;
		}
		return currentGain;
	}
}