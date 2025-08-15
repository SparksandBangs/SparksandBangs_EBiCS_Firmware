/*
 * display_kunteng.c
 *
 *  Created on: 25.09.2019
 *      Author: Admin
 */

#include "main.h"
#include "display_kunteng.h"
#include "stm32f1xx_hal.h"
#include "eeprom.h"

static uint8_t ui8_tx_buffer[12];
uint8_t ui8_j;
uint8_t ui8_crc;
uint8_t ui8_last_XOR;
uint16_t ui16_wheel_period_ms =4500;
uint32_t ui32_battery_volts= 1798;
uint8_t ui8_battery_soc = 12;
uint8_t ui16_error;
static uint8_t ui8_rx_buffer[13];
uint8_t ui8_rx_initial_buffer[13];
uint8_t ui8_rx_buffer_counter = 0;
uint8_t ui8_byte_received;
uint8_t ui8_moving_indication = 0;
uint8_t ui8_UARTCounter = 0;
uint8_t ui8_msg_received=0;
int16_t i16_eeprom_temp=0;
uint8_t ui8_gear_ratio = GEAR_RATIO;
//uint8_t uint8_light_operation_counter=0;
uint8_t uint8_stealth_mode=0;
uint8_t uint8_brake_pull_counter=0;

volatile struc_lcd_configuration_variables lcd_configuration_variables;

extern UART_HandleTypeDef huart1;
void check_recent(void);

void kunteng_init()
{

    if (HAL_UART_Receive_DMA(&huart1, (uint8_t *)ui8_rx_buffer, 13) != HAL_OK)
     {
 	   Error_Handler();
     }

    EE_ReadVariable(EEPROM_KT_B0_B3, &i16_eeprom_temp);
    ui8_rx_buffer[0] = i16_eeprom_temp>>8;
    ui8_rx_initial_buffer[0] = i16_eeprom_temp>>8;
    ui8_rx_buffer[3] = i16_eeprom_temp &0xFF;
    ui8_rx_initial_buffer[3] = i16_eeprom_temp &0xFF;

    EE_ReadVariable(EEPROM_KT_B2_B4, &i16_eeprom_temp);
    ui8_rx_buffer[2] = i16_eeprom_temp>>8;
    ui8_rx_initial_buffer[2] = i16_eeprom_temp>>8;
    ui8_rx_buffer[4] = i16_eeprom_temp &0xFF;
    ui8_rx_initial_buffer[4] = i16_eeprom_temp &0xFF;

    EE_ReadVariable(EEPROM_KT_B6_B7, &i16_eeprom_temp);
    ui8_rx_buffer[6] = i16_eeprom_temp>>8;
    ui8_rx_initial_buffer[6] = i16_eeprom_temp>>8;
    ui8_rx_buffer[7] = i16_eeprom_temp &0xFF;
    ui8_rx_initial_buffer[7] = i16_eeprom_temp &0xFF;

    EE_ReadVariable(EEPROM_KT_B8_B9, &i16_eeprom_temp);
    ui8_rx_buffer[8] = i16_eeprom_temp>>8;
    ui8_rx_initial_buffer[8] = i16_eeprom_temp>>8;
    ui8_rx_buffer[9] = i16_eeprom_temp &0xFF;
    ui8_rx_initial_buffer[9] = i16_eeprom_temp &0xFF;

    EE_ReadVariable(EEPROM_KT_B1_B10, &i16_eeprom_temp);
    ui8_rx_buffer[1] = i16_eeprom_temp>>8;
    ui8_rx_initial_buffer[1] = i16_eeprom_temp>>8;
    ui8_rx_buffer[10] = i16_eeprom_temp &0xFF;
    ui8_rx_initial_buffer[10] = i16_eeprom_temp &0xFF;

    ui8_crc = 0;

    for (ui8_j = 0; ui8_j <= 12; ui8_j++)
    {
      if (ui8_j == 5) continue; // don't xor B5 (B7 in our case)
      ui8_crc ^= ui8_rx_buffer[ui8_j];
    }
    ui8_crc ^=10; //right XOR must be pasted here!!!
    ui8_rx_buffer [5]=ui8_crc;
    ui8_rx_buffer[11]=0x37;
    ui8_rx_buffer[12]=0x0E;

}

