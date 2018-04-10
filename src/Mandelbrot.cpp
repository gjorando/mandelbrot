#include "Mandelbrot.hpp"

Mandelbrot::Mandelbrot():Mandelbrot(vec2<size_t>(500, 500), 4, vec2d(0, 0), 2)
{}

Mandelbrot::Mandelbrot(vec2<size_t> resSize, double plotWidth, vec2d center, double multiplicity): m_pixels(resSize.x, resSize.y), m_multiplicity(multiplicity)
{
	m_result = new double[resSize.x*resSize.y];
	m_xmin = (-plotWidth/2) + center.x;
	m_ymin = (-(plotWidth*resSize.y/resSize.x)/2) - center.y;
	m_dIt = plotWidth/resSize.x;
}

Mandelbrot::~Mandelbrot()
{
	delete[] m_result;
}

void Mandelbrot::compute(unsigned int maxIterations, bool antiAliasing, bool julia, vec2d juliaC)
{
	size_t width = m_pixels.getWidth(),
		   height = m_pixels.getHeight();

	double horizon = pow(2, 40),
		   logarithmicHorizon = log(log(horizon))/log(2);

	// Used to display progression
	//unsigned int progress = 0,
	//			 progressMax = width*height,
	//			 progressStep = progressMax/100;

	// For each pixel of the complex plot...	
	double xi = m_xmin;
	for(size_t i = 0 ; i < width ; i++)
	{
		double yi = m_ymin;
		for(size_t j = 0 ; j < height ; j++)
		{
			// We compute it.

			double k = 0;
			double a = 0;
			double b = 0;

			if(julia)
			{
				a = xi;
				b = yi;
			}

			// We could have used std::complex, but It appears to be waaay slower.
		
			//Optimisation to skip the cardioid and main bulb for the standard mandelbrot set (m_multiplicity = 2)	
			double q = (xi-0.25)*(xi-0.25)+yi*yi;
			if((!julia) && (m_multiplicity == 2) && ((((xi+1)*(xi+1) + yi*yi) < 1.0f/16) || (q*(q+(xi-0.25)) < 0.25*yi*yi)))
			{
				k = maxIterations;
			}
			else
			{
				while(k < maxIterations)
				{
					if(m_multiplicity == 2) // If the multiplicity is 2, we do (z)^2 + c = (a+b*i)^2 + (xi+i*yi) = a^2 - b^2 + xi + 2*a*b*yi*i
					{
						double prevA = a;
						a = a*a - b*b;
						b = 2*prevA*b;
					}
					else //Otherwise we use a temporary complex for convenience. Just keep in mind that it's very slow.
					{
						std::complex<double> zT(a, b);
						zT = std::pow(zT, m_multiplicity);
						a = zT.real() + xi;
						b = zT.imag() + yi;
					}

					if(julia)
					{
						a+= juliaC.x;
						b+= juliaC.y;
					}
					else
					{
						a+= xi;
						b+= yi;
					}

					if(!antiAliasing)
					{
						if(a*a + b*b > 4) // If |z| > 2 <=> sqrt(a^2+b^2) > 2 <=> a^2 + b^2 > 4, the point isn't a point of the mandelbrot set.
						{
							break;
						}
					}
					else
					{
						double absZ = sqrt(a*a+b*b);
						if(absZ > horizon) // If |z| > horizon, c isn't a point of the mandelbrot set.
						{
							k = k - log(log(absZ))/log(2) + logarithmicHorizon; // Much mathematic, such logarithm wow
							break;
						}
					}

					k++;
				}
			}
			

			if(k == maxIterations) //If k == maxIterations then c is a point of the mandelbrot set.
				m_result[j*width+i] = -1;
			else
				m_result[j*width+i] = k/(double)maxIterations;

			//progress++;
			//if(progress%progressStep == 0)
			//	std::cout << "pixel " << progress << " out of " << progressMax << std::endl;
			
			yi+= m_dIt;
		}
		xi+= m_dIt;
	}
}
	
void Mandelbrot::render(double gamma, RGBGradient &gradient)
{
	size_t width = m_pixels.getWidth(),
		   height = m_pixels.getHeight();
	
	double minK = 1;
	double maxK = 0;


	for(size_t i = 0 ; i < width ; i++)
	{
		for(size_t j = 0 ; j < height ; j++)
		{
			double act = m_result[j*width+i];
			if(act != -1)
			{
				if(act > maxK) maxK = act;
				if(act < minK) minK = act;
			}
		}
	}
	
	for(size_t i = 0 ; i < width ; i++)
	{
		for(size_t j = 0 ; j < height ; j++)
		{
			if(m_result[j*width+i] == -1)
				m_pixels.at(i, j).setPixel(0x000000);
			else
			{
				double k = m_result[j*width+i];
				if(maxK != minK)
					k = (k-minK)/(maxK-minK); // We spread k from 0 to 1
				else
					k = 0.5;
				k = pow(k, gamma); // We apply gamma
				
				m_pixels.at(i, j).setPixel(gradient.getPixelValue(1-k)); // 1-k because the gradient goes from the left (nearer) to the right (farer)
			}
		}
	}

}

RGBBitmap &Mandelbrot::getResult()
{
	return m_pixels;
}
