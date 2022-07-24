
#include <stdio.h>

#include "bmp.hpp"
#include "shrink.hpp"

#if _WIN32
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(time) usleep(time * 1000)
#endif

#include <time.h>

typedef struct _ConsoleSize
{
    int width;
    int height;
} CONSOLESIZE;

#if _WIN32
void GetConsoleSize(CONSOLESIZE *pConsize)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    pConsize->width  = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    pConsize->height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}
#else
#include <sys/ioctl.h>
void GetConsoleSize(CONSOLESIZE *pConsize)
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    pConsize->width  = w.ws_col;
    pConsize->height = w.ws_row;
}
#endif

int calcFitSize(int* image_width, int* image_height, int scr_width, int scr_height)
{
    double ri = *image_width / *image_height;
    double rs = scr_width   / scr_height;

    if (ri > rs)
    {
	*image_height = scr_width * (*image_height) / (*image_width); 
	*image_width = scr_width;
    }
    else
    {
	*image_width = scr_height * (*image_width) / (*image_height);
	*image_height = scr_height; 
    }
    return 0;
}

int main(int argc, char* argv[])
{
    Image image_org;
    Image image;

    if (argc == 1)
    {
        printf("need input file name\n");
        return 1;
    }

    if (LoadBMPFile(&image_org, argv[1]))
    {
        printf("failed to load bitmap\n");
        return 1;
    }

    CONSOLESIZE console_size;
    GetConsoleSize(&console_size);

    int console_max_width  = console_size.width / 2;
    int console_max_height = console_size.height;
    int image_width  = image_org.width;
    int image_height = image_org.height;

    calcFitSize(&image_width, &image_height, console_max_width, console_max_height);

    ShrinkImage(&image, &image_org, image_width, image_height);

    for (int yi = 0; yi < image.height; yi++)
    {
        for (int xi = 0; xi < image.width; xi++)
        {
            printf("\033[48;2;%d;%d;%dm  \033[m",
                    image.m_pImageData[yi * image.width + xi].r,
                    image.m_pImageData[yi * image.width + xi].g,
                    image.m_pImageData[yi * image.width + xi].b);
        }
        printf("\n");
    }
    return 0;
}

