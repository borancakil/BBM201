#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gamePlay
    void printGrid(const BlockFall& game);
    void blockInitializer(BlockFall& game, Block* block);
    void moveRight(BlockFall& game, Block* block);
    void moveLeft(BlockFall& game, Block* block);
    void rotateRight(BlockFall& game, Block* block);
    void rotateLeft(BlockFall& game, Block* block);
    void deleteBlock(BlockFall& game, Block* block);
    bool rotateCollision( BlockFall& game, Block* block, std::vector<std::pair<int, int>>& newPosition);
    bool collisionDetected(const BlockFall& game, const Block* block, int X, int Y);
    void drop(BlockFall& game, Block* block);
    void popRow(BlockFall& game);
    int count = 0;
};


#endif //PA2_GAMECONTROLLER_H
