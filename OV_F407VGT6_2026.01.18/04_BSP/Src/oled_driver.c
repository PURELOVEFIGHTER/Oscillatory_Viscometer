/*
 * oled.c
 *
 *  Created on: 2022年8月16日
 *      Author: HP
 */
#include "oled_driver.h"
#include <string.h>

#define OLED_WIDTH 128
#define OLED_PAGES 8

#define OLED_I2C_ADDR  0x78
#define OLED_CMD_ADDR  0x00
#define OLED_DATA_ADDR 0x40

static uint8_t OLED_GRAM[OLED_PAGES][OLED_WIDTH];

static volatile uint8_t oled_dma_busy = 0;

static void OLED_GRAM_WriteByte(uint8_t x, uint8_t y, uint8_t data, uint8_t Color_Turn) {
    if (x >= OLED_WIDTH || y >= OLED_PAGES) {
        return;
    }

    if (Color_Turn) {
        OLED_GRAM[y][x] = ~data;
    } else {
        OLED_GRAM[y][x] = data;
    }
}

/**********************************************************
 * 初始化命令,根据芯片手册书写
 ***********************************************************/
uint8_t CMD_Data[] = {0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,

                      0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,

                      0xD8, 0x30, 0x8D, 0x14, 0xAF, 0x20, 0x00, 0x21, 0x00, 0x7F, 0x22, 0x00, 0x07};

void WriteCmd(void) {
    uint8_t i = 0;
    for (i = 0; i < sizeof(CMD_Data); i++) {
        HAL_I2C_Mem_Write(&hi2c1, OLED_I2C_ADDR, OLED_CMD_ADDR, I2C_MEMADD_SIZE_8BIT, &CMD_Data[i], 1, 0x100);
    }
}
/**
 * @function: void OLED_Init(void)
 * @description: OLED初始化
 * @return {*}
 */
void OLED_Init(void) {
    HAL_Delay(200);

    WriteCmd();
}

/**
 * @function: void OLED_WR_CMD(uint8_t cmd)
 * @description: 向设备写控制命令
 * @param {uint8_t} cmd 芯片手册规定的命令
 * @return {*}
 */
void OLED_WR_CMD(uint8_t cmd) {
    HAL_I2C_Mem_Write(&hi2c1, OLED_I2C_ADDR, OLED_CMD_ADDR, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 0x100);
}

/**
 * @function: void OLED_WR_DATA(uint8_t data)
 * @description: 向设备写控制数据
 * @param {uint8_t} data 数据
 * @return {*}
 */
void OLED_WR_DATA(uint8_t data) {
    HAL_I2C_Mem_Write(&hi2c1, OLED_I2C_ADDR, OLED_DATA_ADDR, I2C_MEMADD_SIZE_8BIT, &data, 1, 0x100);
}

void OLED_ClearDMAFlag(void) { oled_dma_busy = 0; }

void OLED_Refresh_DMA(void) {
    if (oled_dma_busy) {
        return;
    }

    oled_dma_busy = 1;

    if (HAL_I2C_Mem_Write_DMA(&hi2c1, OLED_I2C_ADDR, OLED_DATA_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t *)OLED_GRAM,
                              OLED_WIDTH * OLED_PAGES)
        != HAL_OK) {
        oled_dma_busy = 0;
    }
}

/**
 * @function: void OLED_On(void)
 * @description: 更新显示

 * @return {*}
 */
void OLED_On(void) { memset(OLED_GRAM, 0xFF, sizeof(OLED_GRAM)); }

/**
 * @function: OLED_Clear(void)
 * @description: 清屏,整个屏幕是黑色的!和没点亮一样!!!
 * @return {*}
 */
void OLED_Clear(void) { memset(OLED_GRAM, 0x00, sizeof(OLED_GRAM)); }

/**
 * @function: void OLED_Display_On(void)
 * @description: 开启OLED显示
 * @return {*}
 */
void OLED_Display_On(void) {
    OLED_WR_CMD(0X8D); // SET DCDC命令
    OLED_WR_CMD(0X14); // DCDC ON
    OLED_WR_CMD(0XAF); // DISPLAY ON,打开显示
}

/**
 * @function: void OLED_Display_Off(void)
 * @description: 关闭OLED显示
 * @return {*}
 */
