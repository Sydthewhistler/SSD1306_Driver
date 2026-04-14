#include <Arduino.h>
#include "SSD1306_driver.hpp"

Display myDisplay(0x3C);

void setup()
{
	Serial.begin(115200);
	delay(2000);
	myDisplay.init();
}

void loop()
{
	myDisplay.setTextSize(2);
	myDisplay.setCursor(0, 0);
	myDisplay.clear();
	myDisplay.print("Hello, World!");
	myDisplay.display();
	delay(1000);

	myDisplay.setCursor(0, 40);
	myDisplay.setTextSize(1);
	myDisplay.print("ESP32 + SSD1306");
	myDisplay.display();
	delay(1000);

	myDisplay.clear();
	myDisplay.display();
	delay(1000);
}