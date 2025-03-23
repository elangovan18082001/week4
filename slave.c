
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
void pwm1();
void pwm2();
void uart_rx();
unsigned char speed=0,a,x,ar[3],h[2]={"OK"};
unsigned int volt,val1,val2;
void main(void) {
    init();
     TRISD=00;
        PORTD=0x00;
    while(1)
    {
        PORTD=~PORTD;
        __delay_ms(200);
}
}
void init()
{
       
        INTCON|=0xC0;
        PIE1|=0x20;
        TRISC=0x00;
        TRISC|=0xC0;
        RCSTA=0x90;
        TXSTA=0x20;
        SPBRG=0x09;   
}
void pwm1()
{
              CCP2CON|=~0x0C;
              CCP1CON|=0x0C;
              T2CON=0x06;
              PR2=0x5E;
              volt=val2*300/9;
              CCPR1L=(volt)>>2;
              CCP1CON=((CCP1CON&0xCF)|((volt&0x03)<<4));
    }
void pwm2()
{            CCP1CON|=~0x0C;
              CCP2CON|=0x0C;
               T2CON=0x06;
               PR2=0x5E;
              volt=val2*300/9;
              CCPR2L=(volt)>>2;
              CCP2CON=((CCP2CON&0xCF)|((volt&0x03)<<4));
}
void __interrupt()_ISR()
{
    for(x=0;x<2;x++)
    {
    while(!(PIR1&0x20));
        ar[x]=RCREG;
        TXREG=h[x];
       
    }
     val1=(unsigned char)ar[0];
        val2=(unsigned char)ar[1];
        switch(val1)
        {
            case 'A':
                pwm1();
                break;
            case 'B':
                pwm2();
                break;
            
        }
    PIR1&=0xDF;
}

