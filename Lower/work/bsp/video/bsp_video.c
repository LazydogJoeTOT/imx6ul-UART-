// 视频处理相关功能的实现

#include "bsp_lcdapi.h"
#include "bsp_uart.h"
#include "bsp_video.h"
#include "bsp_clk.h"
#include "bsp_delay.h"

unsigned char video_data[4096][1024]; // 全局二维数组
unsigned int video_size = 0;
unsigned char video_finish = 0;

// 用于显示指定位置的图片
void getpicture(unsigned short x, unsigned short y, unsigned short px, unsigned short py, unsigned char data[1024])
{
	lcd_show_picture(x, y, px, py, data); /* 显示图片 */
}

// 播放视频，通过多次显示图片的方式实现
void playvideo(unsigned short x, unsigned short y, unsigned short px, unsigned short py, unsigned char data[][1024], unsigned int video_size)
{
	int i;
	video_finish = 0;
	for (i = 0; i < video_size; i++)
	{
		getpicture(x, y, px, py, data[i]);
		delayms(100);
	}
	video_finish = 1;
}

// 获取视频数据并填充到全局数组中
void getvideo()
{
	unsigned char size_buffer[4];
	int i;

	// 接收4字节数据表示文件字节大小
	for (i = 0; i < 4; i++)
	{
		size_buffer[i] = getc();
	}

	video_size = (size_buffer[0] << 24) | (size_buffer[1] << 16) | (size_buffer[2] << 8) | (size_buffer[3]);
	// 读取视频数据并填充到二维数组
	for (i = 0; i < video_size; i++)
	{
		getp(video_data[i]);
	}
}