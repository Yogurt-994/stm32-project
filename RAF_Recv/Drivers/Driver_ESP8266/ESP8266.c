#include "ESP8266.h"
#include "usart.h"
#include "tim.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdarg.h"
#include "main.h"

extern DMA_HandleTypeDef hdma_usart6_rx;

/**
 *@brief ESP8266结构体变量初始化
 *@retval None
 **/
ESP8266 wifi;
void wifiInit(void)
{
	wifi.huart=&huart6;
	wifi.dma_rx=&hdma_usart6_rx;
	wifi.tim=&htim6;
	wifi.send=send;
	wifi.sendNoAck=sendNoAck;
	wifi.sendCmd=sendCmd;
	wifi.quitTrans=quitTrans;
	wifi.RxCallback=RxCallback;
	wifi.setTimeout=setTimeout;
	wifi.isTimeout=isTimeout;
	wifi.delay=delay;
	wifi.isResponded=isResponded;
	wifi.init=init;
}

/**
 * @brief  多串口重定向
 * @retval None
 **/
void print(UART_HandleTypeDef* huart, const char* buf, ...)
{
  char str[RxBufferSize] = {0};
  va_list v;
  va_start(v, buf);
  vsprintf(str, buf, v); 	//使用可变参数的字符串打印。类似sprintf
  HAL_UART_Transmit(huart,(uint8_t*)str,strlen(str),0xffffffff);
  va_end(v);
}

/**
 * @brief  ESP8266 串口接收回调函数
 * @retval None
 **/
void RxCallback(){
	UART_HandleTypeDef*  huart = wifi.huart; //ESP8266用到的串口

	/* 判断是否发生空闲中断 */
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET) {
		__HAL_UART_CLEAR_IDLEFLAG(huart); //清除IDLE中断标志位
		HAL_UART_DMAStop(huart);          // 停止DMA接收

		//以下步骤是将接收到的内容存入 AckBuffer 中
		int recv_end    = RxBufferSize - __HAL_DMA_GET_COUNTER(wifi.dma_rx); // 得到接收到的字符长度
		int recv_start  = recv_end % RxBufferSize;                      // 接收到的内容地起始索引
		while(wifi.RxBuffer[recv_start] == 0) { //找出接收的起始
			recv_start = (recv_start + 1) % RxBufferSize;
		}
		int index = recv_start;
		int i;
		for(i=0; index != recv_end; i++)
		{
			wifi.AckBuffer[i] = wifi.RxBuffer[index];
			wifi.RxBuffer[index] = 0;
			index = (index+1)%RxBufferSize;
		}
		wifi.AckBuffer[i] = 0; //用作字符串分隔符

		//调试用，将接收到的响应通过串口输出
		print(&huart3,"\r\n接收:\r\n");
		print(&huart3,"%s",wifi.AckBuffer);
		//使用队列来接收
		if(wifi.uart_state == Sent) {	//如果串口发送完毕，等待接收
			wifi.uart_state = Idle; 	//变为空闲模式
		}
		if(wifi.uart_state == Idle) { //（一般是被动地接收到信息）
		//for(;;); //TODO:
			if(!strcmp(wifi.AckBuffer, "LED")) {
				print(&huart3,"\r\n灯翻转\r\n");
				HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
			};
		}

		/* 准备下一次DMA接收 */
		__HAL_DMA_SET_COUNTER(wifi.dma_rx, 0);
		HAL_UART_Receive_DMA(huart, (uint8_t*)wifi.RxBuffer, RxBufferSize);
	}
}

/**
 * @brief  ESP8266 设置发送指令或内容后，最大的等待超时时间
 * @note ms不能超过43,690
 * @retval None
 **/
void setTimeout(uint16_t ms){
	__HAL_TIM_SET_AUTORELOAD(wifi.tim, ms*3/2);  //要设置延迟10ms的中断，则需要设置 ARR 为 10*3/2;
	__HAL_TIM_SET_COUNTER(wifi.tim, 1);          //因为要根据CNT是否为0来判断是否超时，所以这里设置为1
	__HAL_TIM_ENABLE(wifi.tim);                  //开启定时器，定时器在溢出后自动归0，并停止运作
}

