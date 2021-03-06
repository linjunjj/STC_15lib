#include "common.h"
#include "STC_soft_PWM.h"
#include "STC_gpio.h"
#include "STC_port.h"
#include "STC_PIT.h"
#include "STC_interrupt.h"

/*!
 *	@brief		软件实现PWM初始化
 *	@param		use_Tn	使用哪个定时器作为软件PWM的时钟
							pxn			将PWM信号输出至哪个引脚
							freq		PWM信号初始频率
							duty		PWM信号初始占空比
 *	@warning	软件PWM输出所在IO口工作模式不能为浮空输入模式，上电初始化为准双向口
              STC15F2K60S2没有T3和T4，详情请看手册
 *	@sampleusage:	soft_pwm_init(PWM0_T0 , p11 , 50 , 50);//使用T0定时器作为软件PWM0的时钟，输出端口为p11，频率50Hz，占空比为50/PWM0_PRECISON
 */
 
pxn_e		PWM0_out , PWM1_out , PWM2_out , PWM3_out , PWM4_out;
uint16 data	soft_pwm0_duty = 0 , soft_pwm1_duty = 0 , soft_pwm2_duty = 0,
				    soft_pwm3_duty = 0 , soft_pwm4_duty = 0,
				    Regular0_times_g = 0 , Regular1_times_g = 0 , Regular2_times_g = 0,
				    Regular3_times_g = 0 , Regular4_times_g = 0;

