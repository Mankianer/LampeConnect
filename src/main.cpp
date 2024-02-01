
#include <ArduinoBLE.h>
#include "Lampe.h"
#include "BLESetUp.h"
#include <Arduino.h>

Lampe *lampe;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    BLESetUp::setUpBLE();

    Serial.println("Starte Lampe!");
    lampe = new Lampe(6, 64 * 4);
    BLESetUp::registerBLEHandler([&](BLEEvent<long> event) {
        lampe->inputBLE(event);
    });

    // brightnessCharacteristic.setEventHandler(BLEWritten, [](BLEDevice central, BLECharacteristic characteristic) {
    //     Serial.print("Brightness event, written: ");
    //     auto brightness = (uint8_t) brightnessCharacteristic.value();
    //     Serial.println(brightness);
    //     Lampe::setBrightness(brightness);
    // });

    // brightnessCharacteristic.setEventHandler(BLERead, [](BLEDevice central, BLECharacteristic characteristic) {
    //     Serial.print("Brightness event, read: ");
    //     auto brightness = (int) Lampe::getBrightness();
    //     brightnessCharacteristic.writeValue(brightness);
    //     Serial.println(brightness);
    // });


    // hueCharacteristic.setEventHandler(BLEWritten, [](BLEDevice central, BLECharacteristic characteristic) {
    //     Serial.print("Hue event, written: ");
    //     Serial.println(hueCharacteristic.value());
    //     Lampe::setHue((uint32_t) hueCharacteristic.value());
    // });

    // hueCharacteristic.setEventHandler(BLERead, [](BLEDevice central, BLECharacteristic characteristic) {
    //     Serial.print("Hue event, read: ");
    //     Serial.println(hueCharacteristic.value());
    //     hueCharacteristic.writeValue((int) Lampe::getHue());
    // });


    // modusCharacteristic.setEventHandler(BLEWritten, [](BLEDevice central, BLECharacteristic characteristic) {
    //     Serial.print("Modus event, written: ");
    //     Serial.println(modusCharacteristic.value());
    //     Lampe::setModus((int) modusCharacteristic.value());
    // });

    // modusCharacteristic.setEventHandler(BLERead, [](BLEDevice central, BLECharacteristic characteristic) {
    //     Serial.print("Modus event, read: ");
    //     Serial.println(modusCharacteristic.value());
    //     modusCharacteristic.writeValue((int) Lampe::getModus());
    // });
}

unsigned long lastUpdate = 0;

void loop() {
    BLE.poll();
    lampe->update();
    if(millis() - lastUpdate >= 100) {
        // Serial.println("Update");
        lastUpdate = millis();
    }
}