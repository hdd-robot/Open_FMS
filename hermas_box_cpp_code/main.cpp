
#include <Arduino.h>

#include "Hermas_box.h"
#include "hermas_config.h"
#include "HermasFMS.h"
#include "HermasGPS.h"

Hermas_GPS gps = Hermas_GPS();
Hermas_FMS fms = Hermas_FMS();

int main(void) {
	/**
	 * Init HermasBox
	 */
	init();

	Hermas_box::init_hermas_box();
	Hermas_box::startup_message();
	Hermas_box::init_hermas_sim808();
	Hermas_box::show_phone_number();



	for(;;){
		Hermas_box::debug("--- LOOP ---");
		Hermas_box::signalStrength();

		/**
		 * get data GPS
		 */
		delay(5000);
		Hermas_box::init_hermas_GPS();
		delay(5000);
		Hermas_box::get_GPS_data();
		delay(500);
		Hermas_box::close_GPS_deconnexion();


		delay(1000);

		fms.getTextToSend();

		if(gps.setPosition(Hermas_box::receivedStack) != 0){
			gps.set_default();
		}

		gps.getTextToSend();



		delay(2000);

		Hermas_box::debug("--- init gprs  ---");

		Hermas_box::init_hermas_GPRS();
		Hermas_box::send_data();
		delay(2000);
		//Hermas_box::close_GPRS_deconnexion();;
		Hermas_box::debug("*fin*");
		delay(10 *1000);

	}

	return 0;


}

