/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dcmi.h
  * @brief   This file contains all the function prototypes for
  *          the dcmi.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DCMI_H__
#define __DCMI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stm32h7xx_hal.h"
#include "sccb.h"  
#include "usart.h"
#include "lcd_spi_200.h"
/* USER CODE END Includes */

extern DCMI_HandleTypeDef hdcmi;

/* USER CODE BEGIN Private defines */
 // DCMI״̬��־��������֡�������ʱ���ᱻ HAL_DCMI_FrameEventCallback() �жϻص������� 1 
extern volatile uint8_t OV5640_FrameState;  // �������������������ļ����е���
extern volatile uint8_t OV5640_FPS ;      // ֡��

// ������������ĸ�ʽ���� OV5640_Set_Pixformat() ����
#define 	Pixformat_RGB565   0
#define 	Pixformat_JPEG     1
#define 	Pixformat_GRAY		 2

#define  OV5640_AF_Focusing     	2           // ���ڴ����Զ��Խ���
#define  OV5640_AF_End				1				// �Զ��Խ�����
#define  OV5640_Success   			0           // ͨѶ�ɹ���־
#define  OV5640_Error     			-1          // ͨѶ����

#define  OV5640_Enable    1
#define  OV5640_Disable   0


// OV5640����Чģʽ���� OV5640_Set_Effect() ����
#define  OV5640_Effect_Normal       0  // ����ģʽ
#define  OV5640_Effect_Negative     1  // ��Ƭģʽ��Ҳ������ɫȫ��ȡ��
#define  OV5640_Effect_BW           2  // �ڰ�ģʽ
#define  OV5640_Effect_Solarize  	3  // ����Ƭ����ģʽ

// 1. ����OV5640ʵ�������ͼ���С�����Ը���ʵ�ʵ�Ӧ�û�����ʾ�����е���
// 2. ��������������Ӱ��֡��
// 3. ��Ϊ���õ�OV5640��ISP���ڱ���Ϊ4:3(1280*960)���û����õ�����ߴ�ҲӦ�����������
// 4. �����Ҫ������������Ҫ�޸ĳ�ʼ��������Ĳ���
#define 	OV5640_Width          440   // ͼ�񳤶� 
#define 	OV5640_Height         330   // ͼ����

// 1. ����Ҫ��ʾ�Ļ����С����ֵһ��Ҫ�ܱ�4��������
// 2. RGB565��ʽ�£����ջ���DCMI��OV5640�����4:3ͼ��ü�Ϊ��Ӧ��Ļ�ı���
// 3. JPGģʽ�£���ֵһ��Ҫ�ܱ�8��������
#define 	Display_Width          LCD_Width   	// ͼ�񳤶� 
#define 	Display_Height         LCD_Height   // ͼ����

#define 	Display_BufferSize     Display_Width * Display_Height*2 /4   // DMA�������ݴ�С��32λ��

/*------------------------------------------------------------ ���üĴ��� ------------------------------------------------*/

#define 	OV5640_ChipID_H          	0x300A  	// оƬID�Ĵ��� ���ֽ�
#define 	OV5640_ChipID_L          	0x300B  	// оƬID�Ĵ��� ���ֽ�

#define	OV5640_FORMAT_CONTROL		0x4300	// �������ݽӿ�����ĸ�ʽ	
#define 	OV5640_FORMAT_CONTROL_MUX  0x501F	// ����ISP�ĸ�ʽ

#define	OV5640_JPEG_MODE_SELECT		0x4713	// JPEGģʽѡ����1~6ģʽ���û��������ֲ����˵��
#define	OV5640_JPEG_VFIFO_CTRL00 	0x4600	// ��������JPEGģʽ2�Ƿ�̶�������
#define	OV5640_JPEG_VFIFO_HSIZE_H	0x4602	// JPEG���ˮƽ�ߴ�,���ֽ�
#define	OV5640_JPEG_VFIFO_HSIZE_L	0x4603	// JPEG���ˮƽ�ߴ�,���ֽ�
#define	OV5640_JPEG_VFIFO_VSIZE_H	0x4604	// JPEG�����ֱ�ߴ�,���ֽ�
#define	OV5640_JPEG_VFIFO_VSIZE_L	0x4605	// JPEG�����ֱ�ߴ�,���ֽ�

#define 	OV5640_GroupAccess			0X3212	// �Ĵ��������
#define 	OV5640_TIMING_DVPHO_H		0x3808	// ���ˮƽ�ߴ�,���ֽ�
#define 	OV5640_TIMING_DVPHO_L		0x3809	// ���ˮƽ�ߴ�,���ֽ�
#define 	OV5640_TIMING_DVPVO_H		0x380A	// �����ֱ�ߴ�,���ֽ�
#define 	OV5640_TIMING_DVPVO_L		0x380B   // �����ֱ�ߴ�,���ֽ�
#define 	OV5640_TIMING_Flip			0x3820	// Bit[2:1]���������Ƿ�ֱ��ת
#define 	OV5640_TIMING_Mirror			0x3821	// Bit[2:1]���������Ƿ�ˮƽ����

