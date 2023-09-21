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
#define Water_Level_1  30
#define Water_Level_2  65
#define Water_Level_3  98

#define Wash_Level_1  4
#define Wash_Level_2  8
#define Wash_Level_3  12

#define Raisne_Level_1  1
#define Raisne_Level_2  2

#define Raisne_time_1  3
#define Raisne_time_2  6

#define LEVEL_1     1
#define LEVEL_2     2
#define LEVEL_3     3

#define COLD    1
#define WARM    2
#define HOT     3

#define COLD_TEMP    20
#define WARM_TEMP    30
#define HOT_TEMP     40

#define START 1
#define STOP  0

#define TRUE  1
#define FALSE 0

/* ------------------------------------Macro Function Declarations------------------ */

/* ------------------------------------Data Type Declarations----------------------- */
uint8 Start_status = 0;
uint8 water_level_status = 0;
uint8 wash_level_status = 0;
uint8 rainse_level_status = 0;
uint8 temp_level_status = 0;

uint8 flag_water = FALSE;
uint8 flag_temp = FALSE;

uint8 dummy = 0;
uint16 conv_result_temp ,conv_result_water_level;
uint8 adc_flag = 0 ;
uint16 temperature = 0 , water_level = 0;

volatile uint8 second_  = 0;
uint8 minutes = 0;

uint8 time_of_prog = 0; 
uint8 time_of_washing = 0;
uint8 time_of_raisne = 0;


uint8 end_of_washing = 0;
uint8 num_of_minutes = 0;

servo_cfg Water_drainage = { .ccp_selection = CCP2_SELECT};

SPI_Config spi_slave =
{
  .spi_control.ClockPolarity = SPI_IDLE_STATE_HIGH_LEVEL ,
  .spi_control.ClockSelect  = SPI_TRANSMIT_ACTIVE_TO_IDLE ,
  .spi_control.SampleSelect =SPI_DATA_SAMPLE_MIDDLE,
  .spi_serial_clk_mod_select = SPI_SLAVE_SS_ENABLE ,
   
};


void ADC_Temp_Smk_ISR(void);
ADC_Conf_t adc_temp_wl =
{
  .ADC_IntterruptHandeler = ADC_Temp_Smk_ISR ,
  .ADC_Acquisition_Time =  ADC_12_TAD ,
  .ADC_Conversion_Clock = ADC_CONVERSION_CLOCK_FOSC_DIV_2,
  .ADC_Channel = ADC_CHANNEL_AN1,
  .format_status = ADC_RIGHT_JUSTIFIED ,
  .voltage_ref = ADC_VOLTAGE_REF_DISABLE
};


void timer3_isr(void);
timer3_t timer3 =
{
    .TIMER3_InterruptHandler = timer3_isr ,
    .timer3_reg_rw_mode = TIMER1_16BIT_READ_,
    .timer3_mode = TIMER1_TIMER_MODE_,
    .timer3_prescaler = TIMER3_PRESCALER_DIV_BY_4,
    .timer3_preload_value = 21786,
};

Dc_Motor_t dc_motor1 =
{
  .pin[0].port = PortD_Index ,
  .pin[0].pin = pin4 ,
  .pin[0].logic = MOTOR_OFF ,
  .pin[0].direction = Direction_Outpt ,
  .pin[1].port = PortD_Index ,
  .pin[1].pin = pin5 ,
  .pin[1].logic = MOTOR_OFF ,
  .pin[1].direction = Direction_Outpt ,
  
};

Dc_Motor_t dc_motor2 =
{
  .pin[0].port = PortD_Index ,
  .pin[0].pin = pin6 ,
  .pin[0].logic = MOTOR_OFF ,
  .pin[0].direction = Direction_Outpt ,
  .pin[1].port = PortD_Index ,
  .pin[1].pin = pin7 ,
  .pin[1].logic = MOTOR_OFF ,
  .pin[1].direction = Direction_Outpt ,
  
};

