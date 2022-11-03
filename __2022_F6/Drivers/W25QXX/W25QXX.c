/*
 * W25QXX.c
 *	Brief: SPI1driver:PA5 <->	W25QXX_SCK
 *				   PA6 <->	W25QXX_MISO
 *				   PA7 <->	W25QXX_MOSI
 *		   Configuration：Clock Parameters
 *					Prescaler: 256
 *					CPOL: High
 *					CPHA: 2 Edge
 *		   GPIO：PC4 <->  W25QXX_CS
 *		   		Push-pull output
 *
 *		   W25Q128容量为16M,共有128个Block,4096个Sector
 *         255byte为一页
 *		   4Kbytes为一个扇区
 *         16个扇区为一个块
 *
 *  Created on: Jul 12, 2022
 *      Author: 王志涵
 */

#include "W25QXX.h"

//默认 W25QXX 的芯片类型是 W25Q128
uint16_t W25QXX_TYPE = W25Q128;

//用于保存被擦除的数据（一个扇区）
static uint8_t W25QXX_BUFFER[4096];

/*!
 *  \brief  W25QXX(FLASH芯片)SPI传输与接收一个字节
 *  \param  TxData 发送的数据
 *  \retval 接收的数据
 */
uint8_t W25QXX_ReadWriteByte(uint8_t TxData)
{
	uint8_t RxBuf, TxBuf = TxData;

	HAL_SPI_TransmitReceive(&w25qxx, &TxBuf, &RxBuf, 1, HAL_MAX_DELAY); //传输数据

	return RxBuf;
}

/*!
 *  \brief  W25QXX(FLASH芯片)设置速度
 *  \param  BaudRatePrescaler 波特率分配数
 */
void W25QXX_SET_SPEED(uint8_t BaudRatePrescaler)
{
	__HAL_SPI_DISABLE(&w25qxx);								 //使能w25qxx的spi
	w25qxx.Instance->CR1 &= 0XFFC7;						 //位3-5清零，用来设置波特率
	w25qxx.Instance->CR1 |= BaudRatePrescaler; //设置w25qxx的spi速度(波特率分频数)
	__HAL_SPI_ENABLE(&hspi1);									 //使能w25qxx的spi
}

/*!
 *  \brief  W25QXX(FLASH芯片)初始化
 */
void W25QXX_Init(void)
{
	W25QXX_CS_H;															 // SPI FLASH不选中
	W25QXX_ReadWriteByte(0xff);								 //启用运输
	W25QXX_SET_SPEED(SPI_BAUDRATEPRESCALER_4);			 //设置SPI为高速模式
	W25QXX_TYPE = W25QXX_ReadID();						 //读取FLASH的ID
}

/*!
 *  \brief  读取W25QXX的状态寄存器
 *  \retval W25QXX 繁忙状态(1,忙;0,空闲)
 */
uint8_t W25QXX_Read_Status(void)
{
	uint8_t rxBuf;

	W25QXX_CS_L;															//使能器件
	W25QXX_ReadWriteByte(W25X_ReadStatusReg); //发送读取状态寄存器指令
	rxBuf = W25QXX_ReadWriteByte(0xff);				//读取返回值
	W25QXX_CS_H;															//取消片选

	return rxBuf;
}

/*!
 *  \brief  W25QXX写使能
 */
void W25QXX_Write_Enable(void)
{
	W25QXX_CS_L;														//使能器件
	W25QXX_ReadWriteByte(W25X_WriteEnable); //发送写使能指令
	W25QXX_CS_H;														//取消片选
}

/*!
 *  \brief  W25QXX写不使能
 */
void W25QXX_Write_Disable(void)
{
	W25QXX_CS_L;														 //使能器件
	W25QXX_ReadWriteByte(W25X_WriteDisable); //发送写失能指令
	W25QXX_CS_H;														 //取消片选
}

/*!
 *  \brief  W25QXX读取芯片ID
 *  \retval 芯片ID
 *  \note 0XEF13,表示芯片型号为W25Q80
 *  \note 0XEF14,表示芯片型号为W25Q16
 *  \note 0XEF15,表示芯片型号为W25Q32
 *  \note 0XEF16,表示芯片型号为W25Q64
 *  \note 0XEF17,表示芯片型号为W25Q128
 */
uint16_t W25QXX_ReadID(void)
{
	uint8_t rxBuf[2];

	W25QXX_CS_L;																 //使能器件
	W25QXX_ReadWriteByte(W25X_ManufactDeviceID); //发送读取设备ID指令
	W25QXX_ReadWriteByte(0);										 //缓
	W25QXX_ReadWriteByte(0);
	W25QXX_ReadWriteByte(0);
	rxBuf[0] = W25QXX_ReadWriteByte(0xff); //读取ID高八位
	rxBuf[1] = W25QXX_ReadWriteByte(0xff); //读取ID低八位
	W25QXX_CS_H;													 //取消片选

	return ((uint16_t)rxBuf[0] << 8) + rxBuf[1];
}

/*!
 *  \brief  读取W25QXX内容
 *  \param  pBuffer 读取的数组的地址
 *  \param  ReadAddr FLASH的目标地址
 *  \param  NumByteToRead 要读取的字节数
 */
void W25QXX_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	uint16_t i;

	W25QXX_CS_L; //使能器件

	/* 发送读取指令,地址 */
	W25QXX_ReadWriteByte(W25X_ReadData);
	W25QXX_ReadWriteByte((uint8_t)(ReadAddr >> 16));
	W25QXX_ReadWriteByte((uint8_t)(ReadAddr >> 8));
	W25QXX_ReadWriteByte((uint8_t)(ReadAddr));
	/* 读取数据 */
	for (i = 0; i < NumByteToRead; i++)
	{
		pBuffer[i] = W25QXX_ReadWriteByte(0xff);
	}

	W25QXX_CS_H; //取消片选
}

