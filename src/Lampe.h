//
// Created by Mankianer on 10.07.2023.
//

#ifndef LAMPE_LAMPE_H
#define LAMPE_LAMPE_H

class BaseModus;

#include <stdint.h>
#include "BLESetUp.h"
#include <Adafruit_NeoPixel.h>
#define LAMPE_SCREENS 4

class Lampe {
public:
    Lampe(int16_t pin = 6, int16_t numPixels = 64 * 4);
    ~Lampe();
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness();
    void fill(uint32_t hue);
    uint32_t getHue();
    bool update();
    void inputBLE(BLEEvent<long> event);
    void clear();
    void setPixelColor(uint16_t n, uint32_t c);
    void setPixelColor(uint32_t c, uint16_t x, uint16_t y, bool screen0 = true, bool screen1 = true, bool screen2 = true, bool screen3 = true);
    void shiftPixels(int x, int y = 0, int screen = 0);
    void rainbow();
    int16_t getNumPixels() const;
    int getScreenSize() const;
    int getScreenHeight() const;
private:
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(64);
    BaseModus* modus;
    std::vector<BaseModus*> modusList;
    void loadModi();
    uint32_t lastHue = 0;
    bool isDirty = true;
    int16_t numPixels;
    int screenHeight;
    int screenSize;
};


#endif //LAMPE_LAMPE_H
