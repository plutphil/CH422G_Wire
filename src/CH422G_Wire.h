#ifndef CH422G_Wire_H
#define CH422G_Wire_H

#include <Arduino.h>
#include <Wire.h>

class CH422G_Wire {
public:
    CH422G_Wire(uint8_t i2c_address, TwoWire &wire = Wire);

    bool begin();
    bool setOpenDrain();
    bool setPushPull();
    bool setAllInput();
    bool setAllOutput();
    bool writeOutput(uint16_t value);
    bool readOutput(uint16_t &value);
    bool readInput(uint8_t &value);
    bool digitalWrite(uint8_t pin, uint8_t level);

private:
    uint8_t _address;
    TwoWire &_wire;

    uint8_t _wr_set;
    uint8_t _wr_oc;
    uint8_t _wr_io;

    bool writeRegister(uint8_t reg, uint8_t value);
    bool readRegister(uint8_t reg, uint8_t &value);

    static const uint8_t REG_WR_SET = 0x48 >> 1;
    static const uint8_t REG_WR_OC  = 0x46 >> 1;
    static const uint8_t REG_WR_IO  = 0x70 >> 1;
    static const uint8_t REG_RD_IO  = 0x4D >> 1;

    static const uint8_t REG_WR_SET_DEFAULT = 0x01;
    static const uint8_t REG_WR_OC_DEFAULT  = 0x0F;
    static const uint8_t REG_WR_IO_DEFAULT  = 0xFF;

    static const uint8_t BIT_IO_OE = 1 << 0;
    static const uint8_t BIT_OD_EN = 1 << 2;
};

#endif
