//
// Created by Mankianer on 10.07.2023.
//

#include "Lampe.h"
#include <Arduino.h>
#include "modus/Modus.h"
#include <Adafruit_NeoPixel.h>
// Modulo operator that works for negative numbers
#define MOD(a, m)  ((a % m + m) % m)

Lampe::Lampe(int16_t pin, int16_t numPixels) 
    : pixels(Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800)),
      modus(new ShiftModus(*this)),
      numPixels(numPixels)
    {
    screenSize = getNumPixels()/LAMPE_SCREENS;
    screenHeight = (int) sqrt(screenSize);
    Serial.println("Screen Size and Height " + String(screenSize) + " " + String(screenHeight));
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

void Lampe::setPixelColor(uint32_t c, uint16_t x, uint16_t y, bool screen0, bool screen1, bool screen2, bool screen3)
{
    if (screen0) {
        setPixelColor(MOD(x,screenHeight) + (MOD(y,screenHeight) * screenHeight), c);
    }
    if (screen1) {
        setPixelColor(MOD(x,screenHeight) + (MOD(y,screenHeight) * screenHeight) + (screenSize), c);
    }
    if (screen2) {
        setPixelColor(MOD(x,screenHeight) + (MOD(y,screenHeight) * screenHeight) + (screenSize * 2), c);
    }
    if (screen3) {
        setPixelColor(MOD(x,screenHeight) + (MOD(y,screenHeight) * screenHeight) + (screenSize * 3), c);
    }
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



void Lampe::shiftPixels(int x, int y, int screen) {
    // Helligkeit speichern
    auto lastBrightness = pixels.getBrightness();
    pixels.setBrightness(255);

    // Shift pixel about shift and stores it in temp
    auto temp = new uint32_t[numPixels];
    for (int i = 0; i < numPixels; i++) {
        int _screen = i / screenSize;
        int _y = (i % screenSize) / screenHeight;
        int _x = (i % screenSize) % screenHeight;
        _x -= x;
        _screen -= screen;
        _y -= y;
        // shift x overflow to next screen
        if (_x < 0) {
            _screen += 1; //TODO fals schift größer als ein Screen
        } else if (_x >= screenHeight) {
            _screen -= 1;
        }
        int shifted = MOD(_x,screenHeight) + (MOD(_y,screenHeight) * screenHeight) + (MOD(_screen,LAMPE_SCREENS) * screenSize);
        // Serial.println(String(i) + "," + String(shifted) + "," + String(_x) + "," + String(_y) + "," + String(_screen));
        temp[i] = pixels.getPixelColor(MOD(shifted, numPixels));
    }
    // write temp back to pixels
    for (int i = 0; i < numPixels; i++) {
        pixels.setPixelColor(i, temp[i]);
    }

    // Helligkeit wiederherstellen
    pixels.setBrightness(lastBrightness);

    // Änderungen anzeigen
    isDirty = true;

    // Wichtig: Den Speicher freigeben
    delete[] temp;
}

void Lampe::rainbow()
{
    pixels.rainbow();
}

int16_t Lampe::getNumPixels() const
{
    return numPixels;
}

int Lampe::getScreenSize() const
{
    return screenSize;
}

int Lampe::getScreenHeight() const
{
    return screenHeight;
}
