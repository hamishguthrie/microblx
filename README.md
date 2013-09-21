MICROBLX: light, fast, hard real-time function blocks
=====================================================

What is it?
-----------

Microblx is an lightweight, dynamic, reflective, hard real-time safe
function block implementation:

 - Pure C, no external dependencies
 - Webinterface function block to introspect and control blocks
 - Lua scripting for system configuration and deployment
 - generic Lua scriptable function block
 - Dynamic type handling, no code-generation


Dependencies
------------

 - cproto (generating c prototypes)
 - luajit, libluajit-5.1-dev (>=2.0.0-beta11, for scripting (optional, but recommended)
 - clang (only necessary for compiling C++ blocks)

Building
--------

$ Just run "make".


FAQ
---

To run with real-time priorities, give the luajit binary `cap_sys_raw`
capabilities, e.g:

```
$ sudo setcap cap_sys_nice,cap_net_raw+ep /usr/local/bin/luajit-2.0.2
```


* Youbot Driver

The youbot driver furthermore requires `cap_net_raw` in order to open
a raw socket for EtherCAT communication.

```
$ sudo setcap cap_sys_nice,cap_net_raw+ep /usr/local/bin/luajit-2.0.2

```

License
-------

TBD. GPL with linking exception?


Acknowledgement
---------------

Microblx is considerably inspired by the OROCOS Real-Time
Toolkit. Other influences are the IEC standards covering function
block IEC-61131 and IEC-61499.