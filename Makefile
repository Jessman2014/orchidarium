#
# Makefile:
#       wiringPi - Wiring Compatable library for the Raspberry Pi
#       https://projects.drogon.net/wiring-pi
#
#       Copyright (c) 2012-2015 Gordon Henderson
#################################################################################
# This file is part of wiringPi:
#       Wiring Compatable library for the Raspberry Pi
#
#    wiringPi is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    wiringPi is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
#################################################################################

all:    g++ -o sensor.out -I/usr/local/include -I/usr/local/include/cppconn -lmysqlcppconn -L/usr/local/lib -lwiringPi -lwiringPiDev -lpthread -lm -Wall Sensor.cpp

clean:  rm *.out