/*!
 *  \brief  W25QXX在一页写入少于256个字节的数据
 *  \param  pBuffer        要写入的数组的地址
 *  \param  WriteAddr      FLASH的目标地址
 *  \param  NumByteToWrite 要写入的字节数
 *  \warn   不能翻页写入数据 / 写入前需要擦除
 */
void W25QXX_Write_Page(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t i;

	W25QXX_Write_Enable(); //写使能
	W25QXX_CS_L;					 //使能器件

	/* 发送写入指令,地址 */
	W25QXX_ReadWriteByte(W25X_PageProgram);
	W25QXX_ReadWriteByte((uint8_t)(WriteAddr >> 16));
	W25QXX_ReadWriteByte((uint8_t)(WriteAddr >> 8));
	W25QXX_ReadWriteByte((uint8_t)(WriteAddr));
	for (i = 0; i < NumByteToWrite; i++)
	{
		W25QXX_ReadWriteByte(pBuffer[i]);
	}

	W25QXX_CS_H;				//取消片选
	W25QXX_Wait_Busy(); //等待空闲
}

/*!
 *  \brief  无检验写SPI FLASH
 *  \param  pBuffer        要写入的数组的地址
 *  \param  WriteAddr      FLASH的目标地址
 *  \param  NumByteToWrite 要写入的字节数
 *  \note   写入前需要擦除
 */
void W25QXX_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t pageremain;

	pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
	if (NumByteToWrite <= pageremain)
		pageremain = NumByteToWrite; //不大于256个字节

	for (;;)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if (NumByteToWrite == pageremain)
			break; //写入结束了
		else		 // NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain; //减去已经写入了的字节数
			if (NumByteToWrite > 256)
				pageremain = 256; //一次可以写入256个字节
			else
				pageremain = NumByteToWrite; //不够256个字节了
		}
	}
}

/*!
 *  \brief  检验写SPI FLASH
 *  \param  pBuffer        要写入的数组的地址
 *  \param  WriteAddr      FLASH的目标地址
 *  \param  NumByteToWrite 要写入的字节数
 */
void W25QXX_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i;
	uint8_t *W25QXX_BUF;
	W25QXX_BUF = W25QXX_BUFFER;
	secpos = WriteAddr / 4096; //扇区地址
	secoff = WriteAddr % 4096; //在扇区内的偏移
	secremain = 4096 - secoff; //扇区剩余空间大小

	if (NumByteToWrite <= secremain)
		secremain = NumByteToWrite; //不大于4096个字节
	for (;;)
	{
		W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096); //读出整个扇区的内容
		for (i = 0; i < secremain; i++)								//校验数据
		{
			if (W25QXX_BUF[secoff + i] != 0XFF)
				break; //需要擦除
		}
		if (i < secremain) //需要擦除
		{
			W25QXX_Erase_Sector(secpos);		//擦除这个扇区
			for (i = 0; i < secremain; i++) //复制
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096); //写入整个扇区
		}
		else
			W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
		if (NumByteToWrite == secremain)
			break; //写入结束了
		else		 //写入未结束
		{
			secpos++;										 //扇区地址增1
			secoff = 0;									 //偏移位置为0
			pBuffer += secremain;				 //指针偏移
			WriteAddr += secremain;			 //写地址偏移
			NumByteToWrite -= secremain; //字节数递减
			if (NumByteToWrite > 4096)
				secremain = 4096; //下一个扇区还是写不完
			else
				secremain = NumByteToWrite; //下一个扇区可以写完了
		}
	}
}

/*!
 *  \brief  擦除整个芯片
 *  \warn   需要时间长
 */
void W25QXX_Erase_Chip(void)
{
	W25QXX_Write_Enable();								//写使能
	W25QXX_Wait_Busy();										//等待空闲
	W25QXX_CS_L;													//使能器件
	W25QXX_ReadWriteByte(W25X_ChipErase); //发送擦除整个芯片指令
	W25QXX_CS_H;													//取消片选
	W25QXX_Wait_Busy();										//等待芯片擦除结束
}

/*!
 *  \brief  擦除一个扇区
 *  \param  Dst_Addr 扇区数
 *  \warn   擦除一个扇区至少需要150ms
 */
void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{
	Dst_Addr *= 4096;

	W25QXX_Write_Enable();													 //写使能
	W25QXX_Wait_Busy();															 //等待空闲
	W25QXX_CS_L;																		 //使能器件
	W25QXX_ReadWriteByte(W25X_SectorErase);					 //发送擦除一个山区指令
	W25QXX_ReadWriteByte((uint8_t)(Dst_Addr) >> 16); //发送24位地址
	W25QXX_ReadWriteByte((uint8_t)(Dst_Addr) >> 8);
	W25QXX_ReadWriteByte((uint8_t)(Dst_Addr));
	W25QXX_CS_H;				//取消片选
	W25QXX_Wait_Busy(); //等待芯片擦除结束
}

/*!
 *  \brief  进入掉电模式
 */
void W25QXX_PowerDown(void)
{
	W25QXX_CS_L;													//使能器件
	W25QXX_ReadWriteByte(W25X_PowerDown); //发送擦除一个山区指令
	W25QXX_CS_H;													//取消片选
	delay_ms(3);													//等待TPD
}

/*!
 *  \brief  等待W25QXX空闲
 */
void W25QXX_Wait_Busy(void)
{
	while ((W25QXX_Read_Status() & 0x01) == 0x01)
		;
}

