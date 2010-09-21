#!/bin/bash
env CFLAGS=" -g -O2" LDFLAGS="-L/srv/preware/cross-compile/staging/armv7/usr/lib/ -Wl,--allow-shlib-undefined,-rpath-link=/srv/preware/cross-compile/staging/armv7/usr/lib/" ./configure $@
