/*
 * esp_rocks.h
 *
 *  Created on: 18 Mar 2022
 *      Author: deniz pelen
 */

#ifndef ESP_ROCKS_H_
#define ESP_ROCKS_H_

#define buff_s 100
extern char receivedData[buff_s] = {};

void put_uart_array(char *,unsigned int size);//send string via uart
//send command via uart and wait for response then check response
void esp_command(char *command,unsigned int size_arr,char *pos_response,unsigned int delay);
//clear receive buffer
void clear_buffer();
//delay function waits in LPM0
void delay_sleep(unsigned int sleep_sec);
//put received char to buffer
void get_char_from_uart(char );
//check the response from the esp
int check_response(char *response);

#endif /* ESP_ROCKS_H_ */
