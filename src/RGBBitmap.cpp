#include "RGBBitmap.hpp"

RGBBitmap::RGBBitmap(): m_width(0), m_height(0)
{
	m_map = NULL;
}

RGBBitmap::RGBBitmap(size_t w, size_t h): m_width(w), m_height(h)
{
	m_map = new RGBPixel[m_width*m_height];
}

RGBBitmap::RGBBitmap(size_t w, size_t h, uint32_t c): RGBBitmap(w, h)
{
	for(size_t i = 0 ; i < w ; i++)
		for(size_t j = 0 ; j < h ; j++)
			this->at(i, j).setPixel(c);
}

RGBBitmap::~RGBBitmap()
{
	delete[] m_map;
}

RGBPixel &RGBBitmap::at(size_t x, size_t y)
{
	return m_map[y*m_width+x]; //TODO CHECK OVERFLOW
}

size_t RGBBitmap::getWidth()
{
	return m_width;
}

size_t RGBBitmap::getHeight()
{
	return m_height;
}

// FURTHER IMPROVEMENTS: Get it to work with a fstream instead of odd FILE* C descriptors. Throw exceptions in case of error (I'm lazy).
void RGBBitmap::exportToPNG(const std::string &path)
{
	FILE *destination;
	png_structp pngData = NULL; // Wow libpng is so shitty
	png_infop infoData = NULL;
	png_byte **rows = NULL;

	// We open the file as a binary file
	if(!(destination = fopen(path.c_str(), "wb")))
	{
		std::cerr << "Unable to open " << path << "." << std::endl;
		return;
	}

	if(!(pngData = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)))
	{
		fclose(destination);

		std::cerr << "Unable to create png_struc structure." << std::endl;

		return;
	}

	if(!(infoData = png_create_info_struct(pngData)))
	{
		png_destroy_write_struct(&pngData, &infoData);
		fclose(destination);

		std::cerr << "Unable to create png_info structure." << std::endl;

		return;
	}

	// The fuck is this shit.
	if(setjmp(png_jmpbuf(pngData)))
	{
		png_destroy_write_struct(&pngData, &infoData);
		fclose(destination);

		std::cerr << "An error was encountered during png writing." << std::endl;

		return;
	}

	png_set_IHDR(pngData, infoData, m_width, m_height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	rows = (png_byte**) png_malloc(pngData, m_height*sizeof(png_byte*));

	for(size_t j = 0 ; j < m_height ; j++)
	{
		rows[j] = (png_byte*) png_malloc(pngData, 3*m_width*sizeof(uint8_t));
		for(size_t i = 0 ; i < m_width ; i++)
		{
			rows[j][3*i] = this->at(i, j).getComponent(RGBComponent::R);
			rows[j][3*i+1] = this->at(i, j).getComponent(RGBComponent::G);
			rows[j][3*i+2] = this->at(i, j).getComponent(RGBComponent::B);
		}
	}

	png_init_io(pngData, destination);
	png_set_rows(pngData, infoData, rows);
	png_write_png(pngData, infoData, PNG_TRANSFORM_IDENTITY, NULL);
	
	for(size_t j = 0 ; j < m_height ; j++)
		png_free(pngData, rows[j]);
	png_free(pngData, rows);
	png_destroy_write_struct(&pngData, &infoData);
	fclose(destination);
}
