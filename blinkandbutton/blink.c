#include <msp430.h>				
#include "uart.h"
#define setBit(valor,bit) (valor |= (1<<bit))
#define clearBit(valor,bit) (valor &= ~(1<<bit))
#define toogleBit(valor,bit) (valor^=(1<<bit))
#define testBit(valor,bit) (valor & (1<<bit))

/**
 * blink.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer


    setBit(P1DIR, 6);  //P1.6 SAIDA
    setBit(P1DIR, 0);  //P1.0 SAIDA

    setBit(P2DIR, 0);  //P2.0 SAIDA
    setBit(P2DIR, 1);  //P2.1 SAIDA
    setBit(P2DIR, 2);  //P2.2 SAIDA
    setBit(P2DIR, 3);  //P2.3 SAIDA

    //setBit(P1DIR, 2);  //P1.2 SAIDA

    clearBit(P1DIR,3); //P1.3 ENTRADA
    clearBit(P1DIR,7); //P1.7 ENTRADA

    setBit(P1REN, 3); //PULLUP INTERNO para o P1.3
    ConfigUART(9600);


    setBit(P2OUT, 0);  //P2.0 SAIDA
    setBit(P2OUT, 1);  //P2.1 SAIDA
    clearBit(P2OUT, 2);  //P2.2 SAIDA
    clearBit(P2OUT, 3);  //P2.3 SAIDA


    while(1)
    {
        if(!(testBit(P1IN,3))){
            UARTSend("olar\n\r");
            setBit(P1OUT,0);
        }else{
            clearBit(P1OUT,0);
        }

        if(testBit(P1IN,7)){
            setBit(P1OUT,6);
        }else{
            clearBit(P1OUT,6);
        }
    }
}
