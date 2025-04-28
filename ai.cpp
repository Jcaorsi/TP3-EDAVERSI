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

#define COMPARE(a, b) ((a) >= (b) ? 1 : 0)
#define MAX_LEVELS 4
#define MINUS_INFINITY -100
#define PLUS_INFINITY 100

class Node {
public:

	GameModel model;
	int aiGain;
	int level;                  //A revision
	std::vector<Square> validMoves;
    int alpha;
    int beta;
};

Square getBestMove(GameModel &model)
{
    // To-do: your code goes here...
    
    std::stack<Node*> nodeStack;

    std::vector<Square> validMoves;
    Node rootNode;
    rootNode.model = model;
    rootNode.aiGain = -100;
    rootNode.level = 1;
	getValidMoves(model, rootNode.validMoves);
	rootNode.alpha = MINUS_INFINITY;
	rootNode.beta = PLUS_INFINITY;
    
	nodeStack.push(&rootNode);

    while()




    // +++ TEST
    // Returns a random valid move...
    Moves validMoves;
    getValidMoves(model, validMoves);
    int index = rand() % validMoves.size();
    return validMoves[index];
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