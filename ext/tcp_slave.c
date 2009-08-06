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
#include "modbus4r.h"
#include "errors.h"

typedef struct {
    VALUE id;
    pthread_t sl_thread;
    uint8_t sl_thr_stoped;
    modbus_param_t mb_param;
} mb_tcp_sl_param_t;

void mb_tcp_sl_free(mb_tcp_sl_param_t *mb_tcp_sl_param )
{
    modbus_close(&mb_tcp_sl_param->mb_param);
    pthread_cancel(mb_tcp_sl_param->sl_thread);
    free(mb_tcp_sl_param);
}

void *mb_tcp_sl_thread(void *arg)
{
    while (1)
        continue;
}

VALUE mb_tcp_sl_new(VALUE self, VALUE ip_address, VALUE port, VALUE id)
{
    mb_tcp_sl_param_t *mb_tcp_sl_param;

    mb_tcp_sl_param = malloc(sizeof(mb_tcp_sl_param_t));
    ip_address = rb_funcall(ip_address, rb_intern("to_s"), 0);
    port = rb_funcall(port, rb_intern("to_i"), 0);
    mb_tcp_sl_param->id = rb_funcall(id, rb_intern("to_i"), 0);

    modbus_init_tcp(&mb_tcp_sl_param->mb_param, RSTRING_PTR(ip_address), FIX2INT(port));

    return Data_Wrap_Struct(self, 0, mb_tcp_sl_free, mb_tcp_sl_param);
}

VALUE mb_tcp_sl_id(VALUE self)
{
    mb_tcp_sl_param_t *mb_tcp_sl_param;
    Data_Get_Struct(self, mb_tcp_sl_param_t, mb_tcp_sl_param);

    return  mb_tcp_sl_param->id;
}

VALUE mb_tcp_sl_start(VALUE self)
{    
    mb_tcp_sl_param_t *mb_tcp_sl_param;
    Data_Get_Struct(self, mb_tcp_sl_param_t, mb_tcp_sl_param);

    int ret = pthread_create(&mb_tcp_sl_param->sl_thread, 
            NULL, mb_tcp_sl_thread, NULL);
    if (ret != 0) {
        rb_raise(eModBusError, "Slave has't started (%i)", ret);
    }

    mb_tcp_sl_param->sl_thr_stoped = 0;

    return self;
}

VALUE mb_tcp_sl_stop(VALUE self)
{
    mb_tcp_sl_param_t *mb_tcp_sl_param;
    Data_Get_Struct(self, mb_tcp_sl_param_t, mb_tcp_sl_param);

    int ret = pthread_cancel(mb_tcp_sl_param->sl_thread);
    if (ret != 0) {
        rb_raise(eModBusError, "Slave has't stoped (%i)", ret);
    }

    mb_tcp_sl_param->sl_thr_stoped = 1;
    return self;
}

VALUE mb_tcp_sl_is_stoped(VALUE self)
{
    mb_tcp_sl_param_t *mb_tcp_sl_param;
    Data_Get_Struct(self, mb_tcp_sl_param_t, mb_tcp_sl_param);

    if (mb_tcp_sl_param->sl_thr_stoped) { 
        return Qtrue;
    }

    return Qfalse;
}

VALUE mb_tcp_sl_join(VALUE self)
{
    mb_tcp_sl_param_t *mb_tcp_sl_param;
    Data_Get_Struct(self, mb_tcp_sl_param_t, mb_tcp_sl_param);

    pthread_join(mb_tcp_sl_param->sl_thread, NULL);

    return self;
}
