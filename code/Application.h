/* 
 * File:   Application.h
 * Author: Yossri
 *
 * Created on July 7, 2023, 8:08 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/* ------------------------------------Includes--------------------------------------*/
#include "ECU_Layer/ecu_intialize.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/Interrupt/mcal_internal_interrupt.h"
#include "MCAL_Layer/EEPROM/mcal_eeprom.h"
#include "MCAL_Layer/MCAL_ADC/mcal_adc.h"

#include "MCAL_Layer/Timer0/mcal_timer0.h"
#include "MCAL_Layer/Timer1/mcal_timer1.h"
#include "MCAL_Layer/Timer2/mcal_timer2.h"
#include "MCAL_Layer/Timer3/mcal_timer3.h"
#include "MCAL_Layer/CCP1/hal_ccp.h"
#include "MCAL_Layer/ECCP/hal_eccp.h"
#include "MCAL_Layer/USART/hal_usart.h"
#include "MCAL_Layer/SPI/hal_spi.h"
#include "MCAL_Layer/I2C/hal_i2c.h"
/* ------------------------------------Macro Declarations--------------------------- */

#define OPEN_DOOR   1
#define CLOSE_DOOR  0

#define DOOR_IS_OPEN   90
#define DOOR_IS_CLOSE -95

#define LEVEL_1     1
#define LEVEL_2     2
#define LEVEL_3     3

#define START 1
#define STOP  0
/* ------------------------------------Macro Function Declarations------------------ */

/* ------------------------------------Data Type Declarations----------------------- */

extern Led_t led_door;

extern Led_t led_level_1;
extern Led_t led_level_2;
extern Led_t led_level_3;

extern Led_t wash_L_1;
extern Led_t wash_L_2;
extern Led_t wash_L_3;

extern Led_t led_rainse_1;
extern Led_t led_rainse_2;

extern Led_t led_cold;
extern Led_t led_warm;
extern Led_t led_hot;




volatile uint8 start_programm = STOP;

uint8 water_level_flag = 0;
uint8 wash_level_flag  = 0;
uint8 rainse_level_flag  = 0;
uint8 temp_level_flag  = 0;

uint8 counter_btn_level = 0;
uint8 counter_wash_times = 0;
uint8 counter_rainse_times = 0;
uint8 counter_temp_times = 0;


button_status_t water_level_status = BUTTON_RELEASED;
button_status_t wash_level_status  = BUTTON_RELEASED;
button_status_t rainse_level_button = BUTTON_RELEASED;
button_status_t temp_level_button = BUTTON_RELEASED;

volatile uint8 second = 0;
volatile uint8 minutes = 0;

servo_cfg Door_gate = { .ccp_selection = CCP1_SELECT};


void ISR_INT1(void);
EXT_INTX_t Int_1 ={
  .Interrupt_Handler = ISR_INT1,
  .EXT_pin.port = PortB_Index,
  .EXT_pin.direction = Direction_input,
  .EXT_pin.pin = pin1,
  .edge = EXT_INT_Raising_Edge,
  .source = EXT_INT1
};

void ISR_INT2(void);
EXT_INTX_t Int_2 ={
  .Interrupt_Handler = ISR_INT2,
  .EXT_pin.port = PortB_Index,
  .EXT_pin.direction = Direction_input,
  .EXT_pin.pin = pin2,
  .edge = EXT_INT_Raising_Edge,
  .source = EXT_INT2
};

button_config_t btn_water_L =
{
    .button_pin.port = PortC_Index ,
    .button_pin.pin = pin7,
    .button_pin.direction = Direction_input ,
    .button_pin.logic = Logic_Low ,
    .button_connection = BUTTON_ACTIVE_HIGH ,
    .button_status = BUTTON_RELEASED
};

button_config_t btn_wash =
{
    .button_pin.port = PortB_Index ,
    .button_pin.pin = pin7,
    .button_pin.direction = Direction_input ,
    .button_pin.logic = Logic_Low ,
    .button_connection = BUTTON_ACTIVE_HIGH ,
    .button_status = BUTTON_RELEASED
};

button_config_t btn_rainse =
{
    .button_pin.port = PortB_Index ,
    .button_pin.pin = pin3,
    .button_pin.direction = Direction_input ,
    .button_pin.logic = Logic_Low ,
    .button_connection = BUTTON_ACTIVE_HIGH ,
    .button_status = BUTTON_RELEASED
};

button_config_t btn_temp =
{
    .button_pin.port = PortD_Index ,
    .button_pin.pin = pin3,
    .button_pin.direction = Direction_input ,
    .button_pin.logic = Logic_Low ,
    .button_connection = BUTTON_ACTIVE_HIGH ,
    .button_status = BUTTON_RELEASED
};

void timer0_isr(void);
Timer0_conf_t timer0 =
{
    .TIMER0_IntterruptHandeler = timer0_isr ,
    .Timer0_SELECT_BIT_Mode = TIMER0_16_BIT_MODE_,
    .Timer0_Mode = TIMER0_TIMER_MODE_,
    .TIMER0_PRESCALER_STATUS = TIMER0_PRESCALER_ENABLE_ ,
    .Prescaler_Select = TIMER0_PRESCALER_DIV_BY_2,
    .TIMER0_PRE_LOAD_VALUE = 48036,
};

SPI_Config spi_master =
{
  .spi_control.ClockPolarity = SPI_IDLE_STATE_HIGH_LEVEL ,
  .spi_control.ClockSelect  = SPI_TRANSMIT_ACTIVE_TO_IDLE ,
  .spi_control.SampleSelect =SPI_DATA_SAMPLE_MIDDLE,
  .spi_serial_clk_mod_select = SPI_MASTER_FOSC_DIV4 ,
   
};

Pin_Config_t pin_enable = {
  .port = PortD_Index ,
  .pin = pin1,
  .direction = Direction_Outpt ,
  .logic = Logic_High
};
uint8 data_from_slave = 0;


/* ------------------------------------Software Interface Declarations-------------- */



#endif	/* APPLICATION_H */

