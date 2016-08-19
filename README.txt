* Zaber control using Arduino	*
* Michael Lipski		*
* Summer 2016			*

Repository of sketches used in testing Zaber X-LRM200A linear stages and T-RS60A rotational stages.  Code for tracking algorithms is built upon some of the functions tested in these sketches.

Copy Zaber_X folder into your Arduino/libraries folder before using.

* *  Sketch Descriptions * *
joystick_control: Control crossed Zaber X-LRM200A stages using an analog stick
linear_stage_control_binary: Control linear stages via serial commands; using Zaber Binary protocol
linear_stage_control_test: Control linear stages via serial commands; using Zaber ASCII protocol
rotary_stage_commands: Control rotary stages via serial commands; using Zaber binary protocol
serial_commands_test: Test sketch provided by Zaber engineer; demonstrates some ASCII commands