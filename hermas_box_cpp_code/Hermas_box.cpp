/*
 *
 *  Created on: 10 août 2017
 *      Author: thor
 */
#include <stdio.h>
#include <Arduino.h>
#include <EEPROM.h>

#include "Hermas_box.h"

#include "HermasFMS.h"
#include "HermasGPS.h"

boolean Hermas_box::screen_debug = false;
char Hermas_box::serial_number[];

HardwareSerial * Hermas_box::mySerial = NULL;

extern Stream *serialSIM808;

extern Hermas_GPS gps;
extern Hermas_FMS fms;

char Hermas_box::receivedStack[];

//char Hermas_box::lon[];
//
//char Hermas_box::lat[];
//
//char Hermas_box::hour[7];
//
//char Hermas_box::date[7];

char Hermas_box::phoneNumber[];

/**
 * init_hermas_box
 */
void Hermas_box::init_hermas_box(void) {

	Serial.begin(SPEED_SCREEN_SERIAL);
	Serial1.begin(SPEED_SIM_SERIAL);
	Hermas_box::mySerial = &Serial1;

	// configure debug mode
	pinMode(DEBUG_PIN, INPUT);
	delay(100);
	if (digitalRead(DEBUG_PIN) == HIGH) {
		Hermas_box::screen_debug = true;
	} else {
		Hermas_box::screen_debug = false;
	}

	Hermas_box::debug(" *** Init Hermas Box *** ");

	// read configure data
	Hermas_box::read_eeprom_data();

	sim808_init(mySerial, 1);

	for (byte i = 0; i < 5; i++) {
		Hermas_box::debug(" . ", false);
		delay(50);
	}

}

/**
 * debug
 */
void Hermas_box::debug(String debug, boolean newLine) {

	if (Hermas_box::screen_debug) {
		if (newLine) {
			Serial.println(debug);
		} else {
			Serial.print(debug);
		}

	}

}

/**
 * init_hermas_sim808
 */
void Hermas_box::init_hermas_sim808(void) {

	Hermas_box::debug(" *** Init Hermas Box SIM808*** ");

	while (!Hermas_box::init()) {
		Hermas_box::debug("Sim808 init error ");
		delay(500);
	}
	Hermas_box::debug("Sim808 init OK");
}

/**
 * init_hermas_GPRS
 */
void Hermas_box::init_hermas_GPRS(void) {

	Hermas_box::debug(" *** Init Hermas Box GPRS *** ");

	//*********** Attempt DHCP *******************

	int i = -1;
	while (i < 0) { //warring :change APN operator
		i = Hermas_box::join(F(APN_NAME), F(APN_USER), F(APN_PASS));
		if (i < 0) {
			Hermas_box::debug("Sim808 join network error");
			Hermas_box::debug((String) i);
			delay(1000);
		}
	}
	Hermas_box::debug(" -- Sim808 join network ok -- ");

}

/**
 * init_hermas_CAN
 */
void Hermas_box::init_hermas_CAN(void) {

}

/**
 * init_hermas_GPS
 */
void Hermas_box::init_hermas_GPS(void) {
	Hermas_box::debug(" *** Init GPS *** ");

	if (Hermas_box::attachGPS()) {
		Hermas_box::debug("Open the GPS power success");
	} else {
		Hermas_box::debug("Open the GPS power failure");
	}

}

/**
 * get_GPS_data
 */
void Hermas_box::get_GPS_data(void) {
	int error;
	int i;
	for (i = 0; i < 10; i++) {
		Hermas_box::debug("get GPS data : ");
		error = Hermas_box::getGPS();
		if (error == 0) {
			break;
		}
		delay(700);
		Hermas_box::debug("");
		Hermas_box::debug("Error receved data  : ", false);
		Hermas_box::debug((String) error);
		Hermas_box::debug(Hermas_box::receivedStack);
		Hermas_box::debug("");
		delay(5000);
	}
	delay(1000);
	if (i < 10) {
		Hermas_box::debug("data receved ok: ");
		Hermas_box::debug(Hermas_box::receivedStack);
	} else {
		Hermas_box::debug("No GPS Data :  ");
		Hermas_box::debug(Hermas_box::receivedStack);
	}
}

