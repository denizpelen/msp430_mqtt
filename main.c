/*

created BY deniz pelen

*/
#include <msp430.h> 
#include <esp_rocks.h>
#include <my_uart.h>
#include <mqtt.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * main.c
 */

#define password "ziyaHOCAkalpkalp"
#define ssid "deniz"



int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	//clock adjustments
	///***********************
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO

    DCOCTL = CALDCO_1MHZ;//set DCO to 1MHz
    BCSCTL3 |= LFXT1S_2; // choose the internal VLOCLK,to ACLK
    //there is no oscillator connected to LFXTAL1, indeed XT2 does not exist
    //*******************************

    //timer adjustments to use it as delay
    //TA0CTL = TASSEL_1+ ID_1  + TACLR; //Auxiliary clock, mc_1 up mode(halted in this line), divide 12kHz by two




    //init uart
    uart_init();
    //enable the general interrupt
    __bis_SR_register(GIE);

    P1DIR = BIT0+BIT6;
    P1OUT = 0;

    char connect_to_wifi[] = "AT+CWJAP=\""ssid"\",\"" password"\"\r\n";
    esp_command("AT\r\n",sizeof("AT\r\n"), "OK",3);
    esp_command("AT+CWMODE=1\r\n",sizeof("AT+CWMODE=1\r\n"), "OK",3);
    esp_command("AT+CWDHCP=1,1\r\n",sizeof("AT+CWDHCP=1,1\r\n"), "OK",3);
    esp_command(connect_to_wifi,sizeof(connect_to_wifi), "CONNECTED",5);

    connect_broker();
    subscribe_to_topic("21728702");
    Publish_msg("isi","hello");
    char sendBuff[100];

    while(1){
        clear_buffer(receivedData);
        //go low power mode
        // set waiting for message to one so when the data is received
        // it will wake it up from low power mode then procedd from here
        // process the incoming data then send(publish) your result

        waiting_for_msg = 1;//waiting for other publisher to send data

        result = 0;
        P1OUT = BIT6;
        __bis_SR_register(LPM0_bits);
        P1OUT &= ~BIT6;
        waiting_for_msg = 0;


        
        Publish_msg("ali",sendBuff);

    }
}




//when received smthng put it into buffer
#pragma vector = USCIAB0RX_VECTOR
__interrupt void receive_interrupt(void){
    //store the received char in the buffer
    IFG2 &= ~UCA0RXIFG;//reset the interrupt flag
    //P1OUT ^= BIT0;
    char rc = UCA0RXBUF;
    get_char_from_uart(rc);
    //if it is waiting for response from esp8266 do not exit low power mode
    // but if it is waiting other msp430(publisher to send data) exit lower power mode to process incoming data
    if(waiting_for_msg == 1 ){
        //exit low power mode
        waiting_for_msg = 0;

        __bic_SR_register_on_exit(LPM0_bits);
    }
}


//this timer interupt indicates that the
// the timer is expired, wake it up from the sleep
#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer_interrupt(void){
    //wake up from the sleep
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL0 &= ~CCIE;
    __bic_SR_register_on_exit(LPM0_bits);
}

