
#pragma once


struct Pixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct Image
{
    long width;
    long height;

    Pixel* m_pImageData;

    ~Image();
};


