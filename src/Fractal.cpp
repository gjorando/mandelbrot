#include "Fractal.hpp"

namespace TBDFRACTAL
{
    Fractal::Fractal(): Fractal(vec2d(-2, 2), vec2d(2, -2), vec2d(125, 125))
    {
    }

    Fractal::Fractal(vec2d min, vec2d max, vec2d pixelsPerUnit): _min(min), _max(max), _pixelsPerUnit(pixelsPerUnit)
    {
        reset();
    }

    Fractal::Fractal(double width, vec2d center, vec2<size_t> iSize): _min(center.x-width/2, center.y+(width*iSize.y/iSize.x)/2), _max(center.x+width/2, center.y-(width*iSize.y/iSize.x)/2)
    {
        imageSize(iSize);
        reset();
    }

    Fractal::~Fractal()
    {
    }

    void Fractal::reset()
    {
        vec2<size_t> iSize(imageSize());
        _kData = std::vector<double>(iSize.x*iSize.y, 0);
    }
    
    void Fractal::compute(unsigned int iterations)
    {
        vec2<size_t> iSize(imageSize());
        vec2d increment(unitsPerPixel());
        FractalIteration context(iterations);

        context.point.x = _min.x;
        for(size_t i = 0 ; i < iSize.x ; i++)
        {
            context.point.y = _max.y;
            for(size_t j = 0 ; j < iSize.y ; j++)
            {
                k0(context);

                z0(context);
                
                while(context.k < iterations)
                {
                    zNext(context);
                    
                    if(escape(context))
                        break;

                    context.k++;
                }

                _kData[j*iSize.x+i] = ((context.k==iterations)?(-1):(context.k/static_cast<double>(iterations)));
                context.point.y+= increment.y;
            }
            context.point.x+= increment.x;
        }
    }

    sf::Image Fractal::render(double gamma, const Gradient& grad)
    {
        vec2<size_t> iSize(imageSize());
        sf::Image output;
        output.create(iSize.x, iSize.y,  sf::Color::Transparent);

        std::function<bool (double, double)> minPositive = [](double a, double b) -> bool
        {
            return (b==-1) || (a!=-1) || (a<b);
        };
        double minK = *std::min_element(_kData.begin(), _kData.end(), minPositive);
        double maxK = *std::max_element(_kData.begin(), _kData.end());

        for(size_t i = 0 ; i < iSize.x ; i++)
            for(size_t j = 0 ; j < iSize.y ; j++)
            {
                
                size_t index = j*(iSize.x)+i;
                sf::Color pixelColor;
                if(_kData[index] == -1)
                    pixelColor = sf::Color::Black;
                else
                {
                    double gradientValue = 0.5;
                    if(maxK != minK)
                        gradientValue = (_kData[index]-minK)/(maxK-minK);
                    gradientValue = pow(gradientValue, gamma);
                    
                    // TEMP
                    pixelColor = grad.getPixelValue(gradientValue);
                }
                output.setPixel(i, j, pixelColor);
            }

        return output;
    }

    vec2d Fractal::plotSize() const
    {
        return vec2d(_max.x-_min.x, _min.y-_max.y);
    }

    void Fractal::plotSize(vec2d size)
    {
        vec2<size_t> iSize(imageSize());
        vec2d pSize(plotSize());
        vec2d center(_min.x+(pSize.x/2), _min.y-(pSize.y/2));
        _min.x = center.x-size.x/2.f;
        _min.y = center.y+size.y/2.f;
        _max.x = center.x+size.x/2.f;
        _max.y = center.y-size.y/2.f;
        imageSize(iSize);
        
        reset();
    }

    vec2<size_t> Fractal::imageSize() const
    {
        vec2d pSize(plotSize());
        return vec2<size_t>(std::abs(pSize.x)*_pixelsPerUnit.x, std::abs(pSize.y)*_pixelsPerUnit.y);
    }

    void Fractal::imageSize(vec2<size_t> size)
    {
        vec2d pSize(plotSize());
        _pixelsPerUnit.x = size.x/pSize.x;
        _pixelsPerUnit.y = size.y/pSize.y;

        reset();
    }

    vec2d Fractal::min() const
    {
        return _min;
    }

    void Fractal::min(vec2d min, bool keepImageSize)
    {
        vec2<size_t> iSize(imageSize());
        _min = min;
        if(keepImageSize)
            imageSize(iSize);

        reset();
    }

    vec2d Fractal::max() const
    {
        return _max;
    }

    void Fractal::max(vec2d max, bool keepImageSize)
    {
        vec2<size_t> iSize(imageSize());
        _max = max;
        if(keepImageSize)
            imageSize(iSize);

        reset();
    }

    vec2d Fractal::pixelsPerUnit() const
    {
        return _pixelsPerUnit;
    }

    void Fractal::pixelsPerUnit(vec2d pixelsPerUnit)
    {
        _pixelsPerUnit = pixelsPerUnit;

        reset();
    }

    vec2d Fractal::unitsPerPixel() const
    {
        vec2<size_t> iSize(imageSize());
        vec2d pSize(plotSize());
        return vec2d(pSize.x/iSize.x, pSize.y/iSize.y);
    }

    std::ostream& operator<<(std::ostream& os, const Fractal& f)
    {
        os << "Fractal("
           << "min(" << f._min.x << "," << f._min.y << "), "
           << "max(" << f._max.x << "," << f._max.y << "), "
           << "pixelsPerUnit(" << f._pixelsPerUnit.x << "," << f._pixelsPerUnit.y << "), "
           << ")"
        ;
        return os;
    }
}
