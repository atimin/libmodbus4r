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

#ifndef MASTER_H
#define MASTER_H

extern VALUE mb_mstr_is_closed(VALUE self);
extern VALUE mb_mstr_connect(VALUE self);
extern VALUE mb_mstr_close(VALUE self);
extern VALUE mb_mstr_read_coil_status(VALUE self, VALUE slave, 
                                    VALUE start_addr, VALUE nb);
extern VALUE mb_mstr_read_input_status(VALUE self, VALUE slave,
                                    VALUE start_addr, VALUE nb);
extern VALUE mb_mstr_read_holding_registers(VALUE self, VALUE slave,
                                    VALUE start_addr, VALUE nb);
extern VALUE mb_mstr_read_input_registers(VALUE self, VALUE slave,
                                    VALUE start_addr, VALUE nb);
extern VALUE mb_mstr_force_single_coil(VALUE self, VALUE slave,
                                    VALUE coil_addr, VALUE state);
extern VALUE mb_mstr_preset_single_register(VALUE self, VALUE slave,
                                    VALUE reg_addr, VALUE value);
extern VALUE mb_mstr_force_multiple_coils(VALUE self, VALUE slave, 
                                    VALUE start_addr, VALUE data);
extern VALUE mb_mstr_preset_multiple_registers(VALUE self, VALUE slave,
                                    VALUE start_addr, VALUE data);
#endif 
