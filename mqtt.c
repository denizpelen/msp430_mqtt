/*
 * mqtt.c
 *
 *  Created on: 25 Mar 2022
 *      Author: deniz pelen
 */
#include <msp430.h>
#include <esp_rocks.h>
#include <string.h>
#include <stdio.h>

#define IP1 "10.225.246.90"
#define IP2 "192.168.43.240"
#define Port "1883"

/*connect packet format
 * connect  remainingLength ProtocolLength Protocol Level Flag KeepAlive ClientID_length ClientID
 * */
#define connect 0x10
#define Subscribe 0x82
#define Publish 0x30

#define packetID 0x01
#define Qos 0x00

#define remainingLength 0x11 // according to packet lenght
#define ProtocolLength 0x00,0x04 // length of MQTT, 16 bit
#define Protocol "MQTT"

#define Level 0x04 //
#define Flag 0x02 //flag without username and password, clean session
//8th bit username flag, 7th flag password flag
#define Flag1 0xC2//with username and password
#define KeepAlive 200//0x0F,0xFF //0x003D // the time that the connection will be kept alive by the server, the timer expires connection drops
#define ClientID "g34dp" // client id
//#define ClientID_length 0x00,0x05  //16 bit the length of the client id that will b send at the end of the packet

#define UserName  "g34dp"
#define pwd  "xxxx"



//char connect_packet[] =  {connect,remainingLength, ProtocolLength, Protocol, Level, Flag1, KeepAlive, ClientID_length, ClientID,un_len,UserName,pwd_len,pwd};

//192.168.43.240
char connect_to_server[] = "AT+CIPSTART=\"TCP\",\"iot.ee.hacettepe.edu.tr\",1883\r\n";


char cipsend[];


/*
void connect_broker(){
    //esp_command("AT\r\n",strlen("AT\r\n"), "OK",3);
    esp_command("AT+CIPCLOSE\r\n",sizeof("AT+CIPCLOSE\r\n"),"OK",5);//close alll the server connection

    esp_command("AT+CIPMUX=0\r\n",sizeof("AT+CIPMUX=0\r\n"),"OK",5);//if single connection =1, in case of multiple connection =1

    //esp_command("AT+CIFSR\r\n","OK",3); // returns the IP of the ESP
    //connect the server via IP and Port
    esp_command(connect_to_server,sizeof(connect_to_server),"OK",5);
    //unsigned int len_arr;
    //len_arr = sprintf(cipsend,"AT+CIPSEND=%d\r\n",sizeof(connect_packet));
    //esp_command(cipsend,len_arr,"OK",2);


    //esp_command(connect_packet,sizeof(connect_packet),"OK",5);

}*/
void connect_broker(){
    char tx_buffer[100];
    esp_command("AT+CIPCLOSE\r\n",sizeof("AT+CIPCLOSE\r\n"),"OK",5);
    //close alll the server connection
    esp_command("AT+CIPMUX=0\r\n",sizeof("AT+CIPMUX=0\r\n"),"OK",5);
    //if single connection =1, in case of multiple connection =1
    //connect the server via IP and Port
    esp_command(connect_to_server,sizeof(connect_to_server),"OK",5);
    // get thelength of payload,
    //we need to indicate length of the packet at the beginnign
    int ProtocolNameLength = strlen(Protocol);//length of the protocol
    int ClientIDLength     = strlen(ClientID);//length of the clientID
    int user_len = strlen(UserName);//length of the usernam
    int pwd_len = strlen(pwd);//length of the password
    int Remainlength;
    // sum up the lengths of the payloads
    Remainlength = 2+ProtocolNameLength+6+ClientIDLength+2+2+user_len+pwd_len;
    // we need to tell esp how many bits we will send
    unsigned int length = sprintf(tx_buffer,"%c%c%c%c%s%c%c%c%c%c%c%s%c%c%s%c%c%s",(char)connect,(char)Remainlength,(char)(ProtocolNameLength << 8),(char)ProtocolNameLength,Protocol,(char)Level,(char)Flag1,(char)(KeepAlive << 8),(char)KeepAlive,(char)(ClientIDLength << 8),(char)ClientIDLength,ClientID,(char)(user_len<<8),(char)(user_len),UserName,(char)(pwd_len<<8),(char)(pwd_len),pwd);
    // say to ESP that youare going to transmit it smthng
    int len_arr = sprintf(tx_buffer,"AT+CIPSEND=%d\r\n",length);
    esp_command(tx_buffer,len_arr,"OK",2);
    sprintf(tx_buffer,"%c%c%c%c%s%c%c%c%c%c%c%s%c%c%s%c%c%s",(char)connect,(char)Remainlength,(char)(ProtocolNameLength << 8),(char)ProtocolNameLength,Protocol,(char)Level,(char)Flag1,(char)(KeepAlive << 8),(char)KeepAlive,(char)(ClientIDLength << 8),(char)ClientIDLength,ClientID,(char)(user_len<<8),(char)(user_len),UserName,(char)(pwd_len<<8),(char)(pwd_len),pwd);

    esp_command(tx_buffer,length,"OK",5);
}

void Publish_msg(char *topic, char *message)
{
    char tx_buffer[100];
    // length of the topic
    int topiclength = strlen(topic);
    // indicate the length of the payload
    int remainlength = 2+topiclength+strlen(message);
    unsigned int length = sprintf(tx_buffer,"%c%c%c%c%s%s",(char)Publish,(char)remainlength,(char)(topiclength << 8),(char)topiclength,topic,message);
    esp_command(tx_buffer,sprintf(tx_buffer,"AT+CIPSEND=%d\r\n",length),"OK",2);
    sprintf(tx_buffer,"%c%c%c%c%s%s",(char)Publish,(char)remainlength,(char)(topiclength << 8),(char)topiclength,topic,message);
    esp_command(tx_buffer,length,"OK",2);
    return;
}

void subscribe_to_topic(char *topic)
{
    char tx_buffer[100];
    unsigned int TopicLength = strlen(topic);
    int RemainLength = 2+2+TopicLength+1;
    unsigned int length = sprintf(tx_buffer,"%c%c%c%c%c%c%s%c",(char)Subscribe,(char)RemainLength,(char)(packetID << 8),(char)packetID,(char)(TopicLength << 8),(char)TopicLength,topic,(char)Qos);

    esp_command(tx_buffer,sprintf(tx_buffer,"AT+CIPSEND=%d\r\n",length),"OK",2);

    sprintf(tx_buffer,"%c%c%c%c%c%c%s%c",(char)Subscribe,(char)RemainLength,(char)(packetID << 8),(char)packetID,(char)(TopicLength << 8),(char)TopicLength,topic,(char)Qos);

    esp_command(tx_buffer,length,"OK",3);
}


