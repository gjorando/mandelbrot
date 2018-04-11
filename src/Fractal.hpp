#ifndef _TBDFRACTAL_FRACTAL_HPP_
#define _TBDFRACTAL_FRACTAL_HPP_

#include <iostream>
#include <algorithm>
#include <functional>
#include <cstddef>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Gradient.hpp"

namespace TBDFRACTAL
{
    template<typename T>
    struct vec2
    {
        T x,
          y;

        vec2(): x(0), y(0) {}
        vec2(T xx, T yy): x(xx), y(yy) {}
        vec2(T v): x(v), y(v) {}
        vec2(const vec2& v): x(v.x), y(v.y) {}
    };

    typedef vec2<double> vec2d;

    struct FractalIteration
    {
        double k;
        vec2d point;
        vec2d z;
        const unsigned int iterations;
        FractalIteration(unsigned int i): iterations(i) {}
    };

    class Fractal
    {
    public:

        Fractal();
        Fractal(vec2d min, vec2d max, vec2d pixelsPerUnit);
        Fractal(double width, vec2d center, vec2<size_t> iSize);
        virtual ~Fractal();
        virtual void reset();
        virtual void compute(unsigned int iterations);
        virtual void z0(FractalIteration& context) = 0;
        virtual void zNext(FractalIteration& context) = 0;
        virtual void k0(FractalIteration& context) = 0;
        virtual bool escape(FractalIteration& context) = 0;
        virtual sf::Image render(double gamma, const Gradient& grad);
        vec2d plotSize() const;
        void plotSize(vec2d size);
        vec2<size_t> imageSize() const;
        void imageSize(vec2<size_t> size);
        vec2d min() const;
        void min(vec2d min, bool keepImageSize = false);
        vec2d max() const;
        void max(vec2d max, bool keepImageSize = false);
        vec2d pixelsPerUnit() const;
        void pixelsPerUnit(vec2d pixelsPerUnit);
        vec2d unitsPerPixel() const;
        friend std::ostream& operator<<(std::ostream& os, const Fractal& f);

    protected:
        std::vector<double> _kData; //!< For each point, stores the number of iterations at which it escaped.
        vec2d _min; //!< Upper left point of the complex plot we want to compute.
        vec2d _max; //!< Lower right point of the complex plane we want to compute.
        vec2d _pixelsPerUnit; //!< Number of pixels per unit on the complex plane. It represents horizontal and vertical resolution (if they're different the output picture will be deformed).
    };
}

#endif
