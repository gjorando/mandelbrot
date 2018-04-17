#include "Gradient.hpp"

namespace TBDFRACTAL
{
    void Gradient::create()
    {
        sf::Image::create(255, 1);
        for(size_t i = 0 ; i < 255 ; i++)
            setPixel(i, 0, sf::Color(i, i, i));
    }

    sf::Color Gradient::getPixelValue(double v) const
    {
        if(v < 0) v = 0;
        if(v > 1) v = 1;

        return getPixel(static_cast<unsigned int>(v*(getSize().x-1)), 0);
    }
}
