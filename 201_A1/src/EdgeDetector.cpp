// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

#include "EdgeDetector.h"
#include <cmath>
#include <iostream>

// Default constructor
EdgeDetector::EdgeDetector() {
    for (int i = 0; i < 3; i++) {
        sobel_vertical[i] = new double [3];
    }
    sobel_vertical[0][0] = -1; sobel_vertical[0][1] = 0; sobel_vertical[0][2] = 1;
    sobel_vertical[1][0] = -2; sobel_vertical[1][1] = 0; sobel_vertical[1][2] = 2;
    sobel_vertical[2][0] = -1; sobel_vertical[2][1] = 0; sobel_vertical[2][2] = 1;

    for (int i = 0; i < 3; i++) {
        sobel_horizontal[i] = new double [3];
    }

    sobel_horizontal[0][0] = -1; sobel_horizontal[0][1] = -2; sobel_horizontal[0][2] = -1;
    sobel_horizontal[1][0] =  0; sobel_horizontal[1][1] =  0; sobel_horizontal[1][2] =  0;
    sobel_horizontal[2][0] =  1; sobel_horizontal[2][1] =  2; sobel_horizontal[2][2] =  1;



}

// Destructor
EdgeDetector::~EdgeDetector() {
    for (int i = 0; i < 3; i++) {
        delete[] sobel_vertical[i];
        delete[] sobel_horizontal[i];
    }
    delete[] sobel_vertical;
    delete[] sobel_horizontal;
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {

    double threshold = 0.0;
    // creating our convolutıon objects with our kernels.
    Convolution convolution_vertical(sobel_vertical, 3, 3, 1, true);
    Convolution convolution_horizontal(sobel_horizontal, 3, 3, 1, true);

    // Applying convolutıon
    ImageMatrix image_vertical = convolution_vertical.convolve(input_image);
    ImageMatrix image_horizontal = convolution_horizontal.convolve(input_image);

    // Finding the threshold
    ImageMatrix magnitude(input_image.get_height(), input_image.get_width());
    for (int i = 0; i < magnitude.get_height(); i++) {
        for (int j = 0; j < magnitude.get_width(); j++) {
            double resultX = std::pow(image_vertical.get_data(i,j),2);
            double resultY = std::pow(image_horizontal.get_data(i,j),2);
            magnitude.set_data(i,j,std::sqrt(resultX + resultY));
            threshold += std::sqrt(resultX + resultY) / (magnitude.get_height() * magnitude.get_width());
        }
    }
    std::vector<std::pair<int,int>> edgePixels;

    // Adding our results that bigger than threshold value to our vector
    for (int i = 0; i < magnitude.get_height(); i++) {
        for (int j = 0; j < magnitude.get_width(); j++) {
            if (magnitude.get_data(i,j) > threshold) {
                edgePixels.push_back(std::make_pair(i, j));
            }
        }
    }

    return edgePixels;
}