void display_update(MotorState_t* MS_U)
{

   // prepare moving indication info
  ui8_moving_indication = 0;
if (!HAL_GPIO_ReadPin(Brake_GPIO_Port, Brake_Pin)) { ui8_moving_indication |= (1 << 5); }
  //if (ebike_app_cruise_control_is_set ()) { ui8_moving_indication |= (1 << 3); } //Assist level shows C instead of number and cruise message displayed.
  if (throttle_is_set ()) { ui8_moving_indication |= (1 << 1); }
  if (pas_is_set ()) { ui8_moving_indication |= (1 << 4); }//Assist message flashes

  if (uint8_stealth_mode==1 && uint8_brake_pull_counter == 5){
	  	  ui8_moving_indication |= (1 << 4);//Assist message flashes if stealth mode is engaged.
  }

  // calc battery pack state of charge (SOC)
  ui32_battery_volts =  MS_U->Voltage;


  if(MS_U->Battery_Current<500){
	  if (ui32_battery_volts > 1720) //1720 is 43V (decision if battery is 48V or 36V)
	  {
			  if (ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_4+20)) { ui8_battery_soc = 16; } // 4 bars | full
			 	else if ((ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_4))&&(ui8_battery_soc != 12)) { ui8_battery_soc = 16; } // 4 bars buffer zone
			 	else if (ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_3+20)) { ui8_battery_soc = 12; } // 3 bars
			 	else if ((ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_3))&&(ui8_battery_soc != 8)) { ui8_battery_soc = 12; } // 3 bars buffer zone
			 	else if (ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_2+20)) { ui8_battery_soc = 8; } // 2 bars
			 	else if ((ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_2))&&(ui8_battery_soc != 4)) { ui8_battery_soc = 8; } // 2 bars buffer zone
			 	else if (ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_1)) { ui8_battery_soc = 4; } // 1 bar
			 	else if (ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_0)) { ui8_battery_soc = 3; } // empty
			 	else { ui8_battery_soc = 1; } // below empty level
	  }
	  else{
			  if (ui32_battery_volts > ((uint16_t) (BATTERY_LEVEL_4_36+20))) { ui8_battery_soc = 16; } // 4 bars | full
			  	else if ((ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_4_36))&&(ui8_battery_soc != 12)) { ui8_battery_soc = 16; } // 4 bars | full buffer zone
			  	else if (ui32_battery_volts > ((uint16_t) (BATTERY_LEVEL_3_36+20))) { ui8_battery_soc = 12; } // 3 bars
			  	else if ((ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_3_36))&&(ui8_battery_soc != 8)) { ui8_battery_soc = 12; } // 3 bars buffer zone
			  	else if (ui32_battery_volts > ((uint16_t) (BATTERY_LEVEL_2_36+20))) { ui8_battery_soc = 8; } // 2 bars
			  	else if ((ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_2_36))&&(ui8_battery_soc != 4)) { ui8_battery_soc = 8; } // 2 bars buffer zone
			  	else if (ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_1_36)) { ui8_battery_soc = 4; } // 1 bar
			  	else if (ui32_battery_volts > ((uint16_t) BATTERY_LEVEL_0_36)) { ui8_battery_soc = 3; } // empty
			  	else { ui8_battery_soc = 1; }  // below empty level
	  }
	}

#if (SPEEDSOURCE == INTERNAL)
  	  	 ui16_wheel_period_ms= ((MS_U->Speed)*6*(ui8_gear_ratio/2)/500); //For internal speed sensor *6 because 6 hall interrupts per revolution, /500 because of 500 kHz timer setting
#else
  	  	 ui16_wheel_period_ms = (MS_U->Speed*PULSES_PER_REVOLUTION)>>3; //for External speedsensor
