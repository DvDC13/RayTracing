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
    Image(int w, int h, int samples_per_pixel) 
        : m_width(w), m_height(h), m_samples_per_pixel(samples_per_pixel), pixels(w * h)
    {
        initIterators();
    }

    inline int getWidth() const { return m_width; }

    inline int getHeight() const { return m_height; }

    Pixel getPixel(int x, int y) const { return pixels[y * m_width + x]; }

    void setPixel(int x, int y, const Pixel& pixel)
    {
        pixels[y * m_width + x] = pixel;
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
        file << m_width << " " << m_height << std::endl;
        file << "255" << std::endl;

        for (int y = m_height - 1; y >= 0; y--)
        {
            for (int x = 0; x < m_width; x++)
            {
                Pixel pixel = getPixel(x, y);
                file << pixel.r << " " << pixel.g << " " << pixel.b << std::endl;
            }
        }

        file.close();
    }

    inline const std::vector<u_int32_t>& getHorizontalIter() const { return m_HorizontalIter; }
    inline const std::vector<u_int32_t>& getVerticalIter() const { return m_VerticalIter; }
    inline const std::vector<u_int32_t>& getSampleIter() const { return m_SampleIter; }

private:
    int m_width;
    int m_height;
    int m_samples_per_pixel;
    std::vector<Pixel> pixels;

    std::vector<u_int32_t> m_HorizontalIter;
    std::vector<u_int32_t> m_VerticalIter;
    std::vector<u_int32_t> m_SampleIter;

    void initIterators()
    {
        m_HorizontalIter.resize(m_width);
        for (int i = 0; i < m_width; i++)
        {
            m_HorizontalIter.at(i) = i;
        }

        m_VerticalIter.resize(m_height);
        for (int i = m_height - 1; i >= 0; i--)
        {
            m_VerticalIter.at(m_height - 1 - i) = i;
        }

        m_SampleIter.resize(m_samples_per_pixel);
    }
};
