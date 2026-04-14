#include "Mymain.h"
#include "AllHeader.h"

#define Camera_Buffer	0x24000000    // 摄像头图像缓冲区
int cnt = 0 ;

void Mymain(void)
{
	// 液晶屏以及SPI初始化 
	SPI_LCD_Init();
	
	// 摄像头初始化
	DCMI_OV5640_Init();   			 		// DCMI以及OV5640初始化
	OV5640_AF_Download_Firmware();	// 写入自动对焦固件
	OV5640_AF_Trigger_Constant();		// 自动对焦 ，持续 触发，当OV5640检测到当前画面不在焦点时，会一直触发对焦
	OV5640_DMA_Transmit_Continuous(Camera_Buffer, Display_BufferSize);  // 启动DMA连续传输
	
	while(1)
	{
		if ( OV5640_FrameState == 1 )	// 采集到了一帧图像
		{		
			OV5640_FrameState = 0;		// 清零标志位
			LCD_CopyBuffer(0,0,Display_Width,Display_Height, (uint16_t *)Camera_Buffer);	// 将图像数据复制到屏幕
			LCD_DisplayString( 84 ,240,"FPS:");
			LCD_DisplayNumber( 132,240, OV5640_FPS,2) ;	// 显示帧率	
		}	
		LCD_DisplayNumber( 132,240, cnt++,2) ;	// 显示帧率
	}
}
