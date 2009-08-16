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
#include "errors.h"

typedef struct {
    pthread_t tid;
    int listen_sock;
    modbus_param_t *mb_param;
    modbus_mapping_t *mb_map;
    VALUE coil_status;
    VALUE input_status;
    VALUE holding_registers;
    VALUE input_registers;
} modbus_slave_t;

void mb_tcp_sl_free(modbus_slave_t *mb_slave)
{
    close(mb_slave->listen_sock);
    modbus_close(mb_slave->mb_param);

    modbus_mapping_free(mb_slave->mb_map);
    rb_ary_free(mb_slave->coil_status); 

    free(mb_slave);
}

void mb_push_coil_status(modbus_slave_t *mb_slave)
{
    int len = RARRAY_LEN(mb_slave->coil_status);
    VALUE *coils = RARRAY_PTR(mb_slave->coil_status);
    mb_slave->mb_map->nb_coil_status = len; 
    mb_slave->mb_map->tab_coil_status = malloc(sizeof(uint8_t) * len);
    uint8_t *ptr = mb_slave->mb_map->tab_coil_status;

    int i;
    for (i = 0; i < len; i++) {
        *ptr = (*coils == Qtrue ? 1 : 0);
        ptr++;
        coils++;
    }
}

void mb_pull_coil_status(modbus_slave_t *mb_slave)
{
    uint8_t *ptr_map = mb_slave->mb_map->tab_coil_status;
    VALUE *ptr_coil = RARRAY_PTR(mb_slave->coil_status);
    int i;
    for(i = 0; i < mb_slave->mb_map->nb_coil_status; i++) {
       *ptr_coil = (*ptr_map == 0 ? Qfalse : Qtrue);
       ptr_coil++;
       ptr_map++;
    }
}

void mb_push_input_status(modbus_slave_t *mb_slave)
{
    int len = RARRAY_LEN(mb_slave->input_status);
    VALUE *inputs = RARRAY_PTR(mb_slave->input_status);
    mb_slave->mb_map->nb_input_status = len; 
    mb_slave->mb_map->tab_input_status = malloc(sizeof(uint8_t) * len);
    uint8_t *ptr = mb_slave->mb_map->tab_input_status;

    int i;
    for (i = 0; i < len; i++) {
        *ptr = (*inputs == Qtrue ? 1 : 0);
        ptr++;
        inputs++;
    }
}

void mb_push_holding_registers(modbus_slave_t *mb_slave)
{
    int len = RARRAY_LEN(mb_slave->holding_registers);
    VALUE *regs = RARRAY_PTR(mb_slave->holding_registers);
    mb_slave->mb_map->nb_holding_registers = len; 
    mb_slave->mb_map->tab_holding_registers = malloc(sizeof(uint16_t) * len);
    uint16_t *ptr = mb_slave->mb_map->tab_holding_registers;

    int i;
    for (i = 0; i < len; i++) {
        *ptr = FIX2INT(*regs);
        ptr++;
        regs++;
    }
}

void mb_pull_holding_registers(modbus_slave_t *mb_slave)
{
    uint16_t *ptr_map = mb_slave->mb_map->tab_holding_registers;
    VALUE *ptr_reg = RARRAY_PTR(mb_slave->holding_registers);
    int i;
    for(i = 0; i < mb_slave->mb_map->nb_holding_registers; i++) {
       *ptr_reg = INT2FIX(*ptr_map);
       ptr_reg++;
       ptr_map++;
    }
}

void mb_push_input_registers(modbus_slave_t *mb_slave)
{
    int len = RARRAY_LEN(mb_slave->input_registers);
    VALUE *regs = RARRAY_PTR(mb_slave->input_registers);
    mb_slave->mb_map->nb_input_registers = len; 
    mb_slave->mb_map->tab_input_registers = malloc(sizeof(uint16_t) * len);
    uint16_t *ptr = mb_slave->mb_map->tab_input_registers;

    int i;
    for (i = 0; i < len; i++) {
        *ptr = FIX2INT(*regs);
        ptr++;
        regs++;
    }
}

void *mb_serv(void *arg)
{
    modbus_slave_t *mb_slave = (modbus_slave_t *)arg;

    mb_slave->listen_sock = modbus_slave_listen_tcp(mb_slave->mb_param, 1);

    while (1) {
        modbus_slave_accept_tcp(mb_slave->mb_param, 
                                &mb_slave->listen_sock);

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
                break;
            } else {
                mb_raise_error(ret);
            }
        }
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

VALUE mb_tcp_sl_is_stoped(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    if (read(mb_slave->listen_sock, NULL, 0) 
            || mb_slave->listen_sock == 0) { 
        return Qtrue;
    }
    return Qfalse;
}

VALUE mb_tcp_sl_get_coil_status(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    return mb_slave->coil_status;
}


VALUE mb_tcp_sl_set_coil_status(VALUE self, VALUE value)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    value = rb_funcall(value, rb_intern("to_a"), 0);
    mb_slave->coil_status = rb_ary_dup(value);
    
    return mb_slave->coil_status;
}

VALUE mb_tcp_sl_get_input_status(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    return mb_slave->input_status;
}

VALUE mb_tcp_sl_set_input_status(VALUE self, VALUE value)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    value = rb_funcall(value, rb_intern("to_a"), 0);
    mb_slave->input_status = rb_ary_dup(value);
        
    return mb_slave->input_status;
}

VALUE mb_tcp_sl_get_holding_registers(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    return mb_slave->holding_registers;
}

VALUE mb_tcp_sl_set_holding_registers(VALUE self, VALUE value)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    value = rb_funcall(value, rb_intern("to_a"), 0);
    mb_slave->holding_registers = rb_ary_dup(value);
    VALUE *reg = RARRAY_PTR(mb_slave->holding_registers);
    
    int i;
    for (i = 0; i < RARRAY_LEN(mb_slave->holding_registers); i++) {
         *reg = rb_funcall(*reg, rb_intern("to_i"), 0);
    }
        
    return mb_slave->holding_registers;
}

VALUE mb_tcp_sl_get_input_registers(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    return mb_slave->input_registers;
}

VALUE mb_tcp_sl_set_input_registers(VALUE self, VALUE value)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    value = rb_funcall(value, rb_intern("to_a"), 0);
    mb_slave->input_registers = rb_ary_dup(value);
    VALUE *reg = RARRAY_PTR(mb_slave->input_registers);
    
    int i;
    for (i = 0; i < RARRAY_LEN(mb_slave->input_registers); i++) {
         *reg = rb_funcall(*reg, rb_intern("to_i"), 0);
    }
        
    return mb_slave->input_registers;
}
