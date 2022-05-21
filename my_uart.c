/*
 * my_uart.c
 *
 *  Created on: 6 Mar 2022
 *      Author: deniz pelen
 */
#include <msp430.h>
#include <my_uart.h>

void uart_init(void){
    // 1MHz, 9600 UCBRx = 104, UCBRSx = 1; UCBRFx = 0
    // 104 = 68h, 1101000, UCA0BR0 = 104, UCA0BR1 = 0
    // 1MHz, 115200 UCBRx = 8, UCBRSx = 6; UCBRFx = 0
    //UCAxBR0 + UCAxBR1 * 256 = UCBRx

    P1DIR = 0xFF;                             // All P1.x outputs
    P1OUT = 0;                                // All P1.x reset
    P2DIR = 0xFF;                             // All P2.x outputs
    P2OUT = 0;                                // All P2.x reset
    // select special functionality for P1.1 and P1.2
    P1SEL = BIT1 + BIT2;
    P1SEL2 = BIT1 + BIT2;
    // select the clock source
    UCA0CTL1 |= UCSSEL_2; // SMCLK, subamin clock
    UCA0BR0 = 8 ; // to set baudrate
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_6 + UCBRS0; // according to baudrate select modulation stage
    UCA0CTL1 &= ~UCSWRST;// init USC1 machine
    IE2 =  UCA0RXIE; // enable interrupt

}


