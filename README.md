# STM32H7 QSPI Flash 字库驱动项目
## 
## 概述
使用反客科技的stm32h750xbh6，借住外扩的qspi flash，实现**约7400个汉字、5种大小（12,16,20,24,32）、GB2312+utf8的宋体汉字以及英文数字的显示**。原理图见Doc文档。



调用

```plain
LCD_DisplayText(0,0,"这是一个测试，哈基米南北绿豆，stm32~");
```

即可显示，该函数支持中英文混合，且自动换行。



使用

```plain
LCD_SetTextFont(12);
```

改变字体大小，默认是24

![](https://cdn.nlark.com/yuque/0/2025/jpeg/48302010/1764258384756-66948435-e84d-4d0a-93fe-a5b0e2a073b1.jpeg)



由于我后续移植lvgl、usb等程序，内部flash不够，有代码要放在qspi flash里面，于是我选择把字库放在了32mb外部flash的**最后3mb**。文件下载地址如下：



如果需要修改位置，直接修改flash_font.h的BASE_ADDR定义就行了

```cpp
/**
 * @brief QSPI Flash字库存储区域定义(相对于Flash起始地址的偏移)
 */
#define BASE_ADDR 0x1D00000
#define FONT_12x12_ADDR BASE_ADDR + 0x0 /*!< 12x12字体区域起始地址  */
#define FONT_16x16_ADDR BASE_ADDR + 0x2BBE0 /*!< 16x16字体区域起始地址  */
#define FONT_20x20_ADDR BASE_ADDR + 0x66100 /*!< 20x20字体区域起始地址  */
#define FONT_24x24_ADDR BASE_ADDR + 0xD3680 /*!< 24x24字体区域起始地址 */
#define FONT_32x32_ADDR BASE_ADDR + 0x1569E0 /*!< 32x32字体区域起始地址 */

#define GB2312_TABLE_ADDR BASE_ADDR + 0x23FE00 /*!< GB2312对照表地址 */
#define UTF8_TABLE_ADDR BASE_ADDR + 0x2472D0   /*!< utf8对照表地址 */
#define FONT_FLAG_ADDR BASE_ADDR + 0x2572F0    /*!< 字库标志存储地址 */
#define ASCII_FONTS_ADDR BASE_ADDR + 0x267310  /*!< ASCII字库地址 */
```

 只需要把merged_fonts.bin 烧录到0x91D00000里面就可以了。

---

##  项目结构
```plain
fkh750xbh6_qspi_fontlib/
├── Core/
│   └── Src/
│       └── main.c              # 主程序入口
├── BSP/                        # 板级支持包
│   ├── init.h / init.c         # 统一初始化管理
│   ├── GPIO/
│   │   └── led.h               # LED 驱动
│   ├── SPI/
│   │   ├── lcd_spi.h           # LCD SPI 驱动
│   │   ├── lcd_fonts.h         # LCD 字体库
│   │   └── lcd_image.h         # LCD 图像处理
│   └── QSPI/
│       ├── qspi_flash.h        # QSPI Flash 底层驱动
│       ├── flash_font.h        # 字库管理（头文件）
│       └── flash_font.c        # 字库管理（实现）
└── README.md                   # 说明文档
```

---

## 字库bin文件烧录方式
我使用**cube programmer**下载。

我自己制作了stldr下载文件，位于"fkh750xbh6_qspi_fontlib\BSP\QSPI\FontBin\STM32H750XBH_ARTPIQSPI_W25Q256JV_114.stldr"，将其添加到cube programmer的下载算法目录，cubeprogrammer\bin\ExternalLoader

![](https://cdn.nlark.com/yuque/0/2025/png/48302010/1764259287085-4f546dd5-e760-4ed6-a2b8-97742ebb277c.png)

打开软件，选择烧录算法

![](https://cdn.nlark.com/yuque/0/2025/png/48302010/1764259372886-4cdf6296-80e1-46ad-8bb3-bc7f8ed6ac2e.png)

选择文件，选择烧录地址，烧录

![](https://cdn.nlark.com/yuque/0/2025/png/48302010/1764259436725-05e1e011-2ecc-46a0-a597-922ca955c7a7.png)

按地址一个个烧录即可。



能读取到数据就ok

![](https://cdn.nlark.com/yuque/0/2025/png/48302010/1764259480590-9db529c4-0732-4035-adbf-01dd28183ba3.png)



## 字库代码移植方法
确保烧录好字库

在flash_font.h中，把#include "init.h"全局替换为你自己的单片机头文件，比如#include "stm32h7xx_hal.h"

![](https://cdn.nlark.com/yuque/0/2025/png/48302010/1764335617296-2481ef0f-aaf9-4c8b-8959-63eed9bf6fcc.png)

调用api即可，调用方式非常简单，比如GB2312_FindFont_Flash("你",16)，会自动返回这个汉字的16x16大小在flash中的起始地址。

```cpp
/**
     * @brief  从Flash查找汉字并返回字模数据指针
     * @param  text: 汉字字符串(GBK编码，2字节)
     * @param  font_size: 字体大小(12/16/20/24/32)
     * @retval 字模数据指针，查找失败返回NULL
     */
    const uint8_t *GB2312_FindFont_Flash(const char *text, uint8_t font_size);
```

```cpp
/**
     * @brief  从Flash查找UTF8字符并返回字模数据指针
     * @param  utf8_text: UTF8字符字符串(1-4字节)
     * @param  font_size: 字体大小(12/16/20/24/32)
     * @retval 字模数据指针，查找失败返回NULL
     * @note   自动识别UTF-8字符长度，无需手动指定
     */
    const uint8_t *UTF8_FindFont_Flash(const uint8_t *utf8_text,
                                       uint8_t font_size);
```

## 更新记录
### 11.28更新：
简化字库函数调用逻辑，添加utf8代码逻辑。

使用utf8的时候，keil中，Options → C/C++ → Misc Controls 里加

--no_multibyte_chars

![](https://cdn.nlark.com/yuque/0/2025/png/48302010/1764335369454-605ac7be-adb8-4310-b516-93226b65c143.png)

### 12.5更新
加入各种大小的英文数字，flash用量减少26kb！！！

和多个bin文件为一个，现在只要下一次了！