void soft_pwm_init(Use_Tn_e use_Tn , pxn_e pxn , uint32 freq , uint16 duty)
{
	uint16 Regular_times = 0;//定时次数
	
	if(freq > EXTAL_IN_Hz/(65536L*(1+ACQUIRE(CLK_DIV,2,0))))//定时器计满所需要的周期大于设置频率的信号周期，脉宽调制方可在一次定时周期内完成
	{
		if(use_Tn == PWM0_T0)//若使用PWM0
		{
			if(duty > PWM0_PRECISON)	duty = PWM0_PRECISON;
			PWM0_out = pxn;//确定pwm输出引脚
			Regular0_times_g = EXTAL_IN_Hz/(freq*(1+ACQUIRE(CLK_DIV,2,0)));//根据设置频率计算定时器走时长度
			soft_pwm0_duty = (uint32)duty*Regular0_times_g/PWM0_PRECISON;//确定正占空比定时器走时（全局变量）
			pit_init(T0,Regular_times);//启用T0定时器
			set_vector_handler(TIMER0_VECTORn , PWM0_ONE_TIME_cycle);//将定时器T0中断的服务函数设定为PWM0_ONE_TIME_cycle()
			Whether_IRQ(TIMER0_VECTORn , IRQ_enable);//使能定时器T0中断
		}
		else if(use_Tn == PWM1_T1)//若使用PWM1
				 {
					 if(duty > PWM1_PRECISON)	duty = PWM1_PRECISON;
					 PWM1_out = pxn;//确定pwm输出引脚
					 Regular1_times_g = EXTAL_IN_Hz/(freq*(1+ACQUIRE(CLK_DIV,2,0)));//根据设置频率计算定时器走时长度
					 soft_pwm1_duty = (uint32)duty*Regular1_times_g/PWM1_PRECISON;//确定正占空比定时器走时（全局变量）
					 pit_init(T1,Regular_times);//启用T1定时器
					 set_vector_handler(TIMER1_VECTORn , PWM1_ONE_TIME_cycle);//将定时器T1中断的服务函数设定为PWM1_ONE_TIME_cycle()
					 Whether_IRQ(TIMER1_VECTORn , IRQ_enable);//使能定时器T1中断
				 }
				 else if(use_Tn == PWM2_T2)//若使用PWM2
							{
								if(duty > PWM2_PRECISON)	duty = PWM2_PRECISON;
								PWM2_out = pxn;//确定pwm输出引脚
								Regular2_times_g = EXTAL_IN_Hz/(freq*(1+ACQUIRE(CLK_DIV,2,0)));//根据设置频率计算定时器走时长度
								soft_pwm2_duty = (uint32)duty*Regular2_times_g/PWM2_PRECISON;//确定正占空比定时器走时（全局变量）�
								pit_init(T2,Regular_times);//启用T2定时器
								set_vector_handler(TIMER2_VECTORn , PWM2_ONE_TIME_cycle);//将定时器T2中断的服务函数设定为PWM2_ONE_TIME_cycle()
								Whether_IRQ(TIMER2_VECTORn , IRQ_enable);//使能定时器T2中断
							}
							else if(use_Tn == PWM3_T3)//若使用PWM3
									 {
										 if(duty > PWM3_PRECISON)	duty = PWM3_PRECISON;
										 PWM3_out = pxn;//确定pwm输出引脚
										 Regular3_times_g = EXTAL_IN_Hz/(freq*(1+ACQUIRE(CLK_DIV,2,0)));//根据设置频率计算定时器走时长度
										 soft_pwm3_duty = (uint32)duty*Regular3_times_g/PWM3_PRECISON;//确定正占空比定时器走时（全局变量）
										 pit_init(T3,Regular_times);//启用T3定时器
										 set_vector_handler(TIMER3_VECTORn , PWM3_ONE_TIME_cycle);//将定时器T3中断的服务函数设定为PWM3_ONE_TIME_cycle()
										 Whether_IRQ(TIMER3_VECTORn , IRQ_enable);//使能定时器T3中断
									 }
									 else if(use_Tn == PWM4_T4)//若使用PWM4
												{
													if(duty > PWM4_PRECISON)	duty = PWM4_PRECISON;
													PWM4_out = pxn;//确定pwm输出引脚
													Regular4_times_g = EXTAL_IN_Hz/(freq*(1+ACQUIRE(CLK_DIV,2,0)));//根据设置频率计算定时器走时长度
													soft_pwm4_duty = (uint32)duty*Regular4_times_g/PWM4_PRECISON;//确定正占空比定时器走时（全局变量）
													pit_init(T4,Regular_times);//启用T4定时器
													set_vector_handler(TIMER4_VECTORn , PWM4_ONE_TIME_cycle);//将定时器T4中断的服务函数设定为PWM4_ONE_TIME_cycle()
													Whether_IRQ(TIMER4_VECTORn , IRQ_enable);//使能定时器T4中断
												}
	}
	else //定时器计满所需要的周期小于设置频率的信号周期，脉宽调制将利用单步时间累加方式完成
	{
		if(use_Tn == PWM0_T0)//若使用PWM0
		{
			PWM0_out = pxn;//确定pwm输出引脚
			soft_pwm0_duty = duty;//确定占空比（全局变量）
			Regular_times = EXTAL_IN_Hz/((1+ACQUIRE(CLK_DIV,2,0))*freq*PWM0_PRECISON);//根据设置频率及精度计算定时器走时长度
			pit_init(T0,Regular_times);//启用T0定时器
			set_vector_handler(TIMER0_VECTORn , PWM0_UNIT_TIME_ACC);//将定时器T0中断的服务函数设定为PWM0_UNIT_TIME_ACC()
			Whether_IRQ(TIMER0_VECTORn , IRQ_enable);//使能定时器T0中断
		}
		else if(use_Tn == PWM1_T1)//若使用PWM1
				 {
					 PWM1_out = pxn;//确定pwm输出引脚
					 soft_pwm1_duty = duty;//确定占空比（全局变量）
					 Regular_times = EXTAL_IN_Hz/((1+ACQUIRE(CLK_DIV,2,0))*freq*PWM1_PRECISON);//根据设置频率及精度计算定时器走时长度
					 pit_init(T1,Regular_times);//启用T1定时器
					 set_vector_handler(TIMER1_VECTORn , PWM1_UNIT_TIME_ACC);//将定时器T1中断的服务函数设定为PWM1_UNIT_TIME_ACC()
					 Whether_IRQ(TIMER1_VECTORn , IRQ_enable);//使能定时器T1中断
				 }
				 else if(use_Tn == PWM2_T2)//若使用PWM2
							{
								PWM2_out = pxn;//确定pwm输出引脚
								soft_pwm2_duty = duty;//确定占空比（全局变量）
								Regular_times = EXTAL_IN_Hz/((1+ACQUIRE(CLK_DIV,2,0))*freq*PWM2_PRECISON);//根据设置频率及精度计算定时器走时长度
								pit_init(T2,Regular_times);//启用T2定时器
								set_vector_handler(TIMER2_VECTORn , PWM2_UNIT_TIME_ACC);//将定时器T2中断的服务函数设定为PWM2_UNIT_TIME_ACC()
								Whether_IRQ(TIMER2_VECTORn , IRQ_enable);//使能定时器T2中断
							}
							else if(use_Tn == PWM3_T3)//若使用PWM3
									 {
										 PWM3_out = pxn;//确定pwm输出引脚
										 soft_pwm3_duty = duty;//确定占空比（全局变量）
										 Regular_times = EXTAL_IN_Hz/((1+ACQUIRE(CLK_DIV,2,0))*freq*PWM3_PRECISON);//根据设置频率及精度计算定时器走时长度
										 pit_init(T3,Regular_times);//启用T3定时器
										 set_vector_handler(TIMER3_VECTORn , PWM3_UNIT_TIME_ACC);//将定时器T3中断的服务函数设定为PWM3_UNIT_TIME_ACC()
										 Whether_IRQ(TIMER3_VECTORn , IRQ_enable);//使能定时器T3中断
									 }
									 else if(use_Tn == PWM4_T4)//若使用PWM4
												{
													PWM4_out = pxn;//确定pwm输出引脚
													soft_pwm4_duty = duty;//确定占空比（全局变量）
													Regular_times = EXTAL_IN_Hz/((1+ACQUIRE(CLK_DIV,2,0))*freq*PWM4_PRECISON);//根据设置频率及精度计算定时器走时长度
													pit_init(T4,Regular_times);//启用T4定时器
													set_vector_handler(TIMER4_VECTORn , PWM4_UNIT_TIME_ACC);//将定时器T4中断的服务函数设定为PWM4_UNIT_TIME_ACC()
													Whether_IRQ(TIMER4_VECTORn , IRQ_enable);//使能定时器T4中断
												}
	}
}

