/**
 * Configuration file HERMAS BOX
 */

#ifndef HERMAS_CONFIG_H_
#define HERMAS_CONFIG_H_

/**
 * pins configuration
 */
#define PIN_TX 8
#define PIN_RX 9
#define DEBUG_PIN 2

/**
 * speed configuration
 */

#define SPEED_SIM_SERIAL 9600
#define SPEED_SCREEN_SERIAL 9600


/**
 * GPRS Configuration
 */
#define APN_NAME "free"
#define APN_USER "LOGIN"
#define APN_PASS "PASS"

/**
 * Serveur Configuration
 */

#define SERVEUR_CONNECTION "hermas.halim.info"
#define SERVEUR_PORT 80
#define URL_SEND "http://hermas.halim.info/get/main!index!"



#endif /* HERMAS_CONFIG_H_ */
