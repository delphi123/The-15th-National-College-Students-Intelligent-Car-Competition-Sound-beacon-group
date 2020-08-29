/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��zyf/chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��4��10��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��Hightec4.9.3/Tasking6.3�����ϰ汾
��Target �� TC264DA
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
����iLLD_1_0_1_11_0�ײ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <IfxCpu.h>
#include <IfxScuWdt.h>
#include <LQ_CAMERA.h>
#include <LQ_CCU6.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_I2C_VL53.h>
#include <LQ_ImageProcess.h>
#include <LQ_MotorControl.h>
#include <LQ_SOFTI2C.h>
#include <LQ_TFT18.h>
#include <LQ_VoiceProcess.h>
#include <Main.h>
#include <Platform_Types.h>
#include <stdint.h>
#include <stdio.h>

IfxCpu_mutexLock mutexTFTIsOk = 0;   /** TFT18ʹ�ñ�־λ  */

/**
 * ��ʱ�� 5ms��50ms��־λ
 */
volatile uint8_t cpu1Flage5ms = 0;
volatile uint8_t cpu1Flage50ms = 0;

/* �����ٶ�  */
volatile int16_t targetSpeed = 10;

/* ���ϱ�־λ */
volatile uint8_t evadibleFlage = 0;

int core1_main (void)
{

    IfxCpu_enableInterrupts();
    /*
     * �رտ��Ź�
     * */
    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());

    //�ȴ�CPU0 ��ʼ�����
    while(!IfxCpu_acquireMutex(&mutexCpu0InitIsOk));


    /* ����״̬  �˲��õ�  */
    uint8_t keyStatus = 0;


    /* �������������ʼ�� */
	MotorInit();
	EncInit();

	/* ��ʱ����������ʼ�� */
	CCU6_InitConfig(CCU61, CCU6_Channel0, 5000);
	GPIO_KEY_Init();


    while(1)//��ѭ��
    {
    	if(cpu1Flage5ms)
    	{
    		cpu1Flage5ms = 0;

    		/* ���� ���������ٶ�  */
			switch(KEY_Read_All())
			{
			  case KEY0DOWN:
				  if(keyStatus != 1)
				  {
					  targetSpeed += 5;
				  }
				  keyStatus = 1;
			  break;
			  case KEY1DOWN:
				  if(keyStatus != 2)
				  {
					  targetSpeed = 0;
				  }
				  keyStatus = 2;
			  break;
			  case KEY2DOWN:
				  if(keyStatus != 3)
				  {
					  targetSpeed -= 5;
				  }
				  keyStatus = 3;
			  break;

			  default :
				  keyStatus = 0;
				  break;
			 }

			/* ����PID */
			SpeedCtrl(0, targetSpeed, 0);




    	}



    }

}
