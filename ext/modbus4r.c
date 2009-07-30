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
#include "errors.h"
#include "tcp_master.h"

VALUE mModBus, cTCPMaster;

void Init_modbus4r()
{
    mModBus = rb_define_module("ModBus");

    /* TCPMaster */
    cTCPMaster = rb_define_class_under(mModBus, "TCPMaster", rb_cObject);
    rb_define_singleton_method(cTCPMaster, "new", mb_tcp_mstr_new, 2);
    rb_define_method(cTCPMaster, "closed?", mb_tcp_mstr_is_closed, 0);
    rb_define_method(cTCPMaster, "connect", mb_tcp_mstr_connect, 0);
    rb_define_method(cTCPMaster, "close", mb_tcp_mstr_close, 0);
    rb_define_method(cTCPMaster, "read_coil_status", 
                    mb_tcp_mstr_read_coil_status, 3);
    rb_define_method(cTCPMaster, "read_input_status",
                    mb_tcp_mstr_read_input_status, 3);
    rb_define_method(cTCPMaster, "read_holding_registers", 
                    mb_tcp_mstr_read_holding_registers, 3);
    rb_define_method(cTCPMaster, "read_input_registers", 
                    mb_tcp_mstr_read_holding_registers, 3);
    rb_define_method(cTCPMaster, "force_single_coil", 
                    mb_tcp_mstr_force_single_coil, 3);
    rb_define_method(cTCPMaster, "preset_single_register", 
                    mb_tcp_mstr_preset_single_register, 3);
    rb_define_method(cTCPMaster, "force_multiple_coils", 
                    mb_tcp_mstr_force_multiple_coils, 3);
    rb_define_method(cTCPMaster, "preset_multiple_registers", 
                    mb_tcp_mstr_preset_multiple_registers, 3);
    /* Errors */
    mErrors = rb_define_module_under(mModBus, "Errors");
    eModBusError = rb_define_class_under(mErrors, "ModBusError", 
                                        rb_eStandardError);
    eIllegalFunction = rb_define_class_under(mErrors, "IllegalFunction",
                                        eModBusError);
    eIllegalDataAddress = rb_define_class_under(mErrors, "IllegalDataAddress",
                                        eModBusError);
    eIllegalDataValue = rb_define_class_under(mErrors, "IllegalDataValue",
                                        eModBusError);
    eSlaveDeviceFailure = rb_define_class_under(mErrors, "SlaveDeviceFailure",
                                        eModBusError);
    eAcknowledge = rb_define_class_under(mErrors, "Acknowledge",
                                        eModBusError);
    eSlaveDeviceBusy = rb_define_class_under(mErrors, "SlaveDeviceBusy",
                                        eModBusError);
    eNegativeAcknowledge = rb_define_class_under(mErrors, "NegativeAcknowledge",
                                        eModBusError);
    eMemoryParityError = rb_define_class_under(mErrors, "MemoryParityError",
                                        eModBusError);
    eGatewayProblemPath = rb_define_class_under(mErrors, "GatewayProblemPath", 
                                        eModBusError);
    eGatewayProblemTarget = rb_define_class_under(mErrors, "GatewayProblemTarget",
                                        eModBusError);
}