/**
 * close_GPS_connexion
 */
void Hermas_box::close_GPS_deconnexion(void) {
	Hermas_box::debug("... Close GPS Connxion ...");
	Hermas_box::detachGPS();
}

/**
 * send_data
 * 	//char data[200] = "GET /get/main!index!5!7 HTTP/1.1\r\nHost: www.hermas.halim.info\r\nUser-Agent: HermasBox\r\n\r\n";

 //	sprintf(data,
 //			"GET /get/main!index!%s HTTP/1.1\r\nHost: www.hermas.halim.info\r\nUser-Agent: HermasBox\r\n\r\n",
 //			receivedStack);
 *
 *
 * 	sprintf(data,"POST %s\r\n%s\r\n%s\r\n%s\r\n\r\n%s\r\n",
 "/get/main!index! HTTP/1.1",
 "Host: hermas.halim.info",
 "Content-Type: application/x-www-form-urlencoded",
 "Content-Length: 26",
 "name1=value1&name2=value2");
 *
 *
 */
void Hermas_box::send_data() {
	Hermas_box::debug("send_data ");

	char url[800];
	char data[512];
	char len[8];

	strcpy(data, "T=0&SN=");
	strcat(data, Hermas_box::serial_number);
	strcat(data, "&");
	strcat(data, gps.dataToSend);
	strcat(data, "&");
	strcat(data, fms.dataToSend);

	sprintf(len, "%d\r\n", strlen(data) + 2);

	sprintf(url, "POST %s\r\n%s\r\n%s\r\n", "/get/main!index! HTTP/1.1",
			"Host: hermas.halim.info",
			"Content-Type: application/x-www-form-urlencoded");

	strcat(url, "Content-Length: ");
	strcat(url, len);
	strcat(url, "\r\n\r\n");
	strcat(url, data);
	strcat(url, "\r\n");


	Hermas_box::debug("URL TO SEND :");
	Hermas_box::debug(url);
	delay(500);

	if (!sim808_check_with_cmd("AT+CIPSEND\r\n", ">", CMD)) {
		Hermas_box::debug("pas reçu : > ");
		return ;
	}
	delay(500);
	sim808_send_cmd(url);
	delay(500);
	sim808_send_End_Mark();

	delay(5000);


	sim808_clean_buffer(url, 512);
	sim808_read_buffer(url, 512, DEFAULT_TIMEOUT*2, DEFAULT_INTERCHAR_TIMEOUT*2);

	Hermas_box::debug("REPONSE:");
	Hermas_box::debug((String)url);
	Hermas_box::debug("END REPONSE:");
	delay(2000);


	if (NULL != strstr(url, "SEND OK")) { //ALREADY CONNECT or CONNECT OK
		Hermas_box::debug("---SEND NOT OK---");
		return ;
	}else{
		Hermas_box::debug("---SEND OK---");
		Hermas_box::debug("Envoi termine  ");
		return ;
	}


	Hermas_box::debug("fin send data ");
	delay(2000);
}

/**
 * close_GPRS_connexion
 */
void Hermas_box::close_GPRS_deconnexion(void) {
	Hermas_box::debug(" ... Close GPRS Connxion ...");
	//************* Close TCP or UDP connections **********
	Hermas_box::close();
	delay(5000);
	//*** Disconnect wireless connection, Close Moving Scene *******
	Hermas_box::disconnect();
	delay(3000);
}

/**
 * close_CAN_connexion
 */
void Hermas_box::close_CAN_connexion(void) {

}

/**
 * get_signal_force
 */
int Hermas_box::get_signal_force(void) {
}

/**
 * show_phone_number
 */
