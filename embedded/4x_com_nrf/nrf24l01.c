#include "stm32f10x_lib.h"
#include <stdlib.h>
#include <string.h>
#include "spi.h"
#include "nrf24l01.h"
#include "interpreter.h"

#define TX_PLOAD_WIDTH  32  	// 20 uints TX payload
#define rf_pin_ce_high() 		GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define rf_pin_ce_low() 		GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define CSN_H() 	GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define CSN_L() 	GPIO_ResetBits(GPIOA, GPIO_Pin_4)

///*********************************************NRF24L01*************************************
#define TX_ADR_WIDTH    4   	// 5 uints TX address width
#define RX_ADR_WIDTH    4   	// 5 uints RX address width

#define RX_PLOAD_WIDTH  32  	// 20 uints TX payload
u8  TX_ADDRESS[TX_ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7};	//���ص�ַ
u8  RX_ADDRESS[RX_ADR_WIDTH]= {0x76,0x64,0xB7,0x5A};	//���յ�ַ
///***************************************NRF24L01�Ĵ���ָ��*******************************************************
#define READ_REG        0x00  	// ���Ĵ���ָ��
#define WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PL_WID    0x60  	// ��ȡ��������ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define WR_ACK_PLOAD    0xA8    // дACK����ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP             0xFF  	// ����
///*************************************SPI(nRF24L01)�Ĵ�����ַ****************************************************
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define NRFRegSTATUS    0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��1�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��2�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��3�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��4�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��5�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
   
#define DYN_PLOAD_WIDTH_P0	   	0x1C
#define DYN_PLOAD_WIDTH     	0x1D  
///**************************************************************************************

#ifdef RF_USE_ACK_PAYLOAD
#warning Usb ACK payload mode, make sure the "remote controller", "bootloader", and "app" use the same rf mode
#endif

u8 NRF24L01_TxBuf[32]={0};
u8 RxBuf[32]={0};
u8 NRF_Dev_Addr[5];

u8 nrf_length;

///******************************************************************************************
///*��ʱ����,�Ǿ�ȷ��ʱ
///******************************************************************************************/
void Delay_us(u32 n)
{
	u32 i;
	
	while(n--)
	{
 	   i=2;
 	   while(i--);
  }
}

void NRF24L01_Delay(u32 dly)
{
	while(--dly);	//dly=100: 8.75us; dly=100: 85.58 us (SYSCLK=72MHz)
}

///****************************************************************************************
///*NRF24L01��ʼ��
///***************************************************************************************/
void init_NRF24L01(void)
{
 	u8 buf[5]={0};
 	u8 i;
 
  	Delay_us(100);
  	rf_pin_ce_low();    // chip enable
  	rf_pin_ce_high();   // Spi disable 
  	SPI_Read_Buf(TX_ADDR, buf, TX_ADR_WIDTH);//debug ����ԭ���ı��ص�ַ����λֵ�ǣ�0xE7 0xE7 0xE7 0xE7 0xE7
  	for(i=0;i<5;i++) NRF_Dev_Addr[i]=buf[i];

//	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); 	//д���ն˵�ַ

   	SPI_WR_Reg(WRITE_REG + CONFIG, 0x0F);		//k: enter power-up,enable cc, allow int
	SPI_WR_Reg(WRITE_REG + EN_RXADDR, 0x01);  			//������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21 
	SPI_WR_Reg(WRITE_REG + EN_AA, 0x01);		      	//Ƶ��0�Զ�	ACKӦ������	
 	SPI_WR_Reg(WRITE_REG + SETUP_AW, 0x02);
	SPI_WR_Reg(WRITE_REG + SETUP_RETR, 0x34); 			//�����Զ��ط�ʱ��ʹ�����500us + 86us, 10 retrans...
	SPI_WR_Reg(WRITE_REG + RF_CH, 0);        			//�����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_WR_Reg(WRITE_REG + RF_SETUP, 0x0E);   			//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB	
	SPI_WR_Reg(WRITE_REG + DYN_PLOAD_WIDTH_P0, 0x01);	//k: enable dynamic payload length in P0 only
#ifdef RF_USE_ACK_PAYLOAD
	SPI_WR_Reg(WRITE_REG + DYN_PLOAD_WIDTH, 0x04 | 0x02);		//k: enable dynamic payload lenth
#else
    SPI_WR_Reg(WRITE_REG + DYN_PLOAD_WIDTH, 0x04);		//k: enable dynamic payload lenth
#endif

	rf_pin_ce_high(); 
}

///****************************************************************************************************
///*������uchar SPI_Read(u8 reg)
///*���ܣ�NRF24L01��SPIʱ��
///****************************************************************************************************/
u8 SPI_RD_Reg(u8 reg)
{
	u8 reg_val;
	
	CSN_L();                		// CSN low, initialize SPI communication...
	SPI1_ReadWrite(reg);            // Select register to read from..
	reg_val = SPI1_ReadWrite(0);    // ..then read registervalue
	CSN_H();                		// CSN high, terminate SPI communication
	
	return(reg_val);        // return register value
}

