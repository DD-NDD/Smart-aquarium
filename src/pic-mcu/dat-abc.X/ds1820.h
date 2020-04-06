/* 
 * File:   ds1820.h
 * Author: ngodi
 *
 * Created on March 21, 2020, 9:21 PM
 */

#ifndef DS1820_H
#define	DS1820_H
//#include <16F886.h>
#define ONE_WIRE_PIN PIN_A0
void onewire_reset();
void onewire_write(int8 data);
int onewire_read();
float ds1820_read();
void ds1820_configure(int8 TH, int8 TL, int8 config);
#endif	/* DS1820_H */

