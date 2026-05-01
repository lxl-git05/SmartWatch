/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dcmi.c
  * @brief   This file provides code for the configuration
  *          of the DCMI instances.
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
/* Includes ------------------------------------------------------------------*/
#include "dcmi.h"

/* USER CODE BEGIN 0 */
#include "dcmi_ov5640_cfg.h"  
#include <stdio.h>
#include "usart.h"




volatile uint8_t OV5640_FrameState = 0;  // DCMI״̬��־��������֡�������ʱ���ᱻ HAL_DCMI_FrameEventCallback() �жϻص������� 1     
volatile uint8_t OV5640_FPS ;          // ֡��
/* USER CODE END 0 */

DCMI_HandleTypeDef hdcmi;
DMA_HandleTypeDef hdma_dcmi;

/* DCMI init function */
void MX_DCMI_Init(void)
{

  /* USER CODE BEGIN DCMI_Init 0 */

  /* USER CODE END DCMI_Init 0 */

  /* USER CODE BEGIN DCMI_Init 1 */

  /* USER CODE END DCMI_Init 1 */
  hdcmi.Instance = DCMI;
  hdcmi.Init.SynchroMode = DCMI_SYNCHRO_HARDWARE;
  hdcmi.Init.PCKPolarity = DCMI_PCKPOLARITY_RISING;
  hdcmi.Init.VSPolarity = DCMI_VSPOLARITY_LOW;
  hdcmi.Init.HSPolarity = DCMI_HSPOLARITY_LOW;
  hdcmi.Init.CaptureRate = DCMI_CR_ALL_FRAME;
  hdcmi.Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  hdcmi.Init.JPEGMode = DCMI_JPEG_DISABLE;
  hdcmi.Init.ByteSelectMode = DCMI_BSM_ALL;
  hdcmi.Init.ByteSelectStart = DCMI_OEBS_ODD;
  hdcmi.Init.LineSelectMode = DCMI_LSM_ALL;
  hdcmi.Init.LineSelectStart = DCMI_OELS_ODD;
  if (HAL_DCMI_Init(&hdcmi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DCMI_Init 2 */

  /* USER CODE END DCMI_Init 2 */

}

void HAL_DCMI_MspInit(DCMI_HandleTypeDef* dcmiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(dcmiHandle->Instance==DCMI)
  {
  /* USER CODE BEGIN DCMI_MspInit 0 */

  /* USER CODE END DCMI_MspInit 0 */
    /* DCMI clock enable */
    __HAL_RCC_DCMI_CLK_ENABLE();

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**DCMI GPIO Configuration
    PE4     ------> DCMI_D4
    PE5     ------> DCMI_D6
    PE6     ------> DCMI_D7
    PA4     ------> DCMI_HSYNC
    PA6     ------> DCMI_PIXCLK
    PC6     ------> DCMI_D0
    PC7     ------> DCMI_D1
    PD3     ------> DCMI_D5
    PB7     ------> DCMI_VSYNC
    PE0     ------> DCMI_D2
    PE1     ------> DCMI_D3
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_0
                          |GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* DCMI DMA Init */
    /* DCMI Init */
    hdma_dcmi.Instance = DMA2_Stream7;
    hdma_dcmi.Init.Request = DMA_REQUEST_DCMI;
    hdma_dcmi.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_dcmi.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dcmi.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dcmi.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_dcmi.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_dcmi.Init.Mode = DMA_CIRCULAR;
    hdma_dcmi.Init.Priority = DMA_PRIORITY_LOW;
    hdma_dcmi.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_dcmi.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_dcmi.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_dcmi.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_dcmi) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(dcmiHandle,DMA_Handle,hdma_dcmi);

    /* DCMI interrupt Init */
    HAL_NVIC_SetPriority(DCMI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DCMI_IRQn);
  /* USER CODE BEGIN DCMI_MspInit 1 */

  /* USER CODE END DCMI_MspInit 1 */
  }
}

void HAL_DCMI_MspDeInit(DCMI_HandleTypeDef* dcmiHandle)
{

  if(dcmiHandle->Instance==DCMI)
  {
  /* USER CODE BEGIN DCMI_MspDeInit 0 */

  /* USER CODE END DCMI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DCMI_CLK_DISABLE();

    /**DCMI GPIO Configuration
    PE4     ------> DCMI_D4
    PE5     ------> DCMI_D6
    PE6     ------> DCMI_D7
    PA4     ------> DCMI_HSYNC
    PA6     ------> DCMI_PIXCLK
    PC6     ------> DCMI_D0
    PC7     ------> DCMI_D1
    PD3     ------> DCMI_D5
    PB7     ------> DCMI_VSYNC
    PE0     ------> DCMI_D2
    PE1     ------> DCMI_D3
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_0
                          |GPIO_PIN_1);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

    /* DCMI DMA DeInit */
    HAL_DMA_DeInit(dcmiHandle->DMA_Handle);

    /* DCMI interrupt Deinit */
    HAL_NVIC_DisableIRQ(DCMI_IRQn);
  /* USER CODE BEGIN DCMI_MspDeInit 1 */

  /* USER CODE END DCMI_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/***************************************************************************************************************************************
*	函 数 名: OV5640_Delay
*	入口参数: Delay - 延时时间，单位 ms 
*	函数功能: 简单延时函数，不是很精确
*	说    明: 为了移植的简便性,此处采用软件延时，实际项目中可以替换成RTOS的延时或者HAL库的延时
*****************************************************************************************************************************************/
void OV5640_Delay(uint32_t Delay)
{
	volatile uint16_t i;

	while (Delay --)				
	{
		for (i = 0; i < 40000; i++);
	}	
//	HAL_Delay(Delay);	  // 可使用HAL库的延时
}

/***************************************************************************************************************************************
*	函 数 名: DCMI_OV5640_Init
*
*	函数功能: 初始SCCB、DCMI、DMA以及配置OV5640
*
*****************************************************************************************************************************************/
int8_t DCMI_OV5640_Init(void)
{
	uint16_t	Device_ID;		// 定义变量存储器件ID
	
   SCCB_GPIO_Config();		               // SCCB引脚初始化
	MX_DCMI_Init();                        // 初始化DCMI配置引脚
   OV5640_DMA_Init();                     // 初始化DMA配置
	OV5640_Reset();	                     // 执行软件复位
	Device_ID =  OV5640_ReadID();		      // 读取器件ID

	if( Device_ID == 0x5640 )		// 进行匹配
	{
		printf ("OV5640 OK,ID:0x%X\r\n",Device_ID);		      // 匹配通过

		OV5640_Config();													// 配置各项参数
		printf("Config done\r\n");
printf("0x3035:0x%X\r\n", SCCB_ReadReg_16Bit(0x3035));
printf("0x3036:0x%X\r\n", SCCB_ReadReg_16Bit(0x3036));
printf("0x3037:0x%X\r\n", SCCB_ReadReg_16Bit(0x3037));
printf("0x3103:0x%X\r\n", SCCB_ReadReg_16Bit(0x3103));
printf("0x3008:0x%X\r\n", SCCB_ReadReg_16Bit(0x3008));
		OV5640_Set_Framesize(OV5640_Width,OV5640_Height);		//	设置OV5640输出的图像大小
		OV5640_DCMI_Crop( Display_Width, Display_Height, OV5640_Width, OV5640_Height );	// 将输出图像裁剪成适应屏幕的大小，JPG模式不需要裁剪
				
		return OV5640_Success;	 // 返回成功标志		
	}
	else
	{
		printf ("OV5640 ERROR!!!!!  ID:%X\r\n",Device_ID);	   // 读取ID错误
		return  OV5640_Error;	 // 返回错误标志
	}	

    
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_DMA_Transmit_Continuous
*
*	入口参数:  DMA_Buffer - DMA将要传输的地址，即用于存储摄像头数据的存储区地址
*            DMA_BufferSize - 传输的数据大小，32位宽
*
*	函数功能: 启动DMA传输，连续模式
*
*	说    明: 1. 开启连续模式之后，会一直进行传输，除非挂起或者停止DCMI
*            2. OV5640使用RGB565模式时，1个像素点需要2个字节来存储
*				 3. 因为DMA配置传输数据为32位宽，计算 DMA_BufferSize 时，需要除以4，例如：
*               要获取 240*240分辨率 的图像，需要传输 240*240*2 = 115200 字节的数据，
*               则 DMA_BufferSize = 115200 / 4 = 28800 。
*fanke
*****************************************************************************************************************************************/
void OV5640_DMA_Transmit_Continuous(uint32_t DMA_Buffer,uint32_t DMA_BufferSize)
{
   hdma_dcmi.Init.Mode  = DMA_CIRCULAR;  // 循环模式					
		
	 HAL_DMA_Init(&hdma_dcmi);

  // 使能DCMI采集数据,连续采集模式
   HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)DMA_Buffer,DMA_BufferSize);
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_DMA_Transmit_Snapshot
*
*	入口参数:  DMA_Buffer - DMA将要传输的地址，即用于存储摄像头数据的存储区地址
*            DMA_BufferSize - 传输的数据大小，32位宽
*
*	函数功能: 启动DMA传输，快照模式，传输一帧图像后停止
*
*	说    明: 1. 快照模式，只传输一帧的数据
*            2. OV5640使用RGB565模式时，1个像素点需要2个字节来存储
*				 3. 因为DMA配置传输数据为32位宽，计算 DMA_BufferSize 时，需要除以4，例如：
*               要获取 240*240分辨率 的图像，需要传输 240*240*2 = 115200 字节的数据，
*               则 DMA_BufferSize = 115200 / 4 = 28800 。
*            4. 使用该模式传输完成之后，DCMI会被挂起，再次启用传输之前，需要调用 OV5640_DCMI_Resume() 恢复DCMI
*
*****************************************************************************************************************************************/
void OV5640_DMA_Transmit_Snapshot(uint32_t DMA_Buffer,uint32_t DMA_BufferSize)
{
   hdma_dcmi.Init.Mode  = DMA_NORMAL;  // 正常模式					

   HAL_DMA_Init(&hdma_dcmi);    // 配置DMA

   HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)DMA_Buffer,DMA_BufferSize);
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_DCMI_Suspend
*
*	函数功能: 挂起DCMI，停止捕获数据
*
*	说    明: 1. 开启连续模式之后，再调用该函数，会停止捕获DCMI的数据
*            2. 可以调用 OV5640_DCMI_Resume() 恢复DCMI
*				 3. 需要注意的，挂起DCMI期间，DMA是没有停止工作的
*111KE
*****************************************************************************************************************************************/
void OV5640_DCMI_Suspend(void) 
{
   HAL_DCMI_Suspend(&hdcmi);    // 挂起DCMI
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_DCMI_Resume
*
*	函数功能: 恢复DCMI，开始捕获数据
*
*	说    明: 1. 当DCMI被挂起时，可以调用该函数恢复
*            2. 使用 OV5640_DMA_Transmit_Snapshot() 快照模式，传输完成之后，DCMI也会被挂起，再次启用传输之前，
*				    需要调用本函数恢复DCMI捕获
*
*****************************************************************************************************************************************/
void  OV5640_DCMI_Resume(void) 
{
   (&hdcmi)->State = HAL_DCMI_STATE_BUSY;       // 变更DCMI标志
   (&hdcmi)->Instance->CR |= DCMI_CR_CAPTURE;   // 开启DCMI捕获
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_DCMI_Stop
*
*	函数功能: 禁止DCMI的DMA请求，停止DCMI捕获，禁止DCMI外设
*
*****************************************************************************************************************************************/
void  OV5640_DCMI_Stop(void) 
{
   HAL_DCMI_Stop(&hdcmi);
}


/***************************************************************************************************************************************
*	函 数 名: OV5640_DCMI_Crop
*
*	入口参数:  Displey_XSize 、Displey_YSize - 显示器的长宽
*				  Sensor_XSize、Sensor_YSize - 摄像头传感器输出图像的长宽
*
*	函数功能: 使用DCMI的裁剪功能，将传感器输出的图像裁剪成适应屏幕的大小
*
*	说    明: 1. 因为摄像头输出的画面比例不一定匹配显示器，所以需要裁剪
*				 2. 摄像头的输出画面比例由 OV5640_Config()配置参数决定，最终画面大小由 OV5640_Set_Framesize()决定
*            3. DCMI的水平有效像素也必须要能被4整除！
*				 4. 函数会计算水平和垂直偏移，尽量让画面居中裁剪
*****************************************************************************************************************************************/
int8_t OV5640_DCMI_Crop(uint16_t Displey_XSize,uint16_t Displey_YSize,uint16_t Sensor_XSize,uint16_t Sensor_YSize )
{
	uint16_t DCMI_X_Offset,DCMI_Y_Offset;	// 水平和垂直偏移，垂直代表的是行数，水平代表的是像素时钟数（PCLK周期数）
	uint16_t DCMI_CAPCNT;		// 水平有效像素，代表的是像素时钟数（PCLK周期数）
	uint16_t DCMI_VLINE;			// 垂直有效行数

	if( (Displey_XSize>=Sensor_XSize)|| (Displey_YSize>=Sensor_YSize) )
	{
//		printf("实际显示的尺寸大于或等于摄像头输出的尺寸，退出DCMI裁剪\r\n");
		return OV5640_Error;  //如果实际显示的尺寸大于或等于摄像头输出的尺寸，则退出当前函数，不进行裁剪
	}
// 在设置为RGB565格式时，水平偏移，必须是奇数，否则画面色彩不正确，
// 因为一个有效像素是2个字节，需要2个PCLK周期，所以必须从奇数位开始，不然数据会错乱，
// 需要注意的是，寄存器值是从0开始算起的	！
	DCMI_X_Offset = Sensor_XSize - Displey_XSize; // 实际计算过程为（Sensor_XSize - LCD_XSize）/2*2

// 计算垂直偏移，尽量让画面居中裁剪，该值代表的是行数，	
	DCMI_Y_Offset = (Sensor_YSize - Displey_YSize)/2-1; // 寄存器值是从0开始算起的，所以要-1

// 因为一个有效像素是2个字节，需要2个PCLK周期，所以要乘2
// 最终得到的寄存器值，必须要能被4整除！
	DCMI_CAPCNT = Displey_XSize*2-1;	// 寄存器值是从0开始算起的，所以要-1
	
	DCMI_VLINE = Displey_YSize-1;		// 垂直有效行数
	
//	printf("%d  %d  %d  %d\r\n",DCMI_X_Offset,DCMI_Y_Offset,DCMI_CAPCNT,DCMI_VLINE);
	
	HAL_DCMI_ConfigCrop (&hdcmi,DCMI_X_Offset,DCMI_Y_Offset,DCMI_CAPCNT,DCMI_VLINE);// 设置裁剪窗口
	HAL_DCMI_EnableCrop(&hdcmi);		// 使能裁剪
	
	return OV5640_Success;
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_Reset
*
*	函数功能: 执行软件复位
*
*	说    明: 期间有多个延时操作          
*
*****************************************************************************************************************************************/
void OV5640_Reset(void)
{
	OV5640_Delay(30);  // 等待模块上电稳定，最少5ms，然后拉低PWDN  	
	
	OV5640_PWDN_OFF;  // PWDN 引脚输出低电平，不开启掉电模式，摄像头正常工作，此时摄像头模块的白色LED会点亮
  
// 根据OV5640的上电时序，PWDN拉低之后，要等待1ms再去拉高RESET，111的OV5640模块采用硬件RC复位，持续时间大概在6~10ms，
// 因此加入延时，等待硬件复位完成并稳定下来
	OV5640_Delay(5);    
	
// 复位完成之后，要>=20ms方可执行SCCB配置
	OV5640_Delay(20);    
	
	SCCB_WriteReg_16Bit(0x3103, 0x11);	// 根据手册的建议，复位之前，直接将时钟输入引脚的时钟作为主时钟
	SCCB_WriteReg_16Bit(0x3008, 0x82);	// 执行一次软复位
	OV5640_Delay(5);  //延时5ms
	
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_ReadID
*
*	函数功能: 读取 OV5640 的器件ID
*
*****************************************************************************************************************************************/
uint16_t OV5640_ReadID(void)
{
   uint8_t PID_H,PID_L;     // ID变量
	
   PID_H = SCCB_ReadReg_16Bit(OV5640_ChipID_H); // 读取ID高字节
   PID_L = SCCB_ReadReg_16Bit(OV5640_ChipID_L); // 读取ID低字节
	
	return(PID_H<<8)|PID_L; // 返回完整的器件ID
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_Config
*
*	函数功能: 配置 OV5640 各个寄存器参数
*
*	说    明: 参数定义在 dcmi_ov5640_cfg.h
*            
*****************************************************************************************************************************************/

void OV5640_Config(void)
{
	uint32_t i;	// 计数变量
	
	uint8_t	read_reg; // 读取配置，用于调试

	for(i=0; i<(sizeof(OV5640_INIT_Config)/4); i++)
	{
		SCCB_WriteReg_16Bit(OV5640_INIT_Config[i][0], OV5640_INIT_Config[i][1]); // 写入配置
		
		read_reg = SCCB_ReadReg_16Bit(OV5640_INIT_Config[i][0]);	// 读取配置，用于调试

		if(OV5640_INIT_Config[i][1] != read_reg )	// 配置不成功
		{
			printf("error position %d\r\n",i);	// 打印出错位置
			printf("0x%x-0x%x-0x%x\r\n",OV5640_INIT_Config[i][0],OV5640_INIT_Config[i][1],read_reg);
		}
	}
	
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_Set_Pixformat
*
*	入口参数:  pixformat - 像素格式，可选择 Pixformat_RGB565、Pixformat_GRAY、Pixformat_JPEG
*
*	函数功能: 设置输出的像素格式
*
*****************************************************************************************************************************************/

void OV5640_Set_Pixformat(uint8_t pixformat)
{
   uint8_t OV5640_Reg;  // 寄存器的值

	if( pixformat == Pixformat_JPEG )
	{
		SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL, 		0x30);	//	设置数据接口输出的格式	
		SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL_MUX, 	0x00);	// 设置ISP的格式	
 
		SCCB_WriteReg_16Bit(OV5640_JPEG_MODE_SELECT, 0x02);	 	// JPEG 模式2

		SCCB_WriteReg_16Bit(OV5640_JPEG_VFIFO_CTRL00, 0xA0); 		// JPEG 固定行数
		
		SCCB_WriteReg_16Bit(OV5640_JPEG_VFIFO_HSIZE_H, OV5640_Width>>8);			// JPEG输出水平尺寸,高字节
		SCCB_WriteReg_16Bit(OV5640_JPEG_VFIFO_HSIZE_L, (uint8_t)OV5640_Width);	// JPEG输出水平尺寸,低字节
		SCCB_WriteReg_16Bit(OV5640_JPEG_VFIFO_VSIZE_H, OV5640_Height>>8);			// JPEG输出垂直尺寸,低字节
		SCCB_WriteReg_16Bit(OV5640_JPEG_VFIFO_VSIZE_L, (uint8_t)OV5640_Height);	// JPEG输出垂直尺寸,低字节	
		
	}
	else if( pixformat == Pixformat_GRAY )
	{
		SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL, 		0x10);	//	设置数据接口输出的格式
		SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL_MUX, 	0x00);	// 设置ISP的格式		
	}
	else	// RGB565
	{
		SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL, 		0x6F);	// 此处设置为RGB565格式，序列为 G[2:0]B[4:0], R[4:0]G[5:3]	
		SCCB_WriteReg_16Bit(OV5640_FORMAT_CONTROL_MUX, 	0x01);	// 设置ISP的格式	
	}
	
   OV5640_Reg = SCCB_ReadReg_16Bit(0x3821);   // 读取寄存器值，Bit[5]用于是否使能JPEG模式
	SCCB_WriteReg_16Bit(0x3821,(OV5640_Reg & 0xDF) | ((pixformat == Pixformat_JPEG) ? 0x20 : 0x00));
	 
   OV5640_Reg = SCCB_ReadReg_16Bit(0x3002);   // 读取寄存器值，Bit[7]、Bit[4]和Bit[2]使能 VFIFO、JFIFO、JPG
	SCCB_WriteReg_16Bit(0x3002,(OV5640_Reg & 0xE3) | ((pixformat == Pixformat_JPEG) ? 0x00 : 0x1C));
	 
   OV5640_Reg = SCCB_ReadReg_16Bit(0x3006);   // 读取寄存器值，Bit[5]和Bit[3] 用于是否使能JPG时钟
	SCCB_WriteReg_16Bit(0x3006,(OV5640_Reg & 0xD7) | ((pixformat == Pixformat_JPEG) ? 0x28 : 0x00));

}

/***************************************************************************************************************************************
*	函 数 名: OV5640_Set_JPEG_QuantizationScale
*
*	入口参数: scale - 压缩等级，取值 0x01~0x3F
*
*	函数功能: 数值越大，压缩就越厉害，得到的图片占用空间就越小，但相应的画质会变差，客户可自行调节
*
*****************************************************************************************************************************************/

void OV5640_Set_JPEG_QuantizationScale(uint8_t scale)
{
	SCCB_WriteReg_16Bit(0x4407, scale); 	// JPEG 压缩等级
}


/***************************************************************************************************************************************
*	函 数 名: OV5640_Set_Framesize
*
*	入口参数:  width - 实际输出图像的长度，height - 实际输出图像的宽度
*
*	函数功能: 设置实际输出的图像大小（缩放后）
*
*	说    明: 1. 需要注意的是，要设置的图像长、宽需要满足初始化配置时ISP窗口的比例，不然图像会变形
*            2. 并不是设置输出的图像分辨率越小帧率就越高，帧率只和初始化的配置（PLL、HTS和VTS）有关
*
*****************************************************************************************************************************************/

int8_t OV5640_Set_Framesize(uint16_t width,uint16_t height)
{
// OV5640的很多操作，都要加上这种对应 group 的配置	
    SCCB_WriteReg_16Bit(OV5640_GroupAccess,0X03);  	// 开始 group 3 的配置
	
    SCCB_WriteReg_16Bit(OV5640_TIMING_DVPHO_H,width>>8);			// DVPHO，设置输出水平尺寸
    SCCB_WriteReg_16Bit(OV5640_TIMING_DVPHO_L,width&0xff);
    SCCB_WriteReg_16Bit(OV5640_TIMING_DVPVO_H,height>>8);		// DVPVO，设置输出垂直尺寸
    SCCB_WriteReg_16Bit(OV5640_TIMING_DVPVO_L,height&0xff);

    SCCB_WriteReg_16Bit(OV5640_GroupAccess,0X13);		// 结束配置
    SCCB_WriteReg_16Bit(OV5640_GroupAccess,0Xa3);		// 启用设置
	
	return OV5640_Success; 
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_Set_Horizontal_Mirror
*
*	入口参数:  ConfigState - 置1时，图像会水平镜像，置0时恢复正常
*
*	函数功能: 用于设置输出的图像是否进行水平镜像
*
*****************************************************************************************************************************************/
int8_t OV5640_Set_Horizontal_Mirror( int8_t ConfigState )
{
   uint8_t OV5640_Reg;  // 寄存器的值

   OV5640_Reg = SCCB_ReadReg_16Bit(OV5640_TIMING_Mirror);   // 读取寄存器值

// Bit[2:1]用于设置是否水平镜像
   if ( ConfigState == OV5640_Enable )    // 如果使能镜像
   { 
      OV5640_Reg |= 0X06;  
   } 
   else                    // 取消镜像
   {
      OV5640_Reg &= 0xF9; 	
   }
   return  SCCB_WriteReg_16Bit(OV5640_TIMING_Mirror,OV5640_Reg);   // 写入寄存器
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_Set_Vertical_Flip
*
*	入口参数:  ConfigState - 置1时，图像会垂直翻转，置0时恢复正常
*
*	函数功能: 用于设置输出的图像是否进行垂直翻转
*
*****************************************************************************************************************************************/
int8_t OV5640_Set_Vertical_Flip( int8_t ConfigState )
{
   uint8_t OV5640_Reg;  // 寄存器的值

   OV5640_Reg = SCCB_ReadReg_16Bit(OV5640_TIMING_Flip);          // 读取寄存器值

// Bit[2:1]用于设置是否垂直翻转
   if ( ConfigState == OV5640_Enable )   
   { 
		OV5640_Reg |= 0X06;       
   } 
   else   // 取消翻转
   {
      OV5640_Reg &= 0xF9; 	
   }
   return  SCCB_WriteReg_16Bit(OV5640_TIMING_Flip,OV5640_Reg);   // 写入寄存器
}


/***************************************************************************************************************************************
*	函 数 名: OV5640_Set_Brightness
*
*	入口参数:  Brightness - 亮度，可设置为9个等级：4，3，2，1，0，-1，-2，-3，-4   ，数字越大亮度越高                
*
*	说    明: 1. 直接使用OV5640手册给出的代码
*            2. 亮度越高，画面就越明亮，但是会变模糊一些
*				 2. 亮度太低，噪点会增多
*
*****************************************************************************************************************************************/
void OV5640_Set_Brightness(int8_t Brightness)
{
	Brightness = Brightness+4;
	SCCB_WriteReg_16Bit(OV5640_GroupAccess,0X03);  	// 开始 group 3 的配置

	SCCB_WriteReg_16Bit( 0x5587, OV5640_Brightness_Config[Brightness][0]);	
	SCCB_WriteReg_16Bit( 0x5588, OV5640_Brightness_Config[Brightness][1]);
	
	SCCB_WriteReg_16Bit(OV5640_GroupAccess,0X13);		// 结束配置
	SCCB_WriteReg_16Bit(OV5640_GroupAccess,0Xa3);		// 启用设置	
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_Set_Contrast
*
*	入口参数: Contrast - 对比度，可设置为7个等级：3，2，1，0，-1，-2 ，-3                  
*
*	说    明: 1. 直接使用OV5640手册给出的代码
*            2. 对比度越高，画面越清晰，黑白越加分明
*
*****************************************************************************************************************************************/
void OV5640_Set_Contrast(int8_t Contrast)
{
	Contrast = Contrast+3;
	SCCB_WriteReg_16Bit(OV5640_GroupAccess,0X03);  	// 开始 group 3 的配置

	SCCB_WriteReg_16Bit( 0x5586, OV5640_Contrast_Config[Contrast][0]);	
	SCCB_WriteReg_16Bit( 0x5585, OV5640_Contrast_Config[Contrast][1]);
	
	SCCB_WriteReg_16Bit(OV5640_GroupAccess,0X13);		// 结束配置
	SCCB_WriteReg_16Bit(OV5640_GroupAccess,0Xa3);		// 启用设置	
}
/***************************************************************************************************************************************
*	函 数 名: OV5640_Set_Effect
*
*	入口参数:  effect_Mode - 特效模式，可选择参数 OV5640_Effect_Normal、OV5640_Effect_Negative、
*                          OV5640_Effect_BW、OV5640_Effect_Solarize
*
*	函数功能: 用于设置OV5640的特效，正常、负片、黑白、正负片叠加模式
*
*	说    明: 这里仅列举了4个模式，更多特效模式可以参考手册进行配置
*
*****************************************************************************************************************************************/
void OV5640_Set_Effect(uint8_t effect_Mode)
{
	SCCB_WriteReg_16Bit(OV5640_GroupAccess,0X03);  	// 开始 group 3 的配置

	SCCB_WriteReg_16Bit( 0x5580, OV5640_Effect_Config[effect_Mode][0]);	
	SCCB_WriteReg_16Bit( 0x5583, OV5640_Effect_Config[effect_Mode][1]);
	SCCB_WriteReg_16Bit( 0x5584, OV5640_Effect_Config[effect_Mode][2]);	
	SCCB_WriteReg_16Bit( 0x5003, OV5640_Effect_Config[effect_Mode][3]);	
	
	SCCB_WriteReg_16Bit(OV5640_GroupAccess,0X13);		// 结束配置
	SCCB_WriteReg_16Bit(OV5640_GroupAccess,0Xa3);		// 启用设置		
	
}
/***************************************************************************************************************************************
*	函 数 名: OV5640_Download_AF_Firmware
*
*	函数功能: 将自动对焦固件写入OV5640
*
*	说    明: 因为OV5640片内没有flash，不能保存固件，因此每次上电都要写入一次
*
*****************************************************************************************************************************************/

int8_t OV5640_AF_Download_Firmware(void)
{ 
	uint8_t  AF_Status = 0;		// 对焦状态
	uint16_t i = 0; 				// 计数变量
	uint16_t OV5640_MCU_Addr = 0x8000;	// OV5640 MCU 存储器的起始地址为 0x8000，大小为4KB
	
	SCCB_WriteReg_16Bit(0x3000, 0x20);	// Bit[5]，复位MCU，写入固件之前，需要执行此操作
// 开始写入固件，批量写入，提高写入速度
	SCCB_WriteBuffer_16Bit( OV5640_MCU_Addr,(uint8_t *)OV5640_AF_Firmware,sizeof(OV5640_AF_Firmware) );
	SCCB_WriteReg_16Bit(0x3000,0x00);  // Bit[5]，写入完毕，写0使能MCU
	
// 写入固件之后，会有个初始化的过程，因此尝试读取100次状态，根据状态进行判断
	for(i=0;i<100;i++)	
	{
		AF_Status = SCCB_ReadReg_16Bit(OV5640_AF_FW_STATUS);	// 读取状态寄存器
		if( AF_Status == 0x7E)
		{
			printf("AF鍒濆鍖栦腑>>>\r\n");	
		}			
		if( AF_Status == 0x70)	// 释放马达，镜头回到初始（对焦为无穷远处）位置，意味着固件写入成功
		{
			printf("AF鍒濆鍖栨垚鍔焅r\n");
			return OV5640_Success;  
		}			
	}
// 尝试100次读取之后，还是没有读到0x70状态，说明固件没写入成功	
	printf("AF鍒濆鍖栧啓鍏ュけ璐ワ紒锛侊紒error锛侊紒锛乗r\n");	
	return OV5640_Error;	
}  

/***************************************************************************************************************************************
*	函 数 名: OV5640_AF_QueryStatus
*
*	返 回 值：OV5640_AF_End - 对焦结束， OV5640_AF_Focusing - 正在对焦
*
*	函数功能: 对焦状态查询
*
*	说    明: 1. 对焦过程大概会持续500多ms
*				 2. 对焦没完成时，采集到的的图像不在焦点，会非常模糊
*
*****************************************************************************************************************************************/

int8_t OV5640_AF_QueryStatus(void)
{
	uint8_t  AF_Status = 0;		// 对焦状态	
	
	AF_Status = SCCB_ReadReg_16Bit(OV5640_AF_FW_STATUS);	// 读取状态寄存器
	printf("AF_Status:0x%x\r\n",AF_Status);

// 单次对焦模式	下，返回 0x10，持续对焦模式下，返回0x20
	if( (AF_Status == 0x10)||(AF_Status == 0x20) )		
	{
		return OV5640_AF_End;	// 返回 对焦结束 标志
	}
	else
	{
		return OV5640_AF_Focusing;	// 返回 正在对焦 标志
	}
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_AF_Trigger_Constant
*
*	函数功能: 持续触发对焦，当OV5640检测到当前画面不在焦点时，会一直触发对焦，无需用户干预
*
*	说    明: 1.可以调用 OV5640_AF_QueryStatus() 函数查询对焦状态
*				 2.可以调用 OV5640_AF_Release() 退出持续对焦模式
*				 3.对焦过程大概会持续500多ms
*				 4.有时环境光线太暗，OV5640会反复的进行对焦，用户可根据实际情况切换到单次对焦模式
*				
*****************************************************************************************************************************************/

void OV5640_AF_Trigger_Constant(void)
{
	SCCB_WriteReg_16Bit(0x3022,0x04);	//	持续对焦
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_AF_Trigger_Single
*
*	函数功能: 触发一次自动对焦
*
*	说    明: 对焦过程大概会持续500多ms，用户可以调用 OV5640_AF_QueryStatus() 函数查询对焦状态
*
*****************************************************************************************************************************************/

void OV5640_AF_Trigger_Single(void)
{
	SCCB_WriteReg_16Bit(OV5640_AF_CMD_MAIN,0x03);	// 触发一次自动对焦 
}

/***************************************************************************************************************************************
*	函 数 名: OV5640_AF_Release
*
*	函数功能: 释放马达，镜头回到初始（对焦为无穷远处）位置
*
*****************************************************************************************************************************************/

void OV5640_AF_Release(void)
{
	SCCB_WriteReg_16Bit(OV5640_AF_CMD_MAIN,0x08);	// 对焦释放指令		
}

/***************************************************************************************************************************************
*	函 数 名: HAL_DCMI_FrameEventCallback
*
*	函数功能: 帧回调函数，每传输一帧数据，会进入该中断服务函数
*
*	说    明: 每次传输完一帧，对相应的标志位进行操作，并计算帧率
*****************************************************************************************************************************************/

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	static uint32_t DCMI_Tick = 0;         	// 用于保存当前的时间计数值
   static uint8_t  DCMI_Frame_Count = 0;   	// 帧数计数   

 	if(HAL_GetTick() - DCMI_Tick >= 1000)    // 每隔 1s 计算一次帧率
	{
		DCMI_Tick = HAL_GetTick();        // 重新获取当前时间计数值
		
		OV5640_FPS = DCMI_Frame_Count;   // 获得fps 

		DCMI_Frame_Count = 0;            // 计数清0
	}
	DCMI_Frame_Count ++;    // 每进入一次中断（每次传输完一帧数据），计数值+1

   OV5640_FrameState = 1;  // 传输完成标志位置1
}

/***************************************************************************************************************************************
*	函 数 名: HAL_DCMI_ErrorCallback
*
*	函数功能: 错误回调函数
*
*	说    明: 当发生DMA传输错误或者FIFO溢出错误就会进入
*****************************************************************************************************************************************/

void  HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi)
{
   // if( HAL_DCMI_GetError(hdcmi) == HAL_DCMI_ERROR_OVR)
   // {
   //    printf("FIFO溢出错误！！！\r\n");
   // }
//   printf("error:0x%x!!!!\r\n",HAL_DCMI_GetError(hdcmi));
}

/*********************************************************************************************************************************************************************************************************************************************fanke*************/
// 111

void Display_DCMI_Status(void)
{
    char buf[30];
    uint32_t sr = DCMI->SR;  // 状态寄存器
    uint32_t cr = DCMI->CR;  // 控制寄存器
    
    sprintf(buf, "SR:0x%X", sr);
    LCD_DisplayString(10, 190, buf);
    sprintf(buf, "VSYNC:%u", (sr >> 1) & 0x1);   // 改为 %u
    LCD_DisplayString(10, 210, buf);
    sprintf(buf, "HSYNC:%u", sr & 0x1);          // 改为 %u
    LCD_DisplayString(10, 230, buf);
    sprintf(buf, "CAPTURE:%u", (cr >> 1) & 0x1); // 改为 %u
    LCD_DisplayString(10, 250, buf);
}

void OV5640_DMA_Init(void)
{
   __HAL_RCC_DMA2_CLK_ENABLE();   // 使能DMA2时钟

   hdma_dcmi.Instance                     = DMA2_Stream7;               // DMA2数据流7      
   hdma_dcmi.Init.Request                 = DMA_REQUEST_DCMI;           // DMA请求来自DCMI
   hdma_dcmi.Init.Direction               = DMA_PERIPH_TO_MEMORY;       // 外设到存储器模式
   hdma_dcmi.Init.PeriphInc               = DMA_PINC_DISABLE;           // 外设地址禁止自增
   hdma_dcmi.Init.MemInc                  = DMA_MINC_ENABLE;			   // 存储器地址自增
   hdma_dcmi.Init.PeriphDataAlignment     = DMA_PDATAALIGN_WORD;        // DCMI数据位宽，32位  
   hdma_dcmi.Init.MemDataAlignment        = DMA_MDATAALIGN_WORD;        // 存储器数据位宽，32位
   hdma_dcmi.Init.Mode                    = DMA_CIRCULAR;               // 循环模式					
   hdma_dcmi.Init.Priority                = DMA_PRIORITY_LOW;           // 优先级低
   hdma_dcmi.Init.FIFOMode                = DMA_FIFOMODE_ENABLE;        // 使能fifo
   hdma_dcmi.Init.FIFOThreshold           = DMA_FIFO_THRESHOLD_FULL;    // 全fifo模式，4*32bit大小
   hdma_dcmi.Init.MemBurst                = DMA_MBURST_SINGLE;          // 单次传输
   hdma_dcmi.Init.PeriphBurst             = DMA_PBURST_SINGLE;          // 单次传输

   HAL_DMA_Init(&hdma_dcmi);                        // 配置DMA
   __HAL_LINKDMA(&hdcmi, DMA_Handle, hdma_dcmi);    // 关联DCMI句柄
	
   HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);         // 设置中断优先级
   HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);                 // 使能中断
}
/* USER CODE END 1 */
