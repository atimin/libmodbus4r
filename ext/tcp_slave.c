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

void mb_tcp_sl_free(modbus_param_t *mb_param)
{
    modbus_close(mb_param);
    free(mb_param);
}

VALUE mb_tcp_sl_new(VALUE self, VALUE ip_address, VALUE port, VALUE slave)
{
    modbus_param_t *mb_param;

    mb_param = malloc(sizeof(modbus_param_t));
    ip_address = rb_funcall(ip_address, rb_intern("to_s"), 0);
    port = rb_funcall(port, rb_intern("to_i"), 0);
    slave = rb_funcall(slave, rb_intern("to_i"), 0);

    modbus_init_tcp(mb_param, RSTRING_PTR(ip_address), FIX2INT(port), FIX2INT(slave));

    return Data_Wrap_Struct(self, 0, mb_tcp_sl_free, mb_param);
}

VALUE mb_tcp_sl_start(VALUE self)
{    
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    return self;
}

VALUE mb_tcp_sl_stop(VALUE self)
{
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    return self;
}

VALUE mb_tcp_sl_is_stoped(VALUE self)
{
    modbus_param_t *mb_param;
    Data_Get_Struct(self, modbus_param_t, mb_param);

    if (read(mb_param->fd, NULL, 0) 
        || mb_param->fd == 0) { 
        return Qtrue;
    }

    return Qfalse;
}
