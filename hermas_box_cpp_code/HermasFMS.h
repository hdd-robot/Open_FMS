/*
 * HermasFMS.h
 *
 *  Created on: 25 août 2017
 *      Author: thor
 */

#ifndef HERMASFMS_H_
#define HERMASFMS_H_
#include <Arduino.h>

class Hermas_FMS {
public:
	Hermas_FMS();

	void getTextToSend(void);
	char dataToSend[512];

private:

	char Total_Fuel_Used[5];
	char Dash_Display[5];
	char Electronic_Engine_Controller_1[5];
	char Engine_Hours_Revolutions[5];
	char Vehicle_Identification[5];
	char FMS_standard_Interface_Identity_Capabilities[5];
	char High_Resolution_Vehicle_Distance[5];
	char Tachograph[5];
	char Engine_Temperature1[5];
	char Ambient_Conditions[5];
	char Drivers_Identification[5];
	char Fuel_Economy[5];
	char Air_Supply_Pressure[5];
	char High_Resolution_Fuel_Consumption[5];
	char Aftertreatment_1_Diesel_Exhaust_Fluid_Tank_1_Information[5];
	char FMS_Tell_Tale_Status[5];
	char Cruise_Control_Vehicle_Speed[5];
	char Electronic_Engine_Controller_2[5];
	char Vehicle_Weight[5];
	char Service_Information[5];
	char PTO_Drive_Engagement[5];
	char Combination_Vehicle_Weight[5];
	char Electronic_Retarder_Controller_2[5];




};


#endif /* HERMASFMS_H_ */
