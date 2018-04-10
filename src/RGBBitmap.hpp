#ifndef _MANDELBROT_RGBBITMAP_HPP_
#define _MANDELBROT_RGBBITMAP_HPP_

#include "RGBPixel.hpp"
#include <string>
#include <cstdlib>
#include <png.h>

class RGBBitmap
{
public:

	/*! \brief Empty constructor. It doesn't initialize our members, and it should only be used by derived classes who don't want to use the default constructor.
	 */
	RGBBitmap();
	/*! \brief Default constructor.
	 *  \param w Width of the bitmap.
	 *  \param h Height of the bitmap
	 */
	RGBBitmap(size_t w, size_t h);
	/*! \brief Builds a bitmap with a custom color.
	 *  \param w Width of the bitmap.
	 *  \param h Height of the bitmap.
	 *  \param c Color.
	 */
	RGBBitmap(size_t w, size_t h, uint32_t c);
	/*! \brief Destructor.
	 */
	virtual ~RGBBitmap();
	/*! \brief Accessor of a given pixel of m_map.
	 *  \param x Abscissa of the pixel.
	 *  \param y Ordinate of the pixel.
	 *  \return A reference to the RGBPixel.
	 */
	RGBPixel &at(size_t x, size_t y);
	/*! \brief Accessor of m_width.
	 *  \return m_width.
	 */
	size_t getWidth();
	/*! \brief Accessor of m_height.
	 *  \return m_height.
	 */
	size_t getHeight();
	/*! \brief Exports the bitmap as a png in the given path.
	 *  \param path Path of the image.
	 */
	void exportToPNG(const std::string &path);

protected:
	
	size_t m_width, //!< Width of the bitmap.
		   m_height; //!< Height of the bitmap.
	RGBPixel *m_map;
};

#endif //_MANDELBROT_RGBBITMAP_HPP_
