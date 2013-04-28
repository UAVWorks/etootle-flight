
#include <STM32F10X_SPI.h>
#include <STM32F10X_GPIO.h>
#include <STM32F10X_RCC.h>

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LED_GPIO_Configuration(void)//����ʹ�õ�GPIO��
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //����LEDʹ�õ�GPIO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//����GPIO����ģ��
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

}

