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

#include <pthread.h> 
#include <unistd.h>
#include "modbus4r.h"
#include "slave.h"
#include "errors.h"

void mb_rtu_sl_free(modbus_slave_t *mb_slave)
{
    pthread_cancel(mb_slave->tid);

    modbus_close(mb_slave->mb_param);

    modbus_mapping_free(mb_slave->mb_map);
#ifndef RUBY_1_8
    rb_ary_free(mb_slave->coil_status); 
    rb_ary_free(mb_slave->input_status); 
    rb_ary_free(mb_slave->holding_registers); 
    rb_ary_free(mb_slave->input_registers); 
#else
    free(RARRAY_PTR(mb_slave->coil_status)); 
    free(RARRAY_PTR(mb_slave->input_status)); 
    free(RARRAY_PTR(mb_slave->holding_registers)); 
    free(RARRAY_PTR(mb_slave->input_registers)); 
#endif

    free(mb_slave);
}

void *mb_rtu_serv(void *arg)
{
    modbus_slave_t *mb_slave = (modbus_slave_t *)arg;

    uint8_t query[MAX_MESSAGE_LENGTH];
    int query_size;
    while (1) {
        int ret = modbus_slave_receive(mb_slave->mb_param, -1,
                                        query, &query_size);

        if (ret == 0) {
            mb_push_coil_status(mb_slave);
            mb_push_input_status(mb_slave);
            mb_push_holding_registers(mb_slave);
            mb_push_input_registers(mb_slave);

            modbus_slave_manage(mb_slave->mb_param, query, 
                                    query_size, mb_slave->mb_map);

            mb_pull_coil_status(mb_slave);
            mb_pull_holding_registers(mb_slave);
        } else if (ret == CONNECTION_CLOSED) {
            break;
        } else {
            continue;
        }
    }
}

VALUE mb_rtu_sl_new(VALUE self, VALUE device, VALUE baud, 
                            VALUE parity, VALUE data_bit, 
                            VALUE stop_bit, VALUE slave)
{
    modbus_slave_t *mb_slave;

    mb_slave = malloc(sizeof(modbus_slave_t));
    mb_slave->mb_param = malloc(sizeof(modbus_param_t));
    mb_slave->mb_map = malloc(sizeof(modbus_mapping_t));

    device = rb_funcall(device, rb_intern("to_s"), 0);
    baud = rb_funcall(baud, rb_intern("to_i"), 0);
    parity = rb_funcall(parity, rb_intern("to_s"), 0);
    data_bit = rb_funcall(data_bit, rb_intern("to_i"), 0);
    stop_bit = rb_funcall(stop_bit, rb_intern("to_i"), 0);
    slave = rb_funcall(slave, rb_intern("to_i"), 0);

    modbus_init_rtu(mb_slave->mb_param, RSTRING_PTR(device), FIX2INT(baud),
                    RSTRING_PTR(parity), FIX2INT(data_bit),
                    FIX2INT(stop_bit), FIX2INT(slave));
    int ret = modbus_mapping_new(mb_slave->mb_map, 0, 0, 0, 0);
    if (ret < 0) {
        rb_raise(rb_eStandardError, "Memory allocation failed");
    }
    
    mb_slave->coil_status = rb_ary_new();
    mb_slave->input_status = rb_ary_new();
    mb_slave->holding_registers = rb_ary_new();
    mb_slave->input_registers = rb_ary_new();

    return Data_Wrap_Struct(self, 0, mb_rtu_sl_free, mb_slave);
}

VALUE mb_rtu_sl_start(VALUE self)
{    
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    int ret = pthread_create(&mb_slave->tid, NULL, mb_rtu_serv, 
                                (void *)mb_slave);
    if (ret != 0) {
        rb_raise(rb_eStandardError, "Slave has not started");
    }
    
    return self;
}

VALUE mb_rtu_sl_stop(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    int ret = pthread_cancel(mb_slave->tid);
    if (ret != 0) {
        rb_raise(rb_eStandardError, "Slave has not stoped");
    }
    
    close(mb_slave->listen_sock);

    return self;
}