void Hermas_box::show_phone_number(void) {
	Hermas_box::getPhoneNumber();
	Hermas_box::debug("Phone number : ", false);
	Hermas_box::debug((String) phoneNumber);
}

/**
 * get_serial_number
 */
String Hermas_box::get_serial_number(void) {

}

/**
 * get_apn
 */
String Hermas_box::get_apn(void) {
}

/**
 * get_apn_user
 */
String Hermas_box::get_apn_user(void) {
}

/**
 * get_apn_pass
 */
String Hermas_box::get_apn_pass(void) {
}

void Hermas_box::signalStrength(void) {
	Hermas_box::debug("GPRS Signal : ");
	int i = 0;
	Hermas_box::getSignalStrength(&i);
	Hermas_box::debug((String) i);

}

/**
 * read_eeprom_data
 */
void Hermas_box::read_eeprom_data(void) {

	//read serial number

	Hermas_box::serial_number[0] = EEPROM.read(0)+'0';
	Hermas_box::serial_number[1] = EEPROM.read(1)+'0';
	Hermas_box::serial_number[2] = EEPROM.read(2)+'0';
	Hermas_box::serial_number[3] = '\0';

	if (Hermas_box::screen_debug) {
		Serial.print("Serial Number : ");
		Serial.print(Hermas_box::serial_number[0]);
		Serial.print(Hermas_box::serial_number[1]);
		Serial.println(Hermas_box::serial_number[2]);
	}

}

bool Hermas_box::attachGPS() {
	if (!sim808_check_with_cmd("AT+CGNSPWR=1\r\n", "OK\r\n", CMD)) {
		return false;
	}

	delay(500);

	if (!sim808_check_with_cmd("AT+CGPSOUT=32\r\n", "OK\r\n", CMD)) {
		return false;
	}

	delay(500);
	if (!sim808_check_with_cmd("AT+CGNSTST=0\r\n", "OK\r\n", CMD)) {
		return false;
	}
	delay(500);

	return true;
}

bool Hermas_box::detachGPS() {
	if (!sim808_check_with_cmd("AT+CGNSPWR=0\r\n", "OK\r\n", CMD)) {
		return false;
	}
	return true;
}

bool Hermas_box::init(void) {
	//Verifiez la commande AT est valide
	if (!sim808_check_with_cmd("AT\r\n", "OK\r\n", CMD)) {
		return false;
	}

	if (!sim808_check_with_cmd("AT+CGNSPWR=0\r\n", "OK\r\n", CMD)) {
		return false;
	}

	//Verifiez que la carte SIM peut appeler, envoyer des messages texte
	// 1 : OK
	if (!sim808_check_with_cmd("AT+CFUN=1\r\n", "OK\r\n", CMD)) {
		return false;
	}

	//Verifiez que l'etat de la carte SIM
	if (!checkSIMStatus()) {
		return false;
	}

	delay(3000);

	return true;
}

