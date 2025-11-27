# STM32H7 QSPI Flash 字库驱动项目
## 概述
使用反客科技的stm32h750xbh6，借住外扩的qspi flash，实现**约7400个汉字、5种大小（12,16,20,24,32）的宋体字符显示**。原理图见Doc文档。



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



由于我后续移植lvgl、usb等程序，内部flash不够，有代码要放在qspi flash里面，于是我选择把字库放在了32mb外部flash的**最后3mb**。各个文件下载地址如下：



如果需要修改位置，直接修改flash_font.h的定义就行

```cpp
/**
 * @brief QSPI Flash字库存储区域定义(相对于Flash起始地址的偏移)
 */
#define FONT_12x12_ADDR 0x1D00000 /*!< 12x12字体区域起始地址  */
#define FONT_16x16_ADDR 0x1D2BBE0 /*!< 16x16字体区域起始地址  */
#define FONT_20x20_ADDR 0x1D66100 /*!< 20x20字体区域起始地址  */
#define FONT_24x24_ADDR 0x1DD3680 /*!< 24x24字体区域起始地址 */
#define FONT_32x32_ADDR 0x1E569E0 /*!< 32x32字体区域起始地址 */

#define GB2312_TABLE_ADDR 0x1F3FE00 /*!< GB2312对照表地址 */
#define FONT_FLAG_ADDR 0x1F472D0    /*!< 字库标志存储地址 */
```

| 文件名           | 烧录地址   | 说明          |
| ---------------- | ---------- | ------------- |
| font_12x12_*.bin | 0x91D00000 | 12×12 字库    |
| font_16x16_*.bin | 0x91D2BBE0 | 16×16 字库    |
| font_20x20_*.bin | 0x91D66100 | 20×20 字库    |
| font_24x24_*.bin | 0x91DD3680 | 24×24 字库    |
| font_32x32_*.bin | 0x91E569E0 | 32×32 字库    |
| gb2312_table.bin | 0x91F3FE00 | GB2312 对照表 |
| flag.bin         | 0x91F472D0 | 标志位验证    |


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

