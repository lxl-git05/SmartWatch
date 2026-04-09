#include "ov5640_app.h"
#include "i2c.h"
#include "dma.h"
// ???????
uint16_t camera_buffer[FRAME_SIZE / 2] __attribute__((section(".sram_data2")));
extern DMA_HandleTypeDef hdma_dcmi; 
// ?? OV5640 ??????? (RGB565, QVGA, 30fps)
// ????????????,????
const uint16_t ov5640_reg_table[][2] = {
    // ???
    {0x3103, 0x11}, {0x3008, 0x82}, {0x3008, 0x42},
    // ????
    {0x3103, 0x03}, {0x3600, 0x00}, {0x3703, 0x5A},
    {0x3801, 0x00}, {0x3805, 0x3F}, {0x3806, 0x00}, {0x3809, 0x7F},
    {0x380A, 0x04}, {0x380D, 0x03}, {0x380E, 0x07}, {0x3813, 0x06},
    // ?????? (RGB565)
    {0x4300, 0x30}, // YUV422 -> RGB565 (0x30)
    // ????? QVGA (320x240)
    {0x3800, 0x00}, // X_ADDR_ST[11:8]
    {0x3801, 0x00}, // X_ADDR_ST[7:0]
    {0x3802, 0x00}, // Y_ADDR_ST[11:8]
    {0x3803, 0x00}, // Y_ADDR_ST[7:0]
    {0x3804, 0x0A}, // X_ADDR_END[11:8]
    {0x3805, 0x3F}, // X_ADDR_END[7:0]
    {0x3806, 0x07}, // Y_ADDR_END[11:8]
    {0x3807, 0x9F}, // Y_ADDR_END[7:0]
    {0x3808, 0x01}, // X_OUTPUT_SIZE[11:8] (320)
    {0x3809, 0x40}, // X_OUTPUT_SIZE[7:0]
    {0x380A, 0x00}, // Y_OUTPUT_SIZE[11:8] (240)
    {0x380B, 0xF0}, // Y_OUTPUT_SIZE[7:0]
    {0x380C, 0x07}, // X_TOTAL_SIZE[11:8]
    {0x380D, 0x68}, // X_TOTAL_SIZE[7:0]
    {0x380E, 0x03}, // Y_TOTAL_SIZE[11:8]
    {0x380F, 0xB0}, // Y_TOTAL_SIZE[7:0]
    // ?????????
    {0x3A08, 0x12}, {0x3A09, 0x60}, {0x3A0A, 0x00}, {0x3A0B, 0xF0},
    {0x3A0E, 0x03}, {0x3A0D, 0x04}, {0x3A14, 0x12}, {0x3A15, 0x60},
    {0x4001, 0x02}, {0x4004, 0x02}, {0x3000, 0x00},
    {0x3034, 0x1A}, {0x3037, 0x13},
    {0x3108, 0x01}, {0x3610, 0x52}, {0x3612, 0xAB},
    {0x3708, 0x62}, {0x3709, 0x52}, {0x370C, 0x03},
    {0x3A02, 0x03}, {0x3A03, 0xD8}, {0x3A08, 0x01}, {0x3A09, 0x27},
    {0x3A0A, 0x00}, {0x3A0B, 0xF6}, {0x3A0E, 0x03}, {0x3A0D, 0x04},
    {0x3A14, 0x01}, {0x3A15, 0x27}, {0x4001, 0x02}, {0x4004, 0x02},
    {0x4005, 0x1A}, {0x4300, 0x30}, // RGB565
    {0x503D, 0x00}, // ??????
    {0xFFFF, 0x00}  // ????
};

// ???????
static HAL_StatusTypeDef OV5640_WriteReg(uint16_t reg, uint8_t val) {
    return HAL_I2C_Mem_Write(&hi2c2, 0x78, reg, I2C_MEMADD_SIZE_16BIT, &val, 1, 100);
}

/**
 * @brief ?? OV5640 ???
 */
