
#include "Modus.h"
#include "../BLESetUp.h"
#include <Adafruit_NeoPixel.h>
#include "Arduino.h"

void ShiftModus::enter() {
    lampe.setBrightness(255);
    lampe.clear();
    for ( int z = 0; z < 4; ++z) {
        for (int y = 0; y < 8; ++y) {
            lampe.setPixelColor(3 + y * 8 + z * 64, Adafruit_NeoPixel::Color(0, 20, 0));
        }
    }
    lastUpdate = millis();
}

void ShiftModus::update() {
    auto shift = (millis() - lastUpdate) / maxSpeedms;
    if(shift) {
        this->lampe.shiftPixels(shift);
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
