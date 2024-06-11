#include <iostream>
#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    std::string output;
    // I don't know why I do this but it works.
    ImageMatrix encodedImage = img;

    ImageSharpening imageSharpening;
    ImageMatrix sharpImg = imageSharpening.sharpen(encodedImage, 2);

    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> edges = edgeDetector.detectEdges(sharpImg);

    DecodeMessage decodeMessage;
    output = decodeMessage.decodeFromImage(sharpImg,edges);

    return output;

}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message)
{

    ImageMatrix encodedImage = img;

    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> edges = edgeDetector.detectEdges(encodedImage);

    EncodeMessage encoder;
    encodedImage = encoder.encodeMessageToImage(encodedImage, message, edges);

    return encodedImage;
}
