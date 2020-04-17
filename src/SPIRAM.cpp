#include <SPIRAM.h>

SPIRAM::SPIRAM(DSPI *spi, uint32_t s, uint8_t cs0, uint8_t cs1, uint8_t cs2, uint8_t cs3, uint8_t cs4, uint8_t cs5, uint8_t cs6, uint8_t cs7) {
    _spi = spi;
    _size = 0;
    _chipsize = s;
    _chips = 0;

    if (cs0 != NOT_A_PIN) {
        _size += s;
        _cs[_chips] = cs0;
        _chips++;
    }
        
    if (cs1 != NOT_A_PIN) {
        _size += s;
        _cs[_chips] = cs1;
        _chips++;
    }

    if (cs2 != NOT_A_PIN) {
        _size += s;
        _cs[_chips] = cs2;
        _chips++;
    }

    if (cs3 != NOT_A_PIN) {
        _size += s;
        _cs[_chips] = cs3;
        _chips++;
    }
        
    if (cs4 != NOT_A_PIN) {
        _size += s;
        _cs[_chips] = cs4;
        _chips++;
    }
        
    if (cs5 != NOT_A_PIN) {
        _size += s;
        _cs[_chips] = cs5;
        _chips++;
    }
        
    if (cs6 != NOT_A_PIN) {
        _size += s;
        _cs[_chips] = cs6;
        _chips++;
    }
        
    if (cs7 != NOT_A_PIN) {
        _size += s;
        _cs[_chips] = cs7;
        _chips++;
    }
}


void SPIRAM::begin() {
    _spi->begin();
    _spi->setSpeed(12000000UL);
    for (int i = 0; i < _chips; i++) {
        pinMode(_cs[i], OUTPUT);
        digitalWrite(_cs[i], HIGH);
    }
}

uint8_t SPIRAM::read(uint32_t address) {
    uint8_t b;  

    uint32_t chipno = (address / _chipsize);
    uint32_t chipaddress = address - (chipno * _chipsize);

    digitalWrite(_cs[chipno], LOW);
    _spi->transfer(0x03);
    _spi->transfer((chipaddress >> 16) & 0xFF);
    _spi->transfer((chipaddress >> 8) & 0xFF);
    _spi->transfer((chipaddress >> 0) & 0xFF);
    b = _spi->transfer(0x00);
    digitalWrite(_cs[chipno], HIGH);
    return b;
}

void SPIRAM::read(uint32_t address, uint8_t *b, uint32_t len) {
    uint32_t ptr = 0;
    while (len > 0) {
        uint32_t chipno = (address / _chipsize);
        uint32_t chipaddress = address - (chipno * _chipsize);
        uint32_t chipleft = _chipsize - chipaddress;
        
        uint32_t toread = (chipleft < len) ? chipleft : len;
        digitalWrite(_cs[chipno], LOW);
        _spi->transfer(0x03);
        _spi->transfer((chipaddress >> 16) & 0xFF);
        _spi->transfer((chipaddress >> 8) & 0xFF);
        _spi->transfer((chipaddress >> 0) & 0xFF);
        for (uint32_t i = 0; i < toread; i++) {
            b[ptr++] = _spi->transfer(0x00);
        }
        digitalWrite(_cs[chipno], HIGH);
        address += toread;
        len -= toread;
    }
}

void SPIRAM::write(uint32_t address, uint8_t b) {

    uint32_t chipno = (address / _chipsize);
    uint32_t chipaddress = address - (chipno * _chipsize);

    digitalWrite(_cs[chipno], LOW);
    _spi->transfer(0x02);
    _spi->transfer((chipaddress >> 16) & 0xFF);
    _spi->transfer((chipaddress >> 8) & 0xFF);
    _spi->transfer((chipaddress >> 0) & 0xFF);
    _spi->transfer(b);
    digitalWrite(_cs[chipno], HIGH);
}

void SPIRAM::write(uint32_t address, uint8_t *b, uint32_t len) {
    uint32_t ptr = 0;
    while (len > 0) {
        uint32_t chipno = (address / _chipsize);
        uint32_t chipaddress = address - (chipno * _chipsize);
        uint32_t chipleft = _chipsize - chipaddress;
        
        uint32_t towrite = (chipleft < len) ? chipleft : len;
        digitalWrite(_cs[chipno], LOW);
        _spi->transfer(0x02);
        _spi->transfer((chipaddress >> 16) & 0xFF);
        _spi->transfer((chipaddress >> 8) & 0xFF);
        _spi->transfer((chipaddress >> 0) & 0xFF);
        for (uint32_t i = 0; i < towrite; i++) {
            _spi->transfer(b[ptr++]);
        }
        digitalWrite(_cs[chipno], HIGH);
        address += towrite;
        len -= towrite;
    }
}

