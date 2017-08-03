//-------------------------------------------------------------------------
// 文件名：RTOS.H
// 功能：  ARM9上运行的微型操作系统(头文件)
// 备注：
//	    
//
// 		   By Lisuwei
//-------------------------------------------------------------------------

#ifndef  _RTOS_H_
#define  _RTOS_H_

#ifdef   CPP_GLOBALS
#define  EXTERN
#else
#define  EXTERN  extern
#endif

///******************系统配置***************///

#define StackSizeIdle     512		 /* 定义空闲任务栈大小                 */
#define OS_TICKS_PER_SEC  100        /* 设置一秒内的时钟节拍数             */
#define OS_TASKS		  4			 /* 设定运行任务的数量,不包含空闲任务  */


///******************结构体类型声明***************///

 struct TaskCtrBlock            	 /* 任务控制块数据结构 */
{
	INT32U	OSTCBStkPtr;			 /* 保存任务的堆栈顶   */
	INT32U	OSTCBDly;				 /* 任务延时时钟	   */
};


///******************全局变量定义***************///

EXTERN  INT32U  StackIdle[StackSizeIdle]; 			/* 建立空闲任务栈   		    */
EXTERN  INT8U   OSRdyTbl;     						/* 就绪任务表					*/
EXTERN  INT8U   OSPrioCur;		            		/* 正在运行的任务				*/
EXTERN  INT8U   OSPrioHighRdy	;           		/* 更高优先级的任务				*/
EXTERN  INT32U  Int_Return_Addr_Save;				/* 保存中断后的返回地址			*/	
EXTERN  BOOLEAN OSIntCtxSwFlag;						/* 中断退出切换任务标志			*/
EXTERN  INT32U  cpu_sr;								/* 进入临界代码区时保存CPU状态	*/
EXTERN  INT32U  OSTime;								/* 记录系统运行时间				*/
EXTERN  INT32U  IdleCount;							/* 空闲任务运行记录				*/
EXTERN  INT32U  OSNextTaskPrio;						/* 寻找更高优先级的就绪任务时用 */
EXTERN  struct TaskCtrBlock	 TCB[OS_TASKS + 1];		/* 定义任务控制块				*/
EXTERN  struct TaskCtrBlock  *p_OSTCBCur ;			/* 指向当前任务控制块的指针		*/
EXTERN  struct TaskCtrBlock  *p_OSTCBHighRdy ;		/* 指向更高优先级任务控制块的指针*/


///******************宏定义代码***************///

/* 进入临界代码区 */
#define  OS_ENTER_CRITICAL()  (cpu_sr = OSCPUSaveSR()) /* 关总中断 */
/* 退出临界代码区 */
#define  OS_EXIT_CRITICAL()   (OSCPURestoreSR(cpu_sr)) /* 恢复原来中断状态 */

/* 在就绪表中登记就绪任务 */
#define OSSetPrioRdy(prio)             			  \
{											      \
OSRdyTbl |= 0x01<<prio;	                          \
}		                                          
							          
/* 从就绪表中删除任务 */
#define OSDelPrioRdy(prio)             			  \
{												  \
OSRdyTbl &= ~(0x01<<prio); 						  \
}														   


/* 在就绪表中查找更高级的就绪任务 */
#define OSGetHighRdy()															\
{																				\
	for( OSNextTaskPrio = 0; 													\
	    (OSNextTaskPrio < OS_TASKS) && (!(OSRdyTbl & (0x01<<OSNextTaskPrio))); 	\
	    OSNextTaskPrio ++ );													\
	OSPrioHighRdy = OSNextTaskPrio;												\
}																														
	
	
///******************系统函数声明***************

void OSTaskCreate(void (*Task)(void),INT32U *Stack,INT8U TaskID);
void OSStart(void);
void OSTaskResume(INT8U prio);
void OSTaskSuspend(INT8U prio);
void OSSched(void);
void OSTimeDly(INT32U ticks);
void IdleTask(void);
void OSInit(void);
void OSIntExit(void);
void TickInterrupt(void);

/*调用的汇编函数声明*/
INT32U OSCPUSaveSR(void);
void OSCPURestoreSR(INT32U cpu_sr);
void OSStartHighRdy(void);
void OS_TASK_SW(void);

#endif
