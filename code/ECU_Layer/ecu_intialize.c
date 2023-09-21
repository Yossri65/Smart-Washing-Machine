/* 
 * File:   ecu_intialize.c
 * Author: Yossri
 *
 * Created on July 13, 2023, 5:35 PM
 */

#include "ecu_intialize.h"


Led_t led_door =
{
  .Port_Name = PortC_Index,
  .Pin_Name = pin6 ,
  .Led_Status = LED_OFF
};


Led_t led_level_1 =
{
  .Port_Name = PortB_Index,
  .Pin_Name = pin4 ,
  .Led_Status = LED_OFF
};


Led_t led_level_2 =
{
  .Port_Name = PortB_Index,
  .Pin_Name = pin5 ,
  .Led_Status = LED_OFF
};


Led_t led_level_3 =
{
  .Port_Name = PortB_Index,
  .Pin_Name = pin6 ,
  .Led_Status = LED_OFF
};


Led_t wash_L_1 =
{
  .Port_Name = PortA_Index,
  .Pin_Name = pin0 ,
  .Led_Status = LED_OFF
};


Led_t wash_L_2 =
{
  .Port_Name = PortA_Index,
  .Pin_Name = pin1 ,
  .Led_Status = LED_OFF
};


Led_t wash_L_3 =
{
  .Port_Name = PortA_Index,
  .Pin_Name = pin2 ,
  .Led_Status = LED_OFF
};


Led_t led_rainse_1 =
{
  .Port_Name = PortD_Index,
  .Pin_Name = pin5 ,
  .Led_Status = LED_OFF
};


Led_t led_rainse_2 =
{
  .Port_Name = PortD_Index,
  .Pin_Name = pin6 ,
  .Led_Status = LED_OFF
};


Led_t led_cold =
{
  .Port_Name = PortC_Index,
  .Pin_Name = pin0 ,
  .Led_Status = LED_OFF
};
Led_t led_warm =
{
  .Port_Name = PortA_Index,
  .Pin_Name = pin5 ,
  .Led_Status = LED_OFF
};
Led_t led_hot =
{
  .Port_Name = PortD_Index,
  .Pin_Name = pin0 ,
  .Led_Status = LED_OFF
};


Std_ReturnType ecu_intialize()
{
    Std_ReturnType ret =E_NOT_OK;
    ret = Led_Intialize(&led_door);
    
    ret = Led_Intialize(&led_level_1);
    ret = Led_Intialize(&led_level_2);
    ret = Led_Intialize(&led_level_3);
    
    ret = Led_Intialize(&wash_L_1);
    ret = Led_Intialize(&wash_L_2);
    ret = Led_Intialize(&wash_L_3);
    
    ret = Led_Intialize(&led_rainse_1);
    ret = Led_Intialize(&led_rainse_2);
    
    ret = Led_Intialize(&led_cold);
    ret = Led_Intialize(&led_warm);
    ret = Led_Intialize(&led_hot);
    return ret;
}