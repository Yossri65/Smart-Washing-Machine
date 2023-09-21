/* 
 * File:   hal_spi.c
 * Author: Yossri
 *
 * Created on August 15, 2023, 11:56 AM
 */
#include "hal_spi.h"

#ifdef SPI_INTERRUPT_FEATURE_ENABLE
    interruptHandler MSSP_SPI_IntterruptHandeler = NULL;
#endif

static void MSSP_SPI_Msster_Mode_GPIO_PIN_Configurations();
static void MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(const SPI_Config *_SPI_);

static void MSSP_SPI_Interrupt_ModeN_Configurations(const SPI_Config *_SPI_);

Std_ReturnType SPI_Init(const SPI_Config *_SPI_)
{
    Std_ReturnType ret = E_OK;
    if(_SPI_ == NULL) 
    {
        ret = E_NOT_OK;
    }
    else
    {
        SPI_DISABLE_MODULE();
        /*spi set mode*/
        SSPCON1bits.SSPM = _SPI_->spi_serial_clk_mod_select;
        
        if(SPI_MASTER_FOSC_DIV4 == _SPI_->spi_serial_clk_mod_select ||
                SPI_MASTER_FOSC_DIV16 == _SPI_->spi_serial_clk_mod_select ||
                SPI_MASTER_FOSC_DIV64 == _SPI_->spi_serial_clk_mod_select ||
                SPI_MASTER_FOSC_TMR2  == _SPI_->spi_serial_clk_mod_select)
        {
            MSSP_SPI_Msster_Mode_GPIO_PIN_Configurations();
            SSPSTATbits.SMP = _SPI_->spi_control.SampleSelect;
        }
        else if(SPI_SLAVE_SS_ENABLE ==  _SPI_->spi_serial_clk_mod_select || 
                SPI_SLAVE_SS_DISABLE == _SPI_->spi_serial_clk_mod_select)
        {
            MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(_SPI_);
            SSPSTATbits.SMP = 0;
        }else{/*Nothing*/}
        /* Clock Polarity Select */
        SSPCON1bits.CKP = _SPI_->spi_control.ClockPolarity;
        /* SPI Clock Select */
        SSPSTATbits.CKE = _SPI_->spi_control.ClockSelect;
        /* SPI Sample Select */
        
        /* Clear the Write Collision Detect */
        SSPCON1bits.WCOL = 0;
        
        /* Clear the Receive Overflow Indicator */
        SSPCON1bits.SSPOV = 0; 
/*        
#ifdef SPI_INTERRUPT_FEATURE_ENABLE
        MSSP_SPI_Interrupt_ModeN_Configurations(_SPI_);
#endif
 *  */
        SPI_ENABLE_MODULE();
    }
    return ret;
}

Std_ReturnType SPI_DeInit(const SPI_Config *_SPI_)
{
    Std_ReturnType ret = E_OK;
    if(_SPI_ == NULL) 
    {
        ret = E_NOT_OK;
    }
    else
    {
        /* Disable SPI Module */
        SPI_DISABLE_MODULE();
/* Interrupt Configurations */        
#ifdef SPI_INTERRUPT_FEATURE_ENABLE
        SPI_INTERRUPT_Disable();
#endif
    }
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



Std_ReturnType SPI_Send_Byte_NonBlocking(const SPI_Config *_SPI_, const uint8 _data)
{
    Std_ReturnType ret = E_OK;
    if(_SPI_ == NULL) 
    {
        ret = E_NOT_OK;
    }
    else
    {
        
    }
}

Std_ReturnType SPI_Read_Byte_NonBlocking(const SPI_Config *_SPI_, uint8 *_data)
{
    Std_ReturnType ret = E_OK;
    if(_SPI_ == NULL) 
    {
        ret = E_NOT_OK;
    }
    else
    {
        
    }
}
Std_ReturnType SPI_Send_Byte( const uint8 _data){
    Std_ReturnType ret = E_NOT_OK;
   
        SSPBUF = _data;
        while(!(PIR1bits.SSPIF)); /* Waiting to transmit */
        PIR1bits.SSPIF = 0;
    return ret;
}

Std_ReturnType SPI_Read_Byte( uint8 *_data){
    Std_ReturnType ret = E_NOT_OK;
    
    if( NULL == _data){
        ret = E_NOT_OK;
    }
    else{
        while(SSPSTATbits.BF == 0); /* Receive not complete, SSPBUF is empty */
        *_data = SSPBUF;
    }
    
    return ret;
}

static void MSSP_SPI_Msster_Mode_GPIO_PIN_Configurations()
{
    Std_ReturnType ret = E_NOT_OK;
    Pin_Config_t SPI_SDO = {.port = PortC_Index ,.pin = pin5 ,.direction = Direction_Outpt}; 
    Pin_Config_t SPI_SDI = {.port = PortC_Index ,.pin = pin4 ,.direction = Direction_input}; 
    Pin_Config_t SPI_SCK = {.port = PortC_Index ,.pin = pin3 ,.direction = Direction_Outpt}; 
    
    ret = GPIO_Pin_Direction_Intialize(&SPI_SDO);
    ret = GPIO_Pin_Direction_Intialize(&SPI_SDI);
    ret = GPIO_Pin_Direction_Intialize(&SPI_SCK);
}

static void MSSP_SPI_Slave_Mode_GPIO_PIN_Configurations(const SPI_Config *_SPI_)
{
    Std_ReturnType ret = E_NOT_OK;
    Pin_Config_t SPI_SDO = {.port = PortC_Index ,.pin = pin5 ,.direction = Direction_Outpt}; 
    Pin_Config_t SPI_SDI = {.port = PortC_Index ,.pin = pin4 ,.direction = Direction_input}; 
    Pin_Config_t SPI_SCK = {.port = PortC_Index ,.pin = pin3 ,.direction = Direction_input}; 
    Pin_Config_t SPI_SS = {.port = PortA_Index ,.pin = pin5 ,.direction = Direction_input}; 
    
    ret = GPIO_Pin_Direction_Intialize(&SPI_SDO);
    ret = GPIO_Pin_Direction_Intialize(&SPI_SDI);
    ret = GPIO_Pin_Direction_Intialize(&SPI_SCK);  
    
     if(SPI_SLAVE_SS_ENABLE == _SPI_->spi_serial_clk_mod_select){
        ret = GPIO_Pin_Direction_Intialize(&SPI_SS);
    }
}
#ifdef SPI_INTERRUPT_FEATURE_ENABLE
static void MSSP_SPI_Interrupt_ModeN_Configurations(const SPI_Config *_SPI_)
{
    SPI_INTERRUPT_Enable();
    SPI_INTERRUPT_Clear_Flag();
    MSSP_SPI_IntterruptHandeler = _SPI_->SPI_IntterruptHandeler;
    
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if(High_Priority == _SPI_->priority)
    {
        INTERRUPT_HighPriorityEnable();
        SPI_INTERRUPT_Priority_High();
    }
    else if(Low_Priority == _SPI_->priority)
    {
        INTERRUPT_LowPriorityEnable();
        SPI_INTERRUPT_Priority_Low();
    }else{/*Nothing*/}
#else
    INTERRUPT_AllInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
#endif

}
#endif

/*void SPI_ISR(void)
{
#ifdef SPI_INTERRUPT_FEATURE_ENABLE
    SPI_INTERRUPT_Clear_Flag();
    if(MSSP_SPI_IntterruptHandeler){
        MSSP_SPI_IntterruptHandeler();
    }
#endif
}*/