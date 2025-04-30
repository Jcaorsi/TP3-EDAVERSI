/**
 * @brief Implements the Reversi game AI
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2023-2024
 */

#include <cstdlib>

#include "ai.h"
#include "controller.h"
#include <stack>
#include <algorithm>

#define COMPARE(a, b) ((a) >= (b) ? 1 : 0)

#define MAX_LEVELS 4
#define MINUS_INFINITY -100
#define PLUS_INFINITY 100

class TreeNode {
public:

	GameModel model;
	char aiGain;
	char level;                  //A revision
	std::vector<Square> validMoves;
    char alpha;
    char beta;
    TreeNode* primerHijo;
    TreeNode* hermanoSiguiente; 
	bool inProcess;
    char currentGain;
    // Constructor
    TreeNode(GameModel model, char valAlpha, char valBeta, char level) : model(model), alpha(valAlpha), beta(valBeta), primerHijo(nullptr),
        hermanoSiguiente(nullptr), level(level), aiGain(MINUS_INFINITY) {}

    void agregarHijo(TreeNode* hijo) {
        if (primerHijo == nullptr) {
            primerHijo = hijo;
        }
        else {
            TreeNode* temp = primerHijo;
            while (temp->hermanoSiguiente != nullptr) {
                temp = temp->hermanoSiguiente;
            }
            temp->hermanoSiguiente = hijo;
        }
    }
};

Square getBestMove(GameModel& model)
{
    // To-do: your code goes here...

    std::stack<TreeNode*> nodeStack;

    std::vector<Square> validMoves;
    TreeNode rootNode(model, MINUS_INFINITY, PLUS_INFINITY, 1);
    getValidMoves(model, rootNode.validMoves);

    //rootNode.validMoves = validMoves;

    nodeStack.push(&rootNode);
    //		for (auto move : nodeStack.top()->validMoves)
    Square bestMove = {- 1, -1};
	Square primordialMove = { -1, -1 };

	Square move = { -1, -1 };

	bool returning = false;
    while (!nodeStack.empty())
    {
        if (!nodeStack.top()->validMoves.empty()) 
        {
            move = nodeStack.top()->validMoves.back();
            primordialMove = nodeStack.top()->level == 1 ? move : primordialMove;
        }
        else 
        {
			returning = true;
        }

		nodeStack.top()->inProcess = true;

		if (nodeStack.top()->level != MAX_LEVELS && !returning)
        {

			nodeStack.top()->agregarHijo(new TreeNode(nodeStack.top()->model, nodeStack.top()->alpha, 
                nodeStack.top()->beta, nodeStack.top()->level + 1));

			nodeStack.top()->validMoves.pop_back();


			nodeStack.push(nodeStack.top()->primerHijo);

			nodeStack.top()->currentGain = playMove(nodeStack.top()->model, move);

			if (!nodeStack.top()->inProcess)
			{
				nodeStack.top()->validMoves.clear();
				getValidMoves(nodeStack.top()->model, nodeStack.top()->validMoves);     //Creo hijo, sin lo agrego a stack y no 
			}
        }                                                                           //Quito lo que es la valid move que lo creó
        else if (nodeStack.top()->level == MAX_LEVELS || returning)
        {
			TreeNode* auxNode = nodeStack.top();
			nodeStack.pop();
			auxNode->aiGain = auxNode->currentGain;
            if (auxNode->level % 2) 
			{                        //Caso impar. Nodo final es MAX y el padre es MIN
                if (COMPARE(nodeStack.top()->aiGain, auxNode->aiGain))
                {
                    nodeStack.top()->aiGain = auxNode->aiGain;
					nodeStack.top()->beta = auxNode->aiGain;
                }
                
                if (COMPARE(nodeStack.top()->alpha, nodeStack.top()->beta))
                {
                    nodeStack.top()->level = nodeStack.top()->level > 1 ? nodeStack.top()->level = MAX_LEVELS : 1;
                }
            }
            else
            {                       //Caso par. Nodo final es MIN y el padre es MAX
                if (COMPARE(auxNode->aiGain, nodeStack.top()->aiGain))
                {
                    nodeStack.top()->aiGain = auxNode->aiGain;
                    nodeStack.top()->alpha = auxNode->aiGain;

					if (nodeStack.top()->level == 1)
					{
                        bestMove = primordialMove;
					}

                    if (COMPARE(nodeStack.top()->beta, nodeStack.top()->alpha))
                    {
                        nodeStack.top()->level = nodeStack.top()->level > 1 ? MAX_LEVELS : 1;
                    }
                }
            }

            delete auxNode;

        }
                                                                                    //Cada z debe ser creado con un stack con solo el elemento X


        /*for ()
		playMove(newNode->model, move);
		int score = getScore(newModel, newModel.currentPlayer);
		if (score > nodeStack.top()->aiGain)
		{
			nodeStack.top()->aiGain = score;
			nodeStack.top()->model = newModel;
		}*/
		
    }


    
	return bestMove;
    // +++ TEST
    // Returns a random valid move...
/*    Moves validMoves;*/
   // getValidMoves(model, validMoves);
    //int index = rand() % validMoves.size();
   // return validMoves[index];
    // --- TEST
}

/*int chooseBestMove(GameModel& model) {
    if (model.gameOver || level == MAX_LEVELS) {
        level--;
        return 0;
    }
    TreeNode* node = new TreeNode;
    node->model = 
    
}*/