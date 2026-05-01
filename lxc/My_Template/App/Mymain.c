#include "Mymain.h"
#include "AllHeader.h"

 

#define Camera_Buffer	0x24000000    // ����ͷͼ�񻺳���
int cnt = 0 ;

void Mymain(void)
{
	// Һ�����Լ�SPI��ʼ�� 
	SPI_LCD_Init();
//	HAL_GPIO_TogglePin(LED0_GPIO_Port , LED0_Pin) ;
	SCB_EnableICache();		// ʹ��ICache
	SCB_EnableDCache();		// ʹ��DCache
//	LED_Init();					// ��ʼ��LED����
	MX_USART1_UART_Init();				// USART1��ʼ��	
	
 	SPI_LCD_Init();      	// Һ�����Լ�SPI��ʼ�� 
 	
	DCMI_OV5640_Init();   			 	// DCMI�Լ�OV5640��ʼ��
	
	OV5640_AF_Download_Firmware();	// д���Զ��Խ��̼�
	OV5640_AF_Trigger_Constant();		// �Զ��Խ� ������ ��������OV5640��⵽��ǰ���治�ڽ���ʱ����һֱ�����Խ�?
//	OV5640_AF_Trigger_Single();		//	�Զ��Խ� ������ ���� 
	
//	120�Ⱥ�160�ȵĹ�Ǿ�ͷĬ�ϵķ���ʹ��Զ��Խ��ľ�ͷ��һ�����û����Ը���ʵ��ȥ����
//	OV5640_Set_Vertical_Flip( OV5640_Disable );		// ȡ����ֱ��ת
//	OV5640_Set_Horizontal_Mirror( OV5640_Enable );	// ˮƽ����

	OV5640_DMA_Transmit_Continuous(Camera_Buffer, Display_BufferSize);  // ����DMA��������
	
	while(1)
	{
//		LCD_DisplayString( 84 ,240,"cnt:");
//		LCD_DisplayNumber( 132,240, cnt++,2) ;	// ��ʾ֡��	
		if ( OV5640_FrameState == 1 )	// �ɼ�����һ֡ͼ��
		{		
  	  OV5640_FrameState = 0;		// �����־�?
      LCD_CopyBuffer(0,0,Display_Width,Display_Height, (uint16_t *)Camera_Buffer);	// ��ͼ�����ݸ��Ƶ���Ļ
			LCD_DisplayString( 84 ,240,"FPS:");
			LCD_DisplayNumber( 132,240, OV5640_FPS,2) ;	// ��ʾ֡��	
//			LED1_Toggle;	
		}
	}
}
