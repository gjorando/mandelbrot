#ifndef _TBDFRACTAL_GRADIENT_HPP_
#define _TBDFRACTAL_GRADIENT_HPP_

#include <cstddef>
#include <SFML/Graphics.hpp>

namespace TBDFRACTAL
{
    class Gradient: public sf::Image
    {
    public:

        void create();
        sf::Color getPixelValue(double v) const;
    };
}

#endif
