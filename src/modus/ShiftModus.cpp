
#include "Modus.h"
#include "../BLESetUp.h"
#include <Adafruit_NeoPixel.h>
#include "Arduino.h"

void ShiftModus::enter() {
    lampe.setBrightness(255);
    lampe.clear();

    for (int y = 0; y < 8; ++y) {
        lampe.setPixelColor(Adafruit_NeoPixel::Color(0, 20, 0), 3, y, true,false,true,true);
        lampe.setPixelColor(Adafruit_NeoPixel::Color(0, 0, 20), y, 3, true,false,true,true);
    }
    lampe.setPixelColor(0, Adafruit_NeoPixel::Color(20, 0, 0));
    lampe.setPixelColor(1, Adafruit_NeoPixel::Color(20, 20, 0));
    lastUpdate = millis();
}

void ShiftModus::update() {
    auto shift = (millis() - lastUpdate) / maxSpeedms;
    if(shift) {
        this->lampe.shiftPixels(1, 0);
        lastUpdate = millis();
    }
}

void ShiftModus::inputBLE(BLEEvent<long> event) {
    if(event.eventType == brightness) {
        this->lampe.setBrightness(event.value);
    }
    if(event.eventType == speed) {
        this->maxSpeedms = event.value;
    }
}
