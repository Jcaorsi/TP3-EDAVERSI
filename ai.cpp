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

#define MAX_LEVELS 4
#define MINUS_INFINITY -100
#define PLUS_INFINITY 100

class TreeNode {
public:
	GameModel model;
	char aiGain;
	char level;
    std::vector<Square> validMoves;
    char alpha;
    char beta;
    TreeNode* primerHijo;
    TreeNode* hermanoSiguiente; 
    bool inProcess;
    /* un nodo se considera inProcess cuando su vector de validMoves esta siendo estudiado es decir cuando se estan
       recorriendo sus nodos hijos */
    char currentGain;
    // Constructor
    TreeNode(GameModel model, char valAlpha, char valBeta, TreeNode* primerHijo, TreeNode* hermanoSiguiente, char level, char aiGain, char currentGain)
            : model(model), alpha(valAlpha), beta(valBeta), primerHijo(primerHijo), hermanoSiguiente(hermanoSiguiente), level(level),
              aiGain(aiGain), currentGain(currentGain) {} 

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
    std::stack<TreeNode*> nodeStack;

    std::vector<Square> validMoves;
    TreeNode rootNode(model, MINUS_INFINITY, PLUS_INFINITY, nullptr, nullptr, 1, 0, 0);
    // model, alpha, beta, primerHijo, hermanoSiguiente, level, aiGain, inProcess, currentGain

    rootNode.validMoves.clear();
    getValidMoves(model, rootNode.validMoves);

    nodeStack.push(&rootNode);
    Square bestMove = {- 1, -1};
	Square primordialMove = { -1, -1 }; //primer valid move de la raiz

	Square move = { -1, -1 }; //VALORES INVALIDOS

	bool returning = false;
    while (!nodeStack.empty())
    {
        if (!nodeStack.top()->validMoves.empty()) 
        {
            move = nodeStack.top()->validMoves.back();
            /* back porque sacar elementos de la back de un vector es O(1)
            move = el ultimo validMove del ultimo nodo pusheado
            */
            primordialMove = nodeStack.top()->level == 1 ? move : primordialMove;
            // si el ultimo nodo pusheado es el raiz, entonces primordialMove es el ultimo validMove del nodo raiz
        }
        else 
        {
			returning = true;
        }

		nodeStack.top()->inProcess = true;

		if (nodeStack.top()->level != MAX_LEVELS && !returning)
        {
			nodeStack.top()->agregarHijo(new TreeNode(nodeStack.top()->model, nodeStack.top()->alpha, nodeStack.top()->beta, 
                                        nullptr, nullptr, nodeStack.top()->level + 1, 0, 0) );
            

            if (!nodeStack.top()->validMoves.empty())
    			nodeStack.top()->validMoves.pop_back();
            // ya puse el ulimo validMove del ultimo nodo pusheado en "move", ahora lo borro del vector xq ya no lo necesito

			nodeStack.push(nodeStack.top()->primerHijo);

            nodeStack.top()->currentGain = playMove(nodeStack.top()->model, move) + 1;
            //Mas uno porque cuento las volteadas Y la colocada

			if (!nodeStack.top()->inProcess)
			{
				nodeStack.top()->validMoves.clear();
				getValidMoves(nodeStack.top()->model, nodeStack.top()->validMoves);
			}
        }
        else if (nodeStack.top()->level == MAX_LEVELS || returning)
        {
			TreeNode* auxNode = nodeStack.top(); //auxNode tiene el hijo
			nodeStack.pop();
            if (auxNode->level % 2) 
			{                        //Caso impar. Nodo final (auxNode) es MAX y el padre (nodeStack.top) es MIN
                // si el del padre es menor q el del hijo, se lo asigna
                auxNode->aiGain = -1 * auxNode->currentGain;
                if (nodeStack.top()->aiGain > auxNode->aiGain)
                {
                    nodeStack.top()->aiGain = auxNode->aiGain;
					nodeStack.top()->beta = auxNode->aiGain;
                }
                
                if (nodeStack.top()->alpha >= nodeStack.top()->beta)
                {
                    nodeStack.top()->level = nodeStack.top()->level > 1 ? nodeStack.top()->level = MAX_LEVELS : 1;
                }
            }
            else
            {                       //Caso par. Nodo final (auxNode) es MIN y el padre (nodeStack.top) es MAX
                auxNode->aiGain = auxNode->currentGain;
                if (nodeStack.top()->aiGain < auxNode->aiGain)
                {
                    // si aiGain del hijo es mayor q el del padre, entonces se guarda el aiGain del hijo en del padre
                    nodeStack.top()->aiGain = auxNode->aiGain;
                    nodeStack.top()->alpha = auxNode->aiGain;

					if (nodeStack.top()->level == 1)
					{
                        bestMove = primordialMove;
					}

                    if (nodeStack.top()->beta >= nodeStack.top()->alpha)
                    {
                        nodeStack.top()->level = nodeStack.top()->level > 1 ? MAX_LEVELS : 1;
                    }
                }
            }
            delete auxNode;
        }
         
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