#ifndef SSD1306_DRIVER_HPP
#define SSD1306_DRIVER_HPP

#include <Wire.h>
#include "font5x7.h"

/* ── SSD1306 Commands (datasheet section 9) ──────────────── */
#define SSD1306_DISPLAY_OFF					0xAE
#define SSD1306_DISPLAY_ON					0xAF
#define SSD1306_SET_MULTIPLEX				0xA8
#define SSD1306_SET_DISPLAY_OFFSET			0xD3
#define SSD1306_SET_START_LINE				0x40
#define SSD1306_SEG_REMAP					0xA1
#define SSD1306_COM_SCAN_DEC				0xC8
#define SSD1306_SET_COM_PINS				0xDA
#define SSD1306_SET_CONTRAST				0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME		0xA4
#define SSD1306_NORMAL_DISPLAY				0xA6
#define SSD1306_SET_DISPLAY_CLOCK			0xD5
#define SSD1306_CHARGE_PUMP					0x8D
#define SSD1306_CMD_STREAM					0x00  ///< Control byte: command stream
#define SSD1306_DATA_STREAM					0x40  ///< Control byte: data stream
#define SSD1306_SET_PAGE_ADDRESS			0x22
#define SSD1306_SET_COLUMN_ADDRESS			0x21
#define SSD1306_SET_PRECHARGE				0xD9
#define SSD1306_SET_VCOMDETECT				0xDB
#define SSD1306_DEACTIVATE_SCROLL			0x2E
#define SSD1306_MEMORY_MODE					0x20

/* ── Display dimensions ──────────────────────────────────── */
#define SSD1306_WIDTH	128	 ///< Display width in pixels
#define SSD1306_HEIGHT	64	 ///< Display height in pixels
#define SSD1306_PAGES	8	 ///< Number of pages (HEIGHT / 8)

/**
 * @brief I²C driver for SSD1306 128x64 OLED display.
 *
 * Sends commands and pixel data directly over I²C without any
 * third-party library. Maintains a local framebuffer that is
 * flushed to the GDDRAM on every call to display().
 *
 * @note All drawing operations modify the framebuffer only.
 *	 	Call display() to push changes to the screen.
 */
class Display
{
public:

	/**
	 * @brief  Construct a Display object.
	 * @param  address  I²C address of the display (0x3C or 0x3D).
	 * @param  sda	  SDA pin number (default: 21).
	 * @param  scl	  SCL pin number (default: 22).
	 */
	Display(uint8_t address, uint8_t sda = 21, uint8_t scl = 22);

	~Display();

	/**
	 * @brief  Initialize the I²C bus and configure the SSD1306.
	 * @note   Must be called once in setup(), after Wire is ready.
	 * @return 0 on success, I²C error code otherwise.
	 */
	uint8_t init();

	/**
	 * @brief  Send the framebuffer to the SSD1306 GDDRAM.
	 * @note   Must be called after any drawing operation to update the screen.
	 */
	void display();

	/**
	 * @brief  Clear the local framebuffer.
	 * @note   Call display() afterwards to apply to the screen.
	 */
	void clear();

	/**
	 * @brief  Set a single pixel in the framebuffer.
	 * @param  x	  Column (0–127).
	 * @param  y	  Row (0–63).
	 * @param  color  true = on, false = off.
	 */
	void drawPixel(uint8_t x, uint8_t y, bool color);

	/**
	 * @brief  Draw a single ASCII character at the given position.
	 * @param  x  Column of the top-left corner.
	 * @param  y  Row of the top-left corner.
	 * @param  c  ASCII character (32–126).
	 */
	void drawChar(uint8_t x, uint8_t y, char c);

	/**
	 * @brief  Print a null-terminated string starting at the current cursor.
	 * @param  str  String to display.
	 * @note   Automatically wraps to the next line when reaching the right edge.
	 *	 Stops rendering if the text exceeds the display height.
	 */
	void print(const char* str);

	/**
	 * @brief  Set the text scale factor.
	 * @param  size  Scale multiplier (1 = 5x7 px, 2 = 10x14 px, ...).
	 */
	void setTextSize(uint8_t size);

	/**
	 * @brief  Set the cursor position for the next print() call.
	 * @param  x  Column (0–127).
	 * @param  y  Row (0–63).
	 */
	void setCursor(uint8_t x, uint8_t y);

private:
	uint8_t _address;								///< I²C address
	uint8_t _framebuffer[SSD1306_PAGES * SSD1306_WIDTH]; ///< Local framebuffer
	uint8_t _cursorX;								///< Current cursor X position
	uint8_t _cursorY;								///< Current cursor Y position
	uint8_t _size;									///< Text scale factor
	uint8_t _sda;									///< SDA pin
	uint8_t _scl;									///< SCL pin
};

#endif /* SSD1306_DRIVER_HPP */