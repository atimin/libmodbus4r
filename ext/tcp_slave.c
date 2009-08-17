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

void mb_tcp_sl_free(modbus_slave_t *mb_slave)
{
    close(mb_slave->listen_sock);
    modbus_close(mb_slave->mb_param);

    modbus_mapping_free(mb_slave->mb_map);
    rb_ary_free(mb_slave->coil_status); 

    free(mb_slave);
}

void *mb_session_serv(void *arg)
{
    modbus_slave_t *mb_slave = (modbus_slave_t *)arg;
    
    uint8_t query[MAX_MESSAGE_LENGTH];
    int query_size;
    while(1) {
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
            pthread_exit(NULL);
        } else {
             mb_raise_error(ret);
        }
    }

}

void *mb_serv(void *arg)
{
    modbus_slave_t *mb_slave = (modbus_slave_t *)arg;

    mb_slave->listen_sock = modbus_slave_listen_tcp(mb_slave->mb_param, mb_slave->max_conn);

    while (1) {
        modbus_slave_accept_tcp(mb_slave->mb_param, 
                                &mb_slave->listen_sock);
        pthread_t tid;
        pthread_create(&tid, NULL, mb_session_serv, (void *)mb_slave); 
    }
}

VALUE mb_tcp_sl_new(VALUE self, VALUE ip_address, VALUE port, VALUE slave)
{
    modbus_slave_t *mb_slave;
    mb_slave = malloc(sizeof(modbus_slave_t));
    mb_slave->mb_param = malloc(sizeof(modbus_param_t));
    mb_slave->mb_map = malloc(sizeof(modbus_mapping_t));

    ip_address = rb_funcall(ip_address, rb_intern("to_s"), 0);
    port = rb_funcall(port, rb_intern("to_i"), 0);
    slave = rb_funcall(slave, rb_intern("to_i"), 0);

    modbus_init_tcp(mb_slave->mb_param, RSTRING_PTR(ip_address), FIX2INT(port), FIX2INT(slave));

    int ret = modbus_mapping_new(mb_slave->mb_map, 0, 0, 0, 0);
    if (ret < 0) {
        rb_raise(rb_eStandardError, "Memory allocation failed");
    }
    
    mb_slave->coil_status = rb_ary_new();
    mb_slave->input_status = rb_ary_new();
    mb_slave->holding_registers = rb_ary_new();
    mb_slave->input_registers = rb_ary_new();
    mb_slave->max_conn = 4;
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
    
    close(mb_slave->listen_sock);

    return self;
}

VALUE mb_tcp_sl_get_maxconn_stop(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    return INT2FIX(mb_slave->max_conn);
}

VALUE mb_tcp_sl_set_maxconn_stop(VALUE self, VALUE value)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    value = rb_funcall(value, rb_intern("to_i"), 0);
    mb_slave->max_conn = FIX2INT(value);

    return INT2FIX(mb_slave->max_conn);
}