int Hermas_box::getGPS() {

	if (!getGPRMC()) {
		Hermas_box::debug("error getGPRMC ");
		return -1;
	}

	if (!parseGPRMC(Hermas_box::receivedStack)) {
		Hermas_box::debug("error parseGPRMC ");
		return -2;
	}

	// $GPRMC,165445.000,A,3110.8635,N,12133.4627,E,0.58,70.26,220916,,,A*57";
	//  GET http://home.halim.info/get/main!index!,,,,0.00,0,050180,!0,050180,

	if (Hermas_box::receivedStack[18] != 'A') {
//		Hermas_box::lat[0] = '0';Hermas_box::lat[1] = '\0';
//		Hermas_box::lon[0] = '0';Hermas_box::lon[1] = '\0';
//		Hermas_box::hour[0] = '0';Hermas_box::hour[1] = '\0';
//		Hermas_box::date[0] = '0';Hermas_box::date[1] = '\0';

		receivedStack[0] = '\0';
		return -3;
	}

	int i;

	for (i = 0; i < 150 != ','; i++) {
		if (receivedStack[i] == '*') {
			receivedStack[i + 3] = '\0';
			break;
		}
	}
	receivedStack[BUFFER_RMC-1] = '\0';

	/*

	 for (i=0; receivedStack[i+20]!=',' ; i++){
	 Hermas_box::lat[i] = Hermas_box::receivedStack[i+20];
	 }
	 Hermas_box::lat[i] =  Hermas_box::receivedStack[i+21];
	 i++;
	 Hermas_box::lat[i]='\0';



	 for (i=0; receivedStack[i+32]!=',' ; i++){
	 Hermas_box::lon[i] = Hermas_box::receivedStack[i+32];
	 }
	 Hermas_box::lon[i] =  Hermas_box::receivedStack[i+33];
	 i++;
	 Hermas_box::lon[i]='\0';


	 Hermas_box::hour[0]='\0';
	 for (i=0; receivedStack[i+7]!=',' ; i++){
	 Hermas_box::hour[i] = Hermas_box::receivedStack[i+7];
	 }
	 Hermas_box::hour[i]='\0';


	 Hermas_box::date[0]='\0';
	 for (i=0; receivedStack[i+7]!=',' ; i++){
	 Hermas_box::date[i] = Hermas_box::receivedStack[i+7];
	 }
	 Hermas_box::date[i]='\0';
	 */

	return 0;
}

bool Hermas_box::getSignalStrength(int *buffer) {
	//AT+CSQ						--> 6 + CR = 10
	//+CSQ: <rssi>,<ber>			--> CRLF + 5 + CRLF = 9
	//OK							--> CRLF + 2 + CRLF =  6

	byte i = 0;
	char gprsBuffer[26];
	char *p, *s;
	char buffers[4];
	sim808_flush_serial();
	sim808_send_cmd("AT+CSQ\r");
	sim808_clean_buffer(gprsBuffer, 26);
	sim808_read_buffer(gprsBuffer, 26, DEFAULT_TIMEOUT);
	if (NULL != (s = strstr(gprsBuffer, "+CSQ:"))) {
		s = strstr((char *) (s), " ");
		s = s + 1;  //We are in the first phone number character
		p = strstr((char *) (s), ","); //p is last character """
		if (NULL != s) {
			i = 0;
			while (s < p) {
				buffers[i++] = *(s++);
			}
			buffers[i] = '\0';
		}
		*buffer = atoi(buffers);
		return true;
	}
	return false;
}

int Hermas_box::join(const __FlashStringHelper *apn,
		const __FlashStringHelper *userName,
		const __FlashStringHelper *passWord) {
	char resp[96];
	char ipAddr[50];

	byte i;
	char *p, *s;

	sim808_send_cmd("AT+CIPSHUT\r\n");

	if (!sim808_check_with_cmd("AT+CGATT=1\r\n", "OK\r\n", CMD)) {
		return -2;
	}

	delay(500);
	if (!sim808_check_with_cmd("AT+CIPMUX=0\r\n", "OK\r\n", CMD)) {
		return -3;
	}
	delay(500);

	sim808_check_with_cmd("AT+CSTT=\"free\",\"login\",\"pass\"\r\n", "OK\r\n",
			CMD);

	delay(500);
	if (!sim808_check_with_cmd("AT+CIICR\r\n", "OK\r\n", CMD)) {
		return -4;
	}
	delay(500);

	sim808_send_cmd("AT+CIFSR\r\n");
	sim808_clean_buffer(ipAddr, 32);
	sim808_read_buffer(ipAddr, 32);

	delay(500);

	if (!sim808_check_with_cmd("AT+CIPSPRT=1\r\n", "OK\r\n", CMD)) {
		return -5;
	}

	delay(500);
	if (NULL != strstr(ipAddr, "ERROR")) {
		return -6;
	}

	//strcpy(ip_string,ipAddr);
//
//    s = ipAddr;
//    p = strstr((char *)(s),"\r\n"); //p is last character \r\n
//    if (NULL != s) {
//        i = 0;
//        while (s < p) {
//            ip_string[i++] = *(s++);
//        }
//        ip_string[i] = '\0';
//    }

//
//    Serial.print ("IP Address is : ");
//    Serial.println (ip_string);

	sim808_send_cmd("AT+CIPSTART=\"TCP\",\"hermas.halim.info\",\"80\"\r\n");
	delay(10000);
	sim808_clean_buffer(resp, 96);
	sim808_read_buffer(resp, 96, DEFAULT_TIMEOUT, DEFAULT_INTERCHAR_TIMEOUT);
	Hermas_box::debug("BUFF:");
	Hermas_box::debug((String) resp);

	if (NULL != strstr(resp, "CONNECT")) { //ALREADY CONNECT or CONNECT OK
		Hermas_box::debug("is connected");
		return 0;
	}
	Hermas_box::debug("not  connected");

	return -7;
}