#endif

  ui8_tx_buffer [0] =  65;
  // B1: battery level
  ui8_tx_buffer [1] = ui8_battery_soc;
  // B2: 24V controller
  if (ui32_battery_volts > 1720){
	  ui8_tx_buffer [2] = (uint8_t) COMMUNICATIONS_BATTERY_VOLTAGE;//48V
  }
  else{
	  ui8_tx_buffer [2] = (uint8_t) COMMUNICATIONS_BATTERY_VOLTAGE/1.3;//36V
  }
  // B3: speed, wheel rotation period, ms; period(ms)=B3*256+B4;
  ui8_tx_buffer [3] = (ui16_wheel_period_ms >> 8) & 0xff;
  ui8_tx_buffer [4] = (ui16_wheel_period_ms) & 0xff;
  // B5: error info display
  ui8_tx_buffer [5] = ui16_error;
  // B6: CRC: xor B1,B2,B3,B4,B5,B7,B8,B9,B10,B11
  // 0 value so no effect on xor operation for now
  ui8_tx_buffer [6] = 0;
  // B7: moving mode indication, bit
  // throttle: 2
  ui8_tx_buffer [7] = ui8_moving_indication;
  // B8: 4x controller current
  // Each unit of B8 = 0.25A from the battery current LCD3 then multiplies this number by the line voltage supplied to it.
  // From 0 to 255
  // Example value of 100 is 100 * 0.25A = 25A. if line voltage at display is 48V then
  // 48V * 25A = 1200W
  //
  //ui8_tx_buffer [8] =  (uint8_t)(((ui16_BatteryCurrent-ui16_current_cal_b+1)<<2)/current_cal_a);
#ifdef ENABLE_WATT_METER
  	  ui8_tx_buffer [8] =  (uint8_t)(MS_U->Battery_Current/250);
#else
	  if (uint8_stealth_mode >0){
		  ui8_tx_buffer [8] =  (uint8_t)(MS_U->Battery_Current/250);
	  }
	  else{
		  if (throttle_is_set () && (uint8_t)(MS_U->Battery_Current/250)>2){ui8_tx_buffer [8] =  18;} //Shows near to 250W at 54V.
		  else if (pas_is_set ()){ui8_tx_buffer [8] =  18;} //Shows near to 250W at 54V.
		  else if(lcd_configuration_variables.ui8_assist_level==6 && brake_is_set()!=1){ui8_tx_buffer [8] =  7;}//Shows near 100W at 54V
		  else {ui8_tx_buffer [8] =  0;}
	  }

#endif
  // B9: motor temperature
  ui8_tx_buffer [9] = MS_U->Temperature-15; //according to documentation at endless sphere	
  // B10 and B11: 0
  ui8_tx_buffer [10] = 0;
  ui8_tx_buffer [11] = 0;

  // calculate CRC xor
  ui8_crc = 0;
  for (ui8_j = 1; ui8_j <= 11; ui8_j++)
  {
    ui8_crc ^= ui8_tx_buffer[ui8_j];
  }
  ui8_tx_buffer [6] = ui8_crc;

  // send the package over UART
  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&ui8_tx_buffer, 12);
}

/********************************************************************************************/
 // Process received package from the LCD
 //

 // see if we have a received package to be processed
