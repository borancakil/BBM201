#include <iostream>
#include "ImageSharpening.h"
#include "ImageMatrix.h"
#include "Convolution.h"

// Default constructor
ImageSharpening::ImageSharpening() {
    kernel_height = 3;
    kernel_width = 3;

    // Allocate memory for the 2D array
    blurring_kernel = new double*[kernel_height];
    for (int i = 0; i < kernel_height; i++) {
        blurring_kernel[i] = new double[kernel_width];
    }

    // Initialize the elements with 1.0
    for (int i = 0; i < kernel_height; i++) {
        for (int j = 0; j < kernel_width; j++) {
            blurring_kernel[i][j] = (1.0 / 9.0);
        }
    }

}

ImageSharpening::~ImageSharpening(){
    // Deallocate memory
    if (blurring_kernel != nullptr) {
        for (int i = 0; i < kernel_height; ++i) {
            delete[] blurring_kernel[i];
        }
        delete[] blurring_kernel;
    }

}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    int blur_height = input_image.get_height();
    int blur_width = input_image.get_width();

    ImageMatrix blurred_image(blur_height, blur_width);



    Convolution convolution(blurring_kernel, kernel_height, kernel_width, 1, true);

    blurred_image = convolution.convolve(input_image);
    ImageMatrix sharpened_image = input_image + (input_image - blurred_image) * k;

    for (int i = 0; i < sharpened_image.get_height(); i++) {
        for (int j = 0; j < sharpened_image.get_width() ; j++) {
            if (sharpened_image.get_data(i,j) > 255) {
                sharpened_image.set_data(i,j,255);
            }
            if (sharpened_image.get_data(i,j) < 0) {
                sharpened_image.set_data(i,j,0);
            }
        }
    }

    return sharpened_image;
}
