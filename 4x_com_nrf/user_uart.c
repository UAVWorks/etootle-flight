
/* Includes ------------------------------------------------------------------*/
#include "user_uart.h"


#define SERIAL_TRANSMIT_BUFFER_SIZE 200

static volatile uint8_t serial_transmitBuffer[SERIAL_TRANSMIT_BUFFER_SIZE]; // ���ͻ�������
static volatile uint8_t serial_transmitBufferHead = 0; // ���ͻ�������ͷ������ָ����һ�����е�Ԫ��
static volatile uint8_t serial_transmitBufferTail = 0; // ���ͻ�������β������ָ�����һ����Ч��Ԫ��


enum
{
    UART_SUCCEED = 0,
    UART_FAILED  = 1
};

/*******************************************************************************
* Function Name  : UART1_Configuration
* Description    : Configures the uart1 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART1_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  /* USART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Configure the USART1*/ 
  USART_Init(USART1, &USART_InitStructure);

     /* Enable USART2 Receive and Transmit interrupts */
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_ITConfig(USART1,USART_IT_TC,ENABLE);
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	 //�����ж�ģʽ����


  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);  
}


/*******************************************************************************
* Function Name  : UART1_GPIO_Configuration
* Description    : Configures the uart1 GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART1_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // Configure USART1_Tx as alternate function push-pull 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure USART1_Rx as input floating 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
}


/*******************************************************************************
* Function Name  : Uart1_PutChar
* Description    : printf a char to the uart.
* Input          : ch ��Ҫ���͵��ַ� 
* Output         : None
* Return         : None
*******************************************************************************/
u8 Uart1_PutChar(u8 ch)//����һ���ַ�
{
  /* Write a character to the USART */
  USART_SendData(USART1, (u8) ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {
  }
  return ch;
}

/*******************************************************************************
* Function Name  : Uart1_PutString
* Description    : print a string to the uart1
* Input          : bufΪ�������ݵĵ�ַ , lenΪ�����ַ��ĸ���
* Output         : None
* Return         : None
*******************************************************************************/
void Uart1_PutString(u8* buf , u8 len)//����һ���ַ���
{
	u8 i;
    for(i=0;i<len;i++)
    {
        Uart1_PutChar(*buf++);
    }
}
//����1�ж�
void UART1_NVIC_Configuration(void)
{
	/* Enable the USART1 Interrupt */
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//����2���ж�����
void UART2_NVIC_Configuration(void)
{
    //����2�ж�
   /* Enable the USART2 Interrupt */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
   // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  
}
/*******************************************************************************
* Function Name  : UART2_Configuration
* Description    : Configures the uart1 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART2_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
  /* USART2 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Configure the USART2*/ 
  USART_Init(USART2, &USART_InitStructure);

   /* Enable USART2 Receive and Transmit interrupts */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //�����ж�ģʽ����
 

  /* Enable the USART2*/
  USART_Cmd(USART2, ENABLE);  
}


/*******************************************************************************
* Function Name  : UART1_GPIO_Configuration
* Description    : Configures the uart1 GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART2_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // Configure USART2_Tx as alternate function push-pull 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure USART2_Rx as input floating 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
}


/*******************************************************************************
* Function Name  : Uart2_PutChar
* Description    : printf a char to the uart.
* Input          : ch ��Ҫ���͵��ַ� 
* Output         : None
* Return         : None
*******************************************************************************/
u8 Uart2_PutChar(u8 ch)//����һ���ַ�
{
  /* Write a character to the USART */
  USART_SendData(USART2, (u8) ch);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
  {
  }
  return ch;
}

/*******************************************************************************
* Function Name  : Uart2_PutString
* Description    : print a string to the uart1
* Input          : bufΪ�������ݵĵ�ַ , lenΪ�����ַ��ĸ���
* Output         : None
* Return         : None
*******************************************************************************/
void Uart2_PutString(u8* buf , u8 len)//����һ���ַ���
{
	u8 i;
    for(i=0;i<len;i++)
    {
        Uart2_PutChar(*buf++);
    }
}


// ��ȡ���ͻ��������пռ��С��
uint8_t serial_transmitBufferIdle(void)
{

	if(serial_transmitBufferTail <= serial_transmitBufferHead)
		return SERIAL_TRANSMIT_BUFFER_SIZE - serial_transmitBufferHead + serial_transmitBufferTail;
	return serial_transmitBufferTail - serial_transmitBufferHead - 1; // -1:������������
}

// ��ȡ���ͻ������е����ݳ��ȡ�
uint32_t uart_transmitBufferSizeOccupy(void)
{
	if(serial_transmitBufferTail <= serial_transmitBufferHead)
		return serial_transmitBufferHead - serial_transmitBufferTail;
	return serial_transmitBufferHead  + (SERIAL_TRANSMIT_BUFFER_SIZE - serial_transmitBufferTail);
}

// �ӷ��ͻ�����ȡ��һ���ֽڣ����ͳ�ȥ��
// ����黺�����Ƿ�գ�����鷢��״̬��
void uart_transmitAByte(void)
{
    USART_SendData(USART1,serial_transmitBuffer[serial_transmitBufferTail]);
	serial_transmitBufferTail ++;
	if(serial_transmitBufferTail >= SERIAL_TRANSMIT_BUFFER_SIZE)
		serial_transmitBufferTail = 0;
}


// �������ݡ�
// ����0��ʾ�Ѿ����뻺����������1��ʾʧ�ܡ�
uint8_t serial_transmit(const void * data,uint8_t len)
{
	uint32_t i;

	if(serial_transmitBufferIdle() >= len)
	{
		for(i=0;i<len;i++)
		{
			serial_transmitBuffer[serial_transmitBufferHead] = ((uint8_t *)data)[i];
			serial_transmitBufferHead ++;
			if(serial_transmitBufferHead >= SERIAL_TRANSMIT_BUFFER_SIZE)
			serial_transmitBufferHead = 0;
		}	
	}

		__disable_irq();

		if(uart_transmitBufferSizeOccupy() > 0)
        if(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == SET) uart_transmitAByte();

   	__enable_irq();

	return 0;
}