/*!
 *	@brief		定时器中断服务函数群
							定时器计满所需要的周期大于设置频率的信号周期，脉宽调制方可在一次定时周期内完成
 *	@param		
 *	@warning	不可调用
 *	@sampleusage:	
 */

bit PWM0_STATE = 0,
		PWM1_STATE = 0,
		PWM2_STATE = 0,
		PWM3_STATE = 0,
		PWM4_STATE = 0;

void PWM0_ONE_TIME_cycle(void)
{
	PWM0_STATE = ~PWM0_STATE;
	if(soft_pwm0_duty < 410) gpio_set(PWM0_out , 0);
	else if(410 > Regular0_times_g - soft_pwm0_duty)	gpio_set(PWM0_out , 1);
			 else if(PWM0_STATE)
						{
							gpio_set(PWM0_out , 0);
							pit_change(T0,soft_pwm0_duty);
						}
						else
						{
							gpio_set(PWM0_out , 1);
							pit_change(T0,Regular0_times_g - soft_pwm0_duty);
						}
}

void PWM1_ONE_TIME_cycle(void)
{
	PWM1_STATE = ~PWM1_STATE;
	if(soft_pwm1_duty < 410) gpio_set(PWM1_out , 0);
	else if(410 > Regular1_times_g - soft_pwm1_duty)	gpio_set(PWM1_out , 1);
			 else if(PWM1_STATE)
						{
							gpio_set(PWM1_out , 0);
							pit_change(T1,soft_pwm1_duty);
						}
						else
						{
							gpio_set(PWM1_out , 1);
							pit_change(T1,Regular1_times_g - soft_pwm1_duty);
						}
}

void PWM2_ONE_TIME_cycle(void)
{
	PWM2_STATE = ~PWM2_STATE;
	if(soft_pwm2_duty < 410) gpio_set(PWM2_out , 0);
	else if(410 > Regular2_times_g - soft_pwm2_duty)	gpio_set(PWM2_out , 1);
			 else if(PWM2_STATE)
						{
							gpio_set(PWM2_out , 0);
							pit_change(T2,soft_pwm2_duty);
						}
						else
						{
							gpio_set(PWM2_out , 1);
							pit_change(T2,Regular2_times_g - soft_pwm2_duty);
						}
}

