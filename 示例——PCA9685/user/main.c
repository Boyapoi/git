#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "stm32_pca9685.h"







int main(void)
{	
	delay_init();//��ʱ������ʼ��
	PCA_MG9XX_Init(60,90);//��ʼ���������
	
	
	
while(1)
{		
		PCA_MG9XX(1,0,180,1,10); 
		PCA_MG9XX(2,0,180,1,10); 
		PCA_MG9XX(1,180,0,1,10); 
		PCA_MG9XX(2,180,0,1,10); 
}
	
	

	/*
while(1) //ֱ�ӿ���led,
{
	for(int i=0;i<4096;i++)
	{
	  pca_setpwm(7,0,i);delay_ms(1);//��ͨ��7ֱ�ӿ���Led����
	}
}*/



}
 

