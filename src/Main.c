//-------------------------------------------------------------------------
// 文件名：MAIN.C
// 功能：  测试ARM上运行的微型操作系统
// 备注：  
//		By Lisuwei
//-------------------------------------------------------------------------
#include "Includes.h" 

///******************任务定义***************///
void Task0	(void);
void Task1	(void);
void Task2	(void);
void Task3	(void);

///**************定义任务优先级*************///
#define PrioTask0   		 0
#define PrioTask1        	 1
#define PrioTask2        	 2
#define PrioTask3        	 3

///******************任务堆栈大小定义***********///
#define StackSizeTask0       512
#define StackSizeTask1       512
#define StackSizeTask2       512
#define StackSizeTask3       512

///******************建立任务堆栈***************///
INT32U StackTask0[StackSizeTask0];				
INT32U StackTask1[StackSizeTask1];				
INT32U StackTask2[StackSizeTask2];			   
INT32U StackTask3[StackSizeTask3];

///******************汇编函数声明***************///
void OS_TASK_SW(void);

///******************全局变量***************///


/************************************************************************************************************************
函数名称：  main
函数原型：	int main(void)
函数功能：	主函数
入口参数：	无
出口参数：	无
有关说明：	
************************************************************************************************************************/
int Main(void)
{
	TargetInit();  //初始化目标板
	OSInit();      //初始化操作系统   
	
	OSTaskCreate(Task0,&StackTask0[StackSizeTask0 - 1],PrioTask0); // 创建一个任务
	Uart_Printf("Ready to start OS\n");
	OSStart(); 	   //运行操作系统
	
	return 0;      //程序不会运行至此 
}



/************************************************************************************************************************
函数名称：  Task0
函数原型：	void Task0(void)
函数功能：	任务0
入口参数：	无
出口参数：	无
有关说明：		
************************************************************************************************************************/
void Task0(void)
{
	TargetStart();	//设置中断向量，启动操作系统的硬件定时器中断
	Uart_Printf("Start OS\n");
	// 创建其他任务
	OSTaskCreate(Task1,&StackTask1[StackSizeTask1 - 1],PrioTask1);
	OSTaskCreate(Task2,&StackTask2[StackSizeTask2 - 1],PrioTask2);
	OSTaskCreate(Task3,&StackTask3[StackSizeTask2 - 1],PrioTask3);
	
	while(1)
	{
		OS_ENTER_CRITICAL();
		Uart_Printf("Task0\n");
		OS_EXIT_CRITICAL();
		OSTimeDly(100);		//1秒运行一次
	}
}

/************************************************************************************************************************
函数名称：	Task1
函数原型：	void Task1(void)
函数功能：	任务1
入口参数：	无
出口参数：	无
有关说明：		
************************************************************************************************************************/
void Task1(void)
{
	
	while(1)
	{
		OS_ENTER_CRITICAL();
		Uart_Printf("Task1\n");
		OS_EXIT_CRITICAL();
		OSTimeDly(300);	//3秒运行一次
	}
}

/************************************************************************************************************************
函数名称：	Task2
函数原型：	void Task2(void)
函数功能：	任务2
入口参数：	无
出口参数：	无
有关说明：	
************************************************************************************************************************/
void Task2(void)
{

	while(1)
	{	
		OS_ENTER_CRITICAL();
		Uart_Printf("Task2\n");
		OS_EXIT_CRITICAL();
		OSTaskSuspend(PrioTask2);  // 使自己进入挂起状态 
	}
}


/************************************************************************************************************************
函数名称：	Task3
函数原型：	void Task3(void)
函数功能：	任务3
入口参数：	无
出口参数：	无
有关说明：	
************************************************************************************************************************/
void Task3(void)
{

	while(1)
	{	
		OS_ENTER_CRITICAL();
		Uart_Printf("Resume Task2\n");
		OS_EXIT_CRITICAL();
	    OSTaskResume(PrioTask2);     // 恢复任务2 
		OSTimeDly(800);
	}
}


