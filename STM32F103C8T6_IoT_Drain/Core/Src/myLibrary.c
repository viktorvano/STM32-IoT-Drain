/*
 * ESP8266.c
 *
 *  Created on: Nov 17, 2021
 *      Author: vikto
 */


#include <myLibrary.h>

uint32_t seconds = 0;
uint8_t buffer[2000];
uint16_t buffer_index = 0, timeout = 0, messageHandlerFlag = 0;
uint8_t state = 1;

void ESP_RESET()
{
	HAL_GPIO_WritePin(ESP_ENABLE_GPIO_Port, ESP_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ESP_RESET_GPIO_Port, ESP_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(30);
	HAL_GPIO_WritePin(ESP_ENABLE_GPIO_Port, ESP_ENABLE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ESP_RESET_GPIO_Port, ESP_RESET_Pin, GPIO_PIN_SET);
}

void ESP_Server_Init()
{
	ESP_RESET();
	HAL_Delay(200);
	ESP_Clear_Buffer();

	HAL_UART_Transmit(&huart2, (uint8_t*)"AT+RST\r\n", strlen("AT+RST\r\n"), 100);
	HAL_Delay(200);
	ESP_Clear_Buffer();

	HAL_UART_Transmit(&huart2, (uint8_t*)"AT+CWMODE=1\r\n", strlen("AT+CWMODE=1\r\n"), 100);
	HAL_Delay(200);
	ESP_Clear_Buffer();

	HAL_UART_Transmit(&huart2, (uint8_t*)"AT+CWDHCP=1,1\r\n", strlen("AT+CWDHCP=1,1\r\n"), 100);
	HAL_Delay(200);
	ESP_Clear_Buffer();

	HAL_UART_Transmit(&huart2, (uint8_t*)"AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"), 100);
	HAL_Delay(200);
	ESP_Clear_Buffer();

	HAL_UART_Transmit(&huart2, (uint8_t*)"AT+CIPSERVER=1,80\r\n", strlen("AT+CIPSERVER=1,80\r\n"), 100);
	HAL_Delay(200);
	ESP_Clear_Buffer();

	HAL_UART_Transmit(&huart2, (uint8_t*)WiFi_Credentials, strlen(WiFi_Credentials), 100);
}

void ESP_Clear_Buffer()
{
	memset(buffer, 0, 2000);
	buffer_index = 0;
}

uint8_t string_compare(char array1[], char array2[], uint16_t length)
{
	 uint16_t comVAR=0, i;
	 for(i=0;i<length;i++)
	   	{
	   		  if(array1[i]==array2[i])
	   	  		  comVAR++;
	   	  	  else comVAR=0;
	   	}
	 if (comVAR==length)
		 	return 1;
	 else 	return 0;
}

int string_contains(char bufferArray[], char searchedString[], uint16_t length)
{
	uint8_t result=0;
	for(uint16_t i=0; i<length; i++)
	{
		result = string_compare(&bufferArray[i], &searchedString[0], strlen(searchedString));
		if(result == 1)
			return i;
	}
	return -1;
}

void messageHandler()
{
	__HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
	int position = 0;
	if((position = string_contains((char*)buffer, ",CONNECT\r\n", buffer_index)) != -1)
	{
		ESP_Clear_Buffer();
	}else if((position = string_contains((char*)buffer, "GET", buffer_index)) != -1)
	{
		sendData();
	}else if((position = string_contains((char*)buffer, "OPEN DRAIN", buffer_index)) != -1
			||(position = string_contains((char*)buffer, "open drain", buffer_index)) != -1
			||(position = string_contains((char*)buffer, "open_drain", buffer_index)) != -1
			||(position = string_contains((char*)buffer, "OPEN_DRAIN", buffer_index)) != -1)
	{
		openDrain();
	}else if((position = string_contains((char*)buffer, "CLOSE DRAIN", buffer_index)) != -1
			||(position = string_contains((char*)buffer, "close drain", buffer_index)) != -1
			||(position = string_contains((char*)buffer, "close_drain", buffer_index)) != -1
			||(position = string_contains((char*)buffer, "CLOSE_DRAIN", buffer_index)) != -1)
	{
		closeDrain();
	}else if(string_contains((char*)buffer, "+CWJAP:", buffer_index) != -1
			&& (string_contains((char*)buffer, "FAIL", buffer_index) != -1
			|| string_contains((char*)buffer, "DISCONNECT", buffer_index) != -1))
	{
		HAL_UART_Transmit(&huart2, (uint8_t*)WiFi_Credentials, strlen(WiFi_Credentials), 100);
	}
	ESP_Clear_Buffer();
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
}

void openDrain()
{
	state = 1;
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, PWM_OPEN);
}

void closeDrain()
{
	state = 0;
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, PWM_CLOSED);
}

void sendData()//sends data compatible with a browser
{
	char outputString[300], cipsend[50], response[600];
	memset(outputString, 0, 300);
	memset(cipsend, 0, 50);
	memset(response, 0, 600);

	if(state == 1)
		sprintf(outputString, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>STM32 IoT</title><meta http-equiv=\"refresh\" content=\"5\"></head><body><h1>Drain: OPEN</h1></body></html>");
	else
		sprintf(outputString, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>STM32 IoT</title><meta http-equiv=\"refresh\" content=\"5\"></head><body><h1>Drain: CLOSED</h1></body></html>");
	sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %i\r\nContent-Type: text/html\r\n\r\n%s", strlen(outputString), outputString);
	sprintf(cipsend, "AT+CIPSEND=0,%i\r\n", strlen(response));

	HAL_UART_Transmit(&huart2, (uint8_t*)cipsend, strlen(cipsend), 100);
	HAL_Delay(50);
	HAL_UART_Transmit(&huart2, (uint8_t*)response, strlen(response), 100);
	HAL_Delay(50);
	HAL_UART_Transmit(&huart2, (uint8_t*)"AT+CIPCLOSE=0\r\n", strlen("AT+CIPCLOSE=0\r\n"), 100);
}
