#ifndef IMAGE_SHARPENING_H
#define IMAGE_SHARPENING_H

#include "ImageMatrix.h"
#include "Convolution.h"

class ImageSharpening {
    public:
        ImageSharpening(); // default constructor
        ~ImageSharpening(); 

        ImageMatrix sharpen(const ImageMatrix& input_image, double k);

    private:
    double **blurring_kernel;
    int kernel_height = 3;
    int kernel_width = 3;

};

#endif // IMAGE_SHARPENING_H



