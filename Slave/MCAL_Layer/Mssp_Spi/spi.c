/* 
 * File:   spi.c
 * Author: Yossri
 *
 * Created on September 13, 2023, 5:48 PM
 */
#include "spi.h"


#ifdef SPI_INTERRUPT_FEATURE_ENABLE
    interruptHandler MSSP_SPI_IntterruptHandeler = NULL;
#endif
//static void MSSP_SPI_Interrupt_ModeN_Configurations(const mssp_spi_t *spi_obj);


Std_ReturnType MSSP_SPI_Intiailize(const mssp_spi_t* spi_obj)
{
    Std_ReturnType ret = E_NOT_OK;
    
    if(NULL != spi_obj)
    {
        ret = E_OK;
        
        /* Disable SPI Module */
        MSSP_SPI_DISABLE();
        
        /* Pin Initialize */
        TRISCbits.RC5 = 0;
//        TRISAbits.RA5 = 1;
        // Master mode
        if(0 <= spi_obj->mode && 3 >= spi_obj->mode)
        {
            TRISCbits.RC3 = 0;
            /* Select Sample Middle/End */
            MSSP_SPI_SELECT_SAMPLE(spi_obj->sample_pos);
        }
        // Slave mode
        else if(4 <= spi_obj->mode && 5 >= spi_obj->mode)
        {
            TRISCbits.RC3 = 1;
            /* Sample bit(SMP) must be cleared at Slave mode */
            MSSP_SPI_SELECT_SAMPLE(0);
        }
        else {/* Incorrect mode */}
        
        /* Select mode */
        MSSP_SPI_MODE_SELECT(spi_obj->mode);
        
        /* Select Clock polarity */
        MSSP_SPI_IDLE_STATE(spi_obj->idle_state);
        
        /* Select Sample Edge */
        MSSP_SPI_SAMPLE_EDGE(spi_obj->sampling_edge);
        
        /* Clear the Write Collision Detect */
        SSPCON1bits.WCOL = 0;
        
        /* Clear the Receive Overflow Indicator */
        SSPCON1bits.SSPOV = 0; 
        
#ifdef SPI_INTERRUPT_FEATURE_ENABLE
        MSSP_SPI_Interrupt_ModeN_Configurations(spi_obj);
#endif
        /* Enable SPI Module */
        MSSP_SPI_ENABLE();
    }
    else{/* Nothing */}
    
    return ret;
}

Std_ReturnType MSSP_SPI_DeIntiailize()
{
    Std_ReturnType ret = E_OK;
    /* Disable SPI Module */
    MSSP_SPI_DISABLE();
   
    return ret;
}

Std_ReturnType MSSP_SPI_Trancieve(uint8 send_data, uint8* rec_data)
{
    Std_ReturnType ret = E_NOT_OK;
    
    if(NULL != rec_data)
    {
        ret = E_OK;
        /* load data to SSPBUF */
        SSPBUF = send_data;
        /* Wait until data transfer */
        while(!PIR1bits.SSPIF);
        /* Clear flag */
        PIR1bits.SSPIF = 0;
        /* Read received data */
        *rec_data = SSPBUF;
    }
    else{/* Nothing */}
    
    return ret;
}

/*
static void MSSP_SPI_Interrupt_ModeN_Configurations(const mssp_spi_t *spi_obj)
{
#ifdef SPI_INTERRUPT_FEATURE_ENABLE
    
    SPI_INTERRUPT_Enable();
    SPI_INTERRUPT_Clear_Flag();
    MSSP_SPI_IntterruptHandeler = spi_obj->SPI_IntterruptHandeler;
    
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if(High_Priority == spi_obj->priority)
    {
        INTERRUPT_HighPriorityEnable();
        SPI_INTERRUPT_Priority_High();
    }
    else if(Low_Priority == spi_obj->priority)
    {
        INTERRUPT_LowPriorityEnable();
        SPI_INTERRUPT_Priority_Low();
    }else{}
#else
    INTERRUPT_AllInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif
#endif

}
/*
void SPI_ISR(void)
{
#ifdef SPI_INTERRUPT_FEATURE_ENABLE
    SPI_INTERRUPT_Clear_Flag();
    if(MSSP_SPI_IntterruptHandeler){
        MSSP_SPI_IntterruptHandeler();
    }
#endif
}*/