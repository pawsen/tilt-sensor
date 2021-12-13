# makefiles example
# https://github.com/ladislas/Bare-Arduino-Project/blob/master/Makefile-Linux.mk
# make help


#MONITOR_PORT = /dev/ttyUSB0
MONITOR_PORT = /dev/ttyACM1

MONITOR_BAUDRATE  = 115200
MONITOR_BAUDRATE  = 9600

# make monitor
# make show_board
# make show_submenu
BOARD_TAG = uno

BOARD_TAG = pro
BOARD_SUB = 16MHzatmega328

BOARD_TAG = micro
# BOARD_TAG = diecimila
# BOARD_SUB=atmega328

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(mkfile_path) ))
USER_LIB_PATH = $(abspath $(current_dir)/../lib)
ARDUINO_LIBS := Adafruit_Sensor Adafruit_ADXL345 Adafruit_BusIO Wire SPI SoftwareSerial NeoSWSerial
ARDUINO_LIBS += I2Cdev ADXL345

include $(ARDMK_DIR)/Arduino.mk

# !!! Important. You have to use 'make ispload' when using an ISP.

# enable flymake-mode in ino-files
check-syntax:
	$(CXX) -c -include Arduino.h   -x c++ $(CXXFLAGS)   $(CPPFLAGS)  -fsyntax-only $(CHK_SOURCES)
