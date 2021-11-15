/*
 * HermasFMS.cpp
 *
 *  Created on: 25 août 2017
 *      Author: thor
 */

#include "HermasFMS.h"


Hermas_FMS::Hermas_FMS() {
	// TODO Auto-generated constructor stub

	strcpy(Total_Fuel_Used,"1234");								// TFU
	strcpy(Dash_Display,"1234");
	strcpy(Electronic_Engine_Controller_1,"1234");
	strcpy(Engine_Hours_Revolutions,"1234");
	strcpy(Vehicle_Identification,"1234");
	strcpy(FMS_standard_Interface_Identity_Capabilities,"1234");
	strcpy(High_Resolution_Vehicle_Distance,"1234");
	strcpy(Tachograph,"1234");
	strcpy(Engine_Temperature1,"1234");
	strcpy(Ambient_Conditions,"1234");
	strcpy(Drivers_Identification,"1234");
	strcpy(Fuel_Economy,"1234");
	strcpy(Air_Supply_Pressure,"1234");
	strcpy(High_Resolution_Fuel_Consumption,"1234");
	strcpy(Aftertreatment_1_Diesel_Exhaust_Fluid_Tank_1_Information,"1234");
	strcpy(FMS_Tell_Tale_Status,"1234");
	strcpy(Cruise_Control_Vehicle_Speed,"1234");
	strcpy(Electronic_Engine_Controller_2,"1234");
	strcpy(Vehicle_Weight,"1234");
	strcpy(Service_Information,"1234");
	strcpy(PTO_Drive_Engagement,"1234");
	strcpy(Combination_Vehicle_Weight,"1234");
	strcpy(Electronic_Retarder_Controller_2,"1234");



}

void Hermas_FMS::getTextToSend(void) {

	sprintf(dataToSend,
	"%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s&%s=%s",
	"TFU",Total_Fuel_Used,							// TFU
	"DD",Dash_Display,
	"EEC1",Electronic_Engine_Controller_1,
	"EHR",Engine_Hours_Revolutions,
	"VI",Vehicle_Identification,
	"FMSSIIC",FMS_standard_Interface_Identity_Capabilities,
	"HRVD",High_Resolution_Vehicle_Distance,
	"TAC",Tachograph,
	"ET1",Engine_Temperature1,
	"AC",Ambient_Conditions,
	"DI",Drivers_Identification,
	"FE",Fuel_Economy,
	"ASP",Air_Supply_Pressure,
	"HRFC",High_Resolution_Fuel_Consumption,
	"ADEFT",Aftertreatment_1_Diesel_Exhaust_Fluid_Tank_1_Information,
	"FMSTTS",FMS_Tell_Tale_Status,
	"CCVS",Cruise_Control_Vehicle_Speed,
	"EEC2",Electronic_Engine_Controller_2,
	"VW",Vehicle_Weight,
	"SI",Service_Information,
	"PTO",PTO_Drive_Engagement,
	"CVW",Combination_Vehicle_Weight,
	"ERC2",Electronic_Retarder_Controller_2);
}



















