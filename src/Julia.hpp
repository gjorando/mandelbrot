#ifndef _TBDFRACTAL_JULIA_HPP_
#define _TBDFRACTAL_JULIA_HPP_

#include "Mandelbrot.hpp"

namespace TBDFRACTAL
{

    class Julia: public Mandelbrot
    {
    public:

        using Mandelbrot::Mandelbrot;
        void z0(FractalIteration& context);
        void zNext(FractalIteration& context);
        void k0(FractalIteration& context);
        vec2d c() const;
        void c(vec2d c);

    protected:
        vec2d _c;
    };

}

#endif
