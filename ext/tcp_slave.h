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

#ifndef TCP_SLAVE_H
#define TCP_SLAVE_H

extern VALUE mb_tcp_sl_new(VALUE self, VALUE ip_address, VALUE port, VALUE id);
extern VALUE mb_tcp_sl_start(VALUE self);
extern VALUE mb_tcp_sl_stop(VALUE self);
extern VALUE mb_tcp_sl_is_stoped(VALUE self);
extern VALUE mb_tcp_sl_get_coil_status(VALUE self);
extern VALUE mb_tcp_sl_set_coil_status(VALUE self, VALUE value);
extern VALUE mb_tcp_sl_get_input_status(VALUE self);
extern VALUE mb_tcp_sl_set_input_status(VALUE self, VALUE value);
extern VALUE mb_tcp_sl_get_holding_registers(VALUE self);
extern VALUE mb_tcp_sl_set_holding_registers(VALUE self, VALUE value);
#endif
