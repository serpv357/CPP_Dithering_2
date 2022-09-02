
#ifndef CPP_DITHERING_2_FLOSTE_BUGGED_H
#define CPP_DITHERING_2_FLOSTE_BUGGED_H
// bugged version kinda looks cool... wanna save

#include <opencv2/opencv.hpp>

int __getDiffBetweenPixels(cv::Vec3b *oldPixel, cv::Vec3b *newPixel);
cv::Vec3b __getClosestColor(cv::Vec3b *oldPixel, std::vector<cv::Vec3b> palette);
void __applyErrorColor(cv::Mat *img, int y, int x, cv::Vec3d *dividedError, int multiplier);
bool __outOfBounds(int h, int w, int y, int x);

/*
 * originalImage is BGR
 * palette is array of length 3 arrays representing final possible BGR values in dithered image.
 * */
cv::Mat getDitheredImageColor(cv::Mat originalImage, std::vector<cv::Vec3b> palette){
    double errorFloatTerm = 1.0/16.0;


    if (originalImage.channels() < 3) {
        throw "Tried to color dither grayscale image!";
    }

    int h = originalImage.size[0];
    int w = originalImage.size[1];

    for (int y = 0; y < h; y++) {
        cv::Vec3b* ptr = originalImage.ptr<cv::Vec3b>(y);
        for (int x = 0; x < w; x++, ptr++) {
            cv::Vec3b *oldPixel = ptr;
            cv::Vec3b newPixel = __getClosestColor(oldPixel, palette);

            std::cout << "Current pixel coordinate: (" << y << ", " << x<< ")\n";
            std::cout << "Current pixel value: (" << +(*oldPixel)[0] << ", " << +(*oldPixel)[1] << ", " << +(*oldPixel)[2] << ")\n";

            double bError = (double)(*oldPixel)[0] - (double)(newPixel)[0];
            double gError = (double)(*oldPixel)[1] - (double)(newPixel)[1];
            double rError = (double)(*oldPixel)[2] - (double)(newPixel)[2];

            cv::Vec3d error = cv::Vec3d(bError, gError, rError);


            error *= errorFloatTerm;

            ptr[x] = newPixel;
            std::cout << "Pixel set to: " << +(*oldPixel)[0] << ", " << +(*oldPixel)[1] << ", " << +(*oldPixel)[2] << ")\n";

            if (!__outOfBounds(h, w, y, x + 1))
                std::cout << "Applying error to pixel (" << y << ", " << x + 1<< ")\n";
            std::cout << "Pre-error applied: ";
            auto pre = *originalImage.ptr<cv::Vec3b>(y,  x + 1);
            std:: cout << '(' << +pre[0] << ", " << +pre[1] << ", " << +pre[2] << ")\n";
            __applyErrorColor(&originalImage, y, x + 1, &error, 7);
            auto post = *originalImage.ptr<cv::Vec3b>(y,  x + 1);
            std::cout << "Post-error applied: ";
            std:: cout << '(' << +post[0] << ", " << +post[1] << ", " << +post[2] << ")\n";
            if (!__outOfBounds(h, w, y + 1, x - 1))
                __applyErrorColor(&originalImage, y + 1, x - 1, &error, 3);
            if (!__outOfBounds(h, w, y + 1, x))
                __applyErrorColor(&originalImage, y + 1, x, &error, 5);
            if (!__outOfBounds(h, w, y + 1, x + 1))
                __applyErrorColor(&originalImage, y + 1, x + 1, &error, 1);
            std::cout << '\n';
        }
    }

    return originalImage;
}

cv::Vec3b __getClosestColor(cv::Vec3b *oldPixel, std::vector<cv::Vec3b> palette) {
    int minDiff = __getDiffBetweenPixels(oldPixel, &palette[0]);
    cv::Vec3b closest = palette[0];
    int len = std::size(palette);
    for (int i = 1; i < len; i++) {
        int newDiff = __getDiffBetweenPixels(oldPixel, &palette[i]);
        if (newDiff < minDiff) {
            closest = palette[i];
            minDiff = newDiff;
        }
    }
    return closest;
}

int __getDiffBetweenPixels(cv::Vec3b *oldPixel, cv::Vec3b *newPixel) {
    uchar b = (*oldPixel)[0] - (*newPixel)[0];
    uchar g = (*oldPixel)[1] - (*newPixel)[1];
    uchar r = (*oldPixel)[2] - (*newPixel)[2];

    return b*b + g*g + r*r;
}

void __applyErrorColor(cv::Mat *img, int y, int x, cv::Vec3d *dividedError, int multiplier) {
    float bError = multiplier * (*dividedError)[0];
    float gError = multiplier * (*dividedError)[1];
    float rError = multiplier * (*dividedError)[2];

    cv::Vec3b *p = (*img).ptr<cv::Vec3b>(y, x);
    auto b = (uchar)std::clamp<double>(((double)(*p)[0] + bError), 0.0, 255.0);
    auto g = (uchar)std::clamp<double>(((double)(*p)[1] + gError), 0.0, 255.0);
    auto r = (uchar)std::clamp<double>(((double)(*p)[2] + rError), 0.0, 255.0);
    *p = cv::Vec3b(b, g, r);
}

bool __outOfBounds(int h, int w, int y, int x) {
    if (y > h - 1 || y < 0)
        return true;
    if (x > w - 1 || x < 0)
        return true;
    return false;
}

cv::Mat getDitheredImageGray(cv::Mat originalImage);
#endif //CPP_DITHERING_2_FLOSTE_BUGGED_H
