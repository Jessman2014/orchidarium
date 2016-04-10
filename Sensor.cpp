/*
 *  *      Driver for the MaxDetect series sensors
 *
 * Copyright (c) 2012-2013 Gordon Henderson. <projects@drogon.net>
 ***********************************************************************
 * This file is part of wiringPi:
 *      https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <wiringPi.h>
#include <maxdetect.h>

#define RHT03_PIN       1
#define HOST "localhost"
#define USER "root"
#define PASS "inl@NDj0b"
#define DB "SensorData"

 /*
  ***********************************************************************
  * The main program
  ***********************************************************************
  */

using namespace std;

int insertTempHum(float temp, float hum);

int main(void)
{
	int temp, rh;
	int newTemp, newRh;
	float tempF, rhF;

	cout << "got to initialization" << endl;
	temp = rh = newTemp = newRh = tempF = 0;

	wiringPiSetup();
	piHiPri(55);
	cout << "sleeping" << endl;
	//for (;;)
	//{
	delay(1000);

	if (readRHT03(RHT03_PIN, &newTemp, &newRh)) {
		cout << "successful read " << newTemp << " " << newRh << endl;
		if ((temp != newTemp) || (rh != newRh))
		{
			temp = newTemp;
			rh = newRh;
			if ((temp & 0x8000) != 0) // Negative
			{
				temp &= 0x7FFF;
				temp = -temp;
			}
			//valid temp and humidity
			if (temp < 450 && rh < 1100 && temp > 50 && rh > 0) {
				tempF = (float)(newTemp * 1.8 + 320) / 10.0;
				rhF = (float)rh / 10.0;
				insertTempHum(tempF, rhF);
			}
		}
	}
	//}




	return 0;
}

int insertTempHum(float temp, float hum) {
	cout << "got to insert " << temp << " " << hum << endl;
	try {
		sql::Driver * driver = get_driver_instance();

		std::auto_ptr< sql::Connection > con(driver->connect(HOST, USER, PASS));
		con->setSchema(DB);

		std::auto_ptr< sql::PreparedStatement >  pstmt;

		//first temp then hum
		pstmt.reset(con->prepareStatement("CALL insertTempHumidity(?,?)"));
		std::ostringstream ss;
		ss << temp;
		pstmt->setString(1, ss.str());
		ss << hum;
		pstmt->setString(2, ss.str());

		pstmt->execute();
	}
	catch (sql::SQLException &e) {
		/*
		MySQL Connector/C++ throws three different exceptions:

		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		/* what() (derived from std::runtime_error) fetches error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
