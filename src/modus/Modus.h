#ifndef MODUS_H
#define MODUS_H

#include "../Lampe.h"
#include "../BLESetUp.h"
#include <Arduino.h>

class BaseModus
{
private:
protected:
    Lampe& lampe;
public:
    BaseModus(Lampe& lampe) : lampe(lampe) {
    }
    virtual ~BaseModus() {};
    virtual void update() {};
    virtual void enter() {};
    virtual void inputBLE(BLEEvent<long> event) {};
};


class FillModus : public BaseModus {
protected:
public:
    FillModus(Lampe& lampe) : BaseModus(lampe) {}
    void enter() override;
    void inputBLE(BLEEvent<long> event) override;
};

class RandomModus : public BaseModus {
private:
    long maxSpeedms = 1000;
    long lastUpdate = 0;
    void genRandom();
protected:
public:
    RandomModus(Lampe& lampe) : BaseModus(lampe) {}
    void enter() override;
    void update() override;
    void inputBLE(BLEEvent<long> event) override;
};

class ShiftModus : public BaseModus {
private:
    long maxSpeedms = 1000;
    long lastUpdate = 0;
protected:
public:
    ShiftModus(Lampe& lampe) : BaseModus(lampe) {}
    void enter() override;
    void update() override;
    void inputBLE(BLEEvent<long> event) override;
};


#endif // MODUS_H
