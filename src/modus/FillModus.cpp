
#include "Modus.h"
#include "../BLESetUp.h"
#include "Adafruit_NeoPixel.h"
#include "Arduino.h"

void FillModus::enter() {
    Serial.println("Enter FillModus");
    this->lampe.setBrightness(255);
    this->lampe.fill(Adafruit_NeoPixel::Color(0, 0, 255));
}

void FillModus::inputBLE(BLEEvent<long> event) {
    switch (event.eventType) {
        case brightness:
            this->lampe.setBrightness(event.value);
            break;
        case hue:
            this->lampe.fill(event.value);
            break;
    }
}
