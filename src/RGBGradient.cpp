#include "RGBGradient.hpp"
RGBGradient::RGBGradient(): RGBBitmap(255, 1)
{
	for(size_t i = 0 ; i < 255 ; i++)
	{
		this->at(i, 0).setComponent(RGBComponent::R, 255-(i));
		this->at(i, 0).setComponent(RGBComponent::G, 255-(i));
		this->at(i, 0).setComponent(RGBComponent::B, 255-(i));
	}
}

RGBGradient::RGBGradient(const std::string &path)
{
	FILE *import;
	png_structp pngData = NULL;
	png_infop infoData = NULL;
	png_byte **rows = NULL; // (We will only read the first row.)
	png_uint_32 width, height;
   	int depth, cType;
	unsigned int increment, offset, sigRead = 0;
	
	// We open the file as a binary file
	if(!(import = fopen(path.c_str(), "rb")))
	{
		std::cerr << "Unable to open " << path << "." << std::endl;
		return;
	}

	if(!(pngData = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)))
	{
		fclose(import);

		std::cerr << "Unable to create png_struc structure." << std::endl;

		return;
	}

	if(!(infoData = png_create_info_struct(pngData)))
	{
		png_destroy_read_struct(&pngData, &infoData, NULL);
		fclose(import);

		std::cerr << "Unable to create png_info structure." << std::endl;

		return;
	}
	if(setjmp(png_jmpbuf(pngData)))
	{
		png_destroy_read_struct(&pngData, &infoData, NULL);
		fclose(import);

		std::cerr << "An error was encountered during png reading." << std::endl;

		return;
	}
	
	png_init_io(pngData, import);
	png_set_sig_bytes(pngData, sigRead);
	png_read_png(pngData, infoData, PNG_TRANSFORM_IDENTITY, NULL);
	png_get_IHDR(pngData, infoData, &width, &height, &depth, &cType, NULL, NULL, NULL);

	if(depth!= 8) // We only support 8 bits depth PNG files
	{
		png_destroy_read_struct(&pngData, &infoData, NULL);
		fclose(import);

		std::cerr << "Please use a 8 bits depth PNG." << std::endl;

		return;
	}

	switch(cType) // We set the size of a full pixel.
	{
		case 0: //greyscale PNG
			increment = 1;
			offset = 0;
			break;
		case 2: //RGB PNG
			increment = 3;
			offset = 1;
			break;
		case 4: //greyscale alpha PNG
			increment = 2;
			offset = 0;
			break;
		case 6: //RGBA PNG
			increment = 4;
			offset = 1;
			break;
		default:
			png_destroy_read_struct(&pngData, &infoData, NULL);
			fclose(import);

			std::cerr << "Indexed colors aren't supported." << std::endl;

			return;
	}

	/*
	if(width != 255) // the image must be 255 pixels wide
	{
		png_destroy_read_struct(&pngData, &infoData, NULL);
		fclose(import);

		std::cerr << "Please use a 255 pixels wide PNG." << std::endl;

		return;
	}
	*/

	// We can now initialize our pixel map
	m_width = width;
	m_height = 1;
	m_map = new RGBPixel[m_width];

	rows = png_get_rows(pngData, infoData);
	for(size_t i = 0 ; i < width ; i++)
	{
		this->at(i, 0).setComponent(RGBComponent::R, rows[0][increment*i]);
		this->at(i, 0).setComponent(RGBComponent::G, rows[0][increment*i+offset]);
		this->at(i, 0).setComponent(RGBComponent::B, rows[0][increment*i+2*offset]);
	}

	png_destroy_read_struct(&pngData, &infoData, NULL);
	fclose(import);
}

uint32_t RGBGradient::getPixelValue(double v)
{
	if(v < 0) v = 0;
	if(v > 1) v = 1;

	return this->at((size_t)(v*(m_width-1)), 0).getPixel();
}
