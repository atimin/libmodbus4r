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

void mb_rtu_mstr_free(modbus_param_t *mb_param )
{
    modbus_close(mb_param);
    free(mb_param);
}

VALUE mb_rtu_mstr_new(VALUE self, VALUE device, VALUE baud, 
                            VALUE parity, VALUE data_bit, VALUE stop_bit)
{
    modbus_param_t *mb_param;

    mb_param = malloc(sizeof(modbus_param_t));
    device = rb_funcall(device, rb_intern("to_s"), 0);
    baud = rb_funcall(baud, rb_intern("to_i"), 0);
    parity = rb_funcall(parity, rb_intern("to_s"), 0);
    data_bit = rb_funcall(data_bit, rb_intern("to_i"), 0);
    stop_bit = rb_funcall(stop_bit, rb_intern("to_i"), 0);

    modbus_init_rtu(mb_param, RSTRING_PTR(device), FIX2INT(baud),
                    RSTRING_PTR(parity), FIX2INT(data_bit), FIX2INT(stop_bit));

    return Data_Wrap_Struct(self, 0, mb_rtu_mstr_free, mb_param);
}
