
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exit.h"
#include "bsp_uart.h"
#include "bsp_lcd.h"
#include "bsp_lcdapi.h"
#include "bsp_rtc.h"
#include "bsp_backlight.h"
#include "bsp_adc.h"
#include "bsp_video.h"

#include "stdio.h"

/*
 * @description	: 使能I.MX6U的硬件NEON和FPU
 * @param 		: 无
 * @return 		: 无
 */
void imx6ul_hardfpu_enable(void)
{
	uint32_t cpacr;
	uint32_t fpexc;

	/* 使能NEON和FPU */
	cpacr = __get_CPACR();
	cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk)) | (3UL << CPACR_cp10_Pos) | (3UL << CPACR_cp11_Pos);
	__set_CPACR(cpacr);
	fpexc = __get_FPEXC();
	fpexc |= 0x40000000UL;
	__set_FPEXC(fpexc);
}

volatile

	/*
	 * @description	: main函数
	 * @param 		: 无
	 * @return 		: 无
	 */
	int
	main(void)
{

	imx6ul_hardfpu_enable(); /* 使能I.MX6U的硬件浮点 			*/
	int_init();				 /* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();		 /* 初始化系统时钟 					*/
	delay_init();			 /* 初始化延时 					*/
	clk_enable();			 /* 使能所有的时钟 					*/
	led_init();				 /* 初始化led 					*/
	beep_init();			 /* 初始化beep	 				*/
	key_init();
	uart_init(); /* 初始化串口，波特率115200 */
	lcd_init();	 /* 初始化LCD 					*/
	exit_init();
	adc1ch1_init(); /* ADC1_CH1			 		*/

	unsigned char state = OFF;
	tftlcd_dev.forecolor = LCD_BLACK;

	lcd_show_string(0, 0, 100, 20, 16, "Test Picture"); /* 显示文字 */
	// puts("Just Fire!");
	// getp(gImage);
	// lcd_show_picture(0, 0, 128, 64, gImage);		/* 显示图片 */
	lcd_show_string(0, 64, 100, 20, 16, "Test String"); /* 显示文字 */

	getvideo();
	// playvideo(0,0,128,64,video_data, video_size);
	lcd_show_string(0, 64, 100, 20, 16, "Video Ready"); /* 显示文字 */

	while (!video_finish)
	{
		state = !state;
		led_switch(LED0, state);
		delayms(500);
	}

	lcd_show_string(0, 128, 100, 20, 16, "Video End"); /* 显示文字 */

	return 0;
}
