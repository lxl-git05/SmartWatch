#include "Mymain.h"
#include "AllHeader.h"

#define Camera_Buffer	0x24000000    // 摄像头图像缓冲区
int cnt = 0 ;

void Mymain(void)
{
	// 液晶屏以及SPI初始化 
	SPI_LCD_Init();
	HAL_GPIO_TogglePin(LED0_GPIO_Port , LED0_Pin) ;
	
	while(1)
	{
		LCD_DisplayString( 84 ,240,"cnt:");
		LCD_DisplayNumber( 132,240, cnt++,2) ;	// 显示帧率	
	}
}
