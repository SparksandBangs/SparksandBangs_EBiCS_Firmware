/*
 * config.h
 *
 *  Automatically created by Lishui Parameter Configurator
 *  Author: stancecoke
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include "stdint.h"

// System constants, don't touch!
#define DISPLAY_TYPE_DEBUG (1<<0)							// For ASCII-Output in Debug mode);
#define DISPLAY_TYPE_KUNTENG (1<<1)							// For Kunteng display
#define DISPLAY_TYPE_BAFANG_LCD (1<<2)						// For 'Blaupunkt' Display of Prophete Entdecker
#define DISPLAY_TYPE_BAFANG_850_860 (1<<3)					// Bafang 850/860. Can do 9k6 baud, also compatible with 1200 baud detection at startup
#define DISPLAY_TYPE_KINGMETER_618U (1<<4)                  // King-Meter 618U protocol ( J-LCD)
#define DISPLAY_TYPE_KINGMETER_901U (1<<5)                  // King-Meter 901U protocol (KM5s)
#define DISPLAY_TYPE_EBiCS (1<<6)                  			// Protocol using the ANT+ LEV logic
#define DISPLAY_TYPE_NO2 (1<<7)								// For China Protokoll "No_2" S866 display for example
#define DISPLAY_TYPE_BAFANG         (DISPLAY_TYPE_BAFANG_LCD|DISPLAY_TYPE_BAFANG_850_860)
#define DISPLAY_TYPE_KINGMETER      (DISPLAY_TYPE_KINGMETER_618U|DISPLAY_TYPE_KINGMETER_901U)
#define EXTERNAL 1
#define INTERNAL 0

//----------------------------------------------------------------------
// advanced setting, don't touch, if you don't know what you are doing!
#define TRIGGER_OFFSET_ADC 50
#define TRIGGER_DEFAULT 2020
#define _T 2028
#define CAL_BAT_V 256
#define CAL_V 25
#define CAL_I 38LL<<8
#define INDUCTANCE 6LL
#define RESISTANCE 40LL
#define FLUX_LINKAGE 1200LL
#define GAMMA 9LL
//#define FAST_LOOP_LOG
//#define DISABLE_DYNAMIC_ADC
//#define INDIVIDUAL_MODES
//#define SPEEDTHROTTLE
#define SIXSTEPTHRESHOLD 20000
#define SPEED_PLL 0 //1 for using PLL, 0 for angle extrapolation
#define P_FACTOR_PLL 10
#define I_FACTOR_PLL 10

//----------------------------------------------------------------------
//PI-control factor settings
#define P_FACTOR_I_Q 50
#define I_FACTOR_I_Q 2
#define P_FACTOR_I_D 50
#define I_FACTOR_I_D 2
#define P_FACTOR_SPEED 1
#define I_FACTOR_SPEED 10
//----------------------------------------------------------------------
//Standard user settings below this line.
//----------------------------------------------------------------------
//Battery bar settings for Kunteng/ ADC value, multiply by 25 to get value in mV.
//If battery voltage is below 43V controller decides 36V battery.
//48V Battery
#define BATTERY_LEVEL_0 1900 //Empty
#define BATTERY_LEVEL_1 1920
#define BATTERY_LEVEL_2 1960
#define BATTERY_LEVEL_3 2040
#define BATTERY_LEVEL_4 2120
#define BATTERY_LEVEL_5 2184 //Full [2184 is 54600mV for 48V battery]

//36V Battery
#define BATTERY_LEVEL_0_36 1461 //Empty
#define BATTERY_LEVEL_1_36 1476
#define BATTERY_LEVEL_2_36 1507
#define BATTERY_LEVEL_3_36 1569
#define BATTERY_LEVEL_4_36 1630
#define BATTERY_LEVEL_5_36 1680 //Full [1680 is 42000mV for 36V battery]

//----------------------------------------------------------------------
//PAS mode settings
//#define DIRDET
#define FRAC_HIGH 30
#define FRAC_LOW 15
#define PAS_TIMEOUT 3000//PAS_TIMEOUT must be higher than RAMP_END
#define RAMP_END 2000
#define PAS_IMP_PER_TURN 32//Appears to have no effects in torque simulation mode.
#define PAS_START_DELAY 6//Number of pulses detected before assistance starts.

//---------------------------------------------------------------------
//Throttle settings
#define THROTTLE_OFFSET 1250   //only default value, throttle offset is set at startup automatically
#define THROTTLE_MAX 2850
#define THROTTLE_OVERRIDE
#define THROTTLE_GRADUATED //Throttle is scaled. If commented out throttle works as an on/off switch delivering maximum power when pressed.

//--------------------------------------------------------------------
//Speed settings
#define WHEEL_CIRCUMFERENCE 2076 //In mm.
#define GEAR_RATIO 84 //84 default for AKM128H 36V 201rpm ((16*10.5)/2)
#define PULSES_PER_REVOLUTION 1 //Wheel Speed Pulse Signal Setting
#define SPEEDLIMIT 25 //Speed limit in kmph
#define PUSH_ASSIST_LIMIT 6 //Speed limit for push and walk assist in kmph


#define SOFTSTART 9 // On PAS ramps current at startup between 0kmph and SOFTSTART value from SOFTSTART_LOW_POWER to selected Assistance level power. Setting is in whole kmph.
#define SOFTSTART_LOW_POWER 30 //From 0 to 255. 33 is ~ 13% of phase current. Ramps up from this value to assist level value between 0kmph and SOFTSTART
#define CUTOFF_POWER 20 //Minimum power still provided at point of legal limit cutoff. After this point will drop to 0. From 0 to 255. 33 is ~ 13% of phase current. Should be less than lowest Assist Level 1.

#define SPEEDSOURCE EXTERNAL //INTERNAL speed is derived from Hall sensors. EXTERNAL from 'white wire' or external sensor.
#define SPEEDFILTER 1
#define SPDSHFT 0

//---------------------------------------------------------------------
//power settings

//PH_CURRENT_MAX The code calculates in ADC values, since the processor is completely indifferent to physical units.
//To get the ADC value, which you enter in config.h, you have to divide the value in mV or mA by the respective calibration factor. For current this is 38, for voltage 25.
//
//Forums suggest that the 12FET controller should be able to handle 80A phase current for short periods.
//1200 --> 1200*38 ~ 45A, 1000 --> 1000*38 ~ 38A, 800 --> 800*38 ~ 30A, 600 --> 600*38 ~ 23A

#define PH_CURRENT_MAX 900 //Phase current for 48V battery
#define BATTERYCURRENT_MAX 25000//Battery current for 48V Battery

#define PH_CURRENT_MAX_36 720//Phase current for 36V battery
#define BATTERYCURRENT_MAX_36 20000//Battery current for 36V Battery

#define STARTASSIST_CURRENT 250 // Start assist phase current from throttle when in legal mode.
#define WALKASSIST_CURRENT 80 //Walk assist phase current activated from display.

#define REVERSE 1 //1 for normal direction, -1 for reverse

//VOLTAGE_MIN is another ADC value calibration factor for voltage is 25. 1800*25 is 45V
#define VOLTAGE_MIN 1800 //1800*25 is 45V. Minimum voltage for a 48V battery. If measured voltage is above 43V then controller assumes 48V battery.
#define VOLTAGE_MIN_36 1350 //1350*25 is 33.75V Minimum voltage for a 36V battery. If measured voltage is below 43V then controller assumes 36V battery.

//---------------------------------------------------------------------
//torquesensor settings
#define TS_COEF 2400
//#define TS_MODE
//#define TQONAD1

//---------------------------------------------------------------------
//Display settings
//#define DISPLAY_TYPE DISPLAY_TYPE_DEBUG //Debug
//#define DISPLAY_TYPE DISPLAY_TYPE_KINGMETER_901U //KM5S
#define DISPLAY_TYPE DISPLAY_TYPE_KUNTENG //KT LCD3

//#define ENABLE_WATT_METER //Only for KT Displays. Comment out to disable. If Watt meter is disabled gives a spoof reading and shows below 250W. Can be re-enabled temporarily until switch off by activating stealth mode.

//---------------------------------------------------------------------
//Regen settings

#define REGEN_CURRENT 0 //If set to 0 then Regen is disabled.
#define REGEN_CURRENT_MAX 10000
//#define ADC_BRAKE

//---------------------------------------------------------------------
//Assist Level Profiles for Kunteng Controller. Normal Assist Level may also work with Bafang and Kingmeter but not tested with these displays.
//Low Assist Level - Kunteng Paramater C14 is 1.
#define ASSIST_LEVEL_1_L 28 //From 0 to 255. 28 is ~ 11%
#define ASSIST_LEVEL_2_L 43 //From 0 to 255. 43 is ~ 17%
#define ASSIST_LEVEL_3_L 70 //From 0 to 255. 70 is ~ 27%
#define ASSIST_LEVEL_4_L 107 //From 0 to 255. 107 is ~ 42%
#define ASSIST_LEVEL_5_L 255 //From 0 to 255. 255 is ~ 100%

//Normal Assist Level - Kunteng Paramater C14 is 2. Default in main.c is 2 for other makes of display.
#define ASSIST_LEVEL_1 33 //From 0 to 255. 33 is ~ 13%
#define ASSIST_LEVEL_2 51 //From 0 to 255. 51 is ~ 20%
#define ASSIST_LEVEL_3 84 //From 0 to 255. 84 is ~ 33%
#define ASSIST_LEVEL_4 128 //From 0 to 255. 128 is ~ 50%
#define ASSIST_LEVEL_5 255 //From 0 to 255. 255 is ~ 100%

//High Assist Level - Kunteng Paramater C14 is 3.
#define ASSIST_LEVEL_1_H 40 //From 0 to 255. 40 is ~ 16%
#define ASSIST_LEVEL_2_H 61 //From 0 to 255. 61 is ~ 24%
#define ASSIST_LEVEL_3_H 101 //From 0 to 255. 101 is ~ 40%
#define ASSIST_LEVEL_4_H 154 //From 0 to 255. 154 is ~ 60%
#define ASSIST_LEVEL_5_H 255 //From 0 to 255. 255 is ~ 100%

//---------------------------------------------------------------------
#define AUTODETECT 0

#endif /* CONFIG_H_ */