void PWM3_ONE_TIME_cycle(void)
{
	PWM3_STATE = ~PWM3_STATE;
	if(soft_pwm3_duty < 410) gpio_set(PWM3_out , 0);
	else if(410 > Regular3_times_g - soft_pwm3_duty)	gpio_set(PWM3_out , 1);
			 else if(PWM3_STATE)
						{
							gpio_set(PWM3_out , 0);
							pit_change(T3,soft_pwm3_duty);
						}
						else
						{
							gpio_set(PWM3_out , 1);
							pit_change(T3,Regular3_times_g - soft_pwm3_duty);
						}
}

void PWM4_ONE_TIME_cycle(void)
{
	PWM4_STATE = ~PWM4_STATE;
	if(soft_pwm4_duty < 410) gpio_set(PWM4_out , 0);
	else if(410 > Regular4_times_g - soft_pwm4_duty)	gpio_set(PWM4_out , 1);
			 else if(PWM4_STATE)
						{
							gpio_set(PWM4_out , 0);
							pit_change(T4,soft_pwm4_duty);
						}
						else
						{
							gpio_set(PWM4_out , 1);
							pit_change(T4,Regular4_times_g - soft_pwm4_duty);
						}
}

/*!
 *	@brief		定时器中断服务函数群
							定时器计满所需要的周期小于设置频率的信号周期，脉宽调制将利用单步时间累加方式完成
 *	@param		
 *	@warning	不可调用
 *	@sampleusage:	
 */

uint16 data PWM0_UNIT_TIME_NUM=0 , PWM1_UNIT_TIME_NUM=0 , PWM2_UNIT_TIME_NUM=0,
			      PWM3_UNIT_TIME_NUM=0 , PWM4_UNIT_TIME_NUM=0;

void PWM0_UNIT_TIME_ACC(void)
{
	PWM0_UNIT_TIME_NUM++;
	if(PWM0_UNIT_TIME_NUM >  PWM0_PRECISON)	PWM0_UNIT_TIME_NUM = 0;
	if(PWM0_UNIT_TIME_NUM >= soft_pwm0_duty)	gpio_set(PWM0_out , 0);
	else gpio_set(PWM0_out , 1);
}

void PWM1_UNIT_TIME_ACC(void)
{
	PWM1_UNIT_TIME_NUM++;
	if(PWM1_UNIT_TIME_NUM >  PWM1_PRECISON)	PWM1_UNIT_TIME_NUM = 0;
	if(PWM1_UNIT_TIME_NUM >= soft_pwm1_duty)	gpio_set(PWM1_out , 0);
	else gpio_set(PWM1_out , 1);
}

void PWM2_UNIT_TIME_ACC(void)
{
	PWM2_UNIT_TIME_NUM++;
	if(PWM2_UNIT_TIME_NUM >  PWM2_PRECISON)	PWM2_UNIT_TIME_NUM = 0;
	if(PWM2_UNIT_TIME_NUM >= soft_pwm2_duty)	gpio_set(PWM2_out , 0);
	else gpio_set(PWM2_out , 1);
}

void PWM3_UNIT_TIME_ACC(void)
{
	PWM3_UNIT_TIME_NUM++;
	if(PWM3_UNIT_TIME_NUM >  PWM3_PRECISON)	PWM3_UNIT_TIME_NUM = 0;
	if(PWM3_UNIT_TIME_NUM >= soft_pwm3_duty)	gpio_set(PWM3_out , 0);
	else gpio_set(PWM3_out , 1);
}

void PWM4_UNIT_TIME_ACC(void)
{
	PWM4_UNIT_TIME_NUM++;
	if(PWM4_UNIT_TIME_NUM >  PWM4_PRECISON)	PWM4_UNIT_TIME_NUM = 0;
	if(PWM4_UNIT_TIME_NUM >= soft_pwm4_duty)	gpio_set(PWM4_out , 0);
	else gpio_set(PWM4_out , 1);
}