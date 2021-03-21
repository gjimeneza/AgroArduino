# AgroArduino

## Descripción
TBD

## Requisitos
* [Arduino IDE](https://www.arduino.cc/en/software) para compilar y subir a la placa.
* [Arduino-Makefile](https://github.com/sudar/Arduino-Makefile.git) para compilar y subir a la placa sin la GUI del IDE.

## Utilización
### Con Makefile

Se requiere primero configurar las variables del archivo [Makefile](Makefile) según el sistema utilizado.

Luego correr en terminal para compilar el programa:

```
make 
```
Para subir el programa a la placa:
```
make upload 
```