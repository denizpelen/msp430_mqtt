/*
 * esp_rocks.c
 *
 *  Created on: 18 Mar 2022
 *      Author: deniz pelen
 */
#include <esp_rocks.h>
#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

unsigned int row = 0;
unsigned int col = 0;
unsigned int cntr = 0;
void esp_command(char *command,unsigned int size_arr,char *pos_response,unsigned int delay){
    //clear the buffer
    clear_buffer(receivedData);
    //send the command
    volatile int flag1 = 1;
    while(flag1){
        put_uart_array(command,size_arr);
        //go into deep sleep
        P1OUT &= ~BIT0;
        delay_sleep(delay);
        P1OUT = BIT0;

        flag1 = 0;
        clear_buffer(receivedData);
    }
}

//sends the given array via uar

// no interrupt
void put_uart_array(char *TxArray,unsigned int size_arr)
{
    unsigned int i = 0;
    for(i = 0;i<size_arr;i++)     // loop until binary zero (EOS)
    {
        while(!(IFG2 & UCA0TXIFG));        // Wait for TX buffer to be ready for new data
        UCA0TXBUF = TxArray[i];             // Write the character at the location specified py the pointer
    }
}


/*clears the buffer
 * which is used for storing the response of the ESP
 * resets the column and row pointers
 * */
void clear_buffer()
{
    unsigned int i=0;
    for(i = 0; i<buff_s; i++)
    {
        receivedData[i] = '\0';
    }
    cntr = 0;
}

/*starts the timer then sleeps until
 * awaken up by the timer
 * */
void delay_sleep(unsigned int sleep_sec){
    TA0CTL = TASSEL_1+ ID_1  + TACLR+MC_1;
    TA0CCTL0 |= CCIE;
    TA0CCR0 = (6000)*sleep_sec;
    //go sleep
    __bis_SR_register(LPM0_bits);
    TA0CTL = MC_0; // timer is halted
    return;
}
/*this function will be called in the interrupt
 * received char is stored in the buffer
 *
 * */
void get_char_from_uart(char data1)
{
    if(cntr < buff_s-1){
        if(data1 != 0x00){
            receivedData[cntr] = data1;
            cntr++;
        }
    }
}

/*wait for a response
 *looks into received data buffer(array)
 *esp_data is buffer received from esp
 *response is the key word that we are looking for in that esp_data
 *if found returns 1, if not 0
 * */
int check_response(char *response)     //wait esp return "OK"
{
    char* p;
    p = strstr(receivedData,response);
    if(p != 0){
        return 1;
    }
    return 0;
}



