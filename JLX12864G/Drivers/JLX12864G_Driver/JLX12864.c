/*
 * JLX12864.c
 *
 *  Created on: May 2, 2022
 *      Author: 王志涵
 */
#include "JLX12864.h"
#include "base.h"

void transfer_command_lcd(int data1) //写指令到LCD
{
	char i;
	lcd_cs1_l;
	lcd_rs_l;
	for(i=0;i<8;i++)
	{
		lcd_sclk_l;
		//delay_us(10);
		if(data1&0x80) lcd_sid_h;
		else lcd_sid_l;
		lcd_sclk_h;
		//delay_us(10);
		data1=data1<<=1;
	}
	lcd_cs1_h;
}

void transfer_data_lcd(int data1) //写数据到LCD
{
	char i;
	lcd_cs1_l;
	lcd_rs_h;
	for(i=0;i<8;i++)
	{
		lcd_sclk_l;
		if(data1&0x80) lcd_sid_h;
		else lcd_sid_l;
		lcd_sclk_h;
		data1=data1<<=1;
	}
	lcd_cs1_h;
}

void initial_lcd() //LCD初始化
{
	lcd_reset_l;
	delay_ms(100);
	lcd_reset_h;
	delay_ms(100);
	transfer_command_lcd(0xe2);
	delay_ms(5);
	transfer_command_lcd(0x2c);
	delay_ms(50);
	transfer_command_lcd(0x2e);
	delay_ms(50);
	transfer_command_lcd(0x2f);
	delay_ms(5);
	transfer_command_lcd(0x23);
	transfer_command_lcd(0x81);
	transfer_command_lcd(0x28);
	transfer_command_lcd(0xa2);
	transfer_command_lcd(0xc8);
	transfer_command_lcd(0xa0);
	transfer_command_lcd(0x40);
	transfer_command_lcd(0xaf);
}

void lcd_address(uint page,uint column)
{
	column=column-0x01;
	transfer_command_lcd(0xb0+page-1);
	transfer_command_lcd(0x10+(column>>4&0x0f));
	transfer_command_lcd(column&0x0f);
}

void clear_screen() //全屏清屏
{
	unsigned char i,j;
	for(i=0;i<9;i++)
	{
		transfer_command_lcd(0xb0+i);
		transfer_command_lcd(0x10);
		transfer_command_lcd(0x00);
		for(j=0;j<132;j++)
		{
			transfer_data_lcd(0x00);
		}
	}
}

void display_128x64(uchar *dp) //显示128*64的点阵图像
{
	uint i,j;
	for(j=0;j<8;j++)
	{
		lcd_address(j+1,1);
		for (i=0;i<128;i++)
		{
			transfer_data_lcd(*dp);
			dp++;
		}
	}
}

void display_graphic_16x16(uchar page,uchar column,uchar *dp) //显示16*16的点阵图像、汉字、生僻字或16*16点阵的其它图标
{
	uint i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{
			transfer_data_lcd(*dp);
			dp++;
		}
	}
}


void display_graphic_8x16(uchar page,uchar column,uchar *dp) //显示8*16点阵图像、ASCII、或8*16点阵的自造字符、其它图标
{
	uint i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<8;i++)
		{
			transfer_data_lcd(*dp);
			dp++;
		}
	}
}

void display_graphic_5x8(uchar page,uchar column,uchar *dp) //5*8
{
	uint i;
	lcd_address(page,column);
	for (i=0;i<6;i++)
	{
		transfer_data_lcd(*dp);
		dp++;
	}
}

void send_command_to_ROM( uchar datu ) //送指令到字库
{
	uchar i;
	for(i=0;i<8;i++ )
	{
		Rom_SCK_l;
		delay_us(10);
		if(datu&0x80)Rom_IN_h;
		else Rom_IN_l;
		datu = datu<<1;
		Rom_SCK_h;
		delay_us(10);
	}
}

uchar get_data_from_ROM( ) //从字库中取汉字或字符
{
	uchar i;
	uchar ret_data=0;
	for(i=0;i<8;i++)
	{
//		ROM_OUT_OUT();
		Rom_OUT_h;
		Rom_SCK_l;
		ret_data=ret_data<<1;
//		ROM_OUT_IN();
		if(Rom_OUT_cin)
		ret_data=ret_data+1;
		else
		ret_data=ret_data+0;
		Rom_SCK_h;
	}
	return(ret_data);
}

void get_and_write_16x16(ulong fontaddr,uchar page,uchar column)
{
	uchar i,j,disp_data;
	Rom_CS_l;
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);
	send_command_to_ROM((fontaddr&0xff00)>>8);
	send_command_to_ROM(fontaddr&0xff);
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for(i=0; i<16; i++ )
		{
			disp_data=get_data_from_ROM();
			transfer_data_lcd(disp_data);
		}
	}
	Rom_CS_h;
}



void get_and_write_8x16(ulong fontaddr,uchar page,uchar column)
{
	uchar i,j,disp_data;
	Rom_CS_l;
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);
	send_command_to_ROM((fontaddr&0xff00)>>8);
	send_command_to_ROM(fontaddr&0xff);
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for(i=0; i<8; i++ )
		{
			disp_data=get_data_from_ROM();
			transfer_data_lcd(disp_data);
		}
	}
	Rom_CS_h;
}

void get_and_write_5x8(ulong fontaddr,uchar page,uchar column)
{
	uchar i,disp_data;
	Rom_CS_l;
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);
	send_command_to_ROM((fontaddr&0xff00)>>8);
	send_command_to_ROM(fontaddr&0xff);
	lcd_address(page,column);
	for(i=0; i<5; i++ )
	{
		disp_data=get_data_from_ROM();
		transfer_data_lcd(disp_data);
	}
	Rom_CS_h;
}

volatile ulong fontaddr=0;
void display_GB2312_string(uchar page,uchar column,uchar *text)
{
	uchar i= 0;
	while((text[i]>0x00))
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
		{
			fontaddr = (text[i]- 0xb0)*94;
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);
			get_and_write_16x16(fontaddr,page,column);
			i+=2;
			column+=16;
		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
		{
			fontaddr = (text[i]- 0xa1)*94;
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (ulong)(fontaddr*32);
			get_and_write_16x16(fontaddr,page,column);
			i+=2;
			column+=16;
		}
		else if((text[i]>=0x20) &&(text[i]<=0x7e))
		{
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*16);
			fontaddr = (unsigned long)(fontaddr+0x3cf80);
			get_and_write_8x16(fontaddr,page,column);
			i+=1;
			column+=8;
		}
		else
		i++;
	}
}


void display_string_5x8(uchar page,uchar column,uchar *text)
{
	unsigned char i= 0;
	while((text[i]>0x00))
	{
		if((text[i]>=0x20) &&(text[i]<=0x7e))
		{
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*8);
			fontaddr = (unsigned long)(fontaddr+0x3bfc0);
			get_and_write_5x8(fontaddr,page,column);
			i+=1;
			column+=6;
		}
		else
		i++;
	}
}