/**
 * @brief  ESP8266 判断是否超时
 * @note   必须在setTimeout之后才能使用
 * @retval None
 **/
bool isTimeout(void){
	if(__HAL_TIM_GET_COUNTER(wifi.tim) == 0){
		return true;
	}
	else {
		return false;
	}
}


/*
 *
 * */
void sendNoAck(uint8_t *str) {
	print(&huart6,"%s",(char*)str);
//TODO: 调试用
	if(!(str[0] == '\r' && str[1] == '\n')) {
		print(&huart3,"\r\n发送:\r\n");
		print(&huart3,"%s",(char*)str);
	}
}


result send(char *str,const char* response,uint16_t timeout){
	/* 发送内容 */
	sendNoAck((uint8_t*)str);

	/* 等待响应 */
	wifi.uart_state = Sent;
	setTimeout(timeout); //设置超时时间
	while(!isTimeout() && !isResponded(response));
	wifi.uart_state = Idle;

	/* 判断执行的结果 */
	if(isTimeout()) {
		return Timeout; //发送失败或者超时
	}
	else {
		return Success; //内容成功发送
	}
}

result sendCmd(const char *str,const char* response,uint16_t timeout){
	/* 发送内容 */
	sendNoAck((uint8_t*)str);
	sendNoAck((uint8_t*)"\r\n");

	/* 等待响应 */
	wifi.uart_state = Sent;
	setTimeout(timeout); //设置超时时间
	while(!isTimeout() && !isResponded(response));
	wifi.uart_state = Idle;

	/* 判断执行的结果 */
	if(isResponded(response)) {
		return Success;
	}
	else {
		return Timeout; //发送失败或者超时
	}
}

/**
 * @brief  退出透传模式
 * @retval None
 **/
void quitTrans(void) {
	/* 发送 "+++" 退出透传状态 */
	sendNoAck((uint8_t*)"+"); delay(15);//大于串口组帧时间(10ms)
	sendNoAck((uint8_t*)"+"); delay(15);
	sendNoAck((uint8_t*)"+"); delay(500);
	/* 关闭透传模式 */
	sendCmd("AT+CIPMODE=0", "OK", 200);
}

/**
 * @brief  延迟函数
 * @note   通过定时器实现的延迟，只是使用在quitTrans函数
 * @retval None
 **/
void delay(uint16_t ms) {
	setTimeout(ms);
	while(!isTimeout());
}

/**
 * @brief  判断是否从ESP8266得到期望的响应（母串找子串）
 * @retval 是否得到期望的响应
 **/
bool isResponded(const char* response){
	int responseLen = strlen(response);
	int rxBufferLen = strlen(wifi.RxBuffer);
	for(int i=0,j=0; i<rxBufferLen; i++) {
		if(wifi.RxBuffer[i] == response[j])	{
			if(++j == responseLen) { //找到字串 （即找到响应）
				return true;
			}
		}
		else{
			j = 0;
		}
	}
	return false; //未找到子串
}

/**
 * @brief  ESP8266 初始化函数
 * @retval 初始化是否成功
 **/
result init(){
	wifiInit();
	UART_HandleTypeDef*  huart = wifi.huart; //ESP8266用到的串口
	/* 开启DMA接收 */
	HAL_UART_Receive_DMA(huart, (uint8_t*)wifi.RxBuffer, RxBufferSize);
	__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE); //开启空闲中断

	/* 注意这个时候定时器并没有开始运作 */
	HAL_TIM_Base_Start(wifi.tim);
	HAL_TIM_OnePulse_Init(wifi.tim, TIM_OPMODE_SINGLE);                 //设置为 OnePulse 模式
	__HAL_TIM_SET_PRESCALER(wifi.tim, HAL_RCC_GetHCLKFreq()/3/1000-1);  //设置分频数为 56,000

	delay(100);
	return Success;
}
