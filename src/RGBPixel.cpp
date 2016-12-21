#include "RGBPixel.hpp"

RGBPixel::RGBPixel(): m_r(0), m_g(0), m_b(0)
{}

RGBPixel::RGBPixel(uint32_t v): m_r(v >> 16), m_g(v >> 8), m_b(v)
{}

RGBPixel::RGBPixel(uint8_t r, uint8_t g, uint8_t b): m_r(r), m_g(g), m_b(b)
{}

uint32_t RGBPixel::getPixel()
{
	return (uint32_t)((m_r << 16) + (m_g << 8) + m_b);
}

void RGBPixel::setPixel(uint32_t v)
{
	m_r = v >> 16;
	m_g = v >> 8;
	m_b = v;
}

void RGBPixel::setPixel(uint8_t r, uint8_t g, uint8_t b)
{
	m_r = r;
	m_g = g;
	m_b = b;
}

void RGBPixel::setComponent(RGBComponent c, uint8_t v)
{
	switch(c)
	{
		case RGBComponent::R:
			m_r = v;
			break;
		case RGBComponent::G:
			m_g = v;
			break;
		case RGBComponent::B:
			m_b = v;
			break;
	}
}

uint8_t RGBPixel::getComponent(RGBComponent c)
{
	switch(c)
	{
		case RGBComponent::R:
			return m_r;
			break;
		case RGBComponent::G:
			return m_g;
			break;
		case RGBComponent::B:
			return m_b;
			break;
	}
	return 0;
}

std::ostream &operator<<(std::ostream &stream, const RGBPixel &p)
{
	stream << std::showbase << std::hex << (uint32_t)((p.m_r << 16) + (p.m_g << 8) + p.m_b) << std::dec << std::noshowbase;
	return stream;
}
