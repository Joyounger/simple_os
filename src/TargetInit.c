//-------------------------------------------------------------------------
// 文件名：TargetInit.C
// 功能：  初始化系统函数
// 备注：  ARM9(2440)上运行的微型操作系统
//		   2410只需改SetCPUFre()及它的相关函数 
//-------------------------------------------------------------------------


#define	  GLOBAL_CLK		1
#include "Includes.h" 

/*调用的汇编函数声明*/
void ASM_IRQHandler(void);

/************************************************************************************************************************
函数名称：	Isr_Init
函数原型：  void Isr_Init(void)
函数功能：	初始化中断
入口参数：	无
出口参数：	无
有关说明：	
************************************************************************************************************************/
void Isr_Init(void)
{
	
	rINTMOD = 0x0;	  	         // All=IRQ mode
	rINTMSK = BIT_ALLMSK;	     // All interrupt is masked.
    rINTSUBMSK = BIT_SUB_ALLMSK; // All sub-interrupt is masked. <- April 01, 2002 SOP
}

/************************************************************************************************************************
函数名称：	cal_cpu_bus_clk
函数原型：	static void cal_cpu_bus_clk(void)
函数功能：	
入口参数：	无
出口参数：	无
有关说明：	
************************************************************************************************************************/
static void cal_cpu_bus_clk(void)
{
	U32 val,UPLL;
	U8 m, p, s;
	
	val = rMPLLCON;
	m = (val>>12)&0xff;
	p = (val>>4)&0x3f;
	s = val&3;

	//(m+8)*FIN*2 不要超出32位数!
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
函数名称：	SetCPUFre
函数原型：	void SetCPUFre(void)
函数功能：	初始化系统时钟
入口参数：	无
出口参数：	无
有关说明：	
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
函数名称：	StartTicker
函数原型：	void StartTicker(INT32U TicksPerSec)
函数功能：	初始化定时器0
入口参数：	系统每秒中断节拍数
出口参数：	无
有关说明：	定时器0用作系统心跳，产生时钟节拍
 			Timer input clock Frequency = PCLK / {prescaler value + 1} / {divider value}
 			{prescaler value} = 0~255
 			{divider value} = 2, 4, 8, 16
 			Timer0的时钟频率=50MHz/(99+1)/(16)=31250Hz
*************************************************************************/
void StartTicker(INT32U TicksPerSec)
{
	rTCFG0 = 99;					//Prescaler0 = 99	  	 
	rTCFG1 = 0x03;					//Select MUX input for PWM Timer0:divider=16
	rTCNTB0 = 31250 / TicksPerSec;	//设置中断频率
	rTCON |=  (1<<1);				//Timer 0 manual update
	rTCON = 0x09;					//Timer 0 auto reload on
			 						//Timer 0 output inverter off
			  						//清"Timer 0 manual update"
			 						//Timer 0 start */
	BIT_CLR(rINTMSK, BIT_TIMER0); 	// Enable WatchDog interrupts
	
}
	

/************************************************************************************************************************
函数名称：	TargetInit
函数原型：	void TargetInit(void)
函数功能：	初始化目标板
入口参数：	无
出口参数：	无
有关说明：		
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
函数名称：	TargetStart
函数原型：	void TargetStart(void)
函数功能：	设置中断向量，启动操作系统的硬件定时器中断
入口参数：	无
出口参数：	无
有关说明：		
************************************************************************************************************************/
void TargetStart(void)				
{

	 pISR_TIMER0 = (INT32U)TickInterrupt;
	 StartTicker(OS_TICKS_PER_SEC);   
}


