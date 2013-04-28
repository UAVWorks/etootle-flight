/* interpreter.c
 * 2012-9-13 16:23:12
 * js200300953
 */

#include "interpreter.h"
#include "user_uart.h"
#include "nrf24l01.h"
#include "led.h"

#define INTERPRETER_BUFFER_SIZE 200
#define INTERPRETER_ST_FINDING_55 100	 //0x64
#define INTERPRETER_ST_NEED_AA 101		 //0x65
#define INTERPRETER_ST_MAX_LEN 32		 //0x20

static volatile uint8_t interpreter_buffer[INTERPRETER_BUFFER_SIZE]; // ��������
static volatile uint8_t interpreter_bufferHead = 0; // ������ͷ������ָ����һ�����е�Ԫ��
static volatile uint8_t interpreter_bufferTail = 0; // ������β������ָ�����һ����Ч��Ԫ��
static volatile uint8_t interpreter_status = INTERPRETER_ST_FINDING_55; //״̬��

void serial_received(uint8_t data);
void interpreter_rf2serial(const void * rfData,uint8_t len);
uint8_t interpreter_getBufferValidDataLen(void);
void inpertreter_checkFrame(void);

void serial_received(uint8_t data)
{
	// ������һ��head��ֵ�������жϻ������Ƿ�������
	uint8_t head = interpreter_bufferHead + 1;
	if(head >= INTERPRETER_BUFFER_SIZE)
		head = 0;
	//
	// ����������head����tail���򻺳��������������ݡ�
	if(head == interpreter_bufferTail)
		return;
	//
	// ��ֵ���뻺������
	interpreter_buffer[interpreter_bufferHead] = data;
	interpreter_bufferHead = head;
}

// ��nRF24L01+���յ������ݷ������ڡ�
void interpreter_rf2serial(const void * rfData,uint8_t len)
{
	uint8_t frameHead[] = {0x55,0xAA,0x00};

	frameHead[2]=len;
	serial_transmit(frameHead,sizeof(frameHead));
	serial_transmit(rfData,len);
}

// ��ȡ�������У���Ч���ݵĳ��ȡ�
uint8_t interpreter_getBufferValidDataLen(void)
{
	if(interpreter_bufferTail <= interpreter_bufferHead)
		return interpreter_bufferHead - interpreter_bufferTail;
	return INTERPRETER_BUFFER_SIZE - interpreter_bufferTail + interpreter_bufferHead;
}

// �ػ�������ȡһ���ֽڡ�����ʱ���뱣֤�����ݡ�
uint8_t interpreter_getAByte(void)
{
	uint8_t rt =  interpreter_buffer[interpreter_bufferTail];
	interpreter_bufferTail ++;
	if(interpreter_bufferTail >= INTERPRETER_BUFFER_SIZE)
		interpreter_bufferTail = 0;
	//
	return rt;
}

// ����Ƿ��յ�һ��֡����main��ѭ�����á�
void inpertreter_checkFrame(void)
{
uint8_t i;
uint8_t rfBuf[INTERPRETER_ST_MAX_LEN];

	const uint8_t validLen = interpreter_getBufferValidDataLen();
	//
	if(interpreter_status == INTERPRETER_ST_FINDING_55)
	{
		// ��Ҫ�ж��Ƿ����㹻�����ݡ�
		if(validLen < 1)
			return;
		if(interpreter_getAByte() == (uint8_t)0x55)
			interpreter_status = INTERPRETER_ST_NEED_AA;
	}
	else if(interpreter_status == INTERPRETER_ST_NEED_AA)
	{
		if(validLen < 2)
			return;
		if(interpreter_getAByte() == (uint8_t)0xAA)
		{
			// ��ȡ֡���ȡ�
			interpreter_status = interpreter_getAByte();
			//
			// �жϳ�����Ч�ԡ�
			if(interpreter_status > INTERPRETER_ST_MAX_LEN)
				interpreter_status = INTERPRETER_ST_FINDING_55;
		}
		else
		// ���״̬��NEED_AA����ʵ�ʲ��ǣ�������FINDING_55��
			interpreter_status = INTERPRETER_ST_FINDING_55;
	}
	else if(interpreter_status <= INTERPRETER_ST_MAX_LEN)
	{
		if(validLen < interpreter_status)
			return;

		for(i=0;i<interpreter_status;i++)
			rfBuf[i] = interpreter_getAByte();

		rf_transmit(rfBuf,interpreter_status);
		interpreter_status = INTERPRETER_ST_FINDING_55;
	}
	else

		interpreter_status = INTERPRETER_ST_FINDING_55;
}
