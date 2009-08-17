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



void mb_push_coil_status(modbus_slave_t *mb_slave)
{
    int len = RARRAY_LEN(mb_slave->coil_status);
    VALUE *ptr_coil = RARRAY_PTR(mb_slave->coil_status);
    mb_slave->mb_map->nb_coil_status = len; 
    mb_slave->mb_map->tab_coil_status = malloc(sizeof(uint8_t) * len);
    uint8_t *ptr_map = mb_slave->mb_map->tab_coil_status;

    int i;
    for (i = 0; i < len; i++) {
        *ptr_map = (*ptr_coil == Qtrue ? 1 : 0);
        ptr_coil++;
        ptr_map++;
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
    VALUE *ptr_input = RARRAY_PTR(mb_slave->input_status);
    mb_slave->mb_map->nb_input_status = len; 
    mb_slave->mb_map->tab_input_status = malloc(sizeof(uint8_t) * len);
    uint8_t *ptr_map = mb_slave->mb_map->tab_input_status;

    int i;
    for (i = 0; i < len; i++) {
        *ptr_map = (*ptr_input == Qtrue ? 1 : 0);
        ptr_input++;
        ptr_map++;
    }
}

void mb_push_holding_registers(modbus_slave_t *mb_slave)
{
    int len = RARRAY_LEN(mb_slave->holding_registers);
    VALUE *ptr_reg = RARRAY_PTR(mb_slave->holding_registers);
    mb_slave->mb_map->nb_holding_registers = len; 
    mb_slave->mb_map->tab_holding_registers = malloc(sizeof(uint16_t) * len);
    uint16_t *ptr_map = mb_slave->mb_map->tab_holding_registers;

    int i;
    for (i = 0; i < len; i++) {
        *ptr_map = FIX2INT(*ptr_reg);
        ptr_reg++;
        ptr_map++;
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
    VALUE *ptr_reg = RARRAY_PTR(mb_slave->input_registers);
    mb_slave->mb_map->nb_input_registers = len; 
    mb_slave->mb_map->tab_input_registers = malloc(sizeof(uint16_t) * len);
    uint16_t *ptr_map = mb_slave->mb_map->tab_input_registers;

    int i;
    for (i = 0; i < len; i++) {
        *ptr_map = FIX2INT(*ptr_reg);
        ptr_reg++;
        ptr_map++;
    }
}

VALUE mb_sl_is_stoped(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    if (read(mb_slave->listen_sock, NULL, 0) 
            || mb_slave->listen_sock == 0) { 
        return Qtrue;
    }
    return Qfalse;
}

VALUE mb_sl_get_coil_status(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    return mb_slave->coil_status;
}


VALUE mb_sl_set_coil_status(VALUE self, VALUE value)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    value = rb_funcall(value, rb_intern("to_a"), 0);
    mb_slave->coil_status = rb_ary_dup(value);
    
    return mb_slave->coil_status;
}

VALUE mb_sl_get_input_status(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    return mb_slave->input_status;
}

VALUE mb_sl_set_input_status(VALUE self, VALUE value)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    value = rb_funcall(value, rb_intern("to_a"), 0);
    mb_slave->input_status = rb_ary_dup(value);
        
    return mb_slave->input_status;
}

VALUE mb_sl_get_holding_registers(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    return mb_slave->holding_registers;
}

VALUE mb_sl_set_holding_registers(VALUE self, VALUE value)
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

VALUE mb_sl_get_input_registers(VALUE self)
{
    modbus_slave_t *mb_slave;
    Data_Get_Struct(self, modbus_slave_t, mb_slave);

    return mb_slave->input_registers;
}

VALUE mb_sl_set_input_registers(VALUE self, VALUE value)
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