void OV5640_Config_Init(void) {
    uint8_t id_h, id_l;
    printf("1. Checking ID...\r\n");
    
    // 1. ?? ID
    if (HAL_I2C_Mem_Read(&hi2c2, 0x78, 0x300A, I2C_MEMADD_SIZE_16BIT, &id_h, 1, 100) == HAL_OK) {
        HAL_I2C_Mem_Read(&hi2c2, 0x78, 0x300B, I2C_MEMADD_SIZE_16BIT, &id_l, 1, 100);
        printf("   ID: 0x%02X%02X\r\n", id_h, id_l);
        
        if (id_h == 0x56 && id_l == 0x40) {
            printf("2. OV5640 Detected. Starting Init...\r\n");
            
            // 2. ?????
            for (int i = 0; ov5640_reg_table[i][0] != 0xFFFF; i++) {
                if (OV5640_WriteReg(ov5640_reg_table[i][0], ov5640_reg_table[i][1]) != HAL_OK) {
                    printf("I2C Write Error at 0x%04X\r\n", ov5640_reg_table[i][0]);
                }
            }
            printf("3. OV5640 Initialization Complete!\r\n");
        } else {
            printf("ID Mismatch!\r\n");
        }
    } else {
        printf("I2C Read Error!\r\n");
    }
}

/**
 * @brief ???????
 */
void OV5640_Capture_And_Send(void)
{
    // 1. ???? DCMI ? DMA ?? (?? "Start Failed" ?????)
    HAL_DCMI_Stop(&hdcmi);                // ?? DCMI ?????
//    hdma_dcmi.State = HAL_DMA_STATE_RESET; // ?? DMA ???
//    hdma_dcmi.ErrorCode = HAL_DMA_ERROR_NONE;
//    hdcmi.State = HAL_DCMI_STATE_RESET;    // ?? DCMI ???
//    hdcmi.ErrorCode = HAL_DCMI_ERROR_NONE;

    // 2. ?? DCMI DMA ??
    // ??:Size ??????? DCMI ?????? RGB565 (16?),??? FRAME_SIZE/2
    // ???? FRAME_SIZE ?????? (320*240*2),???? 2 ???? (?????)
    if (HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_SNAPSHOT, (uint32_t)camera_buffer, FRAME_SIZE / 2) != HAL_OK)
    {
        printf("Error: DCMI Start Failed!\r\n");
        return;
    }

    // 3. ?????? (?????,????)
    uint32_t start_tick = HAL_GetTick();
    while (HAL_DCMI_GetState(&hdcmi) == HAL_DCMI_STATE_BUSY)
    {
        // ????:???? 1 ??????,??????
        if ((HAL_GetTick() - start_tick) > 1000)
        {
            printf("Error: DCMI Timeout! Check PCLK/HSYNC wiring.\r\n");
            HAL_DCMI_Stop(&hdcmi); // ????
            return;
        }
    }

    // 4. ????,????
    printf("Captured! Sending Data...\r\n");

    // --- ????? (0xAA 0x55) ---
    uint8_t header[] = {0xAA, 0x55};
    HAL_UART_Transmit(&huart2, header, 2, 100);

    // --- ???????? (?? uint16_t ????) ---
    uint32_t total_size = FRAME_SIZE; // ???? (?? 153600)
    uint32_t offset = 0;
    uint16_t chunk_size = 1000;       // ???? 1000 ??

    while (offset < total_size)
    {
        // ??????????
        uint16_t current_len = (total_size - offset > chunk_size) ? chunk_size : (uint16_t)(total_size - offset);

        // ????
        // ??:(uint8_t*)camera_buffer + offset ?????????
        HAL_UART_Transmit(&huart2, (uint8_t*)camera_buffer + offset, current_len, HAL_MAX_DELAY);

        offset += current_len;
    }

    // --- ????? (??) ---
    // uint8_t tail[] = {0xCC, 0xDD};
    // HAL_UART_Transmit(&huart2, tail, 2, 100);

    printf("Frame Sent! Total: %lu bytes.\r\n", (unsigned long)total_size);
}