#define 	OV5640_AF_CMD_MAIN			0x3022	// AF ������
#define 	OV5640_AF_CMD_ACK				0x3023	// AF ����ȷ��
#define 	OV5640_AF_FW_STATUS			0x3029	// �Խ�״̬�Ĵ���
#define OV5640_PWDN_PIN            			 GPIO_PIN_14        				 	// PWDN ����      
#define OV5640_PWDN_PORT           			 GPIOD                 			 	// PWDN GPIO�˿�     
#define GPIO_OV5640_PWDN_CLK_ENABLE    	__HAL_RCC_GPIOD_CLK_ENABLE() 		// PWDN GPIO�˿�ʱ��

// �͵�ƽ������������ģʽ������ͷ��������
#define	OV5640_PWDN_OFF	HAL_GPIO_WritePin(OV5640_PWDN_PORT, OV5640_PWDN_PIN, GPIO_PIN_RESET)	

// �ߵ�ƽ���������ģʽ������ͷֹͣ��������ʱ���Ľ������
#define 	OV5640_PWDN_ON		HAL_GPIO_WritePin(OV5640_PWDN_PORT, OV5640_PWDN_PIN, GPIO_PIN_SET)	
/* USER CODE END Private defines */

void MX_DCMI_Init(void);

/* USER CODE BEGIN Prototypes */
int8_t   DCMI_OV5640_Init(void);	// ��ʼSCCB��DCMI��DMA�Լ�����OV5640

void     OV5640_DMA_Transmit_Continuous(uint32_t DMA_Buffer,uint32_t DMA_BufferSize);	// ����DMA���䣬����ģʽ
void     OV5640_DMA_Transmit_Snapshot(uint32_t DMA_Buffer,uint32_t DMA_BufferSize);		//  ����DMA���䣬����ģʽ������һ֡ͼ���ֹͣ
void     OV5640_DCMI_Suspend(void);		// ����DCMI��ֹͣ��������
void     OV5640_DCMI_Resume(void);		// �ָ�DCMI����ʼ��������
void     OV5640_DCMI_Stop(void);			// ��ֹDCMI��DMA����ֹͣDCMI���񣬽�ֹDCMI����
int8_t 	OV5640_DCMI_Crop(uint16_t Displey_XSize,uint16_t Displey_YSize,uint16_t Sensor_XSize,uint16_t Sensor_YSize ); // �ü�����

void     OV5640_Reset(void);				//	ִ�������λ
uint16_t OV5640_ReadID(void);				// ��ȡ����ID
void		OV5640_Config(void);				// ����OV5640�������
	
void		OV5640_Set_Pixformat(uint8_t pixformat);					// ����ͼ�������ʽ	
void 		OV5640_Set_JPEG_QuantizationScale(uint8_t scale);		// ����JPEG��ʽ��ѹ���ȼ�,ȡֵ 0x01~0x3F
int8_t 	OV5640_Set_Framesize(uint16_t width,uint16_t height);	// ����ʵ�������ͼ���С
int8_t 	OV5640_Set_Horizontal_Mirror( int8_t ConfigState );	// �������������ͼ���Ƿ����ˮƽ����
int8_t 	OV5640_Set_Vertical_Flip( int8_t ConfigState );			//	�������������ͼ���Ƿ���д�ֱ��ת 
void 		OV5640_Set_Brightness(int8_t Brightness);					// ��������
void		OV5640_Set_Contrast(int8_t Contrast);						// ���öԱȶ�
void 		OV5640_Set_Effect(uint8_t effect_Mode);					// ����������Ч����������Ƭ��ģʽ

int8_t 	OV5640_AF_Download_Firmware(void);		//	���Զ��Խ��̼�д��OV5640
int8_t 	OV5640_AF_QueryStatus(void);				//	�Խ�״̬��ѯ
void 		OV5640_AF_Trigger_Constant(void);		// �Զ��Խ� ������ ����
void 		OV5640_AF_Trigger_Single(void);			// �Զ��Խ� ������ ���� 
void 		OV5640_AF_Release(void);					// �ͷ�����ͷ�ص���ʼ���Խ�Ϊ����Զ����λ��
void Display_DCMI_Status(void);
void OV5640_DMA_Init(void);
uint8_t OV5640_Print_ID();
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __DCMI_H__ */

