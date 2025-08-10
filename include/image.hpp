#ifndef MDIE_IMAGE_H
#define MDIE_IMAGE_H

#include <align.hpp>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <glad/glad.h>

namespace mdie {
struct rgba_8 {
	uint8_t r, g, b, a;
};

struct Image {
	Image(size_t width, size_t height, size_t pixelSize);
	Image(size_t width, size_t height, size_t pixelSize, uint8_t *data);
	Image(size_t width, size_t height, size_t pixelSize, uint8_t *data, size_t dataPitch);
	~Image() { delete[] m_data; }

	uint8_t *data() const { return m_data; }

	size_t &width() { return m_width; }
	size_t get_width() const { return m_width; }

	size_t &height() { return m_height; }
	size_t get_height() const { return m_height; }

	size_t &pitch() { return m_pitch; }
	size_t get_pitch() const { return m_pitch; }

	uint8_t *m_data;
	size_t m_width;
	size_t m_height;
	size_t m_pitch;
	size_t m_pixelSize;
};

class ImageView {
public:
	ImageView(Image *image) : m_image(image) {}

	uint8_t *get_pixel_data(size_t x, size_t y) {
		assert(x <= m_image->m_width);
		assert(y <= m_image->m_height);
		return &m_image->m_data[x * m_image->m_pixelSize + y * m_image->m_pitch];
	}

	template<typename t_pixel = rgba_8>
	t_pixel &get_pixel(size_t x, size_t y) {
		assert(x <= m_image->m_width);
		assert(y <= m_image->m_height);
		return *reinterpret_cast<t_pixel *>(&m_image->m_data[x * m_image->m_pixelSize + y * m_image->m_pitch]);
	}

	template<typename t_pixel = rgba_8>
	const t_pixel &get_pixel(size_t x, size_t y) const {
		assert(x <= m_image->m_width);
		assert(y <= m_image->m_height);
		return *reinterpret_cast<const t_pixel *>(&m_image->m_data[x * m_image->m_pixelSize + y * m_image->m_pitch]);
	}

private:
	Image *m_image;
};
}// namespace mdie

#endif// !MDIE_IMAGE_H
