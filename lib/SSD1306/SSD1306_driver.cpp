#include "SSD1306_driver.hpp"

Display::Display(uint8_t address, uint8_t sda, uint8_t scl) 
    : _address(address), _cursorX(0), _cursorY(0), _size(1), _sda(sda), _scl(scl)
{
    memset(_framebuffer, 0, sizeof(_framebuffer));
}

Display::~Display() {}

uint8_t Display::init()
{
	Wire.begin(_sda, _scl);
	Wire.beginTransmission(_address);
	Wire.write(SSD1306_CMD_STREAM);
	Wire.write(SSD1306_DISPLAY_OFF);
	Wire.write(SSD1306_SET_DISPLAY_CLOCK);
	Wire.write(0x80); // Recommended oscillator frequency
	Wire.write(SSD1306_SET_MULTIPLEX);
	Wire.write(0x3F); // 1/64 duty
	Wire.write(SSD1306_SET_DISPLAY_OFFSET);
	Wire.write(0x00);
	Wire.write(SSD1306_SET_START_LINE);
	Wire.write(SSD1306_CHARGE_PUMP);
	Wire.write(0x14);
	Wire.write(SSD1306_MEMORY_MODE);
	Wire.write(0x00);
	Wire.write(SSD1306_SEG_REMAP);
	Wire.write(SSD1306_COM_SCAN_DEC);
	Wire.write(SSD1306_SET_COM_PINS);
	Wire.write(0x12); // Alternative com pin configuration, required for 128x64
	Wire.write(SSD1306_SET_CONTRAST);
	Wire.write(0xCF); // Recommended contrast for 128x64
	Wire.write(SSD1306_SET_PRECHARGE);
	Wire.write(0xF1); // Recommended pre-charge period
	Wire.write(SSD1306_SET_VCOMDETECT);
	Wire.write(0x40); // Recommended vcomh deselect level
	Wire.write(SSD1306_DISPLAY_ALL_ON_RESUME);
	Wire.write(SSD1306_NORMAL_DISPLAY);
	Wire.write(SSD1306_DEACTIVATE_SCROLL);
	Wire.write(SSD1306_DISPLAY_ON);
	uint8_t error = Wire.endTransmission();
	return error;
}

void Display::display() 
{
	Wire.beginTransmission(_address);
	Wire.write(SSD1306_CMD_STREAM);
	Wire.write(SSD1306_SET_COLUMN_ADDRESS);
	Wire.write(0);// Start column
	Wire.write(SSD1306_WIDTH - 1);// End column
	Wire.write(SSD1306_SET_PAGE_ADDRESS);
	Wire.write(0);// Start page
	Wire.write(SSD1306_PAGES - 1);// End page
	Wire.endTransmission();
	for (size_t i = 0; i < sizeof(_framebuffer); i += 16)
	{
		Wire.beginTransmission(_address);
		Wire.write(SSD1306_DATA_STREAM);
		Wire.write(_framebuffer + i, 16);
		Wire.endTransmission();
	}
}

void Display::clear() 
{
	memset(_framebuffer, 0, sizeof(_framebuffer));
}

void Display::drawPixel(uint8_t x, uint8_t y, bool color) 
{
	if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
		return; // Out of bounds
	}
	uint16_t index = (y / 8) * SSD1306_WIDTH + x;
	if (color)
		_framebuffer[index] |= (uint8_t)(1 << (y % 8));
	else
		_framebuffer[index] &= ~(uint8_t)(1 << (y % 8));
}

void Display::drawChar(uint8_t x, uint8_t y, char c) 
{
	if (c < 32 || c > 126) return;

	for (uint8_t i = 0; i < 5; i++)
	{
		uint8_t line = font[c * 5 + i];
		for (uint8_t j = 0; j < 8; j++)
		{
			bool pixelOn = line & (1 << j);
			for (uint8_t sx = 0; sx < _size; sx++)
				for (uint8_t sy = 0; sy < _size; sy++)
					drawPixel(x + i * _size + sx, y + j * _size + sy, pixelOn);
		}
	}
}

void Display::print(const char* str) 
{
	uint8_t x = _cursorX;
	uint8_t y = _cursorY;

	while (*str) {
		drawChar(x, y, *str++);
		x += 6 * _size; // 5px + 1px kerning, scalés
		if (x + 5 * _size >= SSD1306_WIDTH) {
			x = 0;
			y += 8 * _size;
		}
		if (y + 7 * _size >= SSD1306_HEIGHT)
			break;
	}
}

void Display::setTextSize(uint8_t size) 
{
	_size = size;
}

void Display::setCursor(uint8_t x, uint8_t y) 
{
	_cursorX = x;
	_cursorY = y;
}