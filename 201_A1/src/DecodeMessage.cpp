// DecodeMessage.cpp

#include <iostream>
#include "DecodeMessage.h"
#include "bitset"

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {

    std::string bitStr;
    std::string out;

    for (const auto& edgePixel : edgePixels) {
        // Our coordinates
        int x = edgePixel.first;
        int y = edgePixel.second;
        // Extracting the least significant bit (LSB) from the integer representation of the pixel value
        double pixelValue = image.get_data(x, y);  // Get the pixel value as a double
        int intValue = static_cast<int>(pixelValue); // Convert it to an integer
        int lsb = intValue & 1; // Extract the LSB
        std::string pixel = std::to_string(lsb); // Convert the LSB to a string
        bitStr += pixel; // Append the LSB to the binary string


    }
    // padding the string
    while (bitStr.length() % 7 !=0) {
        bitStr = '0' + bitStr;
    }

    // Taking our strings every chars ASCII values
    out = toASCII(bitStr);
    return out;
}


std::string DecodeMessage::toASCII(const std::string &my_string)  {
    std::string out;
    for (size_t i = 0; i < my_string.size(); i += 7) {
        std::string byte = my_string.substr(i, 7);
        int decimalValue = std::stoi(byte, nullptr, 2);

        // Clipping the values into ASCII range
        if (decimalValue <= 32) {
            decimalValue += 33;
        }
        if (decimalValue >= 127) {
            decimalValue = 126;
        }

        out += static_cast<char>(decimalValue);
    }
    return out;
}

