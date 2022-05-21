/*
 * mqtt.h
 *
 *  Created on: 25 Mar 2022
 *      Author: deniz pelen
 */

#ifndef MQTT_H_
#define MQTT_H_

void connect_broker();
void Publish_msg(char *topic, char *message);
void subscribe_to_topic(char *topic);
#endif /* MQTT_H_ */
