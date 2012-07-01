fuzzy_boar
==========

Arduino software to control a quadcopter

Connections
===========
zlutowalismy zworke CLK(gyro)
3.3v z VIO(gyro)
SCL z SCL(gyro)
SDA z SDA(gyro)
3.3v z VDD(gyro)
GND z GND(gyro)
GND z FSYNC(gyro)
PIN2 (chyba) z INT(gyro)

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
