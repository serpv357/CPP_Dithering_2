#include <iostream>
#include <opencv2/opencv.hpp>

#include "floste_dither.h"

int main() {
    std::string readPath = "";
    std::string outPath = "";
    cv::Mat readImage = cv::imread(readPath, cv::IMREAD_COLOR);
    std::vector<cv::Vec3b> palette = {cv::Vec3b(255, 254, 230), cv::Vec3b(112, 20, 102), cv::Vec3b(38, 126, 115)};
    //std::vector<cv::Vec3b> palette = {cv::Vec3b(0, 0, 0), cv::Vec3b(255, 255, 255)};
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat ditheredImage = getDitheredImageColor(readImage, palette);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << std::endl;

    cv::imwrite(outPath, ditheredImage);

}
