/*
 * Hermasbox.h
 *
 *  Created on: 10 août 2017
 *      Author: Halim DJERROUD
 */

#include <stdio.h>

#ifndef HERMAS_BOX_H_
#define HERMAS_BOX_H_



#include <MemoryFree.h>
#include "sim808.h"
#include "hermas_config.h"
#define BUFFER_RMC 90

class Hermas_box {
public:

	/**
	 * init functions
	 */

	static void init_hermas_box(void);

	static void init_hermas_sim808(void);

	static void init_hermas_GPS(void);

	static void init_hermas_GPRS(void);

	static void init_hermas_CAN(void);

	/**
	 * closes connexions
	 */

	static void close_GPS_deconnexion(void);

	static void close_GPRS_deconnexion(void);

	static void close_CAN_connexion(void);



	/**
	 * get data
	 */

	static int get_signal_force(void);

	static void show_phone_number (void);

	static void get_GPS_data (void);

	static void send_data ();


	/**
	 * get eeprom data
	 */

	static String get_serial_number (void);

	static String get_apn (void);

	static String get_apn_user (void);

	static String get_apn_pass (void);

	static void   signalStrength (void);


	static void debugMemory();

	/**
	 * debug
	 */
	static void debug(String, boolean newLine = true);

	static bool startup_message(void);

	static boolean getScreenDebug() {
		return screen_debug;
	}

	static HardwareSerial *mySerial;

	static char receivedStack[BUFFER_RMC];


private:

	static char phoneNumber[15];





	static boolean screen_debug;

	static char serial_number[10];

	static bool init();

	static void read_eeprom_data(void);

	static bool attachGPS();

	static bool detachGPS();

	static int getGPS();

	static bool getSignalStrength(int *buffer);

	static int join(const __FlashStringHelper *apn, const __FlashStringHelper *userName, const __FlashStringHelper *passWord);

	static int send(const char * str, int len);

	static bool close();

	static void disconnect();

	static int recv(char* buf, int len);

	static void getPhoneNumber(void);

	static bool checkSIMStatus(void);

	static bool  getGPRMC();

	static bool parseGPRMC(char *gpsbuffer);

	static bool is_connected(void);


};


#endif /* HERMAS_BOX_H_ */
