

#include <stdio.h>

#include "imageclass.hpp"
#include "bmp.hpp"



int power(int base, int power)
{
    int retval = 1;
    for (int i = 0; i < power; i++)
    {
        retval *= base;
    }
    return retval;
}

long getImageSizeOfBitmap(unsigned char* bitmap_header)
{
    long file_size = 0;
    for (int i = 0x22; i < 0x26; i++)
    {
        file_size += bitmap_header[i] * power(256, (i - 0x22));
    }
    return file_size;
}

long getWidthOfBitmap(unsigned char* bitmap_header)
{
    long width = 0;
    for (int i = 0x12; i < 0x16; i++)
    {
        width += bitmap_header[i] * power(256, (i - 0x12));
    }
    return width;
}

long getHeightOfBitmap(unsigned char* bitmap_header)
{
    long height = 0;
    for (int i = 0x16; i < 0x1a; i++)
    {
        height += bitmap_header[i] * power(256, (i - 0x16));
    }
    return height;
}

int LoadBMPFile(Image* pImage, char* fileName)
{
    unsigned char bitmap_header[54];

    FILE* image_file;
    image_file = fopen(fileName, "rb");

    if (image_file == NULL)
    {
        printf("couldn't open the file");
        return 1;
    }

    fread(bitmap_header, sizeof(bitmap_header), 1, image_file);


    int image_data_length = getImageSizeOfBitmap(bitmap_header);

    int image_width = getWidthOfBitmap(bitmap_header);
    int image_height = getHeightOfBitmap(bitmap_header);

    int data_length_per_row = image_data_length / image_height;

    unsigned char* bitmap_main_data = new unsigned char [image_data_length];
    fread(bitmap_main_data, image_data_length, 1, image_file);

    int seeking_data_index = 0;


    Pixel pixel;

    pImage->width  = image_width;
    pImage->height = image_height;
    pImage->m_pImageData = new Pixel [image_width * image_height];


    for (int i = image_height - 1; i >= 0; i--)
    {
        seeking_data_index = data_length_per_row * (image_height - i - 1);
        for (int j = 0; j < image_width; j++)
        {
            pixel.b = bitmap_main_data[seeking_data_index++];
            pixel.g = bitmap_main_data[seeking_data_index++];
            pixel.r = bitmap_main_data[seeking_data_index++];
            pImage->m_pImageData[i * image_width + j] = pixel;
        }
    }

    return 0;
}


