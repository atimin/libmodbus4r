/* libmodbus4r - binding use libmodbus for Ruby.
Copyright (C) 2009  Timin Aleksey

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details. */

#include "modbus4r.h"

VALUE mErrors, eModBusError, eIllegalFunction,
    eIllegalDataAddress, eIllegalDataValue,
    eSlaveDeviceFailure, eAcknowledge, 
    eSlaveDeviceBusy, eNegativeAcknowledge,
    eMemoryParityError, eGatewayProblemPath, eGatewayProblemTarget; 

void mb_raise_error(int exception)
{
    switch (exception) {
        case ILLEGAL_FUNCTION:
            rb_raise(eIllegalFunction, "Illegal function code (%i)", ILLEGAL_FUNCTION);
            break;
        case ILLEGAL_DATA_ADDRESS:
            rb_raise(eIllegalDataAddress, "Illegal data address (%i)", ILLEGAL_DATA_ADDRESS);
            break;
        case  ILLEGAL_DATA_VALUE:
            rb_raise(eIllegalDataValue, "Illegal data value (%i)", ILLEGAL_DATA_VALUE);
            break;
        case SLAVE_DEVICE_FAILURE:
            rb_raise(eSlaveDeviceFailure, "Slave device or server failure (%i)", SLAVE_DEVICE_FAILURE);
            break;
        case ACKNOWLEDGE:
            rb_raise(eAcknowledge, "Acknowledge (%i)", ACKNOWLEDGE);
            break;
        case SLAVE_DEVICE_BUSY:
            rb_raise(eSlaveDeviceBusy, "Slave device or server busy (%i)", SLAVE_DEVICE_BUSY);
            break;
        case NEGATIVE_ACKNOWLEDGE:
            rb_raise(eNegativeAcknowledge, "Negative acknowledge (%i)", NEGATIVE_ACKNOWLEDGE);
            break;
        case MEMORY_PARITY_ERROR:
            rb_raise(eMemoryParityError, "Memory parity error (%i)", MEMORY_PARITY_ERROR);
            break;
        case GATEWAY_PROBLEM_PATH:
            rb_raise(eGatewayProblemPath, "Gateway problem path (%i)", GATEWAY_PROBLEM_PATH);
            break;
        case GATEWAY_PROBLEM_TARGET:
            rb_raise(eGatewayProblemTarget, "Gateway problem target (%i)", GATEWAY_PROBLEM_TARGET);
            break;
        case COMM_TIME_OUT:
            rb_raise(eModBusError, "Communication timeout (%i)", COMM_TIME_OUT);
            break;
        case PORT_SOCKET_FAILURE:
            rb_raise(eModBusError, "Port socket failure (%i)", PORT_SOCKET_FAILURE);
            break;
        case SELECT_FAILURE:
            rb_raise(eModBusError, "Select failure (%i)", SELECT_FAILURE);
            break;
        case TOO_MANY_DATA:
            rb_raise(eModBusError, "Too many data (%i)", TOO_MANY_DATA);
            break;
        case INVALID_CRC:
            rb_raise(eModBusError, "Invalid CRC (%i)", INVALID_CRC);
            break;
        case INVALID_EXCEPTION_CODE:
            rb_raise(eModBusError, "Invalid exception code (%i)", INVALID_EXCEPTION_CODE);
            break;
        case CONNECTION_CLOSED:
            rb_raise(eModBusError, "Connection closed (%i)", CONNECTION_CLOSED);
            break;
        default:
            rb_raise(eModBusError, "Unknow exception with code %i", exception);
    }
}


