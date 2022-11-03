/*
 * MLT_BT05.h
 *
 *  Created on: Mar 6, 2022
 *      Author: 王志涵
 */

#ifndef DRIVER_MLT_BT05_MLT_BT05_H_
#define DRIVER_MLT_BT05_MLT_BT05_H_

#include "tim.h"
#include "usart.h"
#include "stdbool.h"
#define RxBufferSize 2048	//接收缓存大小
/***********MLT_BT05数据类型定义***********/
/*MLT_BT05当前工作模式*/
typedef enum{
	NOINIT,		//未初始化
	MASTER,		//主透传
	SLAVE,		//从透传
	LOOPBACK,	//回环
	DISCONNECT	//断开连接
}mode_set;
/*串口执行的状态 */
typedef enum{
	Idle,			//空闲
	Busy,			//繁忙
	Sent			//发送完毕，等待接收
}uart_state_set;
/* 动作执行的结果 */
typedef enum{
		Fail,        //失败
		Success,     //成功
		Timeout      //超时
}result;
typedef struct{
	UART_HandleTypeDef *huart;			//MLT_BT05所用的串口
	DMA_HandleTypeDef *dma_rx;			//用于串口接收的DMA
	TIM_HandleTypeDef *tim;				//用于记录是否超时的定时器
	mode_set mode;                 		//ESP8266的工作模式
	uart_state_set uart_state;    		//记录用于通信的串口的状态
	char RxBuffer[RxBufferSize]; 		//接收缓存
	char AckBuffer[RxBufferSize]; 		//存放响应内容的缓存
	result (*send)(char *str, const char* response, uint16_t timeout);
	void (*sendNoAck)(uint8_t *str);
	result (*sendCmd)(const char *str, const char* response, uint16_t timeout);
//	void (*quitTrans)(void);
	void (*RxCallback)(void);
	void (*setTimeout)(uint16_t ms);
	bool (*isTimeout)(void);
	void (*delay)(uint16_t ms);
	bool (*isResponded)(const char* response);
	result (*init)(void);
}MLT_BT05;

/*ESP8266结构体变量初始化*/
void bleInit(void);
/* 发送内容 */
result send(char *str, const char* response, uint16_t timeout);
/* 发送内容(无要求响应) */
void sendNoAck(uint8_t *str);
/* 发送指令 */
result sendCmd(const char *str, const char* response, uint16_t timeout);
///* 退出透传模式 */
//void quitTrans(void);
/* 接收回调函数（需自己添加） */
void RxCallback(void);
/* 设置超时时间 */
void setTimeout(uint16_t ms);
/* 判断是否超时 */
bool isTimeout(void);
/* 延迟函数 */
void delay(uint16_t ms);
/* 判断是否被响应 */
bool isResponded(const char* response);
/* 初始化 */
result init(void);
/* 多串口重定向输出 */
void print(UART_HandleTypeDef* huart, const char* buf, ...);

#endif /* DRIVER_MLT_BT05_MLT_BT05_H_ */
