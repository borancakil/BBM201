#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>
#include "iostream"
using namespace std;

class Block {
public:

    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    std::vector<std::pair<int, int>> position; // // Two-dimensional vector corresponding to the block's position
    Block * right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block * left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block * next_block = nullptr; // Pointer to the next block to appear in the game
    ~Block() {
                // Deallocate left rotations
        Block* currentLeft = left_rotation;
        while (currentLeft != nullptr && currentLeft != this) {
            Block* nextLeft = currentLeft->left_rotation;
            delete currentLeft;
            currentLeft = nextLeft;
        }

        // Deallocate right rotations
        Block* currentRight = right_rotation;
        while (currentRight != nullptr && currentRight != this) {
            Block* nextRight = currentRight->right_rotation;
            delete currentRight;
            currentRight = nextRight;
        }
    }

    bool operator==(const Block& other) const {
        // TODO: Overload the == operator to compare two blocks based on their shapes

        if (shape.empty() || other.shape.empty() || shape.size() != other.shape.size() || shape[0].size() != other.shape[0].size()) {
            return false;
        }

        // Compare corresponding elements of the shapes
        for (size_t i = 0; i < shape.size(); ++i) {
            for (size_t j = 0; j < shape[i].size(); ++j) {
                if (shape[i][j] != other.shape[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }


    bool operator!=(const Block& other) const {
        // TODO: Overload the != operator to compare two blocks based on their shapes
        

        if (shape.size() != other.shape.size()  || shape[0].size() != other.shape[0].size()) {
            return true;
        }

        // Compare corresponding elements of the shapes
        for (size_t i = 0; i < shape.size(); ++i) {
            for (size_t j = 0; j < shape[i].size(); ++j) {
                if (shape[i][j] != other.shape[i][j]) {
                    return true;
                }
            }
        }

        return false;

    }
};



#endif //PA2_BLOCK_H