void check_message(MotorState_t* MS_D, MotorParams_t* MP_D)
 {
  //printf("Byte recieved \r\n");
  // validation of the package data
   ui8_crc = 0;

   for (ui8_j = 0; ui8_j <= 12; ui8_j++)
   {
     if (ui8_j == 5) continue; // don't xor B5 (B7 in our case)
     ui8_crc ^= ui8_rx_buffer[ui8_j];
   }

   // check if end of message is OK
   if((ui8_rx_buffer[11]==0x32||ui8_rx_buffer[11]==0x37) && ui8_rx_buffer[12]==0x0E ){
	   // check if CRC is ok
   if (((ui8_crc ^ 10) == ui8_rx_buffer [5]) 	|| // some versions of CRC LCD5 (??)
	((ui8_crc ^ ui8_last_XOR) == ui8_rx_buffer [5])
	)
   { //printf("message valid \r\n");
     lcd_configuration_variables.ui8_assist_level = ui8_rx_buffer [1] & 7;
     lcd_configuration_variables.ui8_light = ui8_rx_buffer [1]>>7 & 1;
     lcd_configuration_variables.ui8_motor_characteristic = ui8_rx_buffer [3];
     lcd_configuration_variables.ui8_wheel_size = ((ui8_rx_buffer [4] & 192) >> 6) | ((ui8_rx_buffer [2] & 7) << 2);
     lcd_configuration_variables.ui8_max_speed = (10 + ((ui8_rx_buffer [2] & 248) >> 3)) | (ui8_rx_buffer [4] & 32);
     lcd_configuration_variables.ui8_power_assist_control_mode = ui8_rx_buffer [4] & 8;
     lcd_configuration_variables.ui8_controller_max_current = (ui8_rx_buffer [7] & 15);
     MS_D->assist_level = lcd_configuration_variables.ui8_assist_level;


    	 MP_D->speedLimit = lcd_configuration_variables.ui8_max_speed;

		lcd_configuration_variables.ui8_p1 = ui8_rx_buffer[3];
		lcd_configuration_variables.ui8_p2 = ui8_rx_buffer[4] & 0x07;
		lcd_configuration_variables.ui8_p3 = ui8_rx_buffer[4] & 0x08;
		lcd_configuration_variables.ui8_p4 = ui8_rx_buffer[4] & 0x10;
		lcd_configuration_variables.ui8_p5 = ui8_rx_buffer[0];

		lcd_configuration_variables.ui8_c1 = (ui8_rx_buffer[6] & 0x38) >> 3;
		lcd_configuration_variables.ui8_c2 = (ui8_rx_buffer[6] & 0x37);
		lcd_configuration_variables.ui8_c4 = (ui8_rx_buffer[8] & 0xE0) >> 5;
		lcd_configuration_variables.ui8_c5 = (ui8_rx_buffer[7] & 0x0F);
		lcd_configuration_variables.ui8_c12 = (ui8_rx_buffer[9] & 0x0F);
		lcd_configuration_variables.ui8_c13 = (ui8_rx_buffer[10] & 0x1C) >> 2;
		lcd_configuration_variables.ui8_c14 = (ui8_rx_buffer[7] & 0x60) >> 5;

		if(lcd_configuration_variables.ui8_p1 != ui8_gear_ratio){ ui8_gear_ratio=lcd_configuration_variables.ui8_p1;}
		if(lcd_configuration_variables.ui8_p2 != MP_D->pulses_per_revolution){MP_D->pulses_per_revolution = lcd_configuration_variables.ui8_p2;}
		if(lcd_configuration_variables.ui8_c14 != MP_D->power_assist_tuning){MP_D->power_assist_tuning = lcd_configuration_variables.ui8_c14;}

		//Counts number of times brake is pulled and released
		if(uint8_brake_pull_counter < 6){
			if(brake_is_set()){
				if(uint8_brake_pull_counter % 2 == 0){uint8_brake_pull_counter += 1;}//if brake_pull_counter is even
			}
			else{
				if(uint8_brake_pull_counter % 2 != 0){uint8_brake_pull_counter += 1;}//if brake_pull_counter is odd
			}
		}
		//Stealth mode
		//To activate pull brake and release x 2. Then pull brake and hold on. While brake is on press walk assist button on display.
		//If mode is activated Assist on display will flash.
		if(lcd_configuration_variables.ui8_assist_level==6){
			if (brake_is_set()) {//If brake pulled
				 if(uint8_brake_pull_counter == 5){uint8_stealth_mode = 1;}
			}
		}

		if(lcd_configuration_variables.ui8_light){
			HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
		}
		else{
			HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		}

     display_update(MS_D);
     check_recent(); //byte 1 contains the PAS level, that may be changed quite often. Better run only at system shutdown, due to limited possible write cycles to flash
   }//end CRC OK
   else{ //search for right last XOR

	   ui8_crc = 0;

	   for (ui8_j = 0; ui8_j <= 12; ui8_j++)
	   {
	     if (ui8_j == 5) continue; // don't xor B5 (B7 in our case)
	     ui8_crc ^= ui8_rx_buffer[ui8_j];
	   }
	   for (ui8_j = 0; ui8_j <= 50; ui8_j++)
	   {
	     if((ui8_crc ^ ui8_j) == ui8_rx_buffer [5]) ui8_last_XOR = ui8_j;
	   }
   }
   }// end EOT OK
   else{
	  //resyncronize the communication
	       CLEAR_BIT(DMA1_Channel5->CCR, DMA_CCR_EN);
		   DMA1_Channel5->CNDTR=2;
		   SET_BIT(DMA1_Channel5->CCR, DMA_CCR_EN);

		   if((ui8_rx_buffer[0]==0x32||ui8_rx_buffer[0]==0x37) && ui8_rx_buffer[1]==0x0E ){
	  	   CLEAR_BIT(DMA1_Channel5->CCR, DMA_CCR_EN);
	  	   DMA1_Channel5->CNDTR=13;
	  	   SET_BIT(DMA1_Channel5->CCR, DMA_CCR_EN);
		   }

   }
 }

