//
// Created by Mankianer on 10.07.2023.
//

#include "Lampe.h"
#include <Arduino.h>
#include "modus/Modus.h"
#include <Adafruit_NeoPixel.h>

Lampe::Lampe(int16_t pin, int16_t numPixels) 
    : pixels(Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800)),
      modus(new ShiftModus(*this)),
      numPixels(numPixels)
    {
    loadModi();
    pixels.begin();
    modus->enter();
    pixels.show();
}

Lampe::~Lampe() {
    delete modus; 
}

void Lampe::loadModi() {
    modusList.clear();
    modusList.push_back(modus);
    modusList.push_back(new FillModus(*this));
    modusList.push_back(new RandomModus(*this));
}

void Lampe::inputBLE(BLEEvent<long> event) {
    //if modus wechsel
    if(event.eventType == BLEEventTypes::modus) {
        //validate modus
        if(event.value < 0 || event.value >= (long) modusList.size()) {
            Serial.println("Invalid modus: " + String(event.value));
            return;
        } else {
            // change modus
            auto modus_ = modusList[event.value];
            modus = NULL;
            modus_->enter();
            modus = modus_;
            Serial.println("Modus changed to " + String(event.value));
        }
    } else {
        //gebe event an modus weiter
        modus->inputBLE(event);
    }
}

void Lampe::clear()
{
    pixels.clear();
    isDirty = true;
}

void Lampe::setPixelColor(uint16_t n, uint32_t c)
{
    lastHue = 0;
    pixels.setPixelColor(n, c);
    isDirty = true;
}

void Lampe::setBrightness(uint8_t brightness) {
    if (pixels.getBrightness() != brightness) {
        pixels.setBrightness(brightness);
        isDirty = true;
    }
}

uint8_t Lampe::getBrightness() {
    return pixels.getBrightness();
}

void Lampe::fill(uint32_t hue) {
    if (lastHue != hue) {
        lastHue = hue;
        pixels.fill(hue);
        isDirty = true;
    }
}

uint32_t Lampe::getHue() {
    return lastHue;
}

bool Lampe::update() {
    if(modus != NULL) {
        modus->update();
    }
    if (isDirty) {
        pixels.show();
        isDirty = false;
        return true;
    }
    return false;
}



void Lampe::shiftPixels(int shift) {
    int absShift = abs(shift);
    auto temp = new uint32_t[absShift];
    // Helligkeit speichern
    auto lastBrightness = pixels.getBrightness();
    pixels.setBrightness(255);

    if (shift > 0) {
        // Positive Verschiebung: Speichern der ersten 'shift' Pixel
        for (int i = 0; i < absShift; i++) {
            temp[i] = pixels.getPixelColor(i);
        }

        // Verschieben der Pixel-Daten nach rechts
        for (int i = 0; i < numPixels - absShift; i++) {
            pixels.setPixelColor(i, pixels.getPixelColor(i + absShift));
        }

        // Setzen der gespeicherten Pixel am Ende
        for (int i = 0; i < absShift; i++) {
            pixels.setPixelColor(numPixels - absShift + i, temp[i]);
        }
    } else {
        // Negative Verschiebung: Speichern der letzten 'absShift' Pixel
        for (int i = 0; i < absShift; i++) {
            temp[i] = pixels.getPixelColor(numPixels - 1 - i);
        }

        // Verschieben der Pixel-Daten nach links
        for (int i = numPixels - 1; i >= absShift; i--) {
            pixels.setPixelColor(i, pixels.getPixelColor(i - absShift));
        }

        // Setzen der gespeicherten Pixel am Anfang
        for (int i = 0; i < absShift; i++) {
            pixels.setPixelColor(absShift - 1 - i, temp[i]);
        }
    }

    // Helligkeit wiederherstellen
    pixels.setBrightness(lastBrightness);

    // Änderungen anzeigen
    isDirty = true;

    // Wichtig: Den Speicher freigeben
    delete[] temp;
}

int16_t Lampe::getNumPixels() const //TODO per pre-compiler umsetzen
{
    return numPixels;
}
