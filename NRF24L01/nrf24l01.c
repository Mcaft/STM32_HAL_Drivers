#include "nrf24l01.h"
#include "stm32g0xx_hal_spi.h"
#include "stdio.h"



extern UART_HandleTypeDef huart1;
 extern SPI_HandleTypeDef hspi1;
const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x40,0x43,0x10,0x10,0x01}; //????
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x40,0x43,0x10,0x10,0x01}; //????



/**
  * 函数功能: 往串行Flash读取写入一个字节数据并接收一个字节数据
  * 输入参数: byte：待发送数据
  * 返 回 值: uint8_t：接收到的数据
  * 说    明：无
  */
uint8_t SPIx_ReadWriteByte(SPI_HandleTypeDef* hspi,uint8_t byte)
{
  uint8_t d_read,d_send=byte;
  if(HAL_SPI_TransmitReceive(hspi,&d_send,&d_read,1,0xFF)!=HAL_OK)
  {
    d_read=0xFF;
  }
  return d_read; 
}
 
/**
  * ????: ??24L01????
  * ????: ?
  * ? ? ?: 0,??;1,??
  * ?    ?:?          
  */ 
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
   
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//??5??????.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //???????  
	for(i=0;i<5;i++){if(buf[i]!=0XA5)break;	} 							   
	if(i!=5)return 1; 	
	return 0;		 	//???24L01
}	
 
/**
  * ????: SPI????
  * ????: ?
  * ? ? ?: ?
  * ?    ?:reg:???????
  *           
  */ 
uint8_t NRF24L01_Write_Reg(uint8_t reg,uint8_t value)
{
    uint8_t status;	
  NRF24L01_SPI_CS_ENABLE();                 //??SPI??
  status =SPIx_ReadWriteByte(&hspi1,reg);   //?????? 
  SPIx_ReadWriteByte(&hspi1,value);         //???????
  NRF24L01_SPI_CS_DISABLE();                //??SPI??	   
  return(status);       			//?????
}
 
/**
  * ????: ??SPI????
  * ????: ?
  * ? ? ?: ?
  * ?    ?:reg:??????
  *           
  */ 
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;	    
 	NRF24L01_SPI_CS_ENABLE();          //??SPI??		
  SPIx_ReadWriteByte(&hspi1,reg);   //??????
//	SPIx_ReadWriteByte(&hspi1,0XFF);
  reg_val=SPIx_ReadWriteByte(&hspi1,0XFF);//???????
  NRF24L01_SPI_CS_DISABLE();          //??SPI??		    
  return(reg_val);           //?????
}		
 
/**
  * ????: ??????????????
  * ????: ?
  * ? ? ?: ??????????? 
  * ?    ?:?
  *           
  */ 
uint8_t NRF24L01_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,uint8_t_ctr;	   
  
  NRF24L01_SPI_CS_ENABLE();           //??SPI??
  status=SPIx_ReadWriteByte(&hspi1,reg);//??????(??),??????   	   
for(uint8_t_ctr=0;uint8_t_ctr<len;uint8_t_ctr++)
  {
    pBuf[uint8_t_ctr]=SPIx_ReadWriteByte(&hspi1,0XFF);//????
  }
  NRF24L01_SPI_CS_DISABLE();       //??SPI??
  return status;        //????????
}
 
/**
  * ????: ?????????????
  * ????: ?
  * ? ? ?: ?
  * ?    ?:reg:???(??)  *pBuf:????  len:????
  *           
  */ 
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status,uint8_t_ctr;	    
 	NRF24L01_SPI_CS_ENABLE();          //??SPI??
  status = SPIx_ReadWriteByte(&hspi1,reg);//??????(??),??????
  for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)
  {
    SPIx_ReadWriteByte(&hspi1,*pBuf++); //????	 
  }
  NRF24L01_SPI_CS_DISABLE();       //??SPI??
  return status;          //????????
}		
 
/**
  * ????: ??NRF24L01??????
  * ????: ?
  * ? ? ?: ????TX_OK,????0xff
  * ?    ?:txbuf:????????
  *           
  */ 
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta; 
	NRF24L01_CE_LOW();
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//????TX BUF  32???
 	NRF24L01_CE_HIGH();//????	 
  
	while(NRF24L01_IRQ_PIN_READ()!=0);//??????
  
	sta=NRF24L01_Read_Reg(STATUS);  //?????????	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //??TX_DS?MAX_RT????
	if(sta&MAX_TX)//????????
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//??TX FIFO??? 
		return MAX_TX; 
	}
	if(sta&TX_OK)//????
	{
		return TX_OK;
	}
	return 0xff;//????????
}
 
/**
  * ????:??NRF24L01??????
  * ????: ?
  * ? ? ?: ????0,????1
  * ?    ?:?
  *           
  */ 
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
	uint8_t sta;		 
	sta=NRF24L01_Read_Reg(STATUS);  //?????????    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //??TX_DS?MAX_RT????
	if(sta&RX_OK)//?????
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//????
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//??RX FIFO??? 
		return 0; 
	}	   
	return 1;//???????
}			
 
/**
  * ????: ??????NRF24L01?RX??
  * ????: ?
  * ? ? ?: ?
  * ?    ?:?
  *           
  */ 
void NRF24L01_RX_Mode(void)
{
  NRF24L01_CE_LOW();	  
  NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0F);//???????????;PWR_UP,EN_CRC,16BIT_CRC 
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //????0?????    
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//????0?????  	 
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //??RF????		  
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//??TX????,0db??,2Mbps,???????   
  NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//????0??????? 	       
  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//?RX????
  NRF24L01_CE_HIGH(); //CE??,?????? 
  HAL_Delay(1);
}	
 
/**
  * ????: ??????NRF24L01?TX??
  * ????: ?
  * ? ? ?: ?
  * ?    ?:?
  *           
  */ 
void NRF24L01_TX_Mode(void)
{														 
  NRF24L01_CE_LOW();	    
  NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//?TX???? 
  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //??TX????,??????ACK	  
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x00);     //关闭自动应答 
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能接收地址
  NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//??????????:4000us + 86us;????????:15?
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,45);       //信道45
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //发送
  NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //???????????;PWR_UP,EN_CRC,16BIT_CRC,????,??????
  NRF24L01_CE_HIGH();//CE??,10us?????
  HAL_Delay(1);
}

void NRF24L01_RX_ACK_Mode(void)
{
    NRF24L01_CE_LOW();
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);   //使能通道0的接收地址  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	        //设置RF通信频率		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);    //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
    
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);       //使能通道0的自动应答  
    NRF24L01_Write_Reg(NRF_WRITE_REG+FEATURE,0x06);     //使能动态负载(收发数据)长度
    NRF24L01_Write_Reg(NRF_WRITE_REG+DYNPD,0x01);       //使能通道0动态负载长度
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);     //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
    
    NRF24L01_CE_HIGH();
}

void NRF24L01_Packet_AP(uint8_t *txbuf,uint8_t len)
{
    NRF24L01_CE_LOW();
  	NRF24L01_Write_Buf(ACK_PAYLOAD_W,txbuf,len);//写数据到TX BUF
    NRF24L01_CE_HIGH();
}

 
/**
  * ????: ???NRF24L01???????
  * ????: ?
  * ? ? ?: ?
  * ?    ?:?
  *           
  */
void NRF_LowPower_Mode(void)
{
	NRF24L01_CE_LOW();	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x00);		//??????:????
}