//check if differences between initial values and recent values, store to emulated EEPROM if necessary
/*
EEPROM_KT_B0_B3
EEPROM_KT_B2_B4
EEPROM_KT_B6_B7
EEPROM_KT_B8_B9
EEPROM_KT_B1_B10
*/
//byte 1 contains the PAS level, that may be changed quite often. Better run only at system shutdown.
void check_recent(void){

	if(ui8_rx_buffer[0]!=ui8_rx_initial_buffer[0] || ui8_rx_buffer[3]!=ui8_rx_initial_buffer[3] ){
	    HAL_FLASH_Unlock();
	    EE_WriteVariable(EEPROM_KT_B0_B3,ui8_rx_buffer[0]<<8 | ui8_rx_buffer[3] );
	    HAL_FLASH_Lock();
	    ui8_rx_initial_buffer[0]=ui8_rx_buffer[0];
	    ui8_rx_initial_buffer[3]=ui8_rx_buffer[3];
	}

	if(ui8_rx_buffer[2]!=ui8_rx_initial_buffer[2] || ui8_rx_buffer[4]!=ui8_rx_initial_buffer[4] ){
	    HAL_FLASH_Unlock();
	    EE_WriteVariable(EEPROM_KT_B2_B4,ui8_rx_buffer[2]<<8 | ui8_rx_buffer[4] );
	    HAL_FLASH_Lock();
	    ui8_rx_initial_buffer[2]=ui8_rx_buffer[2];
	    ui8_rx_initial_buffer[4]=ui8_rx_buffer[4];
	}

	if(ui8_rx_buffer[6]!=ui8_rx_initial_buffer[6] || ui8_rx_buffer[7]!=ui8_rx_initial_buffer[7] ){
	    HAL_FLASH_Unlock();
	    EE_WriteVariable(EEPROM_KT_B6_B7,ui8_rx_buffer[6]<<8 | ui8_rx_buffer[7] );
	    HAL_FLASH_Lock();
	    ui8_rx_initial_buffer[6]=ui8_rx_buffer[6];
	    ui8_rx_initial_buffer[7]=ui8_rx_buffer[7];
	}

	if(ui8_rx_buffer[8]!=ui8_rx_initial_buffer[8] || ui8_rx_buffer[9]!=ui8_rx_initial_buffer[9] ){
	    HAL_FLASH_Unlock();
	    EE_WriteVariable(EEPROM_KT_B8_B9,ui8_rx_buffer[8]<<8 | ui8_rx_buffer[9] );
	    HAL_FLASH_Lock();
	    ui8_rx_initial_buffer[8]=ui8_rx_buffer[8];
	    ui8_rx_initial_buffer[9]=ui8_rx_buffer[9];
	}
//only check Byte 10
	if( ui8_rx_buffer[10]!=ui8_rx_initial_buffer[10] ){
	    HAL_FLASH_Unlock();
	    EE_WriteVariable(EEPROM_KT_B1_B10,ui8_rx_buffer[1]<<8 | ui8_rx_buffer[10] );
	    HAL_FLASH_Lock();
	    ui8_rx_initial_buffer[1]=ui8_rx_buffer[1];
	    ui8_rx_initial_buffer[10]=ui8_rx_buffer[10];
	}


}
