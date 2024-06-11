#include "GameController.h"
#include "Block.h"
#include "algorithm"





bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.


    std::ifstream inputFile(commands_file);

    Block* tempBlock = game.initial_block;

    if (inputFile.is_open()) {
        std::string line;
        blockInitializer(game, tempBlock);
        while (std::getline(inputFile, line)) {
            if (line == "PRINT_GRID") {
                printGrid(game);                
            }
            else if (line == "MOVE_RIGHT") {
                moveRight(game, game.active_rotation);
            }
            else if (line == "MOVE_LEFT") {
                moveLeft(game, game.active_rotation);
            }
            else if (line == "ROTATE_RIGHT") {
                rotateRight(game, game.active_rotation);
            }
            else if (line == "ROTATE_LEFT") {
                for (const auto& position : game.active_rotation->position) {
                    int row = position.first;
                    int col = position.second;

                }
                rotateLeft(game, game.active_rotation);
            }
            else if (line == "DROP") {
                count = 0;
                drop(game, game.active_rotation);
                game.current_score += count * game.active_rotation->position.size();
                popRow(game);
                tempBlock = tempBlock->next_block;
                blockInitializer(game, tempBlock);

            }
            else if (line == "") {
                // Do nothing
            }
            else {
                std::cout << "Unknown command: " << line << std::endl;
            }
        }


    }

    inputFile.close();

    return false;

}

void GameController::printGrid(const BlockFall& game) {

    std::cout << "Score: " << game.current_score << std::endl;
    
    for (int i = 0; i < game.rows; ++i) {
        for (int j = 0; j < game.cols; ++j) {
            if (game.grid[i][j] == 1) {
                std::cout << occupiedCellChar;

            }
            else
            {
                std::cout << unoccupiedCellChar;
            }
        }
        std::cout << std::endl;
    }
}

void GameController::blockInitializer(BlockFall& game, Block* block) {

    block->position.clear();
    game.active_rotation = block;
    for (int i = 0; i < game.active_rotation->shape.size(); i++) {
        for (int j = 0; j < game.active_rotation->shape[i].size(); j++) {

            if (block->shape[i][j] == true) {
                game.grid[i][j] = 1;
                block->position.push_back(std::make_pair(i, j));
            }
            else
            {

                game.grid[i][j] = 0;



            }


        }

    }
}

void GameController::moveRight(BlockFall& game, Block* block) {

    std::vector<std::pair<int, int>> newPosition;

    for (const auto& pair : block->position) {
        int newRow = pair.first;
        int newCol = pair.second + 1;

        newPosition.push_back(std::make_pair(newRow, newCol));
    }


    if (!collisionDetected(game, block, 0, 1)) {

        deleteBlock(game, block);

        block->position = newPosition;

        // Draw the block in its new position on the grid
        for (const auto& pair : block->position) {
            game.grid[pair.first][pair.second] = 1;
            
        }

    }
}

void GameController::moveLeft(BlockFall& game, Block* block) {

    std::vector<std::pair<int, int>> newPosition;

    for (const auto& pair : block->position) {
        int newRow = pair.first;
        int newCol = pair.second - 1;

        newPosition.push_back(std::make_pair(newRow, newCol));
    }


    if (!collisionDetected(game, block, 0, -1)) {

        deleteBlock(game, block);

        block->position = newPosition;

        // Draw the block in its new position on the grid
        for (const auto& pair : block->position) {
            game.grid[pair.first][pair.second] = 1;
        }

    }
}

void GameController::rotateRight(BlockFall& game, Block* block) {

    std::vector<std::pair<int, int>> newPosition;

    std::pair<int, int> initialPoint = block->position[0];

    std::vector<std::vector<int>> originalGrid = game.grid;


    deleteBlock(game, block);

    game.active_rotation = block->right_rotation;
    block = block->right_rotation;

    bool includesZeroZero = std::find(block->position.begin(), block->position.end(), std::make_pair(0, 0)) != block->position.end();

    
    for (int i = 0; i < block->shape.size(); i++) {
        for (int j = 0; j < block->shape[i].size(); j++) {
            if (block->shape[i][j] == true) {
                newPosition.push_back(std::make_pair(initialPoint.first + i, initialPoint.second + j));
            }
        }
    }


    if (rotateCollision(game, block, newPosition)) {
        // Collision detected, rollback the grid to its original state
        game.grid = originalGrid;
    }
    else {
        // No collision, update the block's position
        for (const auto& position : newPosition) {

            game.grid[position.first][position.second] = 1;
        }

        game.active_rotation->position = newPosition;
    }


}

