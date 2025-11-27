/**
 ******************************************************************************
 * @file    user_hal_callbacks.c
 * @author  菜菜why（B站：菜菜whyy）
 * @brief   用户HAL回调函数实现文件
 ******************************************************************************
 * @attention
 *
 * 本文件用于实现回调函数。
 * 这些函数会覆盖库中的弱符号默认实现。
 ******************************************************************************
 */

#include "user_hal_callbacks.h"

/**
 * @brief  按键事件处理函数（用户实现）
 * @param  id: 按键ID（KEY_ID枚举值）
 * @param  ev: 事件类型（KEY_Event枚举值）
 * @note   此函数覆盖key.c中的弱符号默认实现
 * @note   使用if判断按键ID，switch判断事件类型
 * @retval None
 */
#ifdef KEY_ENABLE
void KEY_EventHandler(KEY_ID id, KEY_Event ev)
{
    /* 根据按键ID分别处理 */
    if (id == KEY1)
    {
        switch (ev)
        {
        case KEY_EV_PRESS:
            /* 按下事件处理 */
            break;

        case KEY_EV_RELEASE:
            /* 释放事件处理 */
            break;

        case KEY_EV_CLICK:
            /* 单击事件：闪烁LED一次（示例） */
            {
                LED_Blink_All(1000);
            }
            break;

        case KEY_EV_DOUBLE_CLICK:
            /* 双击事件：闪烁LED两次（示例） */
            {
                LED_Blink_All(1000);
                LED_Blink_All(1000);
            }
            break;

        case KEY_EV_LONG_PRESS:
            /* 长按事件：闪烁LED三次（示例） */
            {
                LED_Blink_All(1000);
                LED_Blink_All(1000);
                LED_Blink_All(1000);
            }
            break;
        }
    }
    /* 若有更多按键，在此添加 else if (id == KEY2) { ... } */
}
#endif // KEY_ENABLE
