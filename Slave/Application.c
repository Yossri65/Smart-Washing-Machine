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



int main(void) {
    intialization();
    
    while(1)
    {
        
/*===============adc start conv to get temperature and smoke level===============*/
        if(0 == adc_flag){
             ret = ADC_Start_Conversion_Interrupt_Mode(&adc_temp_wl ,ADC_CHANNEL_AN0);
        }else if(1 == adc_flag){
            ret = ADC_Start_Conversion_Interrupt_Mode(&adc_temp_wl ,ADC_CHANNEL_AN1);
        }
        else{/*Nothing*/}
        
/*=============convert digital output to analog for processing=============*/
        temperature = 4.88f * conv_result_temp;
        water_level = 4.88f * conv_result_water_level;
        
        temperature /= 10;
        water_level /= 10;
        
        water_level = (water_level * 100)/ 500;
        
        /*read from master*/
        while(array_slave[4] == 0){
            for(uint8 count = 0 ; count < 5 ; count++){
                ret = MSSP_SPI_Trancieve(0x01 ,&array_slave[count]);
            }
        }
       
         Start_status = array_slave[0];
         water_level_status = array_slave[1];
         wash_level_status = array_slave[2];
         rainse_level_status = array_slave[3];
         temp_level_status = array_slave[4];
        
        if(START == Start_status){
/*==============================level of water==============================*/
            if(LEVEL_1 == water_level_status && Water_Level_1 == water_level){
                flag_water = TRUE;
                ret = Dc_Motor_Stop(&dc_motor2);
            }else if(LEVEL_2 == water_level_status && Water_Level_2 == water_level){
                flag_water = TRUE;
                ret = Dc_Motor_Stop(&dc_motor2);
            }else if(LEVEL_3 == water_level_status && Water_Level_3 == water_level){
                flag_water = TRUE;
                ret = Dc_Motor_Stop(&dc_motor2);
            }else{
                ret = Dc_Motor_Turn_Right(&dc_motor2);
            }
            
/*==============================temperature chosen==============================*/
            
            if(COLD == temp_level_status && COLD_TEMP == temperature){
                flag_temp = TRUE;
            }else if(WARM == temp_level_status && WARM_TEMP == temperature){
                flag_temp = TRUE;
            }else if(HOT == temp_level_status && HOT_TEMP == temperature){
                flag_temp = TRUE;
            }else{/*Nothing*/}
            
            /*Washing start*/
            if(TRUE == flag_temp && TRUE == flag_water){
                
/*==============================calculate time of all program==============================*/
                if(Raisne_Level_1 == rainse_level_status){
                        time_of_raisne = Raisne_time_1;
                }else{
                        time_of_raisne = Raisne_time_2;
                    }
                if(LEVEL_1 == wash_level_status){
                    
                    time_of_washing = Wash_Level_1;  
                }else if(LEVEL_2 == wash_level_status){
                    time_of_washing = Wash_Level_2;
                }else if(LEVEL_3 == wash_level_status){
                    time_of_washing = Wash_Level_3;
                }else{/*Nothing*/}
                
                time_of_prog = time_of_washing + time_of_raisne;
                num_on_7seg = time_of_prog;
/*==============================washing system==============================*/
                second_ = 0;
                minutes = 0 ;
                ret = Timer0_Intialization(&timer0);
                ret = CCP_PWM_Set_Duty(&ccp1 , 50);
                ret = CCP_PWM_Start(&ccp1);
                while(minutes < time_of_washing){
                    ret = Led_Turn_On(&led_motor);
                    ret = Dc_Motor_Turn_Right(&dc_motor1);
                    __delay_ms(1000);
                    ret = Dc_Motor_Stop(&dc_motor1);
                    __delay_ms(500);
                    ret = Dc_Motor_Turn_Left(&dc_motor1);
                    __delay_ms(500);
                    while(flag_stop % 2 == 0){
                    ret = Dc_Motor_Stop(&dc_motor1);
                    ret = Led_Toggle(&led_motor);
                    __delay_ms(500);
                 }
                if(minutes > num_of_minutes){
                        num_on_7seg--;
                        num_of_minutes++;
                    }
                }
/*==============================dry system==============================*/
                second_ = 0;
                minutes = 0 ;
                ret = CCP_PWM_Set_Duty(&ccp1 , 100);
                ret = CCP_PWM_Start(&ccp1);
                servo_motor_set_angle(&Water_drainage ,90);
                while(minutes < time_of_raisne){
                    ret = Dc_Motor_Turn_Right(&dc_motor1);
                    ret = Led_Turn_On(&led_motor);
                    while(flag_stop % 2 == 0){
                    ret = Dc_Motor_Stop(&dc_motor1);
                    ret = Led_Toggle(&led_motor);
                    __delay_ms(500); 
                    }
                    end_of_washing = 1;
                    if(minutes > num_of_minutes){
                        num_on_7seg--;
                        num_of_minutes++;
                    }
                }
/*==============================end of washing system==============================*/
                if(end_of_washing == 1){
                        ret = Dc_Motor_Stop(&dc_motor1);
                        ret = Led_Turn_Off(&led_motor);
                        second_ = 0;
                        while(second_ < 8){
                            ret = GPIO_Pin_Write_Logic(&Buzzer ,Logic_High);
                         __delay_ms(500);
                            ret = GPIO_Pin_Write_Logic(&Buzzer ,Logic_Low);
                            __delay_ms(500);
                            Start_status = STOP;
                        }
                    }
                
            } 
    }
    }
    return (EXIT_SUCCESS);
}