void OLED_Display_Off(void) {
    OLED_WR_CMD(0X8D); // SET DCDC命令
    OLED_WR_CMD(0X10); // DCDC OFF
    OLED_WR_CMD(0XAE); // DISPLAY OFF，关闭显示
}

/**
 * @function: void OLED_Set_Pos(uint8_t x, uint8_t y)
 * @description: 坐标设置
 * @param {uint8_t} x,y
 * @return {*}
 */
void OLED_Set_Pos(uint8_t x, uint8_t y) {
    OLED_WR_CMD(0xb0 + y);                 // 设置页地址（0~7）
    OLED_WR_CMD(((x & 0xf0) >> 4) | 0x10); // 设置显示位置—列低地址
    OLED_WR_CMD(x & 0x0f);                 // 设置显示位置—列高地址
}

/**
 * @function: unsigned int oled_pow(uint8_t m,uint8_t n)
 * @description: m^n函数
 * @param {uint8_t} m,n
 * @return {unsigned int} result
 */
unsigned int oled_pow(uint8_t m, uint8_t n) {
    unsigned int result = 1;

    while (n--)
        result *= m;

    return result;
}

/**
 * @function: void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size,uint8_t Color_Turn)
 * @description: 在OLED12864特定位置开始显示一个字符
 * @param {uint8_t} x字符开始显示的横坐标
 * @param {uint8_t} y字符开始显示的纵坐标
 * @param {uint8_t} chr待显示的字符
 * @param {uint8_t} Char_Size待显示字符的字体大小,选择字体 16/12
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size, uint8_t Color_Turn) {
    uint8_t c = 0;
    uint8_t i = 0;

    if (chr < ' ' || chr > '~') {
        chr = ' ';
    }

    c = chr - ' ';

    if (x >= OLED_WIDTH || y >= OLED_PAGES) {
        return;
    }

    if (Char_Size == 16) {
        if (x > OLED_WIDTH - 8 || y > OLED_PAGES - 2) {
            return;
        }

        for (i = 0; i < 8; i++) {
            OLED_GRAM_WriteByte(x + i, y, F8X16[c * 16 + i], Color_Turn);
        }

        for (i = 0; i < 8; i++) {
            OLED_GRAM_WriteByte(x + i, y + 1, F8X16[c * 16 + i + 8], Color_Turn);
        }
    } else {
        if (x > OLED_WIDTH - 6) {
            return;
        }

        for (i = 0; i < 6; i++) {
            OLED_GRAM_WriteByte(x + i, y, F6x8[c][i], Color_Turn);
        }
    }
}

/**
 * @function: void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_tChar_Size, uint8_t Color_Turn)
 * @description: 在OLED12864特定位置开始显示字符串
 * @param {uint8_t} x待显示字符串的开始横坐标x:0~127
 * @param {uint8_t} y待显示字符串的开始纵坐标
 * y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {uint8_t} *chr待显示的字符串
 * @param {uint8_t} Char_Size待显示字符串的字体大小,选择字体 16/12，16为8X16，12为6x8
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowString(uint8_t x, uint8_t y, char *chr, uint8_t Char_Size, uint8_t Color_Turn) {
    uint8_t j = 0;

    while (chr[j] != '\0') {
        if (y >= OLED_PAGES) {
            break;
        }

        OLED_ShowChar(x, y, chr[j], Char_Size, Color_Turn);

        if (Char_Size == 12) {
            x += 6;
        } else {
            x += 8;
        }

        if (x > 122 && Char_Size == 12) {
            x = 0;
            y++;
        }

        if (x > 120 && Char_Size == 16) {
            x = 0;
            y += 2;
        }

        j++;
    }
}

/**
 * @function: void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2, Color_Turn)
 * @description: 显示数字
 * @param {uint8_t} x待显示的数字起始横坐标,x:0~126
 * @param {uint8_t} y待显示的数字起始纵坐标,
 * y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {unsigned int} num:输入的数据
 * @param {uint8_t } len:输入的数据位数
 * @param {uint8_t} size2:输入的数据大小，选择 16/12，16为8X16，12为6x8
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowNum(uint8_t x, uint8_t y, unsigned int num, uint8_t len, uint8_t size2, uint8_t Color_Turn) {
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2, Color_Turn);
                continue;
            } else
                enshow = 1;
        }

        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2, Color_Turn);
    }
}

/**
 * @function: void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2, uint8_t
 * Color_Turn)
 * @description: 显示正负浮点数
 * @param {uint8_t} x待显示的数字起始横坐标,x:0~126
 * @param {uint8_t} y待显示的数字起始纵坐标,
 * y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {float} num:输入的浮点型数据
 * @param {uint8_t } z_ len:整数部分的位数
 * @param {uint8_t } f_len: 小数部分的位数
 * @param {uint8_t} size2:输入的数据大小，选择 16/12，16为8X16，12为6x8
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_Showdecimal(uint8_t x, uint8_t y, float num, uint8_t z_len, uint8_t f_len, uint8_t size2,
                      uint8_t Color_Turn) {
    uint8_t t, temp, i = 0; // i为负数标志位
    uint8_t enshow = 0;
    int z_temp, f_temp;

    if (num < 0) {
        z_len += 1;
        i   = 1;
        num = -num;
    }

    z_temp = (int)num;

    // 整数部分
    for (t = 0; t < z_len; t++) {
        temp = (z_temp / oled_pow(10, z_len - t - 1)) % 10;

        if (enshow == 0 && t < (z_len - 1)) {
            if (temp == 0) {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2, Color_Turn);
                continue;
            } else
                enshow = 1;
        }

        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2, Color_Turn);
    }

    // 小数点
    OLED_ShowChar(x + (size2 / 2) * (z_len), y, '.', size2, Color_Turn);

    f_temp = (int)((num - z_temp) * (oled_pow(10, f_len)));

    // 小数部分
    for (t = 0; t < f_len; t++) {
        temp = (f_temp / oled_pow(10, f_len - t - 1)) % 10;
        OLED_ShowChar(x + (size2 / 2) * (t + z_len) + 5, y, temp + '0', size2, Color_Turn);
    }

    if (i == 1) // 如果为负，就将最前的一位赋值‘-’
    {
        OLED_ShowChar(x, y, '-', size2, Color_Turn);
        i = 0;
    }
}

/**
 * @function: void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no, uint8_t Color_Turn)
 * @description: 在OLED特定位置开始显示16X16汉字
 * @param {uint8_t} x待显示的汉字起始横坐标x: 0~112，两列汉字之间需要间隔16
 * @param {uint8_t} y待显示的汉字起始纵坐标 y: 0~6 , 两行汉字之间需要间隔2
 * @param {uint8_t} no待显示的汉字编号
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no, uint8_t Color_Turn) {
    uint8_t t = 0;

    if (x > OLED_WIDTH - 16 || y > OLED_PAGES - 2) {
        return;
    }

    for (t = 0; t < 16; t++) {
        OLED_GRAM_WriteByte(x + t, y, Hzk[2 * no][t], Color_Turn);
    }

    for (t = 0; t < 16; t++) {
        OLED_GRAM_WriteByte(x + t, y + 1, Hzk[2 * no + 1][t], Color_Turn);
    }
}

/**
 * @function: void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *  BMP,uint8_t Color_Turn)
 * @description: 在OLED特定区域显示BMP图片
 * @param {uint8_t} x0图像开始显示横坐标  x0:0~127
 * @param {uint8_t} y0图像开始显示纵坐标  y0:0~7
 * @param {uint8_t} x1图像结束显示横坐标  x1:1~128
 * @param {uint8_t} y1图像结束显示纵坐标  y1:1~8
 * @param {uint8_t} *BMP待显示的图像数据
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t *BMP, uint8_t Color_Turn) {
    uint32_t j = 0;
    uint8_t x  = 0;
    uint8_t y  = 0;

    if (x0 >= OLED_WIDTH || y0 >= OLED_PAGES) {
        return;
    }

    if (x1 > OLED_WIDTH) {
        x1 = OLED_WIDTH;
    }

    if (y1 > OLED_PAGES) {
        y1 = OLED_PAGES;
    }

    for (y = y0; y < y1; y++) {
        for (x = x0; x < x1; x++) {
            OLED_GRAM_WriteByte(x, y, BMP[j++], Color_Turn);
        }
    }
}

/**
 * @function: void OLED_HorizontalShift(uint8_t direction)
 * @description: 屏幕内容水平全屏滚动播放
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @return {*}
 */
