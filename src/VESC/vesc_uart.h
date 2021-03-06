//
// Created by Daniel Claes on 01/05/2016.
//

#ifndef PEDELEC_CONTROLLER_VESC_UART_H
#define PEDELEC_CONTROLLER_VESC_UART_H

/*
Copyright 2015 - 2017 Andreas Chaitidis Andreas.Chaitidis@gmail.com

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.

*/

/*TThis library was created on an Adruinio 2560 with different serial ports to have a better possibility
to debug. The serial ports are define with #define:
#define SERIALIO Serial1  		for the UART port to VESC
#define DEBUGSERIAL Serial		for debuging over USB
So you need here to define the right serial port for your arduino.
If you want to use debug, uncomment DEBUGSERIAL and define a port.*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "datatypes.h"

bool unpack_payload(uint8_t *message, int lenMes, uint8_t *payload, int lenPa);
bool process_read_package(uint8_t *message, mc_values &values, int len);

///PackSendPayload Packs the payload and sends it over Serial.
///Define in a Config.h a SERIAL with the Serial in Arduino Style you want to you
///@param: payload as the payload [unit8_t Array] with length of int lenPayload
///@return the number of bytes send
int send_payload(uint8_t* payload, uint8_t lenPay);

///ReceiveUartMessage receives the a message over Serial
///Define in a Config.h a SERIAL with the Serial in Arduino Style you want to you
///@parm the payload as the payload [unit8_t Array]
///@return the number of bytes receeived within the payload

int process_received_msg(uint8_t* payloadReceived);

///Help Function to print struct bldcMeasure over Serial for Debug
///Define in a Config.h the DEBUGSERIAL you want to use

void serial_print(const mc_values& values);

///Help Function to print uint8_t array over Serial for Debug
///Define in a Config.h the DEBUGSERIAL you want to use

void serial_print(uint8_t* data, int len);

///Sends a command to VESC and stores the returned data
///@param bldcMeasure struct with received data
//@return true if sucess
bool vesc_get_values(mc_values &values);

///Sends a command to VESC to control the motor current
///@param current as float with the current for the motor

void set_motor_current(float current);

///Sends a command to VESC to control the motor brake
///@param breakCurrent as float with the current for the brake

void set_brake_current(float brakeCurrent);

/** Struct to hold the nunchuck values to send over UART */
struct nunchuckPackage {
	int	valueX;
	int	valueY;
	bool upperButton; // valUpperButton
	bool lowerButton; // valLowerButton
};

void set_nunchuck_values(nunchuckPackage *package);


#endif //PEDELEC_CONTROLLER_VESC_UART_H
