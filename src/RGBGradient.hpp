#ifndef _MANDELBROT_RGBGRADIENT_HPP_
#define _MANDELBROT_RGBGRADIENT_HPP_

#include "RGBBitmap.hpp"
#include <string>

class RGBGradient: public RGBBitmap
{
public:
	
	/*! \brief Empty constructor.
	 *  
	 *  Creates a default grayscale gradient.
	 */
	RGBGradient();
	/*! \brief Default constructor.
	 *  \param path Path of the gradient to import.
	 *
	 *  This constructor makes a gradient based on an image. Only the first line of the png will be taken into account. The size of the image should be 255 pixels wide.
	 */
	RGBGradient(const std::string &path);
	/*! \brief Gets the pixel value corresponding to a percentage.
	 *  \param v Value to get (should be between 0 and 1).
	 *  \return The pixel.
	 */
	uint32_t getPixelValue(double v);
	
};

#endif //_MANDELBROT_RGBGRADIENT_HPP_
