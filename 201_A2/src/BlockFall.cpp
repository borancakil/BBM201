#include "BlockFall.h"
#include "iostream"

// TODO BİR YERDE LEAK VAR VECTOR LE İLGİLİ
BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string& leaderboard_file_name, const string& player_name) : gravity_mode_on(
    gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string& input_file) {

    std::ifstream inputFile(input_file);

    if (inputFile.is_open()) {

        int count = 0;


        std::string line;
        while (std::getline(inputFile, line)) {

            if (line[0] == '[') {
                count++;
            }

        }

        // Reset the file pointer to the beginning
        inputFile.clear();              // Clear any flags set due to reaching the end of the file
        inputFile.seekg(0, std::ios::beg); // Move the file pointer to the beginning


        char currentChar;
        Block* currentBlock = nullptr;
        int secondCount = 0;

        while (inputFile.get(currentChar)) {

            std::vector<bool> grids;

            if (currentChar == '[' && secondCount + 1 < count) {
                secondCount++;

                if (!currentBlock) {

                    // Create a new block if currentBlock is nullptr (first block)
                    currentBlock = new Block;
                    initial_block = currentBlock;  // Set head to the first block

                }
                else {

                    // Create a new block and link it to the previous one
                    currentBlock->next_block = new Block;
                    currentBlock = currentBlock->next_block;

                }


                while (inputFile.get(currentChar) && currentChar != ']') {

                    // Stop adding to grids when newline character is encountered
                    if (currentChar == '\n') {

                        currentBlock->shape.push_back(grids);
                        grids.clear();


                    }
                    else {

                        grids.push_back(charToBool(currentChar));

                    }
                }

                currentBlock->shape.push_back(grids);

                rotateRight(currentBlock, currentBlock);
            }
            else if (currentChar == '[' && secondCount + 1 >= count)
            {
                while (inputFile.get(currentChar) && currentChar != ']') {

                    // Stop adding to grids when newline character is encountered
                    if (currentChar == '\n') {

                        power_up.push_back(grids);
                        grids.clear();

                    }
                    else {
                        grids.push_back(charToBool(currentChar));

                    }
                }

                power_up.push_back(grids);

                
            }
        }
    }
}

void BlockFall::initialize_grid(const string& input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main

    std::ifstream inputFile(input_file);

    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {

            stringstream ss(line);

            vector<int> row;

            int value;

            while (ss >> value) {
                row.push_back(value);
            }

            grid.push_back(row);
        }


        inputFile.close();

    }

    rows = grid.size();
    cols = grid[0].size();

}

bool BlockFall::charToBool(char c) {

    return (c == '1');
}

void BlockFall::rotateRight(Block* origBlock, Block* block) {
    if (block == nullptr) {
        // Handle the case where block is nullptr
        return;
    }

    int rows = block->shape.size();
    int cols = block->shape[0].size();


    if (rows == 0 || cols == 0) {
        // Handle the case where shape is empty
        return;
    }

    // Create a temporary matrix to store the rotated values
    std::vector<std::vector<bool>> temp(cols, std::vector<bool>(rows));

    std::vector<std::vector<bool>> transposedMatrix(block->shape[0].size(), std::vector<bool>(block->shape.size(), 0));
    
    for (size_t i = 0; i < block->shape.size(); ++i) {
        for (size_t j = 0; j < block->shape[0].size(); ++j) {
            transposedMatrix[j][i] = block->shape[i][j];
        }
    }

    // Reverse each row
    std::vector<std::vector<bool>> rotatedMatrix(transposedMatrix.size(), std::vector<bool>(transposedMatrix[0].size(), 0));

    for (size_t i = 0; i < transposedMatrix.size(); ++i) {
        for (size_t j = 0; j < transposedMatrix[0].size(); ++j) {
            rotatedMatrix[i][j] = transposedMatrix[i][transposedMatrix[0].size() - 1 - j];
        }
    }

    
    // Create a new block with the rotated shape
    Block* rightBlock = new Block;
    rightBlock->shape = rotatedMatrix;
    rightBlock->left_rotation = nullptr;
    rightBlock->right_rotation = nullptr;
    rightBlock->next_block = nullptr; 


   if (rightBlock != nullptr && origBlock != nullptr && *rightBlock == *origBlock) {
        if (block != nullptr) {
            block->right_rotation = origBlock;
            origBlock->left_rotation = block;
        }
        delete rightBlock;
    }
    else {
        if (rightBlock != nullptr && block != nullptr) {
            rightBlock->left_rotation = block;
            block->right_rotation = rightBlock;
            rightBlock->next_block = block->next_block;
            rotateRight(origBlock, rightBlock);
        }
    } 


}

void BlockFall::rotateLeft(Block* origBlock, Block* block) {



    int rows = block->shape.size();
    int cols = block->shape[0].size();

    std::vector<std::vector<bool>> temp(cols, std::vector<bool>(rows));

    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            if (block->shape.empty() || block->shape[0].empty()) {
                // Handle the case where block->shape is empty
                return;
            }

            int rotatedRow = rows - 1 - j;
            int rotatedCol = i;

            if (rotatedRow >= 0 && rotatedRow < rows && rotatedCol >= 0 && rotatedCol < cols) {
                temp[i][j] = block->shape[rotatedRow][rotatedCol];
            }
        }

        // Create a new block with the rotated shape
        Block* leftBlock = new Block;
        leftBlock->shape = temp;
        leftBlock->left_rotation = nullptr;
        leftBlock->right_rotation = nullptr;
        leftBlock->next_block = block->next_block;


        if (leftBlock != nullptr && origBlock != nullptr && *leftBlock == *origBlock) {
            if (block != nullptr) {
                block->left_rotation = origBlock;
                origBlock->right_rotation = block;
            }
            delete leftBlock;
        }
        else {
            if (leftBlock != nullptr && block != nullptr) {
                leftBlock->right_rotation = block;
                block->left_rotation = leftBlock;
                leftBlock->next_block = block->next_block;
                rotateLeft(origBlock, leftBlock);
            }
        }


    }
}

void BlockFall::printBlock(const Block* block) {
    
    if (block != nullptr) {
        for (const auto& row : block->shape) {
            std::cout << "[ ";
            for (bool value : row) {
                std::cout << value << " ";
            }
            std::cout << "]\n";
        }
        std::cout << "---------------------------\n";
    }
}

BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks

    Block* currentBlock = initial_block;
    Block* nextBlock = nullptr;
    while (currentBlock != nullptr) {
        Block* current = currentBlock->right_rotation;
        while (current != currentBlock) {
            nextBlock = current->right_rotation;
            delete current;
            current = nextBlock;
        }
        currentBlock->right_rotation = currentBlock->left_rotation = currentBlock;
        nextBlock = currentBlock->next_block;
        delete currentBlock;
        currentBlock = nextBlock;

    }

    delete active_rotation;
    delete initial_block;

}


