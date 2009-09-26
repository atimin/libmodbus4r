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

#include <unistd.h>
#include "modbus4r.h"
#include "errors.h"

void mb_mstr_free(modbus_param_t *mb_param )
{
    modbus_close(mb_param);
    free(mb_param);
}


VALUE mb_mstr_is_closed(VALUE self)
{
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    if (read(mb_param->fd, NULL, 0) || mb_param->fd == 0) { 
        return Qtrue;
    }
    return Qfalse;
}

VALUE mb_mstr_connect(VALUE self)
{
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    if (modbus_connect(mb_param)) {
        rb_raise(eModBusError, "Connection refused");
    }
    return self;
}

VALUE mb_mstr_close(VALUE self)
{
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    modbus_close(mb_param);

    return self;
}

VALUE mb_mstr_read_coil(VALUE self, VALUE start_addr, VALUE nb, 
                    int (*func)(modbus_param_t*, int, int, uint8_t*))
{
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    start_addr = rb_funcall(start_addr, rb_intern("to_i"), 0);
    nb = rb_funcall(nb, rb_intern("to_i"), 0);
    
    uint8_t dest[FIX2INT(nb)];
    int status = (*func)(mb_param, FIX2INT(start_addr), FIX2INT(nb), dest);

    if (status < 0) {
        mb_raise_error(status);
    }

    int i;
    VALUE ret = rb_ary_new();
    for (i = 0; i < FIX2INT(nb); i++) {
        dest[i] == 1 ? rb_ary_push(ret, Qtrue) : rb_ary_push(ret, Qfalse);
    }

    return ret;
}

VALUE mb_mstr_read_coil_status(VALUE self, VALUE start_addr, VALUE nb)
{
    return mb_mstr_read_coil(self, start_addr, nb, read_coil_status);
}

VALUE mb_mstr_read_input_status(VALUE self, VALUE start_addr, VALUE nb)
{
    return mb_mstr_read_coil(self, start_addr, nb, read_input_status);
}

VALUE mb_mstr_read_registers(VALUE self, VALUE start_addr, VALUE nb,
                    int (*func)(modbus_param_t*, int, int, uint16_t*))
{
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    start_addr = rb_funcall(start_addr, rb_intern("to_i"), 0);
    nb = rb_funcall(nb, rb_intern("to_i"), 0);
    
    uint16_t dest[FIX2INT(nb)];
    int status = (*func)(mb_param, FIX2INT(start_addr), FIX2INT(nb), dest);

    if (status < 0) {
        mb_raise_error(status);
    }

    int i;
    VALUE ret = rb_ary_new();
    for (i = 0; i < FIX2INT(nb); i++) {
        rb_ary_push(ret, INT2FIX(dest[i]));
    }


    return ret;
}

VALUE mb_mstr_read_holding_registers(VALUE self, VALUE start_addr, VALUE nb)
{
    return mb_mstr_read_registers(self, start_addr, nb,
                                read_holding_registers);
}

VALUE mb_mstr_read_input_registers(VALUE self, VALUE start_addr, VALUE nb)
{
    return mb_mstr_read_registers(self, start_addr, nb,
                                read_input_registers);
}

VALUE mb_mstr_force_single_coil(VALUE self, VALUE coil_addr, VALUE state)
{
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    coil_addr = rb_funcall(coil_addr, rb_intern("to_i"), 0);

    int status = force_single_coil(mb_param, FIX2INT(coil_addr), (state == Qfalse ? 0 : 1));

    if (status < 0) {
        mb_raise_error(status);
    }

    return self;
}


VALUE mb_mstr_preset_single_register(VALUE self, VALUE reg_addr, VALUE value)
{
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    reg_addr = rb_funcall(reg_addr, rb_intern("to_i"), 0);
    value = rb_funcall(value, rb_intern("to_i"), 0);

    int status = preset_single_register(mb_param, FIX2INT(reg_addr), FIX2INT(value));

    if (status < 0) {
        mb_raise_error(status);
    }

    return self;
}

VALUE mb_mstr_force_multiple_coils(VALUE self, VALUE start_addr, VALUE data) 
{
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    start_addr = rb_funcall(start_addr, rb_intern("to_i"), 0);
    data = rb_funcall(data, rb_intern("to_a"), 0);

    uint8_t buf[RARRAY_LEN(data)];
    VALUE *ary = RARRAY_PTR(data);
    int i;
    
    for (i = 0; i < RARRAY_LEN(data); i++) {
        buf[i] = (*ary == Qfalse ? 0 : 1);
        ary++;
    }

    int status = force_multiple_coils(mb_param, FIX2INT(start_addr), RARRAY_LEN(data), buf);

    if (status < 0) {
        mb_raise_error(status);
    }

    return self;
}

VALUE mb_mstr_preset_multiple_registers(VALUE self, VALUE start_addr, VALUE data)
{
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    start_addr = rb_funcall(start_addr, rb_intern("to_i"), 0);
    data = rb_funcall(data, rb_intern("to_a"), 0);

    uint16_t buf[RARRAY_LEN(data)];
    VALUE *ary = RARRAY_PTR(data);
    int i;
    
    for (i = 0; i < RARRAY_LEN(data); i++) {
        *ary = rb_funcall(*ary, rb_intern("to_i"), 0);
        buf[i] = FIX2INT(*ary);
        ary++;
    }

    int status = preset_multiple_registers(mb_param, FIX2INT(start_addr), RARRAY_LEN(data), buf);

    if (status < 0) {
        mb_raise_error(status);
    }

    return self;
}
