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

#ifndef ERRORS_H
#define ERRORS_H

extern VALUE mErrors, eModBusError, eIllegalFunction,
    eIllegalDataAddress, eIllegalDataValue,
    eSlaveDeviceFailure, eAcknowledge, 
    eSlaveDeviceBusy; 

#endif
