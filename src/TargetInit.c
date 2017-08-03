//-------------------------------------------------------------------------
// �ļ�����TargetInit.C
// ���ܣ�  ��ʼ��ϵͳ����
// ��ע��  ARM9(2440)�����е�΢�Ͳ���ϵͳ
//		   2410ֻ���SetCPUFre()��������غ��� 
//-------------------------------------------------------------------------


#define	  GLOBAL_CLK		1
#include "Includes.h" 

/*���õĻ�ຯ������*/
void ASM_IRQHandler(void);

/************************************************************************************************************************
�������ƣ�	Isr_Init
����ԭ�ͣ�  void Isr_Init(void)
�������ܣ�	��ʼ���ж�
��ڲ�����	��
���ڲ�����	��
�й�˵����	
************************************************************************************************************************/
void Isr_Init(void)
{
	
	rINTMOD = 0x0;	  	         // All=IRQ mode
	rINTMSK = BIT_ALLMSK;	     // All interrupt is masked.
    rINTSUBMSK = BIT_SUB_ALLMSK; // All sub-interrupt is masked. <- April 01, 2002 SOP
}

/************************************************************************************************************************
�������ƣ�	cal_cpu_bus_clk
����ԭ�ͣ�	static void cal_cpu_bus_clk(void)
�������ܣ�	
��ڲ�����	��
���ڲ�����	��
�й�˵����	
************************************************************************************************************************/
static void cal_cpu_bus_clk(void)
{
	U32 val,UPLL;
	U8 m, p, s;
	
	val = rMPLLCON;
	m = (val>>12)&0xff;
	p = (val>>4)&0x3f;
	s = val&3;

	//(m+8)*FIN*2 ��Ҫ����32λ��!
	FCLK = ((m+8)*(FIN/100)*2)/((p+2)*(1<<s))*100;
	
	val = rCLKDIVN;
	m = (val>>1)&3;
	p = val&1;	
	val = rCAMDIVN;
	s = val>>8;
	
	switch (m) {
	case 0:
		HCLK = FCLK;
		break;
	case 1:
		HCLK = FCLK>>1;
		break;
	case 2:
		if(s&2)
			HCLK = FCLK>>3;
		else
			HCLK = FCLK>>2;
		break;
	case 3:
		if(s&1)
			HCLK = FCLK/6;
		else
			HCLK = FCLK/3;
		break;
	}
	
	if(p)
		PCLK = HCLK>>1;
	else
		PCLK = HCLK;
	
	val = rUPLLCON;
	m = (val>>12)&0xff;
	p = (val>>4)&0x3f;
	s = val&3;
	UPLL = ((m+8)*FIN)/((p+2)*(1<<s));
	UCLK = (rCLKDIVN&8)?(UPLL>>1):UPLL;
}

/************************************************************************************************************************
�������ƣ�	SetCPUFre
����ԭ�ͣ�	void SetCPUFre(void)
�������ܣ�	��ʼ��ϵͳʱ��
��ڲ�����	��
���ڲ�����	��
�й�˵����	
************************************************************************************************************************/
void SetCPUFre(void)
{
	U8 i;
	U8 key;
	U32 mpll_val = 0;
    
	i = 0 ;	//don't use 100M!
		//boot_params.cpu_clk.val = 3;
	switch ( i ) {
	case 0:	//200
		key = 12;
		mpll_val = (92<<12)|(4<<4)|(1);
		break;
	case 1:	//300
		key = 13;
		mpll_val = (67<<12)|(1<<4)|(1);
		break;
	case 2:	//400
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
	case 3:	//440!!!
		key = 14;
		mpll_val = (102<<12)|(1<<4)|(1);
		break;
	default:
		key = 14;
		mpll_val = (92<<12)|(1<<4)|(1);
		break;
			}
	ChangeMPllValue((mpll_val>>12)&0xff, (mpll_val>>4)&0x3f, mpll_val&3);
	ChangeClockDivider(key, 12);	
    cal_cpu_bus_clk();
}


/*************************************************************************
�������ƣ�	StartTicker
����ԭ�ͣ�	void StartTicker(INT32U TicksPerSec)
�������ܣ�	��ʼ����ʱ��0
��ڲ�����	ϵͳÿ���жϽ�����
���ڲ�����	��
�й�˵����	��ʱ��0����ϵͳ����������ʱ�ӽ���
 			Timer input clock Frequency = PCLK / {prescaler value + 1} / {divider value}
 			{prescaler value} = 0~255
 			{divider value} = 2, 4, 8, 16
 			Timer0��ʱ��Ƶ��=50MHz/(99+1)/(16)=31250Hz
*************************************************************************/
void StartTicker(INT32U TicksPerSec)
{
	rTCFG0 = 99;					//Prescaler0 = 99	  	 
	rTCFG1 = 0x03;					//Select MUX input for PWM Timer0:divider=16
	rTCNTB0 = 31250 / TicksPerSec;	//�����ж�Ƶ��
	rTCON |=  (1<<1);				//Timer 0 manual update
	rTCON = 0x09;					//Timer 0 auto reload on
			 						//Timer 0 output inverter off
			  						//��"Timer 0 manual update"
			 						//Timer 0 start */
	BIT_CLR(rINTMSK, BIT_TIMER0); 	// Enable WatchDog interrupts
	
}
	

/************************************************************************************************************************
�������ƣ�	TargetInit
����ԭ�ͣ�	void TargetInit(void)
�������ܣ�	��ʼ��Ŀ���
��ڲ�����	��
���ڲ�����	��
�й�˵����		
************************************************************************************************************************/
void TargetInit(void)				
{
	SetCPUFre();
	Port_Init();
	Isr_Init();
	Uart_Init( 0,115200 );
	Uart_Select(0);
	pISR_IRQ = (INT32U)ASM_IRQHandler;	
	
	Uart_Printf("Board is running OK\n");		
}
	
	
/************************************************************************************************************************
�������ƣ�	TargetStart
����ԭ�ͣ�	void TargetStart(void)
�������ܣ�	�����ж���������������ϵͳ��Ӳ����ʱ���ж�
��ڲ�����	��
���ڲ�����	��
�й�˵����		
************************************************************************************************************************/
void TargetStart(void)				
{

	 pISR_TIMER0 = (INT32U)TickInterrupt;
	 StartTicker(OS_TICKS_PER_SEC);   
}


