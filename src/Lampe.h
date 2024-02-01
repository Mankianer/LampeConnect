//
// Created by Mankianer on 10.07.2023.
//

#ifndef LAMPE_LAMPE_H
#define LAMPE_LAMPE_H

class BaseModus;

#include <stdint.h>
#include "BLESetUp.h"
#include <Adafruit_NeoPixel.h>

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
    void shiftPixels(int shift);
private:
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(64);
    BaseModus* modus;
    std::vector<BaseModus*> modusList;
    void loadModi();
    uint32_t lastHue = 0;
    bool isDirty = true;
    int16_t numPixels;
};


#endif //LAMPE_LAMPE_H
