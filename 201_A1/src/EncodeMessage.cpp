#include "EncodeMessage.h"
#include <cmath>
#include <iostream>
#include <bitset>


// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    ImageMatrix imageMatrix = img;


    std::string encodedMsg = message;
    double asciiValue;
    int messageIndex = 0;

    while (messageIndex < message.length() - 1) {
        // Check if the our index is a prime number
        if (isPrime(messageIndex)) {
            char ch = message[messageIndex];
            // Get the fibonaccı number with the our index
            asciiValue = ch + fib(messageIndex);

            if (asciiValue < 33) {
                asciiValue += 33;  // Ensure it's within the printable ASCII range
            } else if (asciiValue >= 127) {
                asciiValue = 126;  // Clip to the maximum printable ASCII value
            }
            encodedMsg[messageIndex] = asciiValue;
        }
        messageIndex++;
    }
    // Apply rightcırcularshift
    encodedMsg = rightCircularShift(encodedMsg);

    messageIndex = 0;
    for (const auto &coordinate: positions) {
        int x = coordinate.first;
        int y = coordinate.second;

        int pixelValue = imageMatrix.get_data(x, y);

        if (messageIndex < encodedMsg.size()) {
            int nextBit = encodedMsg[messageIndex] - '0' + '0';
            nextBit = nextBit & 1;

            // Update the LSB of the pixel at (x, y) with the next bit

            pixelValue = (pixelValue & 0xFE) | nextBit;
            // Set the updated pixel value back in the image
            imageMatrix.set_data(x, y, pixelValue);

            // Increment the message index to get the next bit in the message
        }
        messageIndex++;
    }
    return imageMatrix;

}

bool EncodeMessage::isPrime(int n) {
    if (n == 1 || n == 0) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

std::string EncodeMessage::rightCircularShift(std::string str) {
    int middle = str.length() / 2;
    std::cout << middle << std::endl;
    std::string output =  str.substr(middle + 1) + str.substr(0,middle + 1);
    return output;
}

int EncodeMessage::fib(int n) {
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        int next = a + b;
        a = b;
        b = next;
    }
    return b;
}
