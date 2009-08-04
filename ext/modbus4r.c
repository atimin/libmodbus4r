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
#include "master.h"
#include "tcp_master.h"
#include "rtu_master.h"
#include "tcp_slave.h"

VALUE mModBus, cMaster, cTCPMaster, cRTUMaster, cTCPSlave;

void Init_modbus4r()
{
    mModBus = rb_define_module("ModBus");
    /* Master */
    cMaster = rb_define_class_under(mModBus, "Master", rb_cObject);
    rb_define_method(cMaster, "closed?", mb_mstr_is_closed, 0);
    rb_define_method(cMaster, "connect", mb_mstr_connect, 0);
    rb_define_method(cMaster, "close", mb_mstr_close, 0);
    rb_define_method(cMaster, "read_coil_status", 
                    mb_mstr_read_coil_status, 3);
    rb_define_method(cMaster, "read_input_status",
                    mb_mstr_read_input_status, 3);
    rb_define_method(cMaster, "read_holding_registers", 
                    mb_mstr_read_holding_registers, 3);
    rb_define_method(cMaster, "read_input_registers", 
                    mb_mstr_read_holding_registers, 3);
    rb_define_method(cMaster, "force_single_coil", 
                    mb_mstr_force_single_coil, 3);
    rb_define_method(cMaster, "preset_single_register", 
                    mb_mstr_preset_single_register, 3);
    rb_define_method(cMaster, "force_multiple_coils", 
                    mb_mstr_force_multiple_coils, 3);
    rb_define_method(cMaster, "preset_multiple_registers", 
                    mb_mstr_preset_multiple_registers, 3);

    /* TCPMaster */
    cTCPMaster = rb_define_class_under(mModBus, "TCPMaster", cMaster);
    rb_define_singleton_method(cTCPMaster, "new", mb_tcp_mstr_new, 2);

    /* RTUMaster */
    cRTUMaster = rb_define_class_under(mModBus, "RTUMaster", cMaster);
    rb_define_singleton_method(cRTUMaster, "new", mb_rtu_mstr_new, 5);

    /* TCPSlave */
    cTCPSlave = rb_define_class_under(mModBus, "TCPSlave", rb_cObject);
    rb_define_singleton_method(cTCPSlave, "new", mb_tcp_sl_new, 3);
    rb_define_method(cTCPSlave, "start", mb_tcp_sl_start, 0);
    rb_define_method(cTCPSlave, "stop", mb_tcp_sl_stop, 0);
    rb_define_method(cTCPSlave, "stoped?", mb_tcp_sl_is_stoped, 0);
    rb_define_method(cTCPSlave, "id", mb_tcp_sl_id, 0);

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