CCP_config_t ccp1 =
{
  .CCP_Mode = CCP_PWM_Mode_Select,
  .CCP_Select = CCP1_SELECT ,
  .PWM_Frequncy = 200,
  .Prescaler_Select = CCP_PRESCALER_DIV_BY_1,
  .Postscaler_Select = CCP_POSTSCALER_DIV_BY_4,
  .ccp_mode_varient = CCP_PWM_Mode_ ,
  .CCP_pin.port = PortC_Index,
  .CCP_pin.direction = Direction_Outpt,
  .CCP_pin.pin = pin2,
  .CCP2_IntterruptHandeler = NULL
};

 Timer2_conf_t timer_2={
       .Postscaler_Select = TIMER2_POSTSCALER_DIV_BY_1,
       .Prescaler_Select = TIMER2_PRESCALER_DIV_BY_4,
       .TIMER2_PRE_LOAD_VALUE = 0,
 };
 
 Segment_t segment ={
  .segment_pin[0].port = PortB_Index ,
  .segment_pin[0].pin = pin4 ,
  .segment_pin[0].direction = Direction_Outpt,
  .segment_pin[0].logic = Logic_Low,
  
  .segment_pin[1].port = PortB_Index ,
  .segment_pin[1].pin = pin5 ,
  .segment_pin[1].direction = Direction_Outpt,
  .segment_pin[1].logic = Logic_Low,
  
  .segment_pin[2].port = PortB_Index ,
  .segment_pin[2].pin = pin6 ,
  .segment_pin[2].direction = Direction_Outpt,
  .segment_pin[2].logic = Logic_Low,
  
  .segment_pin[3].port = PortB_Index ,
  .segment_pin[3].pin = pin7 ,
  .segment_pin[3].direction = Direction_Outpt,
  .segment_pin[3].logic = Logic_Low,
  
  .segment_type = Common_Cathode
};
  Segment_t segment2 ={
  .segment_pin[0].port = PortA_Index ,
  .segment_pin[0].pin = pin2 ,
  .segment_pin[0].direction = Direction_Outpt,
  .segment_pin[0].logic = Logic_Low,
  
  .segment_pin[1].port = PortA_Index ,
  .segment_pin[1].pin = pin3 ,
  .segment_pin[1].direction = Direction_Outpt,
  .segment_pin[1].logic = Logic_Low,
  
  .segment_pin[2].port = PortA_Index ,
  .segment_pin[2].pin = pin4 ,
  .segment_pin[2].direction = Direction_Outpt,
  .segment_pin[2].logic = Logic_Low,
  
  .segment_pin[3].port = PortD_Index ,
  .segment_pin[3].pin = pin0 ,
  .segment_pin[3].direction = Direction_Outpt,
  .segment_pin[3].logic = Logic_Low,
  
  .segment_type = Common_Cathode
};
 Pin_Config_t seg_en_1={
  .port = PortD_Index ,
  .pin = pin1 ,
  .direction = Direction_Outpt,
  .logic = Logic_Low
};
Pin_Config_t seg_en_2={
  .port = PortD_Index ,
  .pin = pin2 ,
  .direction = Direction_Outpt,
  .logic = Logic_Low
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

Led_t led_motor =
{
  .Port_Name = PortC_Index,
  .Pin_Name = pin0 ,
  .Led_Status = LED_OFF
};

void ISR_INT1(void);
EXT_INTX_t Int_1 ={
  .Interrupt_Handler = ISR_INT1,
  .EXT_pin.port = PortB_Index,
  .EXT_pin.direction = Direction_input,
  .EXT_pin.pin = pin1,
  .edge = EXT_INT_Raising_Edge,
  .source = EXT_INT1
};


Pin_Config_t Buzzer =
{
  .port = PortC_Index,
  .pin = pin6 ,
  .direction = Direction_Outpt,
  .logic = Logic_Low
};
/* ------------------------------------Software Interface Declarations-------------- */



#endif	/* APPLICATION_H */

