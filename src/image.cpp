#include <image.hpp>

mdie::Image::Image(size_t width, size_t height, size_t pixelSize)
	: m_width(width), m_height(height), m_pixelSize(pixelSize) {
	assert(width != 0);
	assert(height != 0);
	assert(pixelSize != 0);

	m_pitch = mdie::align_up<size_t>(width, 256) * m_pixelSize;
	m_data = new uint8_t[m_height * m_pitch];
	std::memset(m_data, 0, m_height * m_pitch);
}

mdie::Image::Image(size_t width, size_t height, size_t pixelSize, uint8_t *data)
	: m_width(width), m_height(height), m_pixelSize(pixelSize) {
	assert(width != 0);
	assert(height != 0);
	assert(pixelSize != 0);
	assert(data != nullptr);

	m_pitch = mdie::align_up<size_t>(width, 256) * m_pixelSize;
	m_data = new uint8_t[m_height * m_pitch];

	size_t dataPitch = m_width * m_pixelSize;
	for (size_t i = 0; i < height; i++) {
		uint8_t *src = data + dataPitch * i;
		uint8_t *dst = m_data + m_pitch * i;
		std::memcpy(dst, src, dataPitch);
		std::memset(dst + dataPitch, 0, m_pitch - dataPitch);
	}
}

mdie::Image::Image(size_t width, size_t height, size_t pixelSize, uint8_t *data, size_t dataPitch)
	: m_width(width), m_height(height), m_pixelSize(pixelSize) {
	assert(width != 0);
	assert(height != 0);
	assert(pixelSize != 0);
	assert(data != nullptr);
	assert(dataPitch != 0);

	m_pitch = mdie::align_up<size_t>(m_width, 256) * m_pixelSize;
	assert(dataPitch <= m_pitch);
	m_data = new uint8_t[m_height * m_pitch];

	for (size_t i = 0; i < height; i++) {
		uint8_t *src = data + dataPitch * i;
		uint8_t *dst = m_data + m_pitch * i;
		std::memcpy(dst, src, dataPitch);
		std::memset(dst + dataPitch, 0, m_pitch - dataPitch);
	}
}
