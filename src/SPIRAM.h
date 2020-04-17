#ifndef _SPIRAM_H
#define _SPIRAM_H

#include <WProgram.h>
#include <DSPI.h>

#define MAXCHIPS 8

class SPIRAM {
    public:
        SPIRAM(DSPI *spi, uint32_t s, uint8_t cs0, uint8_t cs1 = NOT_A_PIN, uint8_t cs2 = NOT_A_PIN, uint8_t cs3 = NOT_A_PIN, uint8_t cs4 = NOT_A_PIN, uint8_t cs5 = NOT_A_PIN, uint8_t cs6 = NOT_A_PIN, uint8_t cs7 = NOT_A_PIN);
        uint8_t read(uint32_t address);
        void read(uint32_t address, uint8_t *data, uint32_t len);
        void write(uint32_t address, uint8_t data);
        void write(uint32_t address, uint8_t *data, uint32_t len);
        void begin();
        uint8_t operator[](uint32_t address) { return read(address); }
        uint32_t size() { return _size; }
    private:
        uint32_t _size;
        uint32_t _chipsize;
        uint8_t _chips;
        uint8_t _cs[MAXCHIPS];
        DSPI *_spi;
};

#endif
