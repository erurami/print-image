
#include "imageclass.hpp"

#include "shrink.hpp"

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

int ShrinkImage(Image* pImageOut, Image* pImageIn, int widthTo, int heightTo)
{
    pImageOut->width  = widthTo;
    pImageOut->height = heightTo;
    pImageOut->m_pImageData = new Pixel [widthTo * heightTo];

    double ratio_width  = pImageIn->width  / pImageOut->width;
    double ratio_height = pImageIn->height / pImageOut->height;
    for (int yi = 0; yi < heightTo; yi++)
    {
        for (int xi = 0; xi < widthTo; xi++)
        {
            pImageOut->m_pImageData[yi * widthTo + xi] = GetAvgPixel(pImageIn, ratio_width * xi, ratio_height * yi);
        }
    }

    return 0;
}

