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
volatile int16_t targetSpeed = 15;

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

    /* �û�����  */
    char txt[32];
    char txt1[32];

    /* ����״̬  �˲��õ�  */
    uint8_t keyStatus = 0;

    /* VL53 ���ʹ�ñ���  */
    unsigned char vl53Dis_buff[2];
    uint16 vl53Dis = 0, vL53Last_dis = 0;

    /* �������������ʼ�� */
	MotorInit();
	EncInit();

	/* ��ʱ����������ʼ�� */
	CCU6_InitConfig(CCU61, CCU6_Channel0, 5000);
	GPIO_KEY_Init();

	/* VL53ʹ�õ�IIC�ӿڳ�ʼ�� */
	IIC_Init();

    /* ����ͷ��ʼ�� */
    //CAMERA_Init(50);

	/* VL53��ʼ��� */
	VL53_Write_Byte(VL53ADDR, VL53L0X_REG_SYSRANGE_START, 0x02);



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
					  targetSpeed += 2;
				  }
				  keyStatus = 1;
			  break;
			  case KEY1DOWN:
				  if(keyStatus != 2)
				  {
					  targetSpeed = 15;
				  }
				  keyStatus = 2;
			  break;
			  case KEY2DOWN:
				  if(keyStatus != 3)
				  {
					  targetSpeed -= 2;
				  }
				  keyStatus = 3;
			  break;

			  default :
				  keyStatus = 0;
				  break;
			 }

			/* 50ms ��ȡһ�� VL53���� */
			if(cpu1Flage50ms >= 10)
			{
				cpu1Flage50ms = 0;

				/* ��ȡ�������� */
				VL53_Read_nByte(VL53ADDR, VL53_REG_DIS, 2, vl53Dis_buff);

				/* ת������ */
				vl53Dis = (vl53Dis_buff[0]<<8) | (vl53Dis_buff[1]);

				/* ���β���������� ��ʹ���ϴ�����  */
				if(vl53Dis == 20)
				{
					vl53Dis = vL53Last_dis;
				}

				vL53Last_dis = vl53Dis;

				static uint8_t vl53Count = 0;

				/* С��ǰ15cm�����ϰ�  ��λ���ϱ�־λ  */
				if(vl53Dis < 150)

				{
					vl53Count++;
					if(vl53Count > 10)
					{
						evadibleFlage = 1;
					}

				}
				else
				{
					vl53Count = 0;
					evadibleFlage = 0;
				}

				if(IfxCpu_acquireMutex(&mutexTFTIsOk))
				{
					sprintf(txt,"%-5d", vl53Dis);//��ʾ����
					TFTSPI_P8X16Str(0, 3, "VL-53:", u16WHITE, u16BLUE);
					TFTSPI_P8X16Str(6, 3, txt, u16WHITE, u16BLUE);
					sprintf(txt1, "expect: %d", targetSpeed);             //�����ٶ�
					TFTSPI_P8X16Str(0, 4, txt1, u16WHITE, u16BLUE);		//�ַ�����ʾ
					//TFTSPI_P8X8Str(15,15, txt,u16RED,u16BLUE);
					IfxCpu_releaseMutex(&mutexTFTIsOk);
				}


			}



//			sprintf(txt, "angle: %-5.2f", g_Angle);             //��ʾ�Ƕ���Ϣ
//			TFTSPI_P8X8Str(0, 15, txt, u16WHITE, u16BLUE);		//�ַ�����ʾ


			/* ͼ����  */
//			ImageProcess();


    	}



    }

}
