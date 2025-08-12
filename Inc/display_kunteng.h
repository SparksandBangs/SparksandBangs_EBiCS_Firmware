/*
 * display_kunteng.h
 *
 *  Created on: 25.09.2019
 *      Author: Admin
 */

#ifndef DISPLAY_KUNTENG_H_
#define DISPLAY_KUNTENG_H_

#include "config.h"

void kunteng_init();
void display_update(MotorState_t* MS_U);
void check_message(MotorState_t* MS_D, MotorParams_t* MP_D);

typedef struct _lcd_configuration_variables
{
  uint8_t ui8_assist_level;
  uint8_t ui8_light;
  uint8_t ui8_motor_characteristic;
  uint8_t ui8_wheel_size;
  uint8_t ui8_max_speed;
  uint8_t ui8_power_assist_control_mode;
  uint8_t ui8_controller_max_current;
  uint8_t ui8_p1;//P1 Motor Characteristic Parameter Setting
  uint8_t ui8_p2;//P2 Wheel Speed Pulse Signal Setting
  uint8_t ui8_p3;//P3 Power Assist Control Setting
  uint8_t ui8_p4;//P4 Throttle Start-Up Setting
  uint8_t ui8_p5;//Power Monitoring Setting
  uint8_t ui8_c1;//C1 Power-assist Sensor and Parameter Selection Setting
  uint8_t ui8_c2;//C2 Motor Phase Classification Coding Setting
  uint8_t ui8_c4;//C4 Throttle Function Setting
  uint8_t ui8_c5;//C5 Controller Maximum Current Adjustment Setting
  uint8_t ui8_c12;//C12 Controller Minimum Voltage Adjustment Setting
  uint8_t ui8_c13;//ABS brakes of the controller and parameters of anti-charge control Setting
  uint8_t ui8_c14;//Power-assist Tuning Parameters Setting
  uint8_t u18_c15;//Push-assist Speed Parameters Setting
} struc_lcd_configuration_variables;

#define COMMUNICATIONS_BATTERY_VOLTAGE	(BATTERY_LI_ION_CELLS_NUMBER * 3.45) // example: 7S battery, should be = 24

// Considering the follow voltage values for each li-ion battery cell
// State of charge 		| voltage
//#define LI_ION_CELL_VOLTS_MAX 4.20
//#define LI_ION_CELL_VOLTS_100 4.20
//#define LI_ION_CELL_VOLTS_80 4.08 //3.86 // 4.02
//#define LI_ION_CELL_VOLTS_60 3.93 //3.68 // 3.87
//#define LI_ION_CELL_VOLTS_40 3.77 //3.46 // 3.80
//#define LI_ION_CELL_VOLTS_20 3.69 //3.28 // 3.73
//#define LI_ION_CELL_VOLTS_0 3.64 //3.10 // 3.27
//#define LI_ION_CELL_VOLTS_MIN 3.64 //3.10

//#define BATTERY_PACK_VOLTS_100	(LI_ION_CELL_VOLTS_100 * BATTERY_LI_ION_CELLS_NUMBER) * 256
//#define BATTERY_PACK_VOLTS_80 	(LI_ION_CELL_VOLTS_80 * BATTERY_LI_ION_CELLS_NUMBER) * 256
//#define BATTERY_PACK_VOLTS_60	(LI_ION_CELL_VOLTS_60 * BATTERY_LI_ION_CELLS_NUMBER) * 256
//#define BATTERY_PACK_VOLTS_40	(LI_ION_CELL_VOLTS_40 * BATTERY_LI_ION_CELLS_NUMBER) * 256
//#define BATTERY_PACK_VOLTS_20	(LI_ION_CELL_VOLTS_20 * BATTERY_LI_ION_CELLS_NUMBER) * 256
//#define BATTERY_PACK_VOLTS_0	(LI_ION_CELL_VOLTS_0 * BATTERY_LI_ION_CELLS_NUMBER) * 256

//#define ADC_BATTERY_VOLTAGE_K 73 // 0.272 << 8
#define BATTERY_LI_ION_CELLS_NUMBER 13 //13 for 48V 10 for 36V

#endif /* DISPLAY_KUNTENG_H_ */
