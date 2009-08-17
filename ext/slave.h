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

#ifndef SLAVE_H
#define SLAVE_H

typedef struct {
    pthread_t tid;
    int listen_sock;
    int max_conn;
    modbus_param_t *mb_param;
    modbus_mapping_t *mb_map;
    VALUE coil_status;
    VALUE input_status;
    VALUE holding_registers;
    VALUE input_registers;
} modbus_slave_t;

extern void mb_push_coil_status(modbus_slave_t *mb_slave);
extern void mb_pull_coil_status(modbus_slave_t *mb_slave);
extern void mb_push_input_status(modbus_slave_t *mb_slave);
extern void mb_push_holding_registers(modbus_slave_t *mb_slave);
extern void mb_pull_holding_registers(modbus_slave_t *mb_slave);
extern void mb_push_input_registers(modbus_slave_t *mb_slave);
extern VALUE mb_sl_is_stoped(VALUE self);
extern VALUE mb_sl_get_coil_status(VALUE self);
extern VALUE mb_sl_set_coil_status(VALUE self, VALUE value);
extern VALUE mb_sl_get_input_status(VALUE self);
extern VALUE mb_sl_set_input_status(VALUE self, VALUE value);
extern VALUE mb_sl_get_holding_registers(VALUE self);
extern VALUE mb_sl_set_holding_registers(VALUE self, VALUE value);
extern VALUE mb_sl_get_input_registers(VALUE self);
extern VALUE mb_sl_set_input_registers(VALUE self, VALUE value);

#endif
