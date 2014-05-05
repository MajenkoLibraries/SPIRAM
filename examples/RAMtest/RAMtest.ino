#include <DSPI.h>
#include <SPIRAM.h>

/*
 * RAM test routine for SPI RAM chips.
 * 
 * Configure SPI channel, chip select line and chip size below.
 */

DSPI0 spi;
const uint8_t CS = 10;
const uint32_t SIZE = 131072;

/*
 * This creates the SPI RAM device.
 */
SPIRAM ram(&spi, CS, SIZE);

void setup() {
	Serial.begin(115200);
	ram.begin();
}

void loop() {
	walkingBit();
	checkerboard();
	randomValues();
	writeTime();
	readTime();
	Serial.println();
	delay(1000);
}

bool testPattern(uint8_t *b, uint32_t len) {
	for (uint32_t i = 0; i < ram.size(); i++) {
		ram.write(i, b[i % len]);
		uint8_t r = ram[i]; // <-- Note here, how you can read the RAM memory as an array.  Nice.
		if (r != b[i % len]) {
			Serial.print(" [");
			Serial.print(i);
			Serial.print(": ");
			Serial.print(b[i % len], HEX);
			Serial.print(" != ");
			Serial.print(r, HEX);
			Serial.print("] ");
			return false;
		}
	}
	return true;
}

// tests

void walkingBit() {
	uint8_t b;
	Serial.print("Walking bit... 1...");
	b = 0b00000001; if (!testPattern(&b, 1)) { Serial.println("fail!"); return; }
	Serial.print(" 2...");
	b = 0b00000010; if (!testPattern(&b, 1)) { Serial.println("fail!"); return; }
	Serial.print(" 4...");
	b = 0b00000100; if (!testPattern(&b, 1)) { Serial.println("fail!"); return; }
	Serial.print(" 8...");
	b = 0b00001000; if (!testPattern(&b, 1)) { Serial.println("fail!"); return; }
	Serial.print(" 16...");
	b = 0b00010000; if (!testPattern(&b, 1)) { Serial.println("fail!"); return; }
	Serial.print(" 32...");
	b = 0b00100000; if (!testPattern(&b, 1)) { Serial.println("fail!"); return; }
	Serial.print(" 64...");
	b = 0b01000000; if (!testPattern(&b, 1)) { Serial.println("fail!"); return; }
	Serial.print(" 128...");
	b = 0b10000000; if (!testPattern(&b, 1)) { Serial.println("fail!"); return; }
	Serial.println("pass");
}

void checkerboard() {
	uint8_t b[2] = {0x55, 0xAA};
	Serial.print("Checkerboard... 0x55AA...");
	if (!testPattern(b, 2)) { Serial.println("fail!"); return; }
	b[0] = 0xAA;
	b[1] = 0x55;
	Serial.print(" 0xAA55...");
	if (!testPattern(b, 2)) { Serial.println("fail!"); return; }
	Serial.println("pass");
}

void randomValues() {
	uint8_t data[256];
	for (uint32_t i = 0; i < 256; i++) {
		data[i] = rand();
	}
	Serial.print("Random Values...");
	if (!testPattern(data, 256)) { Serial.println("fail!"); return; }
	Serial.println("pass");
}

void writeTime() {
	uint32_t i;
	float ans;
	Serial.print("Write time (individual bytes)...");
	uint32_t ts = millis();
	for (i = 0; i < ram.size(); i ++) {
		ram.write(i, 0);
	}
	uint32_t tl = millis() - ts;
	Serial.print(" ");
	Serial.print(tl);
	Serial.print("ms, ");
	ans = (float)ram.size() / ((float)tl / 1000.0);
	Serial.print(ans);
	Serial.println("B/s");

	uint8_t block[1024];
	Serial.print("Write time (1K blocks)...");
	ts = millis();
	for (i = 0; i < ram.size(); i += 1024) {
		ram.write(i, block, 1024);
	}
	tl = millis() - ts;
	Serial.print(" ");
	Serial.print(tl);
	Serial.print("ms, ");
	ans = (float)ram.size() / ((float)tl / 1000.0);
	Serial.print(ans);
	Serial.println("B/s");
}

void readTime() {
	uint32_t i;
	float ans;
	Serial.print("Read time (individual bytes)...");
	uint32_t ts = millis();
	for (i = 0; i < ram.size(); i ++) {
		(void) ram.read(i);
	}
	uint32_t tl = millis() - ts;
	Serial.print(" ");
	Serial.print(tl);
	Serial.print("ms, ");
	ans = (float)ram.size() / ((float)tl / 1000.0);
	Serial.print(ans);
	Serial.println("B/s");

	uint8_t block[1024];
	Serial.print("Read time (1K blocks)...");
	ts = millis();
	for (i = 0; i < ram.size(); i += 1024) {
		ram.read(i, block, 1024);
	}
	tl = millis() - ts;
	Serial.print(" ");
	Serial.print(tl);
	Serial.print("ms, ");
	ans = (float)ram.size() / ((float)tl / 1000.0);
	Serial.print(ans);
	Serial.println("B/s");
}