//****************************************************************************************************/
//*���ܣ�NRF24L01��д�Ĵ�������
//****************************************************************************************************/
u8 SPI_WR_Reg(u8 reg, u8 value)
{
	u8 status;
	
	CSN_L();                   		// CSN low, init SPI transaction
	status = SPI1_ReadWrite(reg);	// select register
	SPI1_ReadWrite(value);          // ..and write value to it..
	CSN_H();                   		// CSN high again
	
	return(status);            		// return nRF24L01 status u8
}
///****************************************************************************************************/
//*������uint SPI_Read_Buf(u8 reg, u8 *pBuf, u8 Len)
//*����: ���ڶ����ݣ�reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uchars���������ݵĸ���
//****************************************************************************************************/
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 Len)
{
	u8 status,i;
	
	CSN_L();                    		// Set CSN low, init SPI tranaction
	status = SPI1_ReadWrite(reg);       		// Select register to write to and read status u8
	
  	for(i=0;i<Len;i++)
  	{
     	pBuf[i] = SPI1_ReadWrite(0);
  	}
	
	CSN_H();                           
	
	return(status);                    // return nRF24L01 status u8
}
//*********************************************************************************************************
//*������uint SPI_Write_Buf(u8 reg, u8 *pBuf, u8 Len)
//*����: ����д���ݣ�Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uchars��д�����ݵĸ���
//*********************************************************************************************************/
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 Len)
{
	u8 status,i;
	
	CSN_L();            //SPIʹ��       
	status = SPI1_ReadWrite(reg);   
	for(i=0; i<Len; i++) //
	{
	   SPI1_ReadWrite(*pBuf);
		 pBuf ++;
	}
	CSN_H();           //�ر�SPI
	return(status);    // 
}


//***********************************************************************************************************
//*������void nRF24L01_TxPacket(unsigned char * tx_buf)
//*���ܣ����� tx_buf������
//**********************************************************************************************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	rf_pin_ce_low();			//StandBy Iģʽ	
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // װ������	
	rf_pin_ce_high();		 //�ø�CE���������ݷ���
}

//****************************************************************************************************/
//*������void SetRX_Mode(void)
//*���ܣ����ݽ������� 
//****************************************************************************************************/


void rf_startReceive(void)
{
    rf_pin_ce_low();
#ifndef RF_USE_ACK_PAYLOAD
	SPI_WR_Reg(WRITE_REG + CONFIG, 0x0F);
#endif
    rf_pin_ce_high();
}

void rf_stopReceive(void)
{
    rf_pin_ce_low();
}



/*
 * �������ݡ�
 * ����ֵ��
 *  0���Ѿ��͵�RFоƬ��
 *  1��RFоƬ������������*/
u8 rf_transmit(u8 * data,u8 len)
{
	u8 status;
    /*
     * ��ȡ״̬��*/

//	status=SPI_RD_Reg(NOP);
	status=SPI_RD_Reg(NRFRegSTATUS);

    /*
     * �ж��Ƿ��п�λ��*/
    if(status & 0x01)	   //RF_REG_STATUS_TX_FULL
        return 1;
    //

    rf_pin_ce_low();
#ifdef RF_USE_ACK_PAYLOAD
    SPI_Write_Buf(WR_ACK_PLOAD,data,len);
#else
   	SPI_WR_Reg(WRITE_REG + CONFIG, 0x0E);  //TX


	SPI_Write_Buf(WR_TX_PLOAD,data,len);
#endif
    rf_pin_ce_high();
    //
    return 0;
}



void rf_checkEvent(void)
{
 	u8 status;
	u8 rx_buf[32]={0};

 	
	status=SPI_RD_Reg(NRFRegSTATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��

        /*
         * �յ����ݡ� */
        if(status & 0x40)
        {


			rf_stopReceive();
			nrf_length=SPI_RD_Reg(RD_RX_PL_WID);
		 	if((nrf_length>0)&&(nrf_length<33))
		 	SPI_Read_Buf(RD_RX_PLOAD,rx_buf,nrf_length);// read receive payload from RX_FIFO buffer

			interpreter_rf2serial(rx_buf,nrf_length);
 
			rf_startReceive();
        }
        /*
         * ���������ݡ� */
        if(status & 0x20)
        {
            /*
             * û�еȴ������ݾ�ֹͣ���͡� */

		  if(status & 0x01)	rf_pin_ce_low();      
          rf_startReceive();
        }
        /*
         * �ط�������� */
        if(status & 0x10)
        {
            /*
             * ��ջ�������*/
          	SPI_WR_Reg(FLUSH_TX,0xFF);
			rf_startReceive();
        }
        /*
         * ȡ���жϱ�ʶ��*/

		SPI_WR_Reg(WRITE_REG+NRFRegSTATUS, status);
			
  
}