void intialization (void)
{
    ecu_intialize();
    ret = SPI_Init(&spi_slave);
    ret = ADC_Intialization(&adc_temp_wl);
    
    ret = EXT_INTX_Intialize(&Int_1);
    
    ret = timer3_init(&timer3);
    ret = servo_motor_intialize(&Water_drainage);
    
    ret = Dc_Motor_Intialize(&dc_motor1);
    ret = Dc_Motor_Intialize(&dc_motor2);
    
    ret = CCP_intialize(&ccp1);
    
    ret = GPIO_Pin_Intialize(&seg_en_1);
    ret = GPIO_Pin_Intialize(&seg_en_2);
    
    ret = GPIO_Pin_Intialize(&Buzzer);
    
    ret = Segment_Intialize(&segment);
    ret = Segment_Intialize(&segment2);
    
    ret = Led_Intialize(&led_motor);
}
/*==============================ADC==============================*/
void adc_isr(void)
{
    if(0 == adc_flag){
        ret = ADC_Get_Conversion_Result(&adc_temp_wl ,&conv_result_temp);
        adc_flag = 1;
    }else if(1 == adc_flag){
        ret = ADC_Get_Conversion_Result(&adc_temp_wl ,&conv_result_water_level);
        adc_flag = 0;
    }else{/*Nothing*/}
}
/*=========================real time by timer3=========================*/
void timer3_isr(void){
    second_++;
    if (second_==60){
        second_=0;
        minutes++;
    }
    if (minutes==60){
        minutes=0;
    }
}
/*==============================seven segment==============================*/
void timer0_isr(void){
    if(num_on_7seg >= 10){
    ret = GPIO_Pin_Write_Logic(&seg_en_1 ,Logic_High);
    ret = Segment_Write_Number(&segment , num_on_7seg /10);

    ret = GPIO_Pin_Write_Logic(&seg_en_2 ,Logic_High);
    ret = Segment_Write_Number(&segment2 , num_on_7seg %10);
    }else{
        ret = GPIO_Pin_Write_Logic(&seg_en_1 ,Logic_High);
        ret = Segment_Write_Number(&segment , num_on_7seg);
    }
}

void ISR_INT1(void){
    flag_stop++;
}