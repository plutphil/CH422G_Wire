#include <CH422G_Wire.h>

CH422G_Wire io(0x24); // Replace with your actual I2C address

void setup() {
    Serial.begin(115200);
    if (!io.begin()) {
        Serial.println("CH422G init failed");
        while (1);
    }

    io.setAllOutput();
    io.writeOutput(0x0F0F);
    delay(1000);
    io.writeOutput(0x0000);
}

void loop() {
    uint8_t input;
    if (io.readInput(input)) {
        Serial.print("Input: 0x");
        Serial.println(input, HEX);
    }
    delay(500);
}
