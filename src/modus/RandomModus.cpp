
#include "Modus.h"
#include "../BLESetUp.h"
#include <Adafruit_NeoPixel.h>
#include "Arduino.h"

void RandomModus::genRandom()
{
    for ( int i = 0; i < lampe.getNumPixels(); i++) {
        lampe.setPixelColor(i, Adafruit_NeoPixel::Color(rand() % 256, rand() % 256, rand() % 256));
    }
}

void RandomModus::enter() {
    lampe.setBrightness(255);
    lampe.clear();
    this->genRandom();
    lastUpdate = millis();
}

void RandomModus::update() {
    auto _delta = (millis() - lastUpdate) / maxSpeedms;
    if(_delta) {
        this->genRandom();
        lastUpdate = millis();
    }
}

void RandomModus::inputBLE(BLEEvent<long> event) {
    if(event.eventType == brightness) {
        this->lampe.setBrightness(event.value);
    }
    if(event.eventType == speed) {
        this->maxSpeedms = event.value;
    }
}
