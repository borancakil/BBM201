#include "SpaceSectorBST.h"
#include "Sector.h"
#include "fstream"
#include <sstream>
using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST()
{
    // Free any dynamically allocated memory in this class.

    deleteTree(root);
    sectorHashMap.clear();
}

void SpaceSectorBST::deleteTree(Sector *&tree)
{
    if (tree != nullptr)
    {
        // Recursively delete the left and right subtrees
        deleteTree(tree->left);
        deleteTree(tree->right);

        // Delete the current node
        delete tree;

        // Set the pointer to nullptr after deletion
        tree = nullptr;
    }
}

void SpaceSectorBST::readSectorsFromFile(const std::string &filename)
{
    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison criteria based on the sector coordinates.

    // Create an input file stream object
    std::ifstream inputFile(filename);

    // Check if the file is open
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;

    std::getline(inputFile, line);
    while (std::getline(inputFile, line))
    {
        std::stringstream ss(line);

        int x = 0, y = 0, z = 0;

        if (ss >> x >> std::ws && ss.get() == ',' &&
            ss >> y >> std::ws && ss.get() == ',' &&
            ss >> z)
        {
            // Process the coordinates as needed
            insertSectorByCoordinates(x, y, z);
        }
        else
        {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }

    inputFile.close();
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z)
{
    // Instantiate and insert a new sector into the space sector BST map according to the
    // coordinates-based comparison criteria.

    if (root == nullptr)
    {
        root = new Sector(x, y, z);
        sectorHashMap[root->sector_code] = root;
    }
    else
    {
        insertSectorByCoordinatesRecursive(root, x, y, z);
    }
}

void SpaceSectorBST::insertSectorByCoordinatesRecursive(Sector *&current, int x, int y, int z)
{

    if (current == nullptr)
    {
        current = new Sector(x, y, z);
        sectorHashMap[current->sector_code] = current;
    }
    else
    {
        if (Sector(x, y, z) < *current)
        {
            insertSectorByCoordinatesRecursive(current->left, x, y, z);
        }
        else if (*current < Sector(x, y, z))
        {
            insertSectorByCoordinatesRecursive(current->right, x, y, z);
        }
    }
}

Sector *SpaceSectorBST::findSuccessor(Sector *sector)
{
    while (sector->left != nullptr)
    {
        sector = sector->left;
    }
    return sector;
}

Sector *SpaceSectorBST::getSector(const std::string &sector_code)
{
    auto sector = sectorHashMap.find(sector_code);
    if (sector != sectorHashMap.end())
    {
        return sector->second;
    }
    return nullptr; // Sector not found
}

void SpaceSectorBST::deleteSector(const std::string &sector_code)
{
    // TODO: Delete the sector given by its sector_code from the BST.

    Sector *sectorToDelete = getSector(sector_code);

    // If the sector is found, perform deletion
    if (sectorToDelete != nullptr)
    {
        root = deleteSectorRecursive(root, sectorToDelete);
        sectorHashMap.erase(sector_code); // Remove the sector from the hashmap
    }
}

Sector *SpaceSectorBST::deleteSectorRecursive(Sector *current, Sector *sectorToDelete)
{
    if (current == nullptr)
    {
        return nullptr;
    }

    if ((sectorToDelete->x < current->x) ||
        ((sectorToDelete->x == current->x) && (sectorToDelete->y < current->y)) ||
        ((sectorToDelete->x == current->x) && (sectorToDelete->y == current->y) && (sectorToDelete->z < current->z)))
    {

        current->left = deleteSectorRecursive(current->left, sectorToDelete);
    }
    else if ((sectorToDelete->x > current->x) ||
             ((sectorToDelete->x == current->x) && (sectorToDelete->y > current->y)) ||
             ((sectorToDelete->x == current->x) && (sectorToDelete->y == current->y) && (sectorToDelete->z > current->z)))
    {

        current->right = deleteSectorRecursive(current->right, sectorToDelete);
    }
    else
    {
        // Node found, perform deletion
        if (current->left == nullptr)
        {

            // Node has no or only a right child
            Sector *temp = current->right;
            delete current;
            return temp;
        }
        else if (current->right == nullptr)
        {
            // Node has only a left child

            Sector *temp = current->left;
            delete current;
            return temp;
        }
        else
        {

            Sector *successor = findSuccessor(current->right);
            current->right = deleteSectorRecursive(current->right, successor);
            current->x = successor->x;
            current->y = successor->y;
            current->z = successor->z;
            current->sector_code = successor->sector_code;
        }
    }

    return current;
}

void SpaceSectorBST::displaySectorsInOrder()
{
    // TODO: Traverse the space sector BST map in-order and print the sectors
    // to STDOUT in the given format.
    if (root != nullptr)
    {
        std::cout << "Space sectors inorder traversal:" << std::endl;
        displaySectorsInOrderRecursive(root);
        std::cout << std::endl;
    }
    else
    {
        std::cout << "no tree" << std::endl;
    }
}

void SpaceSectorBST::displaySectorsInOrderRecursive(Sector *&current)
{

    if (current != nullptr)
    {
        displaySectorsInOrderRecursive(current->left);
        std::cout << *current << std::endl;
        displaySectorsInOrderRecursive(current->right);
    }
}

void SpaceSectorBST::displaySectorsPreOrder()
{
    // TODO: Traverse the space sector BST map in pre-order traversal and print
    // the sectors to STDOUT in the given format.

    if (root != nullptr)
    {
        std::cout << "Space sectors preorder traversal:" << std::endl;
        displaySectorsPreOrderRecursive(root);
        std::cout << std::endl;
    }
    else
    {
        std::cout << "no tree" << std::endl;
    }
}

void SpaceSectorBST::displaySectorsPreOrderRecursive(Sector *&current)
{
    if (current != nullptr)
    {
        std::cout << *current << std::endl;
        displaySectorsPreOrderRecursive(current->left);
        displaySectorsPreOrderRecursive(current->right);
    }
}

void SpaceSectorBST::displaySectorsPostOrder()
{
    // TODO: Traverse the space sector BST map in post-order traversal and print
    // the sectors to STDOUT in the given format.

    if (root != nullptr)
    {
        std::cout << "Space sectors postorder traversal:" << std::endl;
        displaySectorsPostOrderRecursive(root);
        std::cout << std::endl;
    }
    else
    {
        std::cout << "no tree" << std::endl;
    }
}

void SpaceSectorBST::displaySectorsPostOrderRecursive(Sector *&current)
{

    if (current != nullptr)
    {
        displaySectorsPostOrderRecursive(current->left);
        displaySectorsPostOrderRecursive(current->right);
        std::cout << *current << std::endl;
    }
}

std::vector<Sector *> SpaceSectorBST::getStellarPath(const std::string &sector_code)
{
    std::vector<Sector *> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!

    Sector *sectorToFind = getSector(sector_code);

    if (sectorToFind == nullptr)
    {
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
    }

    path = findPathRecursive(root, sectorToFind);

    return path;
}

std::vector<Sector *> SpaceSectorBST::findPathRecursive(Sector *current, Sector *SectorToFind)
{
    std::vector<Sector *> path;

    if (current == nullptr || SectorToFind == nullptr)
    {
        return path; // Empty vector if either current or SectorToFind is null
    }

    if (*SectorToFind < *current)
    {
        path.push_back(current);
        std::vector<Sector *> leftPath = findPathRecursive(current->left, SectorToFind);
        path.insert(path.end(), leftPath.begin(), leftPath.end());
    }
    else if (*current < *SectorToFind)
    {
        path.push_back(current);
        std::vector<Sector *> rightPath = findPathRecursive(current->right, SectorToFind);
        path.insert(path.end(), rightPath.begin(), rightPath.end());
    }
    else
    {
        // *current == *SectorToFind
        path.push_back(current);
    }

    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector *> &path)
{
    // TODO: Print the stellar path obtained from the getStellarPath() function
    // to STDOUT in the given format.

    if (path.size() > 0)
    {
        std::cout << "The stellar path to Dr. Elara: ";
        for (size_t i = 0; i < path.size(); ++i)
        {
            std::cout << path[i]->sector_code;
            if (i + 1 != path.size())
            {
                std::cout << "->";
            }
        }
        std::cout << std::endl;
    }
}