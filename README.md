fuzzy_boar
==========

Arduino software to control a quadcopter

Connections
===========

Solder CLK on MPU6050

```
Gyro  | Arduino
------+--------
VIO   | 3.3v
SCL   | SCL
SDA   | SDA
VDD   | 3.3v
GND   | GND
FSYNC | GND
INT   | PIN2
```

Setup
=====

Install `ino` for CLI arduino

`$ make setup`

Building
========

`$ make build`

Running
=======
`$ make upload`
