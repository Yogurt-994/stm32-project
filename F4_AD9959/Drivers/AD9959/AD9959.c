#include "ad9959.h"

u8 CSR_DATA0[1] = {0x10};     // 开 CH0
u8 CSR_DATA1[1] = {0x20};      // 开 CH1
u8 CSR_DATA2[1] = {0x40};      // 开 CH2
u8 CSR_DATA3[1] = {0x80};      // 开 CH3

sin_freq sinfrq;
sweep_freq sweepfreq;

uint8_t FR2_DATA[2] = {0x20, 0x00};//default Value = 0x0000
uint8_t CFR_DATA[3] = {0x00, 0x03, 0x02};//default Value = 0x00030
uint8_t CPOW0_DATA[2] = {0x00, 0x00};//default Value = 0x0000   @ = POW/2^14*360
uint8_t LSRR_DATA[2] = {0x00, 0x00};//default Value = 0x----
uint8_t RDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};//default Value = 0x--------
uint8_t FDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};//default Value = 0x--------



/**
* @brief AD9959略微延时
* */
static void ad9959_delay(uint32_t length)
 {
    length = length * 12;
    while (length--);
 }

/**
* @brief AD9959初始化
* */
void ad9959_init(void) {
    uint8_t FR1_DATA[3] = {0xD0, 0x00,0x00};//20倍频 Charge pump control = 75uA FR1<23> -- VCO gain control =0时 system clock below 160 MHz;

    ad9959_io_init();//引脚初始化
    ad9959_reset();//9959复位

    ad9959_write_data(AD9959_REG_FR1, 3, FR1_DATA, 1);//写功能寄存器1
    //ad9959_write_data(AD9959_REG_FR2, 2, FR2_DATA, 0);//写功能寄存器2

    //ad9959_write_data(0x00,1,CSR_DATA0,1);//使能通道1
    //写入初始频率、相位、幅值给四个通道
    ad9959_write_frequency(AD9959_CHANNEL_0, 1000);
    ad9959_write_phase(AD9959_CHANNEL_0, 0);
    ad9959_write_amplitude(AD9959_CHANNEL_0, 1023);
}

/**
* @brief AD9959复位
* */
void ad9959_reset(void) {
    AD9959_RESET_0;
    ad9959_delay(1);
    AD9959_RESET_1;
    ad9959_delay(30);
    AD9959_RESET_0;
}

/**
* @brief AD9959IO口初始化
* */
void ad9959_io_init(void) {

    AD9959_PDC_0;
    AD9959_CS_1;
    AD9959_SCLK_0;
    AD9959_UPDATE_0;
    AD9959_PS0_0;
    AD9959_PS1_0;
    AD9959_PS2_0;
    AD9959_PS3_0;
    AD9959_SDIO0_0;
    AD9959_SDIO1_0;
    AD9959_SDIO2_0;
    AD9959_SDIO3_0;
}

/**
 * @brief AD9959更新IO寄存器
 * */
void ad9959_io_update(void) {
    AD9959_UPDATE_0;
    ad9959_delay(2);
    AD9959_UPDATE_1;
    ad9959_delay(4);
    AD9959_UPDATE_0;
}

/**
 * @brief 通过SPI向AD9959写数据
 * @param register_address 寄存器地址
 * @param number_of_registers 所含字节数
 * @param register_data 数据起始地址
 * @param update 是否更新IO寄存器
 * */
void ad9959_write_data(AD9959_REG_ADDR register_address, uint8_t number_of_registers, const uint8_t *register_data,bool update)
    {
    uint8_t ControlValue = 0;
    uint8_t ValueToWrite = 0;
    uint8_t RegisterIndex = 0;
    uint8_t i = 0;

    assert_param(IS_AD9959_REG_ADDR(register_address));//检查寄存器地址的参数是否正确


    ControlValue = register_address;
//写入地址
    AD9959_SCLK_0;
    AD9959_CS_0;
    for (i = 0; i < 8; i++) {
        AD9959_SCLK_0;
        if (0x80 == (ControlValue & 0x80))
            AD9959_SDIO0_1;
        else
            AD9959_SDIO0_0;
            AD9959_SCLK_1;
        ControlValue <<= 1;
    }
    AD9959_SCLK_0;
//写入数据
    for (RegisterIndex = 0; RegisterIndex < number_of_registers; RegisterIndex++)
    {
        ValueToWrite = register_data[RegisterIndex];
        for (i = 0; i < 8; i++) {
            AD9959_SCLK_0;
            if (0x80 == (ValueToWrite & 0x80))
                AD9959_SDIO0_1;
            else
                AD9959_SDIO0_0;
            AD9959_SCLK_1;
            ValueToWrite <<= 1;
        }
        AD9959_SCLK_0;
    }
    if (update) ad9959_io_update();
    AD9959_CS_1;
}

