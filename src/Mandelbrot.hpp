#ifndef _TBDFRACTAL_MANDELBROT_HPP_
#define _TBDFRACTAL_MANDELBROT_HPP_

#include <cmath>
#include <complex>
#include "Fractal.hpp"

namespace TBDFRACTAL
{
    class Mandelbrot: public Fractal
    {
    public:
        
        using Fractal::Fractal;
        void z0(FractalIteration& context);
        void zNext(FractalIteration& context);
        void k0(FractalIteration& context);
        bool escape(FractalIteration& context);
        bool useLogHorizon() const;
        void useLogHorizon(bool b);
        double multiplicity() const;
        void multiplicity(double m);

    protected:
        bool _useLogHorizon = true;
        double _multiplicity = 2;
        constexpr static double _horizon = pow(2, 40),
                                _logHorizon = log(log(_horizon))/log(2);
    };
}

#endif
