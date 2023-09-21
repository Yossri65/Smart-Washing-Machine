/* 
 * File:   Application.c
 * Author: Yossri
 *
 * Created on July 7, 2023, 8:09 PM
 */ 
#include <builtins.h>

#include "Application.h"
Std_ReturnType ret = E_NOT_OK;
void intialization (void);

uint8 flag_spi = 0;
uint8 array[5];
int main(void) {
    intialization();
 
    while(1)
    {
        
        if(start_programm != START){
/*======================================water level======================================*/
            ret = Button_Read_Status(&btn_water_L ,&water_level_status);
            if(water_level_status == BUTTON_PRESSED){
                Timer0_Write_Value(&timer0 , 48036);
                if(counter_btn_level > 2){
                    water_level_flag++;
                    if(LEVEL_1 == water_level_flag){
                        ret = Led_Turn_On(&led_level_1);
                        ret = Led_Turn_Off(&led_level_2);
                        ret = Led_Turn_Off(&led_level_3);
                    }else if(LEVEL_2 == water_level_flag){
                        ret = Led_Turn_On(&led_level_2);
                    }else if(LEVEL_3 == water_level_flag){
                        ret = Led_Turn_On(&led_level_3);
                    }else{
                        water_level_flag = 0;
                    } 
                    counter_btn_level = 0;
                }
            }
/*======================================wash level======================================*/
            ret = Button_Read_Status(&btn_wash ,&wash_level_status);
            if(wash_level_status == BUTTON_PRESSED){
                Timer0_Write_Value(&timer0 , 48036);
                if(counter_wash_times > 1){
                    wash_level_flag++;
                    if(LEVEL_1 == wash_level_flag){
                        ret = Led_Turn_On(&wash_L_1);
                        ret = Led_Turn_Off(&wash_L_2);
                        ret = Led_Turn_Off(&wash_L_3);
                    }else if(LEVEL_2 == wash_level_flag){
                        ret = Led_Turn_Off(&wash_L_1);
                        ret = Led_Turn_On(&wash_L_2);
                        ret = Led_Turn_Off(&wash_L_3);
                    }else if(LEVEL_3 == wash_level_flag){
                        ret = Led_Turn_Off(&wash_L_1);
                        ret = Led_Turn_Off(&wash_L_2);
                        ret = Led_Turn_On(&wash_L_3);
                    }else{
                        wash_level_flag = 0;
                    } 
                    counter_wash_times = 0;
                }
            }
/*======================================Rainse level======================================*/
            ret = Button_Read_Status(&btn_rainse ,&rainse_level_button);
            if(rainse_level_button == BUTTON_PRESSED){
                Timer0_Write_Value(&timer0 , 48036);
                if(counter_rainse_times > 1){
                    rainse_level_flag++;
                    if(LEVEL_1 == rainse_level_flag){
                        ret = Led_Turn_On(&led_rainse_1);
                        ret = Led_Turn_Off(&led_rainse_2);
                    }else if(LEVEL_2 == rainse_level_flag){
                        ret = Led_Turn_Off(&led_rainse_1);
                        ret = Led_Turn_On(&led_rainse_2);
                    }else{
                        rainse_level_flag = 0;
                    } 
                    counter_rainse_times = 0;
            }
            }
/*======================================tempreture level======================================*/
            ret = Button_Read_Status(&btn_temp ,&temp_level_button);
            if(temp_level_button == BUTTON_PRESSED){
                Timer0_Write_Value(&timer0 , 48036);
                if(counter_temp_times > 1){
                    temp_level_flag++;
                    if(LEVEL_1 == temp_level_flag){
                        ret = Led_Turn_On(&led_cold);
                        ret = Led_Turn_Off(&led_warm);
                        ret = Led_Turn_Off(&led_hot);
                    }else if(LEVEL_2 == temp_level_flag){
                        ret = Led_Turn_Off(&led_cold);
                        ret = Led_Turn_On(&led_warm);
                        ret = Led_Turn_Off(&led_hot);
                    }else if(LEVEL_3 == temp_level_flag){
                       ret = Led_Turn_Off(&led_cold);
                        ret = Led_Turn_Off(&led_warm);
                        ret = Led_Turn_On(&led_hot);
                    }else{
                        temp_level_flag = 0;
                    } 
                    counter_temp_times = 0;
            }
            }
            array[0] = 1;
            array[1] = water_level_flag;
            array[2] = wash_level_flag;
            array[3] = rainse_level_flag;
            array[4] = temp_level_flag;
        }else{
            if(flag_spi > 0){
            ret = GPIO_Pin_Write_Logic(&pin_enable , Logic_Low);
            ret = MSSP_SPI_Trancieve(array[0] ,&data_from_slave);
            ret = GPIO_Pin_Write_Logic(&pin_enable , Logic_High);
            for(uint8 count = 1 ; count < 5 ; count++){
            ret = GPIO_Pin_Write_Logic(&pin_enable , Logic_Low);
            ret = MSSP_SPI_Trancieve(array[count] ,&data_from_slave);
            ret = GPIO_Pin_Write_Logic(&pin_enable , Logic_High);
            }
            flag_spi--;
            }
        }
    }
    return (EXIT_SUCCESS);
}
void intialization (void)
{
    ecu_intialize();
    ret = Button_Initialize(&btn_water_L);
    ret = Button_Initialize(&btn_wash);
    ret = Button_Initialize(&btn_rainse);
    ret = Button_Initialize(&btn_temp);
    
    ret = Timer0_Intialization(&timer0);
    
    
    ret = EXT_INTX_Intialize(&Int_1);
    ret = EXT_INTX_Intialize(&Int_2);
    
    ret = servo_motor_intialize(&Door_gate);
    
    
    ret = SPI_Init(&spi_master);
    
    ret = GPIO_Pin_Intialize(&pin_enable);
    
}
volatile uint8 flag = CLOSE_DOOR; 
void ISR_INT1(void){
    if(CLOSE_DOOR == flag){
        flag = OPEN_DOOR;
        ret = Led_Turn_On(&led_door);
        servo_motor_set_angle(&Door_gate ,DOOR_IS_OPEN);
    }else if(OPEN_DOOR == flag){
        flag = CLOSE_DOOR;
        ret = Led_Turn_Off(&led_door);
        servo_motor_set_angle(&Door_gate ,DOOR_IS_CLOSE);
    }
} 

void ISR_INT2(void){
    if(STOP == start_programm){
        start_programm = START;
        flag_spi = 5;
    }else if(START == start_programm){
        start_programm = STOP;
    }else{/*Nothing*/}
}


void timer0_isr(void){
    counter_wash_times++;
    counter_btn_level++;
    counter_rainse_times++;
    counter_temp_times++;
}


