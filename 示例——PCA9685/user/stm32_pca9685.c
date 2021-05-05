#include "stm32_pca9685.h"
#include "delay.h"
#include "math.h"



void pca_write(u8 adrr,u8 data)//��PCAд����,adrrd��ַ,data����
{ 
	IIC_Start();
	
	IIC_Send_Byte(pca_adrr);
	IIC_Wait_Ack();
	
	IIC_Send_Byte(adrr);
	IIC_Wait_Ack();
	
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	
	IIC_Stop();
}

u8 pca_read(u8 adrr)//��PCA������
{
	u8 data;
	IIC_Start();
	
	IIC_Send_Byte(pca_adrr);
	IIC_Wait_Ack();
	
	IIC_Send_Byte(adrr);
	IIC_Wait_Ack();
	
	IIC_Start();
	
	IIC_Send_Byte(pca_adrr|0x01);
	IIC_Wait_Ack();
	
	data=IIC_Read_Byte(0);
	IIC_Stop();
	
	return data;
}


void pca_setfreq(float freq)//����PWMƵ��
{
		u8 prescale,oldmode,newmode;
		double prescaleval;
		freq *= 0.92; 
		prescaleval = 25000000;
		prescaleval /= 4096;
		prescaleval /= freq;
		prescaleval -= 1;
		prescale =floor(prescaleval + 0.5f);

		oldmode = pca_read(pca_mode1);
	
		newmode = (oldmode&0x7F) | 0x10; // sleep
	
		pca_write(pca_mode1, newmode); // go to sleep
	
		pca_write(pca_pre, prescale); // set the prescaler
	
		pca_write(pca_mode1, oldmode);
		delay_ms(2);
	
		pca_write(pca_mode1, oldmode | 0xa1); 
}

void pca_setpwm(u8 num, u32 on, u32 off)
{
		pca_write(LED0_ON_L+4*num,on);
		pca_write(LED0_ON_H+4*num,on>>8);
		pca_write(LED0_OFF_L+4*num,off);
		pca_write(LED0_OFF_H+4*num,off>>8);
}
/*num:���PWM�������0~15��on:PWM��������ֵ0~4096,off:PWM�½�����ֵ0~4096
һ��PWM���ڷֳ�4096�ݣ���0��ʼ+1�������Ƶ�onʱ����Ϊ�ߵ�ƽ������������offʱ
����Ϊ�͵�ƽ��ֱ������4096���¿�ʼ�����Ե�on������0ʱ������ʱ,��on����0ʱ��
off/4096��ֵ����PWM��ռ�ձȡ�*/

/*
	�������ã���ʼ�����������
	������1.PWMƵ��
		  2.��ʼ������Ƕ�
*/
void PCA_MG9XX_Init(float hz,u8 angle)
{
	u32 off=0;
	IIC_Init();
	pca_write(pca_mode1,0x0);
	pca_setfreq(hz);//����PWMƵ��
	off=(u32)(145+angle*2.4);
	pca_setpwm(0,0,off);pca_setpwm(1,0,off);pca_setpwm(2,0,off);pca_setpwm(3,0,off);
	pca_setpwm(4,0,off);pca_setpwm(5,0,off);pca_setpwm(6,0,off);pca_setpwm(7,0,off);
	pca_setpwm(8,0,off);pca_setpwm(9,0,off);pca_setpwm(10,0,off);pca_setpwm(11,0,off);
	pca_setpwm(12,0,off);pca_setpwm(13,0,off);pca_setpwm(14,0,off);pca_setpwm(15,0,off);
	delay_ms(500);
}

/*
	�������ã����ƶ��ת����
	������1.����˿ڣ���ѡ0~15��
		  2.��ʼ�Ƕȣ���ѡ0~180��
		  3.�����Ƕȣ���ѡ0~180��
		  4.ģʽѡ��0 ��ʾ����������ʱ������ʱ��Ҫ�ں������������ʱ�������Ҳ��ɵ��٣������������������ֵ��
					  1 ��ʾ����������ʱ������ʱ����Ҫ�ں������������ʱ�������Ҳ��ɵ��٣������������������ֵ��
					  2 ��ʾ�ٶȿɵ��������������ʾ�ٶ�ֵ��
		  5.�ٶȣ�������� 0 ������ֵ���� 1 ʱ�ٶ���죬��ֵԽ���ٶ�ԽС��
	ע�����ģʽ 0��1 ���ٶȱ�ģʽ 2 ������ٶȴ�
*/
void PCA_MG9XX(u8 num,u8 start_angle,u8 end_angle,u8 mode,u8 speed)
{
	u8 i;
	u32 off=0;
	switch(mode)
	{
		case 0:
			off=(u32)(158+end_angle*2.2);
			pca_setpwm(num,0,off);
			break;
		case 1:
			off=(u32)(158+end_angle*2.2);
			pca_setpwm(num,0,off);
			if(end_angle>start_angle){delay_ms((u16)((end_angle-start_angle)*2.7));}
			else{delay_ms((u16)((start_angle-end_angle)*2.7));}
			break;
		case 2:
			if(end_angle>start_angle)
			{
				for(i=start_angle;i<=end_angle;i++)
				{
					off=(u32)(158+i*2.2);
					pca_setpwm(num,0,off);
					delay_ms(2);
					delay_us(speed*250);
				}
			}
			else if(start_angle>end_angle)
			{
				for(i=start_angle;i>=end_angle;i--)
				{
					off=(u32)(158+i*2.2);
					pca_setpwm(num,0,off);
					delay_ms(2);
					delay_us(speed*250);
				}
			}
			break;
	}
}


//----------------
//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9); 	//PB6,PB7 �����
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�                                                                                                                                            
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

