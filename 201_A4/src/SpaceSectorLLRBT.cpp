#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.

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

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    deleteTree(root);
    sectorHashMap.clear();
}

void SpaceSectorLLRBT::deleteTree(Sector *&tree)
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

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.

    if (root == nullptr)
    {
        root = new Sector(x, y, z);
        sectorHashMap[root->sector_code] = root;
    }
    else
    {
        insertSectorByCoordinatesRecursive(root, nullptr, x, y, z);
    }

}

bool SpaceSectorLLRBT::isRed(Sector *sector)
{
    return sector != nullptr && sector->color;
}

Sector *SpaceSectorLLRBT::rotateLeft(Sector *sector)
{
    Sector *x = sector->right;
    sector->right = x->left;
    if (x->left != nullptr)
    {
        x->left->parent = sector;
    }
    x->left = sector;
    x->parent = sector->parent;
    sector->parent = x;
    x->color = sector->color;
    sector->color = RED;
    return x;
}

Sector *SpaceSectorLLRBT::rotateRight(Sector *sector)
{
    Sector *x = sector->left;
    sector->left = x->right;
    if (x->right != nullptr)
    {
        x->right->parent = sector;
    }
    x->right = sector;
    x->parent = sector->parent;
    sector->parent = x;
    x->color = sector->color;
    sector->color = RED;
    return x;
}

void SpaceSectorLLRBT::flipColors(Sector *h)
{
    h->color = RED;
    h->left->color = BLACK;
    h->right->color = BLACK;
}

void SpaceSectorLLRBT::insertSectorByCoordinatesRecursive(Sector *&current,Sector *parent, int x, int y, int z)
{
    if (current == nullptr)
    {
        current = new Sector(x, y, z);
        current->parent = parent;

        if (current->parent == nullptr) {

            current->color = BLACK; 
        }else
        {
            current->color = RED;
        }

        sectorHashMap[current->sector_code] = current;

        }
    else
    {
        // Recursive insertion
        if (Sector(x, y, z) < *current)
        {
            insertSectorByCoordinatesRecursive(current->left, current, x, y, z);
        }
        else if (*current < Sector(x, y, z))
        {
            insertSectorByCoordinatesRecursive(current->right, current, x, y, z);
        }

        // Fix violations after insertion
        if (isRed(current->right) && !isRed(current->left))
        {
            current = rotateLeft(current);
        }
        if (isRed(current->left) && isRed(current->left->left))
        {
            current = rotateRight(current);
        }
        if (isRed(current->left) && isRed(current->right))
        {
            flipColors(current);
        }

        if (current->parent == nullptr)
        {
            current->color = BLACK;
        }
    }
}


void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
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

void SpaceSectorLLRBT::displaySectorsInOrderRecursive(Sector *current)
{
    if (current != nullptr)
    {
        displaySectorsInOrderRecursive(current->left);
        if (!current->color) {
            std::cout << "BLACK ";
        }
        else
        {
            std::cout << "RED ";
        }

        std::cout << "sector: "<<  *current << std::endl;
        displaySectorsInOrderRecursive(current->right);
    }

}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
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

void SpaceSectorLLRBT::displaySectorsPreOrderRecursive(Sector *current)
{
    if (current != nullptr)
    {
        if (!current->color)
        {
            std::cout << "BLACK ";
        }
        else
        {
            std::cout << "RED ";
        }

        std::cout << "sector: " << *current << std::endl;
        displaySectorsPreOrderRecursive(current->left);
        displaySectorsPreOrderRecursive(current->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
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

void SpaceSectorLLRBT::displaySectorsPostOrderRecursive(Sector *current)
{
    if (current != nullptr)
    {

        displaySectorsPostOrderRecursive(current->left);
        displaySectorsPostOrderRecursive(current->right);
        if (!current->color)
        {
            std::cout << "BLACK ";
        }
        else
        {
            std::cout << "RED ";
        }

        std::cout << "sector: " << *current << std::endl;
    }
}

Sector *SpaceSectorLLRBT::getSector(const std::string &sector_code)
{
    auto sector = sectorHashMap.find(sector_code);
    if (sector != sectorHashMap.end())
    {
        return sector->second;
    }
    return nullptr; // Sector not found

}

std::vector<Sector *> SpaceSectorLLRBT::getStellarPath(const std::string &sector_code)
{
    std::vector<Sector*> path;
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

std::vector<Sector *> SpaceSectorLLRBT::findPathRecursive(Sector *current, Sector *SectorToFind)
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

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
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