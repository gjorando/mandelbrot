#include "Mandelbrot.hpp"

namespace TBDFRACTAL
{
    void Mandelbrot::z0(FractalIteration& context)
    {
        context.z = vec2d(0);
    }

    void Mandelbrot::zNext(FractalIteration& context)
    {
        if(_multiplicity - trunc(_multiplicity))
        {
            std::complex<double> zT(context.z.x, context.z.y);
            zT = std::pow(zT, _multiplicity);
            context.z.x = zT.real();
            context.z.y = zT.imag();
        }
        else
        {
            for(size_t i = 1 ; i < _multiplicity ; i++)
            {
                double prevZX = context.z.x;
                context.z.x = context.z.x*context.z.x - context.z.y*context.z.y;
                context.z.y = 2*prevZX*context.z.y;
            }
        }
        
        context.z.x+= context.point.x;
        context.z.y+= context.point.y;
    }

    void Mandelbrot::k0(FractalIteration& context)
    {
        context.k = 0;
        double q = (context.point.x-0.25)*(context.point.x-0.25)+context.point.y*context.point.y;
        if((_multiplicity == 2) && ((((context.point.x+1)*(context.point.x+1) + context.point.y*context.point.y) < 1.0f/16) || (q*(q+(context.point.x-0.25)) < 0.25*context.point.y*context.point.y)))
            context.k = context.iterations;
    }

    bool Mandelbrot::escape(FractalIteration& context)
    {
        double absZSquare = context.z.x*context.z.x + context.z.y*context.z.y;
        if(_useLogHorizon)
        {
            double absZ = sqrt(absZSquare);
            if(absZ > Mandelbrot::_horizon)
            {
                context.k-= log(log(absZ))/log(2) + Mandelbrot::_logHorizon;
                return true;
            }
            else
                return false;
        }
        else
            return absZSquare > 4;
    }

    bool Mandelbrot::useLogHorizon() const
    {
        return _useLogHorizon;
    }

    void Mandelbrot::useLogHorizon(bool b)
    {
        _useLogHorizon = b;
    }

    double Mandelbrot::multiplicity() const
    {
        return _multiplicity;
    }

    void Mandelbrot::multiplicity(double m)
    {
        _multiplicity = m;
    }
}
