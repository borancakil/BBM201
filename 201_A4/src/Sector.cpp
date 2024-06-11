#include "Sector.h"
#include <cmath>
#include <iostream>

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code

        distance_from_earth = sqrt(x*x + y*y + z*z);

        int distance_int = rounder(distance_from_earth);

        sector_code += std::to_string(distance_int);

        coder(x, y, z);

}

void Sector::coder(int x, int y, int z) {
    if (x > 0)
    {
        sector_code += "R";
    }
    else if (x < 0)
    {
        sector_code += "L";
    }
    else
    {
        sector_code += "S";
    }

    if (y > 0)
    {
        sector_code += "U";
    }
    else if (y < 0)
    {
        sector_code += "D";
    }
    else
    {
        sector_code += "S";
    }

    if (z > 0)
    {
        sector_code += "F";
    }
    else if (z < 0)
    {
        sector_code += "B";
    }
    else
    {
        sector_code += "S";
    }
}

int Sector::rounder(double x){

    int roundX = static_cast<int>(x);

    return roundX;
}

Sector::~Sector()
{
    // TODO: Free any dynamically allocated memory if necessary
}



Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator

    if (this != &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;

    }
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}

bool Sector::operator<(const Sector &other) const
{
    if (x < other.x)
    {
        return true;
    }
    else if (x > other.x)
    {
        return false;
    }
    else
    {
        if (y < other.y)
        {

            return true;
        }
        else if (y > other.y)
        {

            return false;
        }
        else
        {
            if (z < other.z)
            {

                return true;
            }
            else if (z > other.z)
            {
                return false;
            }
            else
            {
                return false;
            }
        }
    }
}

bool Sector::operator>(const Sector &other) const
{
    if (x > other.x)
    {
        return true;
    }
    else if (x < other.x)
    {
        return false;
    }
    else
    {
        if (y > other.y)
        {
            return true;
        }
        else if (y < other.y)
        {
            return false;
        }
        else
        {
            if (z > other.z)
            {
                return true;
            }
            else if (z < other.z)
            {
                return false;
            }
            else
            {
                return false; // Equal in all dimensions
            }
        }
    }
}



std::ostream &operator<<(std::ostream &os, const Sector &sector)
{
    os << sector.sector_code;
    return os;
}