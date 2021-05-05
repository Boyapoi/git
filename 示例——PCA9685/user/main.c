#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "stm32_pca9685.h"







int main(void)
{	
	delay_init();//延时函数初始化
	PCA_MG9XX_Init(60,90);//初始化舵机驱动
	
	
	
while(1)
{		
		PCA_MG9XX(1,0,180,1,10); 
		PCA_MG9XX(2,0,180,1,10); 
		PCA_MG9XX(1,180,0,1,10); 
		PCA_MG9XX(2,180,0,1,10); 
}
	
	

	/*
while(1) //直接控制led,
{
	for(int i=0;i<4096;i++)
	{
	  pca_setpwm(7,0,i);delay_ms(1);//用通道7直接控制Led亮度
	}
}*/



}
 