int Hermas_box::send(const char * str, int len) {

	if (len > 0) {
		if (!sim808_check_with_cmd("AT+CIPSEND\r\n", ">", CMD)) {
			Hermas_box::debug("pas reçu : > ");
			return 0;
		}
		delay(500);
		sim808_send_cmd(str);
		delay(500);
		sim808_send_End_Mark();

//        if(!sim808_wait_for_resp("SEND OK", DATA, DEFAULT_TIMEOUT * 2, DEFAULT_INTERCHAR_TIMEOUT * 2)) {
//        	Hermas_box::debug("send not ok :");
//        	return -1;
//        }

		char resp[512];
		sim808_clean_buffer(resp, 512);
		sim808_read_buffer(resp, 512, DEFAULT_TIMEOUT*3, DEFAULT_INTERCHAR_TIMEOUT*3);
		Hermas_box::debug("REPONSE:");
		Hermas_box::debug((String) resp);

		if (NULL != strstr(resp, "SEND OK")) { //ALREADY CONNECT or CONNECT OK
			Hermas_box::debug("---SEND NOT OK---");
			return 0;
		}else{
			Hermas_box::debug("---SEND OK---");
		}

		Hermas_box::debug("Envoi termine  ");

	}
	return len;

}

bool Hermas_box::close() {
	// if not connected, return
	if (!is_connected()) {
		return true;
	}
	return sim808_check_with_cmd("AT+CIPCLOSE\r\n", "CLOSE OK\r\n", CMD);
}

void Hermas_box::disconnect() {
	sim808_send_cmd("AT+CIPSHUT\r\n");
}

int Hermas_box::recv(char* buf, int len) {
	sim808_clean_buffer(buf, len);
	sim808_read_buffer(buf, len); //Je l'ai deja appele la longueur tampon de fonction - 1, puis j'ajoute le 0
	Hermas_box::debug("buffer : ---begin ->");
	Hermas_box::debug((String) buf);
	Hermas_box::debug("buffer : ---end ->");
	return strlen(buf);
}

void Hermas_box::getPhoneNumber(void) {

	sim808_send_cmd(F("AT+CNUM\r\n")); // +CNUM: "Messagerie","+33769921847",145,0,4

	byte i;
	char *p, *s;
	char recv[50];

	sim808_clean_buffer(recv, 50);
	sim808_read_buffer(recv, 50);

	if (NULL != strstr(recv, "ERROR")) {
		phoneNumber[0] = '\0';
		return;
	}

	s = strstr((char *) (recv), "+33");
	p = strstr((char *) (s), "\""); //p is last character
	if (NULL != s) {
		i = 0;
		while (s < p) {
			phoneNumber[i++] = *(s++);
		}
		phoneNumber[i] = '\0';
	}
}

bool Hermas_box::checkSIMStatus(void) {
	char gprsBuffer[32];
	int count = 0;
	sim808_clean_buffer(gprsBuffer, 32);
	while (count < 3) {
		sim808_send_cmd("AT+CPIN?\r\n");
		sim808_read_buffer(gprsBuffer, 32, DEFAULT_TIMEOUT);
		if ((NULL != strstr(gprsBuffer, "+CPIN: READY"))) { //SIM represente l'etat normal
			break;
		}
		count++;
		delay(300);
	}
	if (count == 3) {
		return false;
	}
	return true;
}

