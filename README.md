# AgroArduino

## Descripción
Sensor de Humedad de Suelo y Luz Solar

Toma medidas de sensores de humedad del suelo y de una celda fotorresistiva
y las despliega en una pantalla LCD.

El sensor de humedad se encuentra en el pin A0 y el de la luz en el A1. Los
sensores son alimentados con los pines digitales D8 y D9, mientras que la
pantalla LCD es alimentado del pin 5V del Arduino

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
