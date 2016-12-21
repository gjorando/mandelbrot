#ifndef _MANDELBROT_RGBPIXEL_HPP_
#define _MANDELBROT_RGBPIXEL_HPP_

#include <cstdint>
#include <iostream>

enum RGBComponent{R, G, B};

class RGBPixel
{
public:

	/*! \brief Empty constructor.
	 */
	RGBPixel();
	/*! \brief Grayscale constructor.
	 *  \param v Value of the pixel.
	 */
	RGBPixel(uint32_t v);
	/*! \brief Default constructor.
	 *  \param r Red component.
	 *  \param g Green component.
	 *  \param b Blue component.
	 */
	RGBPixel(uint8_t r, uint8_t g, uint8_t b);
	/*! \brief Get the value of the pixel.
	 * 	\return A 32 bits value containing the pixel value as follows: 8 null bits, 8 red bits, 8 green bits, 8 blue bits.
	 */
	uint32_t getPixel();
	/*! \brief Set the value of a pixel.
	 *  \param v New value for the pixel.
	 */
	void setPixel(uint32_t v);
	/*! \brief Set the value of a pixel.
	 *  \param r New red component.
	 *  \param g New green component.
	 *  \param b New blue component.
	 */
	void setPixel(uint8_t r, uint8_t g, uint8_t b);
	/*! \brief Set the component of a pixel.
	 *  \param c Component to set.
	 *  \param v New value of the component.
	 */
	void setComponent(RGBComponent c, uint8_t v);
	/*! \brief Get the component of a pixel.
	 *  \param c Component to get.
	 *  \return The component.
	 */
	uint8_t getComponent(RGBComponent c);
	/*! \brief Output stream operator overloading.
	 */
	friend std::ostream &operator<<(std::ostream &stream, const RGBPixel &p);
	
private:

	uint8_t m_r, //!< Red component.
			m_g, //!< Green component.
			m_b; //!< Blue component.
};

#endif //_MANDELBROT_RGBPIXEL_HPP_
