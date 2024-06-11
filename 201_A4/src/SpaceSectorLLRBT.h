#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <unordered_map>

class SpaceSectorLLRBT {
public:
    Sector* root;
    std::unordered_map<std::string, Sector *> sectorHashMap;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void deleteTree(Sector*& tree);
    void readSectorsFromFile(const std::string &filename);

    void insertSectorByCoordinates(int x, int y, int z);
    void insertSectorByCoordinatesRecursive(Sector *&current,Sector *parent, int x, int y, int z);

    bool isRed(Sector *sector);
    Sector *rotateLeft(Sector *h);
    Sector *rotateRight(Sector *h);
    void flipColors(Sector *h);

    void displaySectorsInOrder();
    void displaySectorsInOrderRecursive(Sector *current);
    void displaySectorsPreOrder();
    void displaySectorsPreOrderRecursive(Sector *current);
    void displaySectorsPostOrder();
    void displaySectorsPostOrderRecursive(Sector *current);

    Sector *getSector(const std::string &sector_code);
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    std::vector<Sector *> findPathRecursive(Sector *current, Sector *SectorToFind);
    void printStellarPath(const std::vector<Sector*>& path);
};

#endif // SPACESECTORLLRBT_H
