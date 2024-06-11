#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"
#include <unordered_map>

class SpaceSectorBST {
  
public:
    Sector *root;
    std::unordered_map<std::string, Sector *> sectorHashMap;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void deleteTree(Sector *&tree);

    void readSectorsFromFile(const std::string& filename); 

    void insertSectorByCoordinates(int x, int y, int z);
    void insertSectorByCoordinatesRecursive(Sector *&current, int x, int y, int z);

    Sector *findSuccessor(Sector *sector);
    Sector *getSector(const std::string &sector_code);
    void deleteSector(const std::string &sector_code);
    Sector* deleteSectorRecursive(Sector *current, Sector *SectorToDelete);

    void displaySectorsInOrder();
    void displaySectorsInOrderRecursive(Sector *&current);

    void displaySectorsPreOrder();
    void displaySectorsPreOrderRecursive(Sector *&current);

    void displaySectorsPostOrder();
    void displaySectorsPostOrderRecursive(Sector *&current);

    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    std::vector<Sector *> findPathRecursive(Sector *current, Sector *SectorToFind);

    void printStellarPath(const std::vector<Sector *> &path);
};

#endif // SPACESECTORBST_H
