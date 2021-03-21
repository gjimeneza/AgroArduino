# Arduino Make file. Refiérase a https://github.com/sudar/Arduino-Makefile


# Cambiar variables según su ambiente
ARDUINO_DIR   = /usr/share/arduino
ARDMK_DIR     = /usr/share/arduino
AVR_TOOLS_DIR = /usr
AVRDUDE_CONF = $(BUNDLED_AVR_TOOLS_DIR)/etc/avrdude.conf

# Arduino UNO
BOARD_TAG    = uno
MONITOR_PORT = /dev/ttyS3

# Agregar bibliotecas adicionales a la lista
ARDUINO_LIBS += Wire
ARDUINO_LIBS += LiquidCrystal_I2C

# Directorio adicional de bibliotecas
# USER_LIB_PATH += libs/

include $(ARDMK_DIR)/Arduino.mk


