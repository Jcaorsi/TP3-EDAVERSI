/**
 * @brief Implements the Reversi game controller
 * @author Marc S. Ressl
 * @author Juan Ignacio Caorsi
 * @author Rita Moschini
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <algorithm>
#include "raylib.h"

#include "ai.h"
#include "view.h"
#include "controller.h"

static void updateTimer(GameModel& model)
{
    double currentTime = GetTime();
    model.playerTime[model.currentPlayer] += currentTime - model.turnTimer;
    model.turnTimer = currentTime;
}

bool updateView(GameModel &model)
{
    if (WindowShouldClose())
        return false;

    if (model.gameOver)
    {
        if (IsMouseButtonPressed(0))
        {
            if (isMousePointerOverPlayBlackButton())
            {
                model.humanPlayer = PLAYER_BLACK;

                startModel(model);
            }
            else if (isMousePointerOverPlayWhiteButton())
            {
                model.humanPlayer = PLAYER_WHITE;

                startModel(model);
            }
        }
    }
    else if (model.currentPlayer == model.humanPlayer)
    {
        if (IsMouseButtonPressed(0))
        {
            Square square = getSquareOnMousePointer();

            if (isSquareValid(square))
            {
                Moves validMoves;
                getValidMoves(model, validMoves);

                for (auto move : validMoves)
                {
                    if ((square.x == move.x) &&
                        (square.y == move.y)) {
                        playMove(model, square);
						updateTimer(model);
                    }
                }
            }
        }
    }
    else
    {
        Square square = getBestMove(model);

        playMove(model, square);
		updateTimer(model);

    }

    if ((IsKeyDown(KEY_LEFT_ALT) ||
         IsKeyDown(KEY_RIGHT_ALT)) &&
        IsKeyPressed(KEY_ENTER))
        ToggleFullscreen();

    drawView(model);

    return true;
}
