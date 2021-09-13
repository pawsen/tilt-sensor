# makefiles example
# https://github.com/ladislas/Bare-Arduino-Project/blob/master/Makefile-Linux.mk

# make help
# make upload
# make monitor
# make show_board
# make show_submenu

TARGET=uno
TARGET=micro
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
USER_LIB_PATH = $(abspath $(current_dir)/../lib)

# Wire: I2C communication. SPI: not used but included by Adafruit_BusIO
ARDUINO_LIBS := Adafruit_Sensor Adafruit_ADXL345 Adafruit_BusIO Wire SPI
ARDUINO_LIBS += BigCrystal LiquidCrystal_I2C

# define USE_I2C for #ifdef in BigCrystal.h
CPPFLAGS += -DUSE_I2C=1

include $(ARDMK_DIR)/Arduino.mk

# !!! Important. You have to use 'make ispload' when using an ISP.

# enable flymake-mode in ino-files
check-syntax:
	$(CXX) -c -include Arduino.h   -x c++ $(CXXFLAGS)   $(CPPFLAGS)  -fsyntax-only $(CHK_SOURCES)
