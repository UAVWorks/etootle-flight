#ifndef __SPI_H
#define __SPI_H


//SPI1��ʼ��
void	SPI1_Init(void);
void 	SPI1_GPIO_Configuration(void);

//SPI1��дһ�ֽ�����
unsigned char	SPI1_ReadWrite(unsigned char writedat);

#endif
