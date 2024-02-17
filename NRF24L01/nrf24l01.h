#ifndef __nrf24L01_H
#define __nrf24L01_H
#include "stdint.h"
#include "main.h"
/* ??? --------------------------------------------------------------------*/

#define NRF24L01_SPI_CS_ENABLE()    HAL_GPIO_WritePin(NRF_CS_GPIO_Port,NRF_CS_Pin,GPIO_PIN_RESET)
#define NRF24L01_SPI_CS_DISABLE()   HAL_GPIO_WritePin(NRF_CS_GPIO_Port,NRF_CS_Pin,GPIO_PIN_SET)
 
#define NRF24L01_CE_LOW()   HAL_GPIO_WritePin(NRF_CE_GPIO_Port,NRF_CE_Pin,GPIO_PIN_RESET)
#define NRF24L01_CE_HIGH() HAL_GPIO_WritePin(NRF_CE_GPIO_Port,NRF_CE_Pin,GPIO_PIN_SET)
 
#define NRF24L01_IRQ_PIN_READ()  HAL_GPIO_ReadPin(NRF_IRQ_GPIO_Port,NRF_IRQ_Pin)
 
 
// NRF24L01??????????
#define TX_ADR_WIDTH                                  5   	//5???????
#define RX_ADR_WIDTH                                  5   	//5???????
#define TX_PLOAD_WIDTH                                32  	//32?????????
#define RX_PLOAD_WIDTH                                32  	//32?????????
 
//NRF24L01???????
#define NRF_READ_REG                                  0x00  //??????,?5???????
#define NRF_WRITE_REG                                 0x20  //??????,?5???????
#define RD_RX_PLOAD                                   0x61  //?RX????,1~32??
#define WR_TX_PLOAD                                   0xA0  //?TX????,1~32??

#define ACK_PAYLOAD_W                                 0xA8 // Write ACK payload
#define FLUSH_TX                                      0xE1  //??TX FIFO???.??????
#define FLUSH_RX                                      0xE2  //??RX FIFO???.??????
#define REUSE_TX_PL                                   0xE3  //?????????,CE??,????????.
#define NOP                                           0xFF  //???,??????????	 
//SPI(NRF24L01)?????
#define CONFIG                                        0x00  //???????;bit0:1????,0????;bit1:???;bit2:CRC??;bit3:CRC??;
                                                            //bit4:??MAX_RT(??????????)??;bit5:??TX_DS??;bit6:??RX_DR??
#define EN_AA                                         0x01  //????????  bit0~5,????0~5
#define EN_RXADDR                                     0x02  //??????,bit0~5,????0~5
#define SETUP_AW                                      0x03  //??????(??????):bit1,0:00,3??;01,4??;02,5??;
#define SETUP_RETR                                    0x04  //??????;bit3:0,???????;bit7:4,?????? 250*x+86us
#define RF_CH                                         0x05  //RF??,bit6:0,??????;
#define RF_SETUP                                      0x06  //RF???;bit3:????(0:1Mbps,1:2Mbps);bit2:1,????;bit0:????????
#define STATUS                                        0x07  //?????;bit0:TX FIFO???;bit3:1,???????(??:6);bit4,???????
                                                            //bit5:????????;bit6:??????;
#define MAX_TX  		                                  0x10  //??????????
#define TX_OK   		                                  0x20  //TX??????
#define RX_OK   		                                  0x40  //???????
 
#define OBSERVE_TX                                    0x08  //???????,bit7:4,????????;bit3:0,?????
#define CD                                            0x09  //???????,bit0,????;
#define RX_ADDR_P0                                    0x0A  //????0????,????5???,?????
#define RX_ADDR_P1                                    0x0B  //????1????,????5???,?????
#define RX_ADDR_P2                                    0x0C  //????2????,???????,???,???RX_ADDR_P1[39:8]??;
#define RX_ADDR_P3                                    0x0D  //????3????,???????,???,???RX_ADDR_P1[39:8]??;
#define RX_ADDR_P4                                    0x0E  //????4????,???????,???,???RX_ADDR_P1[39:8]??;
#define RX_ADDR_P5                                    0x0F  //????5????,???????,???,???RX_ADDR_P1[39:8]??;
#define TX_ADDR                                       0x10  //????(?????),ShockBurstTM???,RX_ADDR_P0??????
#define RX_PW_P0                                      0x11  //??????0??????(1~32??),???0???
#define RX_PW_P1                                      0x12  //??????1??????(1~32??),???0???
#define RX_PW_P2                                      0x13  //??????2??????(1~32??),???0???
#define RX_PW_P3                                      0x14  //??????3??????(1~32??),???0???
#define RX_PW_P4                                      0x15  //??????4??????(1~32??),???0???
#define RX_PW_P5                                      0x16  //??????5??????(1~32??),???0???
#define FEATURE 									                    0x1D  //ack负载使能
#define DYNPD                                         0x1C  //ack负载长度
#define NRF_FIFO_STATUS                               0x17  //FIFO?????;bit0,RX FIFO??????;bit1,RX FIFO???;bit2,3,??
                                                            //bit4,TX FIFO???;bit5,TX FIFO???;bit6,1,?????????.0,???;
                              
 
/* ???? ------------------------------------------------------------------*/
void NRF24L01_RX_Mode(void);					//???????
void NRF24L01_TX_Mode(void);					//???????
void NRF24L01_RX_ACK_Mode(void);
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts);//????
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts);	//????		  
uint8_t NRF24L01_Read_Reg(uint8_t reg);					//????
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value);		//????
uint8_t NRF24L01_Check(void);						//??24L01????
uint8_t NRF24L01_TxPacket(uint8_t *txbuf);				//????????
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);				//????????
void NRF24L01_Packet_AP(uint8_t *txbuf,uint8_t len);
void NRF_LowPower_Mode(void);	
 
 
#endif


