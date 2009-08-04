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
#include "master.h"

VALUE mb_tcp_mstr_new(VALUE self, VALUE ip_address, VALUE port)
{
    modbus_param_t *mb_param;

    mb_param = malloc(sizeof(modbus_param_t));
    ip_address = rb_funcall(ip_address, rb_intern("to_s"), 0);
    port = rb_funcall(port, rb_intern("to_i"), 0);

    modbus_init_tcp(mb_param, RSTRING_PTR(ip_address), FIX2INT(port));

    return Data_Wrap_Struct(self, 0, mb_mstr_free, mb_param);
}
