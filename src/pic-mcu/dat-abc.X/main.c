/* 
 * File:   main.c
 * Author: ngodinhduy
 *
 * Created on March 18, 2020, 9:45 PM
 */


#include <16F886.h>
#use delay(internal=8MHz)
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#use timer(tick=100us,bits=32,NOISR)    
#define TICK_TYPE unsigned int32    

// define for pin in board----------------------------------------------------//
#define FAN   PIN_A1
#define LIGHT   PIN_A2
#define LED_DB   PIN_A3
#define BT1   PIN_A4
#define BT2   PIN_A5
#define LIGHT_ON() output_high(LIGHT);
#define LIGHT_OFF() output_low(LIGHT);
#define FAN_ON() output_high(FAN);
#define FAN_OFF() output_low(FAN);
// end define ----------------------------------------------------------------//

// define for LCD ------------------------------------------------------------//
#define ONE_WIRE_PIN PIN_A0
#define LCD_ENABLE_PIN PIN_C1
#define LCD_RS_PIN PIN_C3
#define LCD_RW_PIN PIN_C2
#define LCD_DATA4 PIN_C7
#define LCD_DATA5 PIN_C6
#define LCD_DATA6 PIN_C5
#define LCD_DATA7 PIN_C4
// define for LCD ------------------------------------------------------------//
#include "lcd.c"
#use rs232(baud=9600, xmit=PIN_B6,rcv= PIN_B7,parity=n,bits = 8)
/*
 * 
 */
bool fan_state;
bool light_state;
int  temp_setpoint;
float  temp_current;
bool auto_hand;
TICK_TYPE GetTickDifference(TICK_TYPE currTick, TICK_TYPE prevTick)
{
	return(currTick-prevTick);
}        
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
void display();
void check_temp();
void main( void )
{
    lcd_init();
    char s;
    lcd_putc("\f");
    puts("START");
    fan_state = false;
    light_state = false;
    temp_current = 0;
    temp_setpoint = 0;
    
    TICK_TYPE CurrentTick,PreviousTick;
    //Example program using Tick Timer
	CurrentTick = PreviousTick = get_ticks(); 
    while(TRUE) 
    {
        CurrentTick = get_ticks();
                                     
		if(GetTickDifference(CurrentTick, PreviousTick) >= (TICK_TYPE)TICKS_PER_SECOND)
		{			
            temp_current = ds1820_read();
            display();
			PreviousTick = CurrentTick;
		}
        if(input(BT1) == 0)
        {
            auto_hand = true;
        }
        if(input(BT2) == 0)
        {
            auto_hand = false;
        }
        if (kbhit())
        {
            s = getc();          
            switch(s)
            {            

                case '1':
                {
                    fan_state = true;
                    check_temp();
                    display();
                    break;
                }
                case '2':
                {
                    fan_state = false;
                    check_temp();
                    display();
                    break;
                }
                case '3':
                {
                    light_state = true;
                    check_temp();
                    display();
                    break;
                }
                case '4':
                {
                    light_state = false;
                    check_temp();
                    display();
                    break;
                }
                case '5':
                {
                    temp_setpoint = 23;
                    check_temp();
                    display();
                    break;
                }
                case '6':
                {
                    temp_setpoint = 24;
                    check_temp();
                    display();
                    break;
                }
                case '7':
                {
                    temp_setpoint = 25;
                    check_temp();
                    display();
                    break;
                }
                case '8':
                {
                    temp_setpoint = 26;
                    check_temp();
                    display();
                    break;
                }
                case '9':
                {
                    temp_setpoint = 27;
                    check_temp();
                    display();
                    break;
                }
                case 'q':
                {
                    temp_setpoint = 28;
                    check_temp();
                    display();
                    break;
                }
                case 'w':
                {
                    temp_setpoint = 29;
                    check_temp();
                    display();
                    break;
                }
                case 'e':
                {
                    temp_setpoint = 30;
                    check_temp();
                    display();
                    break;
                }
                case 'r':
                {
                    temp_setpoint = 31;
                    check_temp();
                    display();
                    break;
                }
                default:
                {
                    break;
                }
            }
            s = NULL;
        }
        
    }
}

void display()
{
    check_temp();
    lcd_putc("\f");
    if(fan_state == true)
    {
        lcd_gotoxy(0,0); // line top
        lcd_putc("F-ON");
    }
    else
    {
        lcd_gotoxy(0,0); // line top
        lcd_putc("F-OFF");
    }
    if(light_state == true)
    {
        lcd_gotoxy(8,0); // line top
        lcd_putc("L-ON");
    }
    else
    {
        lcd_gotoxy(8,0); // line top
        lcd_putc("L-OFF");
    }
    lcd_gotoxy(15,0);
    if(auto_hand)
    {
        lcd_putc("A");
    }
    else
    {
        lcd_putc("H");
    }
    lcd_gotoxy(0,1); // line botton
    printf(lcd_putc,"SP-%d   CR-%3.1f",temp_setpoint,temp_current);
    printf("%3.1f\n",temp_current);
}
void check_temp()
{
    if(auto_hand)
    {
        if(temp_setpoint > temp_current)
        {
            FAN_ON();
            fan_state = true;
            LIGHT_ON();
            light_state = true;
        }
        else
        {
            FAN_OFF();
            fan_state = false;
            LIGHT_OFF();
            light_state = false;
        }
    }
    else
    {
        if(fan_state) 
        {FAN_ON();}
        else {FAN_OFF();}
        if(light_state) {LIGHT_ON();}
        else
        {LIGHT_OFF();}
    }
}