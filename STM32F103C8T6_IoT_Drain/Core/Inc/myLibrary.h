/*
 * ESP8266.h
 *
 *  Created on: Nov 17, 2021
 *      Author: vikto
 */

#ifndef INC_MYLIBRARY_H_
#define INC_MYLIBRARY_H_

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WiFi_Credentials	"AT+CWJAP=\"WiFiSSID\",\"WiFiPASSWORD\"\r\n"
#define PWM_OPEN			0x700
#define PWM_CLOSED			0x620

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart2;

uint32_t seconds;
uint8_t buffer[2000];
uint16_t buffer_index, timeout, messageHandlerFlag;
uint8_t state;

void ESP_RESET();
void ESP_Server_Init();
void ESP_Clear_Buffer();
uint8_t string_compare(char array1[], char array2[], uint16_t length);
int string_contains(char bufferArray[], char searchedString[], uint16_t length);
void messageHandler();
void openDrain();
void closeDrain();
void sendData();

#endif /* INC_MYLIBRARY_H_ */
