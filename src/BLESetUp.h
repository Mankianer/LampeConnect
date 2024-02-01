
#ifndef BLESETUP_H
#define BLESETUP_H

#include <vector>
#include <map>
#include <string>
#include <functional>
#include <ArduinoBLE.h>
#include <iostream>

enum BLEEventTypes {
    brightness,
    hue,
    speed,
    modus
};



template <class T>
class BLEEvent {
    public:
        BLEEvent(BLEEventTypes eventType, T value) {
            this->value = value;
            this->eventType = eventType;
        }
        T value;
        BLEEventTypes eventType;
};

class BLESetUp {
    public:
        static void registerBLEHandler(std::function<void(BLEEvent<long>)> eventHandler);
        static void setUpBLE(std::map<std::string, BLEEventTypes> characteristicMap = {
            {("fff1"), brightness},
            {("fff2"), hue},
            {("fff3"), speed},
            {("fff4"), modus}
        });
        static const char* getBLEEventTypeString(BLEEventTypes eventType) {
            switch (eventType) {
                case brightness:
                    return "brightness";
                case hue:
                    return "hue";
                case speed:
                    return "speed";
                case modus:
                    return "modus";
                default:
                    return "unknown";
            }
        }
    private:
        static std::map<std::string, BLEEventTypes> characteristicMap;
        static std::vector<std::function<void(BLEEvent<long>)>> handlers;
};


#endif // BLESETUP_H