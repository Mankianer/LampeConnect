#include <ArduinoBLE.h>
#include "BLESetUp.h"
#include <Arduino.h>

// Advertising parameters should have a global scope. Do NOT define them in 'setup' or in 'loop'
const uint8_t completeRawAdvertisingData[] = {0x02, 0x01, 0x06, 0x09, 0xff, 0x01, 0x01, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

std::vector<std::function<void(BLEEvent<long>)>> BLESetUp::handlers;
std::map<std::string, BLEEventTypes> BLESetUp::characteristicMap;

void BLESetUp::registerBLEHandler(std::function<void(BLEEvent<long>)> eventHandler)
{
    handlers.push_back(eventHandler);
}

void setUpCharacteristicMap()
{
    // characteristicMap.clear();
    // characteristicMap["fff1"] = brightness;
    // characteristicMap["fff2"] = hue;
    // characteristicMap["fff3"] = modus;

    // characteristic handling
    // modusCharacteristic.setEventHandler(BLEWritten, [](BLEDevice central, BLECharacteristic characteristic)
    //                                     {
    //     Serial.print("Modus event, written: ");
    //     Serial.println( modusCharacteristic.value());
    //     for(auto& h : handlers) {
    //         h(BLEEvent<long>(modus, modusCharacteristic.value()));
    //     } });
    // hueCharacteristic.setEventHandler(BLEWritten, [](BLEDevice central, BLECharacteristic characteristic)
    //                                   {
    //     Serial.print("Hue event, written: ");
    //     Serial.println(hueCharacteristic.value());
    //     for(auto& h : handlers) {
    //         h(BLEEvent<long>(hue, hueCharacteristic.value()));
    //     } });
    // brightnessCharacteristic.setEventHandler(BLEWritten, [](BLEDevice central, BLECharacteristic characteristic)
    //                                          {
    //     Serial.print("Brightness event, written: ");
    //     Serial.println(brightnessCharacteristic.value());
    //     for(auto& h : handlers) {
    //         h(BLEEvent<long>(brightness, brightnessCharacteristic.value()));
    //     } });
}

void BLESetUp::setUpBLE(std::map<std::string, BLEEventTypes> characteristicMap)
{
    if (!BLE.begin())
    {
        Serial.println("failed to initialize BLE!");
        while (1)
            ;
    }

    BLESetUp::characteristicMap = characteristicMap;

    BLEService myService("fff0");
    // BLE.addService(myService);

    // verarbeite charakteristicMap
    for (auto& pair : characteristicMap) {
        // convert pair.first (std::string) to char*
        BLELongCharacteristic characteristic(pair.first.c_str(), BLEWrite);
        Serial.println("Registering " + String(characteristic.uuid()) + " -> " + BLESetUp::getBLEEventTypeString(pair.second));
        characteristic.setEventHandler(BLEWritten, [](BLEDevice central, BLECharacteristic characteristic) {
            std::string uuid = characteristic.uuid();
            // validate characteristic.uuid() with characteristicMap
            if (BLESetUp::characteristicMap.count(uuid) > 0) {
                auto evntType = BLESetUp::characteristicMap[uuid];
                long value = *(long*)characteristic.value();
                Serial.println("Event, written: " + String(characteristic.uuid()) + " -> " + String(evntType) + " -> " + String(value));
                for(auto& h : handlers) {
                    h(BLEEvent<long>(evntType, value));
                }
            } else {
                Serial.println("Invalid characteristic UUID: " + String(characteristic.uuid()));
            }
        });

        myService.addCharacteristic(characteristic);
    }

    BLE.addService(myService);


    // start advertising
    BLEAdvertisingData scanData;
    scanData.setLocalName("Test123");
    // Copy set parameters in the actual scan response packet
    BLE.setScanResponseData(scanData);

    BLE.setEventHandler(BLEConnected, [](BLEDevice central){
        Serial.print("Connected event, central: ");
        Serial.println(central.address()); 
    });

    BLE.setEventHandler(BLEDisconnected, [](BLEDevice central) {
        Serial.print("Disconnected event, central: ");
        Serial.println(central.address()); 
    });

    BLE.advertise();
    Serial.println("advertising ...");
}