#include "Mymain.h"
#include "AllHeader.h"

void Mymain(void)
{
	
	while(1)
	{
		HAL_GPIO_TogglePin(LED0_GPIO_Port , LED0_Pin) ;
		HAL_GPIO_TogglePin(LED1_GPIO_Port , LED1_Pin) ;
		HAL_Delay(500) ;
	}
}
