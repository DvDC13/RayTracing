#pragma once

#include <iostream>
#include <vector>
#include <fstream>

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

    void toPPM(const std::string& path) const
    {
        std::ofstream file;
        file.open(path);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open file " << path << std::endl;
            exit(EXIT_FAILURE);
        }

        file << "P3" << std::endl;
        file << width << " " << height << std::endl;
        file << "255" << std::endl;

        for (int y = height - 1; y >= 0; y--)
        {
            for (int x = 0; x < width; x++)
            {
                Pixel pixel = getPixel(x, y);
                file << pixel.r << " " << pixel.g << " " << pixel.b << std::endl;
            }
        }

        file.close();
    }

private:
    int width;
    int height;
    std::vector<Pixel> pixels;
};