void GameController::rotateLeft(BlockFall& game, Block* block) {


    std::vector<std::pair<int, int>> newPosition;

    std::pair<int, int> initialPoint = block->position[0];

    std::vector<std::vector<int>> originalGrid = game.grid;


    deleteBlock(game, block);

    game.active_rotation = block->left_rotation;
    block = block->left_rotation;

    for (int i = 0; i < block->shape.size(); i++) {
        for (int j = 0; j < block->shape[i].size(); j++) {
            if (block->shape[i][j] == true) {
                newPosition.push_back(std::make_pair(initialPoint.first + i, initialPoint.second + j));
            }
        }
    }


    if (rotateCollision(game, block, newPosition)) {
        // Collision detected, rollback the grid to its original state
        game.grid = originalGrid;
    }
    else {
        // No collision, update the block's position
        for (const auto& position : newPosition) {

            game.grid[position.first][position.second] = 1;
        }

        game.active_rotation->position = newPosition;
    }


}

bool GameController::collisionDetected(const BlockFall& game, const Block* block, int X, int Y) {

    

    for (const auto& pair : block->position) {

        int newRow = pair.first + X;
        int newCol = pair.second + Y;

        // Check if the new position is out of bounds or collides with another block
        if (newRow < 0 || newRow >= game.rows || newCol < 0 || newCol >= game.cols || game.grid[newRow][newCol] == 1) {
            if (!(std::find(block->position.begin(), block->position.end(), std::make_pair(pair.first + X, pair.second + Y)) != block->position.end())) {
                return true;
            }
        }
    }

    return false;  // No collision
}

void GameController::deleteBlock(BlockFall& game, Block* block) {

    for (const auto& pair : block->position) {
        game.grid[pair.first][pair.second] = 0;
    }

}

bool GameController::rotateCollision(BlockFall& game, Block* block, std::vector<std::pair<int, int>>& newPosition)
{


    for (const auto& position : newPosition) {
        int row = position.first;
        int col = position.second;


        // Check for collision with existing blocks
        if (game.grid[row][col] == 1) {
            return true; // Collision detected
        }

        // Check if the new position is within the game boundaries
        if (row < 0 || row >= game.grid.size() || col < 0 || col >= game.grid[row].size()) {
            return true; // Out of bounds
        }
    }

    return false;
}


void GameController::drop(BlockFall& game, Block* block) {

    std::vector<std::pair<int, int>> newPosition;

    for (const auto& pair : block->position) {
        int newRow = pair.first + 1;
        int newCol = pair.second;

        newPosition.push_back(std::make_pair(newRow, newCol));
    }


    if (!collisionDetected(game, block, 1, 0)) {

        deleteBlock(game, block);


        if (game.gravity_mode_on) {

            block->position = newPosition;

            for (const auto& pair : block->position) {
                game.grid[pair.first][pair.second] = 1;
            }
            count++;
            drop(game, block);
        }
        else
        {
            for (auto& position : newPosition) {
                while (position.first + 1 < game.grid.size() && game.grid[position.first + 1][position.second] == 0) {
                    position.first += 1;
                }
            }

            block->position = newPosition;

            for (const auto& pair : block->position) {
                game.grid[pair.first][pair.second] = 1;
            }

            count++;
            drop(game, block);
        }
        


    }


}

void GameController::popRow(BlockFall& game) {
    for (int i = 0; i < game.grid.size(); i++) {
        bool isFullRow = true;  // Assume the row is full of 1s

        for (int j = 0; j < game.grid[i].size(); j++) {
            if (game.grid[i][j] != 1) {
                isFullRow = false;
                break;  // Break the loop if a cell is not 1
            }
        }

        if (isFullRow) {
            // If the row is full of 1s, set all elements in that row to 0
            for (int j = 0; j < game.grid[i].size(); j++) {
                game.grid[i][j] = 0;
            }
        }
    }
}