/**
 * @brief 设置通道输出相位
 * @param channel 输出通道
 * @param phase 输出相位 14bit 输出相位范围：0~16383(对应角度：0°~360°)
 * */
void ad9959_write_phase(AD9959_CHANNEL channel, uint16_t phase) {
    uint8_t cs_data = channel;
    assert_param(IS_AD9959_CHANNEL(channel));//检查关于通道的参数是否正确

    CPOW0_DATA[1] = (uint8_t) phase;
    CPOW0_DATA[0] = (uint8_t) (phase >> 8);
    ad9959_write_data(AD9959_REG_CSR, 1, &cs_data, 1);
    ad9959_write_data(AD9959_REG_CPOW0, 2, CPOW0_DATA, 1);

}

/**
 * @brief 设置通道输出频率
 * @param channel 输出通道
 * @param amplitude 输出频率 (频率范围 1 ~ 200000000Hz)
 * */
void ad9959_write_frequency(AD9959_CHANNEL channel, uint32_t Freq) {
    uint8_t CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00};    //中间变量
    uint32_t frequency;
    uint8_t cs_data = channel;

    assert_param(IS_AD9959_CHANNEL(channel));//检查关于通道的参数是否正确


    frequency = (uint32_t) Freq * 8.589934592;       //将输入频率因子分为四个字节  8.589934592=(2^32)/500000000 其中500M=25M*20(倍频数可编程)
    CFTW0_DATA[3] = (uint8_t) frequency;
    CFTW0_DATA[2] = (uint8_t) (frequency >> 8);
    CFTW0_DATA[1] = (uint8_t) (frequency >> 16);
    CFTW0_DATA[0] = (uint8_t) (frequency >> 24);

    ad9959_write_data(AD9959_REG_CSR, 1, &cs_data, 1);
    ad9959_write_data(AD9959_REG_CFTW0, 4, CFTW0_DATA, 1);


}


/**
 * @brief 设置通道输出幅度
 * @param channel 输出通道
 * @param amplitude 输出幅度 10bit 输出相位范围：0~1023(对应幅度：0 ~ 530mV)
 * */
void ad9959_write_amplitude(AD9959_CHANNEL channel, uint16_t amplitude) {
    uint8_t ACR_DATA[3] = {0x00, 0x00, 0x00};//default Value = 0x--0000 Rest = 18.91/Iout
    uint8_t cs_data = channel;

    assert_param(IS_AD9959_CHANNEL(channel));//检查关于通道的参数是否正确

    amplitude = amplitude | 0x1000;
    ACR_DATA[2] = (uint8_t) amplitude;
    ACR_DATA[1] = (uint8_t) (amplitude >> 8);

    ad9959_write_data(AD9959_REG_CSR, 1, &cs_data, 1);
    ad9959_write_data(AD9959_REG_ACR, 3, ACR_DATA, 1);

}
/**
 * @brief 幅度调节
 * */
void Out_mV(u8 ch, float nmV)
{
    //nmV=nmV/590*1024;
    nmV=nmV/500*1024;   //阻抗匹配会衰减一半，所以这里多输出一倍
    if(nmV>1023)nmV=1023;
    //else if(nmV<=0)nmV=2;
    ad9959_write_amplitude(ch,nmV);
    //Write_Amplitude(ch,nmV*Amp_Freq5V[(u32)sinwave.freq/1000000]);
}
/**
 * @brief 单频输出
 * */
void Out_freq(u8 ch, u32 freq)
{
//    static float freq_before=100;
    if(freq>FreqMax)freq=FreqMax;
    if(freq<FreqMin)freq=FreqMin;
    ad9959_write_frequency(ch,freq);
//    if(freq_before != sinwave.freq) //如果频率变化则也触发幅度变化
//    {
//        Out_mV(ch, sinwave.amp);
//        freq_before=sinwave.freq;
//    }
}

/**
 * @brief 扫频输出
 * */
void Sweep_Freq(void)
{
    if(sinfrq.mode==0)
    {
        sinfrq.Fre+=1000;
        ad9959_write_frequency(AD9959_CHANNEL_0, sinfrq.Fre);
        if(sinfrq.Fre==FreqMax)
            sinfrq.mode=1;
    }
    if(sinfrq.mode==1)
    {
        sinfrq.Fre-=1000;
        ad9959_write_frequency(AD9959_CHANNEL_0, sinfrq.Fre);
        if(sinfrq.Fre==FreqMin)
            sinfrq.mode=0;
    }
}
void Sweep_Freq_1(void)
{
	u16 i,times;
	times=(sweepfreq.end-sweepfreq.start)/sweepfreq.step+1;
	for(i=0;i<times;i++)
	{
        Out_freq(0,sweepfreq.start+sweepfreq.step*i);
        delay_ms(sweepfreq.time);
	}
}
