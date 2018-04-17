#include "Julia.hpp"

namespace TBDFRACTAL
{
    void Julia::z0(FractalIteration& context)
    {
        context.z = context.point;
    }

    void Julia::zNext(FractalIteration& context)
    {
        Mandelbrot::zNext(context);

        context.z.x-= context.point.x;
        context.z.y-= context.point.y;

        context.z.x+= _c.x;
        context.z.y+= _c.y;
    }

    void Julia::k0(FractalIteration& context)
    {
        context.k = 0;
    }

    vec2d Julia::c() const
    {
        return _c;
    }

    void Julia::c(vec2d c)
    {
        _c = c;
    }
}
