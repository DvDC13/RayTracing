#pragma once

#include <iostream>
#include <vector>

class Pixel
{
public:
    Pixel() : r(0), g(0), b(0) {}
    Pixel(int red, int green, int blue) : r(red), g(green), b(blue) {}

    int r;
    int g;
    int b;
};

class Image
{
public:
    Image(int w, int h) : width(w), height(h), pixels(w * h) {}

    int getWidth() const { return width; }

    int getHeight() const { return height; }

    Pixel getPixel(int x, int y) const { return pixels[y * width + x]; }

    void setPixel(int x, int y, const Pixel& pixel)
    {
        pixels[y * width + x] = pixel;
    }

    void toPPM() const
    {
        std::cout << "P3" << std::endl;
        std::cout << width << " " << height << std::endl;
        std::cout << "255" << std::endl;

        for (int y = height - 1; y >= 0; y--)
        {
            for (int x = 0; x < width; x++)
            {
                Pixel pixel = getPixel(x, y);
                std::cout << pixel.r << " " << pixel.g << " " << pixel.b << std::endl;
            }
        }
    }

private:
    int width;
    int height;
    std::vector<Pixel> pixels;
};
