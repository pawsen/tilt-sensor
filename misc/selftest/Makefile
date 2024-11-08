# makefiles example
# https://github.com/ladislas/Bare-Arduino-Project/blob/master/Makefile-Linux.mk

# make help
# make upload
# make monitor
# make show_board
# make show_submenu

TARGET=uno
#TARGET=micro
ifeq ($(TARGET), uno)
    MONITOR_PORT = /dev/ttyACM0
    BOARD_TAG = uno
else
    MONITOR_PORT = /dev/ttyUSB0
    BOARD_TAG = pro
    BOARD_SUB = 16MHzatmega328
endif

MONITOR_BAUDRATE  = 9600

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(mkfile_path) ))
USER_LIB_PATH = $(abspath $(current_dir)/../../lib)

# Wire: I2C communication. SPI: not used but included by Adafruit_BusIO
ARDUINO_LIBS := Wire I2Cdev ADXL345

include $(ARDMK_DIR)/Arduino.mk
