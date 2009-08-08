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

typedef struct {
    pthread_t tid;
    uint8_t is_stoped;
    modbus_param_t mb_param;
    modbus_mapping_t mb_map;
} modbus_slave_t;

void mb_tcp_sl_free(modbus_slave_t *mb_slave)
{
    modbus_close(&mb_slave->mb_param);
    modbus_mapping_free(&mb_slave->mb_map);
    free(mb_slave);
}

void *mb_serv(void *arg)
{
    modbus_slave_t *mb_slave = (modbus_slave_t *)arg;
        
    while(1)
        continue;
}

VALUE mb_tcp_sl_new(VALUE self, VALUE ip_address, VALUE port, VALUE slave)
{
    modbus_slave_t *mb_slave;
    mb_slave = malloc(sizeof(modbus_slave_t));

    ip_address = rb_funcall(ip_address, rb_intern("to_s"), 0);
    port = rb_funcall(port, rb_intern("to_i"), 0);
    slave = rb_funcall(slave, rb_intern("to_i"), 0);

    modbus_init_tcp(&mb_slave->mb_param, RSTRING_PTR(ip_address), FIX2INT(port), FIX2INT(slave));

    int ret = modbus_mapping_new(&mb_slave->mb_map, 0, 0, 0, 0);
    if (ret < 0) {
        rb_raise(rb_eStandardError, "Memory allocation failed");
    }

    mb_slave->is_stoped = 1;
    
    return Data_Wrap_Struct(self, 0, mb_tcp_sl_free, mb_slave);
}

VALUE mb_tcp_sl_start(VALUE self)
{    
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    int ret = pthread_create(&mb_slave->tid, NULL, mb_serv, 
                                (void *)mb_slave);
    if (ret != 0) {
        rb_raise(rb_eStandardError, "Slave has not started");
    }
    
    mb_slave->is_stoped = 0;
    
    return self;
}

VALUE mb_tcp_sl_stop(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    int ret = pthread_cancel(mb_slave->tid);
    if (ret != 0) {
        rb_raise(rb_eStandardError, "Slave has not stoped");
    }
    
    mb_slave->is_stoped = 1;

    return self;
}

VALUE mb_tcp_sl_is_stoped(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    return mb_slave->is_stoped ? Qtrue : Qfalse;
}
