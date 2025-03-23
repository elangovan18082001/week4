
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#define _XTAL_FREQ 6000000
void init();
void cmd(unsigned char);
void data(unsigned char);
void adc();
void uart_tx();
void keyscan();
unsigned char x,count=0,msb,volt,lsb,ar[3],a[3],array[20]={"MOTOR "},speed[10]={"SPD "};
unsigned int value=0;
unsigned char val[2];
void main(void)
{
    init();
    while(1)
    {
        cmd(0x80);
        for(x=0;x<=5;x++)
        {
            data(array[x]);
        }
        cmd(0x89);
        for(x=0;x<=3;x++)
        {
            data(speed[x]);
        }
        keyscan();
    }
}
void init()
{  
    TRISC=0x00;
    TRISD=0x00;
    ADCON0=0x81;
    ADCON1=0x8E;
    TRISC|=0xC0;
    RCSTA=0x90;
    TXSTA=0x20;
    SPBRG=0x09;
    cmd(0x30);
   __delay_ms(30);
    cmd(0x30);
    __delay_ms(30);
    cmd(0x30);
     __delay_ms(30);
    cmd(0x38);
   __delay_ms(30);
    cmd(0x06);
   __delay_ms(30);
    cmd(0x0C);
   __delay_ms(30);
    cmd(0x01);
    __delay_ms(30);
}
void keyscan()
{
    TRISB = 0x0E;  
    PORTB = 0x01; 
        if (PORTB&0x02) 
        {
           ar[0]='A';   
        } 
        else if(PORTB&0x04) 
        {  
            ar[0]='B';
        } 
    cmd(0x87);
    data(ar[0]);
    a[0]=ar[0];
    if(ar[0]=='A'||ar[0]=='B')
    {
    adc();
    }
}
void adc()
{
    ADCON0|=0x04;
     while(ADCON0&0x40);
         msb=ADRESH;
         lsb=ADRESL;
         value=(msb<<8)+lsb;
         volt=value*9/1023;
         cmd(0x8D);
         data(0x30+volt);
         a[1]=volt;
    uart_tx();
}
void uart_tx()
{
    for(x=0;x<2;x++)
    {
        TXREG=(a[x]);
        while(!(PIR1&0x20));
        val[x]=RCREG;
    }
}
void cmd(unsigned char i)
{
    PORTC&=~0x08;
    PORTD=i;
    PORTC|=0x01;
   __delay_ms(10);
    PORTC&=~0x01;   
}
void data(unsigned char i)
{
    PORTC|=0x08;
    PORTD=i;
    PORTC|=0x01;
    __delay_ms(10);
    PORTC&=~0x01;
}
