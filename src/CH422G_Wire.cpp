#include "CH422G_Wire.h"

CH422G_Wire::CH422G_Wire(uint8_t i2c_address, TwoWire &wire)
    : _address(i2c_address), _wire(wire),
      _wr_set(REG_WR_SET_DEFAULT),
      _wr_oc(REG_WR_OC_DEFAULT),
      _wr_io(REG_WR_IO_DEFAULT) {}

bool CH422G_Wire::begin() {
    _wire.begin();
    return writeRegister(REG_WR_SET, _wr_set) &&
           writeRegister(REG_WR_OC, _wr_oc) &&
           writeRegister(REG_WR_IO, _wr_io);
}

bool CH422G_Wire::setOpenDrain() {
    _wr_set |= BIT_OD_EN;
    return writeRegister(REG_WR_SET, _wr_set);
}

bool CH422G_Wire::setPushPull() {
    _wr_set &= ~BIT_OD_EN;
    return writeRegister(REG_WR_SET, _wr_set);
}

bool CH422G_Wire::setAllInput() {
    _wr_set &= ~BIT_IO_OE;
    bool ok = writeRegister(REG_WR_SET, _wr_set);
    delay(2);
    return ok;
}

bool CH422G_Wire::setAllOutput() {
    _wr_set |= BIT_IO_OE;
    return writeRegister(REG_WR_SET, _wr_set);
}

bool CH422G_Wire::writeOutput(uint16_t value) {
    uint8_t wr_oc = (value >> 8) & 0x0F;
    uint8_t wr_io = value & 0xFF;

    bool ok1 = (_wr_oc != wr_oc) ? writeRegister(REG_WR_OC, wr_oc) : true;
    bool ok2 = (_wr_io != wr_io) ? writeRegister(REG_WR_IO, wr_io) : true;

    if (ok1) _wr_oc = wr_oc;
    if (ok2) _wr_io = wr_io;
    return ok1 && ok2;
}

bool CH422G_Wire::readOutput(uint16_t &value) {
    value = (_wr_oc << 8) | _wr_io;
    return true;
}

bool CH422G_Wire::readInput(uint8_t &value) {
    return readRegister(REG_RD_IO, value);
}

bool CH422G_Wire::writeRegister(uint8_t reg, uint8_t value) {
    _wire.beginTransmission(reg);
    _wire.write(value);
    return (_wire.endTransmission() == 0);
}

bool CH422G_Wire::readRegister(uint8_t reg, uint8_t &value) {
    _wire.requestFrom((int)reg, 1);
    if (_wire.available()) {
        value = _wire.read();
        return true;
    }
    return false;
}

bool CH422G_Wire::digitalWrite(uint8_t pin, uint8_t level) {

    if(level) {
        _wr_oc |= (1 << pin);
    } else {
        _wr_oc &= ~(1 << pin);
    }
    return writeRegister(REG_WR_IO, _wr_oc);
}