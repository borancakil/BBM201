#include "string"
#include "Convolution.h"
#include "iostream"


// Default constructor
Convolution::Convolution() {
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad)
: kernelHeight(kh), kernelWidth(kw), stride(stride_val), padding(pad) {

    this->kernel = new double*[kernelHeight];
    for (int i = 0; i < kh; i++) {
        this->kernel[i] = new double[kw];
        for (int j = 0; j < kw; j++) {
            this->kernel[i][j] = customKernel[i][j];
        }
    }
}


// Destructor
Convolution::~Convolution() {
    // Deallocate memory
    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; i++) {
            delete[] kernel[i];
        }
        delete[] kernel;
        kernel = nullptr;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other){

    if (this != &other) { // Check for self-assignment
        this->kernelHeight = other.kernelHeight;
        this->kernelWidth = other.kernelWidth;
        this->padding = other.padding;
        this->stride = other.stride;
    }

    // Allocate memory for the matrix and copy data

    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            kernel[i][j] = other.kernel[i][j];
        }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this != &other) {
        // Copy dimensions and flags
        this->kernelHeight = other.kernelHeight;
        this->kernelWidth = other.kernelWidth;
        this->padding = other.padding;
        this->stride = other.stride;

        // Allocate memory for the matrix and copy data
        kernel = new double*[kernelHeight];
        for (int i = 0; i < kernelHeight; ++i) {
            kernel[i] = new double[kernelWidth];
            for (int j = 0; j < kernelWidth; ++j) {
                kernel[i][j] = other.kernel[i][j];
            }
        }
    }
    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {

    int outputHeight =  (input_image.get_height() - kernelHeight + (2 * padding)) / stride + 1;
    int outputWidth = (input_image.get_width() - kernelWidth + (2 * padding)) / stride + 1;

    ImageMatrix imageMatrix(outputHeight, outputWidth);

    // If we use padding
    if (padding) {
        // Image
        for (int row = 0; row < outputHeight; row++) {
            for (int col = 0; col < outputWidth; col++) {
                // Value for holding the sum of the operation with the kernel
                double sum = 0.0;

                // kernel
                for (int krow = 0; krow < kernelHeight; krow++) {
                    for (int kcol = 0; kcol < kernelWidth; kcol++) {

                        // Using stride
                        int X = row * stride;
                        int Y = col * stride;

                        // Checking whether the our indices are in the image
                        if (X + krow - padding >= 0 && X + krow - padding < input_image.get_height() && Y + kcol - padding >= 0 &&
                        Y + kcol - padding < input_image.get_width()) {
                            sum += input_image.get_data(X  + krow - padding, Y + kcol - padding) * kernel[krow][kcol];
                        }
                    }
                }
                // Store the result in the output matrix
                if (row < outputHeight && col < outputWidth) {
                    imageMatrix.set_data(row, col, sum);
                }
            }
        }
} else {
        for (int row = 0; row < outputHeight; row++) {
            for (int col = 0; col < outputWidth; col++) {

                // Value for holding the sum of the operation with the kernel
                double sum = 0.0;

                // kernel
                for (int krow = 0; krow < kernelHeight; krow++) {
                    for (int kcol = 0; kcol < kernelWidth; kcol++) {

                        // Using stride
                        int X = row * stride;
                        int Y = col * stride;

                        // Checking whether the our indices are in the image
                        if (X + krow >= 0 && X + krow  < input_image.get_height() && Y + kcol >= 0 &&
                            Y + kcol < input_image.get_width()) {
                            sum += input_image.get_data(X  + krow, Y + kcol) * kernel[krow][kcol];
                        }
                    }
                }
                // Store the result in the output matrix
                if (row < outputHeight && col < outputWidth) {
                    imageMatrix.set_data(row, col, sum);
                }
            }
        }
    }


    return imageMatrix;
}
