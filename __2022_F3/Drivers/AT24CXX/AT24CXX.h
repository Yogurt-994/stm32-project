/*
 * AT24CXX.h
 *
 *  Created on: Jul 11, 2022
 *      Author: 王志涵
 */

#ifndef AT24CXX_AT24CXX_H_
#define AT24CXX_AT24CXX_H_

#include "main.h"
#include "i2c.h"

//对应型号芯片容量（单位：Byte）
#define AT24C01 127
#define AT24C02 255
#define AT24C04 511
#define AT24C08 1023
#define AT24C16 2047
#define AT24C32 4095
#define AT24C64 8191
#define AT24C128 16383
#define AT24C256 32767

/*! Private macros define */
#define Write_ADDR 0xA0
#define Read_ADDR 0XA1

/* 自行填写AT芯片型号以及对应的i2c */
#define EE_TYPE AT24C02
#define at24cxx hi2c1

uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr);                                        //指定地址读取一个字节
HAL_StatusTypeDef AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite);       //指定地址写入一个字节
HAL_StatusTypeDef AT24CXX_Write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite); //从指定地址开始写入指定长度的数据
HAL_StatusTypeDef AT24CXX_Read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);    //从指定地址开始读出指定长度的数据

#endif /* AT24CXX_AT24CXX_H_ */
