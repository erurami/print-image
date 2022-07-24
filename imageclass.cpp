
#include "imageclass.hpp"

Image::~Image()
{
    if (m_pImageData)
    {
        delete m_pImageData;
    }
}

