/**
 * @brief Implements the Reversi game AI
 * @author Marc S. Ressl
 * @author Juan Ignacio Caorsi
 * @author Rita Moschini
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <cstdlib>

#include "ai.h"
#include "controller.h"
#include <stack>
#include <algorithm>

#define MINUS_INFINITY -100
#define PLUS_INFINITY 100

#define MAX_LEVELS 4
#define MAX_NODES 10000

class TreeNode {
public:
	GameModel model;
	std::vector<Square> validMoves;
	char currentGain;
	int aiGain;
	int alpha;
	int beta;
	TreeNode* primerHijo;
	int level;
	bool inProcess;

	TreeNode(GameModel model, char valAlpha, char valBeta, TreeNode* primerHijo, char level,
		char aiGain, char currentGain, bool inProcess)
		: model(model), alpha(valAlpha), beta(valBeta), primerHijo(primerHijo), level(level),
		aiGain(aiGain), currentGain(currentGain), inProcess(inProcess) {
	}

	void agregarHijo(TreeNode* hijo) {
		if (primerHijo == nullptr)
			primerHijo = hijo;
		else
			perror("Error al agregar nodo hijo");
	}
};

/**
 * @brief Gets best move for current player.
 *
 * @param model Model.
 * @return Square Best move.
 */
Square getBestMove(GameModel& model)
{
	std::stack<TreeNode*> nodeStack;

	bool returning = false;
	int numberOfNodes = 1;

	TreeNode rootNode(model, MINUS_INFINITY, PLUS_INFINITY, nullptr, 1, 0, 0, false);

	rootNode.validMoves.clear();
	getValidMoves(model, rootNode.validMoves);

	nodeStack.push(&rootNode);
	Square bestMove = { -1, -1 };
	Square primordialMove = { -1, -1 };
	Square move = { -1, -1 };

	while (!nodeStack.empty())
	{
		nodeStack.top()->inProcess = true;

		if (nodeStack.top()->validMoves.empty())
			returning = true;

		if (numberOfNodes == MAX_NODES)
			return primordialMove;

		if (nodeStack.top()->level != MAX_LEVELS && !returning)
		{
			move = nodeStack.top()->validMoves.back();
			nodeStack.top()->validMoves.pop_back();

			if (nodeStack.top()->level == 1)
				primordialMove = move;

			nodeStack.top()->agregarHijo(new TreeNode(nodeStack.top()->model,
				nodeStack.top()->alpha, nodeStack.top()->beta, nullptr, nodeStack.top()->level + 1,
				0, 0, false));

			nodeStack.push(nodeStack.top()->primerHijo);
			numberOfNodes++;

			nodeStack.top()->currentGain = playMove(nodeStack.top()->model, move) + 1;

			if (!nodeStack.top()->inProcess && nodeStack.top()->level != MAX_LEVELS) {
				nodeStack.top()->validMoves.clear();
				getValidMoves(nodeStack.top()->model, nodeStack.top()->validMoves);
			}
		}
		else if (nodeStack.top()->level == MAX_LEVELS || returning) {

			returning = false;

			if (nodeStack.top()->level != 1) {
				TreeNode* auxNode = nodeStack.top();
				nodeStack.pop();
				nodeStack.top()->primerHijo = NULL;

				if (auxNode->level % 2) {

					auxNode->aiGain = -1 * auxNode->currentGain;

					if (nodeStack.top()->aiGain > auxNode->aiGain) {

						nodeStack.top()->aiGain = auxNode->aiGain;
						nodeStack.top()->beta = auxNode->aiGain;

						if (nodeStack.top()->alpha >= nodeStack.top()->beta)
							returning = true;
					}
				}

				else {

					auxNode->aiGain = auxNode->currentGain;

					if (nodeStack.top()->aiGain < auxNode->aiGain) {

						nodeStack.top()->aiGain = auxNode->aiGain;
						nodeStack.top()->alpha = auxNode->aiGain;

						if (nodeStack.top()->level == 1)
							bestMove = primordialMove;

						if (nodeStack.top()->alpha >= nodeStack.top()->beta)
							returning = true;

					}
				}

				delete auxNode;
			}
			else
				nodeStack.pop();
		}
	}

	return bestMove;
}