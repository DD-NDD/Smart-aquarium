#include "ds1820.h"
#include <16F886.h>
void onewire_reset() {
    output_low(ONE_WIRE_PIN);
    delay_us(500);
    output_float(ONE_WIRE_PIN);
    delay_us(500);
    output_float(ONE_WIRE_PIN);
}
void onewire_write(int8 data) {
    int8 count;

    for(count = 0; count < 8; ++count) {
        output_low(ONE_WIRE_PIN);
        delay_us(2);
        output_bit(ONE_WIRE_PIN, shift_right(&data, 1, 0));
        delay_us(60);
        output_float(ONE_WIRE_PIN);
        delay_us(2);
    }
}
int onewire_read() {
    int count, data;

    for(count = 0; count < 8; ++count) {
        output_low(ONE_WIRE_PIN);
        delay_us(2);
        output_float(ONE_WIRE_PIN);
        delay_us(8);
        shift_right(&data, 1, input(ONE_WIRE_PIN));
        delay_us(120);
    }
    return data;
} 
float ds1820_read() {
    int8 busy=0, temp1, temp2;
    signed int16 temp3;
    float result;
    onewire_reset();
    onewire_write(0xCC);
    onewire_write(0x44);
    while(busy == 0)
        busy = onewire_read();
    onewire_reset();
    onewire_write(0xCC);
    onewire_write(0xBE); 
    temp1 = onewire_read();
    temp2 = onewire_read();
    temp3 = make16(temp2, temp1);
    result = (float) temp3 / 16.0;
    delay_ms(200);
    return(result);
}
void ds1820_configure(int8 TH, int8 TL, int8 config) {
    onewire_reset();
    onewire_write(0xCC);
    onewire_write(0x4E);
    onewire_write(TH);
    onewire_write(TL);
    onewire_write(config);
}
