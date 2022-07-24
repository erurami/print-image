
#include "imageclass.hpp"

#include "shrink.hpp"

#include <stdio.h>


int Bilinear(Image* pImageOut, Image* pImageIn, int widthTo, int heightTo);
int BoxSampling(Image* pImageOut, Image* pImageIn, int widthTo, int heightTo);

int ShrinkImage(Image* pImageOut, Image* pImageIn, int widthTo, int heightTo, int method)
{
    switch (method)
    {
        case METHOD_BOX_SAMPLING:
            return BoxSampling(pImageOut, pImageIn, widthTo, heightTo);
        case METHOD_BILINEAR:
            return Bilinear(pImageOut, pImageIn, widthTo, heightTo);
    }

    return 1;
}


Pixel GetAvgPixel(Image* pImage, int x1, int y1)
{
    int x2 = x1 + 1;
    int y2 = y1 + 1;

    Pixel avg_pix;

    avg_pix.r = (pImage->m_pImageData[y1 * pImage->width + x1].r +
                 pImage->m_pImageData[y1 * pImage->width + x2].r +
                 pImage->m_pImageData[y2 * pImage->width + x1].r +
                 pImage->m_pImageData[y2 * pImage->width + x2].r) / 4;

    avg_pix.g = (pImage->m_pImageData[y1 * pImage->width + x1].g +
                 pImage->m_pImageData[y1 * pImage->width + x2].g +
                 pImage->m_pImageData[y2 * pImage->width + x1].g +
                 pImage->m_pImageData[y2 * pImage->width + x2].g) / 4;

    avg_pix.b = (pImage->m_pImageData[y1 * pImage->width + x1].b +
                 pImage->m_pImageData[y1 * pImage->width + x2].b +
                 pImage->m_pImageData[y2 * pImage->width + x1].b +
                 pImage->m_pImageData[y2 * pImage->width + x2].b) / 4;

    return avg_pix;
}

int Bilinear(Image* pImageOut, Image* pImageIn, int widthTo, int heightTo)
{
    pImageOut->width  = widthTo;
    pImageOut->height = heightTo;
    pImageOut->m_pImageData = new Pixel [widthTo * heightTo];

    double ratio_width  = (double)pImageIn->width  / (double)pImageOut->width;
    double ratio_height = (double)pImageIn->height / (double)pImageOut->height;

    for (int yi = 0; yi < heightTo; yi++)
    {
        for (int xi = 0; xi < widthTo; xi++)
        {
            pImageOut->m_pImageData[yi * widthTo + xi] = GetAvgPixel(pImageIn, ratio_width * xi, ratio_height * yi);
        }
    }
    return 0;
}

Pixel GetBoxAvgPixel(Image* pImage, int x, int y, int box_size_x, int box_size_y)
{
    Pixel avg_pix;

    int x_min = x - box_size_x / 2;
    int x_max = x_min + box_size_x;

    int y_min = y - box_size_y / 2;
    int y_max = y_min + box_size_y;

    if (x_min < 0) x_min = 0;
    if (y_min < 0) y_min = 0;
    if (x_max >= pImage->width)  x_max = pImage->width;
    if (y_max >= pImage->height) y_max = pImage->height;

    long r = 0;
    long g = 0;
    long b = 0;

    int pixels_count = 0;
    for (int yi = y_min; yi < y_max; yi++)
    {
        for (int xi = x_min; xi < x_max; xi++)
        {
            r += pImage->m_pImageData[yi * pImage->width + xi].r;
            g += pImage->m_pImageData[yi * pImage->width + xi].g;
            b += pImage->m_pImageData[yi * pImage->width + xi].b;
            pixels_count++;
        }
    }

    avg_pix.r = r / pixels_count;
    avg_pix.g = g / pixels_count;
    avg_pix.b = b / pixels_count;

    return avg_pix;
}

int BoxSampling(Image* pImageOut, Image* pImageIn, int widthTo, int heightTo)
{
    pImageOut->width  = widthTo;
    pImageOut->height = heightTo;
    pImageOut->m_pImageData = new Pixel [widthTo * heightTo];

    double ratio_width  = (double)pImageIn->width  / (double)pImageOut->width;
    double ratio_height = (double)pImageIn->height / (double)pImageOut->height;

    int box_size_x = ratio_width  + 1;
    int box_size_y = ratio_height + 1;

    for (int yi = 0; yi < heightTo; yi++)
    {
        for (int xi = 0; xi < widthTo; xi++)
        {
            pImageOut->m_pImageData[yi * widthTo + xi] = GetBoxAvgPixel(pImageIn, ratio_width * xi, ratio_height * yi, box_size_x, box_size_y);
        }
    }
    return 0;
}

