#ifndef __STM32PCA9685_H
#define __STM32PCA9685_H	

#include "stm32f10x.h"


#define pca_adrr 0x80

#define pca_mode1 0x0
#define pca_pre 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define jdMIN  115 // minimum
#define jdMAX  590 // maximum
#define jd000  130 //0�ȶ�Ӧ4096���������ֵ
#define jd180  520 //180�ȶ�Ӧ4096���������ֵ



//IO��������
 
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO��������	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7 )  //����SDA 





void pca_write(u8 adrr,u8 data);
u8 pca_read(u8 adrr);
void PCA_MG9XX_Init(float hz,u8 angle);
void pca_setfreq(float freq);
void pca_setpwm(u8 num, u32 on, u32 off);
void PCA_MG9XX(u8 num,u8 start_angle,u8 end_angle,u8 mode,u8 speed);




//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�


#endif