void OLED_HorizontalShift(uint8_t direction)

{
    OLED_WR_CMD(0x2e);      // 停止滚动
    OLED_WR_CMD(direction); // 设置滚动方向
    OLED_WR_CMD(0x00);      // 虚拟字节设置，默认为0x00
    OLED_WR_CMD(0x00);      // 设置开始页地址
    OLED_WR_CMD(0x07);      // 设置每个滚动步骤之间的时间间隔的帧频
    //  0x00-5帧， 0x01-64帧， 0x02-128帧， 0x03-256帧， 0x04-3帧， 0x05-4帧， 0x06-25帧， 0x07-2帧，
    OLED_WR_CMD(0x07); // 设置结束页地址
    OLED_WR_CMD(0x00); // 虚拟字节设置，默认为0x00
    OLED_WR_CMD(0xff); // 虚拟字节设置，默认为0xff
    OLED_WR_CMD(0x2f); // 开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}

/**
 * @function: void OLED_Some_HorizontalShift(uint8_t direction,uint8_t start,uint8_t end)
 * @description: 屏幕部分内容水平滚动播放
 * @param {uint8_t} direction			LEFT	   0x27     	RIGHT  0x26
 * @param {uint8_t} start 开始页地址  0x00~0x07
 * @param {uint8_t} end  结束页地址  0x01~0x07
 * @return {*}
 */
void OLED_Some_HorizontalShift(uint8_t direction, uint8_t start, uint8_t end) {
    OLED_WR_CMD(0x2e);      // 停止滚动
    OLED_WR_CMD(direction); // 设置滚动方向
    OLED_WR_CMD(0x00);      // 虚拟字节设置，默认为0x00
    OLED_WR_CMD(start);     // 设置开始页地址
    OLED_WR_CMD(0x07);      // 设置每个滚动步骤之间的时间间隔的帧频,0x07即滚动速度2帧
    OLED_WR_CMD(end);       // 设置结束页地址
    OLED_WR_CMD(0x00);      // 虚拟字节设置，默认为0x00
    OLED_WR_CMD(0xff);      // 虚拟字节设置，默认为0xff
    OLED_WR_CMD(0x2f);      // 开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}

/**
 * @function: void OLED_VerticalAndHorizontalShift(uint8_t direction)
 * @description: 屏幕内容垂直水平全屏滚动播放
 * @param {uint8_t} direction				右上滚动	 0x29
 *                                          左上滚动     0x2A
 * @return {*}
 */
void OLED_VerticalAndHorizontalShift(uint8_t direction) {
    OLED_WR_CMD(0x2e);      // 停止滚动
    OLED_WR_CMD(direction); // 设置滚动方向
    OLED_WR_CMD(0x01);      // 虚拟字节设置
    OLED_WR_CMD(0x00);      // 设置开始页地址
    OLED_WR_CMD(0x07);      // 设置每个滚动步骤之间的时间间隔的帧频，即滚动速度
    OLED_WR_CMD(0x07);      // 设置结束页地址
    OLED_WR_CMD(0x01);      // 垂直滚动偏移量
    OLED_WR_CMD(0x00);      // 虚拟字节设置，默认为0x00
    OLED_WR_CMD(0xff);      // 虚拟字节设置，默认为0xff
    OLED_WR_CMD(0x2f);      // 开启滚动-0x2f，禁用滚动-0x2e，禁用需要重写数据
}

/**
 * @function: void OLED_DisplayMode(uint8_t mode)
 * @description: 屏幕内容取反显示
 * @param {uint8_t} direction			ON	0xA7
 *                                      OFF	0xA6	默认此模式，设置像素点亮
 * @return {*}
 */
void OLED_DisplayMode(uint8_t mode) { OLED_WR_CMD(mode); }

/**
 * @function: void OLED_IntensityControl(uint8_t intensity)
 * @description: 屏幕亮度调节
 * @param  {uint8_t} intensity	0x00~0xFF,RESET=0x7F
 * @return {*}
 */
void OLED_IntensityControl(uint8_t intensity) {
    OLED_WR_CMD(0x81);
    OLED_WR_CMD(intensity);
}
// ————————————————
// 版权声明：本文为CSDN博主「想要亿只独角兽」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https://blog.csdn.net/LYH6767/article/details/126032948