bool Hermas_box::getGPRMC() {
	//char *Hermas_box::receivedStack="$GPRMC,165445.000,A,3110.8635,N,12133.4627,E,0.58,70.26,220916,,,A*57";
	char c;
	static bool endflag = false;
	static char count;

	unsigned int receivedStackIndex = 0;
	Serial1.flush();
	while (serialSIM808->available()) {

		c = serialSIM808->read();

	//	Hermas_box::debug((String)c);

		if (endflag == true) {
			if (count--) {
				if (c == '$') {
					return false;
				}
				Hermas_box::receivedStack[receivedStackIndex++] = c;
			} else {
				endflag = false;
				Hermas_box::receivedStack[receivedStackIndex] = '\0';
				return true;
			}
		} else {
			switch (c) {

			case '$':
				receivedStackIndex = 0;
				Hermas_box::receivedStack[receivedStackIndex++] = c;
				break;
			case '*':
				endflag = true;
				count = 2;
				Hermas_box::receivedStack[receivedStackIndex++] = c;
				break;
			default:
				if (receivedStackIndex < BUFFER_RMC-2) {
					Hermas_box::receivedStack[receivedStackIndex++] = c;
				} else {
					Hermas_box::receivedStack[receivedStackIndex] = '\0';
					return false;
				}

				break;
			}
		}
	}

	return false;
}

bool Hermas_box::parseGPRMC(char *gpsbuffer) {

	if (strstr(gpsbuffer, "$GPRMC") == NULL) // $ GPRMC n'est pas le debut d'une chaîne d'information GPS
	{
		return false;
	} else {
		if (gpsbuffer[18] == 'A')    //  determiner si les donnees sont valides)
			return true;
		if (gpsbuffer[18] == 'V')   //  determiner si les donnees sont invalides
			return true;
		else {
			return false;
		}

	}

}

void Hermas_box::debugMemory() {
	Hermas_box::debug("mem free : ", false);
	Hermas_box::debug((String) freeMemory());
}

bool Hermas_box::is_connected(void) {
	char resp[128];
	sim808_send_cmd("AT+CIPSTATUS\r\n");
	sim808_read_buffer(resp, sizeof(resp), DEFAULT_TIMEOUT*2);
	if (NULL != strstr(resp, "CONNECTED")) {
		//+CIPSTATUS: 1,0,"TCP","216.52.233.120","80","CONNECTED"
		return true;
	} else {
		//+CIPSTATUS: 1,0,"TCP","216.52.233.120","80","CLOSED"
		//+CIPSTATUS: 0,,"","","","INITIAL"
		return false;
	}
}

bool Hermas_box::startup_message(void) {

	if (Hermas_box::screen_debug) {
//		Serial.write(27);
//		Serial.print("[2J");	// clear screen
//		Serial.write(27); 		// ESC
//		Serial.print("[H"); 	// cursor to home
		Hermas_box::debug("");

		Serial.println(
				" _   _                               ____            	");
		Serial.println(
				"| | | | ___ _ __ _ __ ___   __ _ ___| __ )  _____  __   ");
		Serial.println(
				"| |_| |/ _ \\ '__| '_ ` _ \\ / _` / __|  _ \\ / _ \\ \\/ /   ");
		Serial.println(
				"|  _  |  __/ |  | | | | | | (_| \\__ \\ |_) | (_) >  <    ");
		Serial.println(
				"|_| |_|\\___|_|  |_| |_| |_|\\__,_|___/____/ \\___/_/\\_\\ 	");
		Serial.println();
	}

	for (byte i = 0; i < 10; i++) {
		Hermas_box::debug(" . ", false);
		delay(200);
	}
	Hermas_box::debug("");
}

