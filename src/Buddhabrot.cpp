#include "Mandelbrot.hpp"

Mandelbrot::Mandelbrot():Mandelbrot(vec2<size_t>(500, 500), 4, vec2d(0, 0), 2)
{}

Mandelbrot::Mandelbrot(vec2<size_t> resSize, double plotWidth, vec2d center, double multiplicity): m_pixels(resSize.x, resSize.y), m_multiplicity(multiplicity)
{
	m_result = new double[resSize.x*resSize.y];
	for(size_t i = 0 ; i < resSize.x*resSize.y ; i++) m_result[i] = 0;
	m_xmin = (-plotWidth/2) + center.x;
	m_ymin = (-(plotWidth*resSize.y/resSize.x)/2) - center.y;
	m_dIt = plotWidth/resSize.x;
}

Mandelbrot::~Mandelbrot()
{
	delete[] m_result;
}

void Mandelbrot::compute(unsigned int maxIterations, bool antiAliasing)
{
	size_t width = m_pixels.getWidth(),
		   height = m_pixels.getHeight();
	size_t step = 2;

	double horizon = pow(2, 40),
		   logarithmicHorizon = log(log(horizon))/log(2);

	// For each pixel of the complex plot...	
	double xi = m_xmin;
	for(size_t i = 0 ; i < width ; i+=step)
	{
		double yi = m_ymin;
		for(size_t j = 0 ; j < height ; j+=step)
		{
			// We compute it.

			double k = 0;
			double a = 0;
			double b = 0; 

			// We could have used std::complex, but It appears to be waaay slower.
		
			//Optimisation to skip the cardioid and main bulb for the standard mandelbrot set (m_multiplicity = 2)	
			double q = (xi-0.25)*(xi-0.25)+yi*yi;
			if((m_multiplicity == 2) && ((((xi+1)*(xi+1) + yi*yi) < 1.0f/16) || (q*(q+(xi-0.25)) < 0.25*yi*yi)))
			{
				k = maxIterations;
			}
			else
			{
				bool inSet = true;
				std::vector<vec2d> points;
				while(k < maxIterations)
				{
					points.push_back(vec2d(a, b));
					
					if(m_multiplicity == 2) // If the multiplicity is 2, we do (z)^2 + c = (a+b*i)^2 + (xi+i*yi) = a^2 - b^2 + xi + 2*a*b*yi*i
					{
						double prevA = a;
						a = a*a - b*b + xi;
						b = 2*prevA*b + yi;
					}
					else //Otherwise we use a temporary complex for convenience. Just keep in mind that it's very slow.
					{
						std::complex<double> zT(a, b);
						zT = std::pow(zT, m_multiplicity);
						a = zT.real() + xi;
						b = zT.imag() + yi;
					}

					if(!antiAliasing)
					{
						if(a*a + b*b > 4) // If |z| > 2 <=> sqrt(a^2+b^2) > 2 <=> a^2 + b^2 > 4, the point isn't a point of the mandelbrot set.
						{
							inSet = false;
							break;
						}
					}
					else
					{
						double absZ = sqrt(a*a+b*b);
						if(absZ > horizon) // If |z| > horizon, c isn't a point of the mandelbrot set.
						{
							inSet = false;
							k = k - log(log(absZ))/log(2) + logarithmicHorizon; // Much mathematic, such logarithm wow
							break;
						}
					}

					k++;
				}

				if(!inSet)
				{
					for(size_t p = 0 ; p < points.size() ; p++)
					{
						//std::cout << points[p].x << ";" << points[p].y << std::endl;
						size_t aa = (size_t)((points[p].x - m_xmin)/m_dIt),
							   bb = (size_t)((points[p].y - m_ymin)/m_dIt);
						if((bb < height) && (aa < width))
							m_result[aa*width+bb]++; //We increment the resulting point a+bi, rotated by 90°
					}
				}
				points.clear();
			}

			yi+= step*m_dIt;
		}
		xi+= step*m_dIt;
	}
}
	
void Mandelbrot::render(double gamma, RGBGradient &gradient)
{
	size_t width = m_pixels.getWidth(),
		   height = m_pixels.getHeight();
	
	double minK = 1;
	double maxK = 0;

	/*
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
	}//*/
	
	for(size_t i = 0 ; i < width ; i++)
	{
		for(size_t j = 0 ; j < height ; j++)
		{
			if(m_result[j*width+i] == 0)
				m_pixels.at(i, j).setPixel(0x000000);
			else
			{
				double k = m_result[j*width+i]; /*
				if(maxK != minK)
					k = (k-minK)/(maxK-minK); // We spread k from 0 to 1
				else
					k = 0.5;
				k = pow(k, gamma); // We apply gamma
				
				m_pixels.at(i, j).setPixel(gradient.getPixelValue(1-k)); // 1-k because the gradient goes from the left (nearer) to the right (farer)//*/

				m_pixels.at(i, j).setComponent(RGBComponent::R, k*4);
				m_pixels.at(i, j).setComponent(RGBComponent::G, k*4);
				m_pixels.at(i, j).setComponent(RGBComponent::B, k*4);
			}
		}
	}

}

RGBBitmap &Mandelbrot::getResult()
{
	return m_pixels;
}
