//-------------------------------------------------------------------------
// �ļ�����RTOS.H
// ���ܣ�  ARM9�����е�΢�Ͳ���ϵͳ(ͷ�ļ�)
// ��ע��
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

///******************ϵͳ����***************///

#define StackSizeIdle     512		 /* �����������ջ��С                 */
#define OS_TICKS_PER_SEC  100        /* ����һ���ڵ�ʱ�ӽ�����             */
#define OS_TASKS		  4			 /* �趨�������������,��������������  */


///******************�ṹ����������***************///

 struct TaskCtrBlock            	 /* ������ƿ����ݽṹ */
{
	INT32U	OSTCBStkPtr;			 /* ��������Ķ�ջ��   */
	INT32U	OSTCBDly;				 /* ������ʱʱ��	   */
};


///******************ȫ�ֱ�������***************///

EXTERN  INT32U  StackIdle[StackSizeIdle]; 			/* ������������ջ   		    */
EXTERN  INT8U   OSRdyTbl;     						/* ���������					*/
EXTERN  INT8U   OSPrioCur;		            		/* �������е�����				*/
EXTERN  INT8U   OSPrioHighRdy	;           		/* �������ȼ�������				*/
EXTERN  INT32U  Int_Return_Addr_Save;				/* �����жϺ�ķ��ص�ַ			*/	
EXTERN  BOOLEAN OSIntCtxSwFlag;						/* �ж��˳��л������־			*/
EXTERN  INT32U  cpu_sr;								/* �����ٽ������ʱ����CPU״̬	*/
EXTERN  INT32U  OSTime;								/* ��¼ϵͳ����ʱ��				*/
EXTERN  INT32U  IdleCount;							/* �����������м�¼				*/
EXTERN  INT32U  OSNextTaskPrio;						/* Ѱ�Ҹ������ȼ��ľ�������ʱ�� */
EXTERN  struct TaskCtrBlock	 TCB[OS_TASKS + 1];		/* ����������ƿ�				*/
EXTERN  struct TaskCtrBlock  *p_OSTCBCur ;			/* ָ��ǰ������ƿ��ָ��		*/
EXTERN  struct TaskCtrBlock  *p_OSTCBHighRdy ;		/* ָ��������ȼ�������ƿ��ָ��*/


///******************�궨�����***************///

/* �����ٽ������ */
#define  OS_ENTER_CRITICAL()  (cpu_sr = OSCPUSaveSR()) /* �����ж� */
/* �˳��ٽ������ */
#define  OS_EXIT_CRITICAL()   (OSCPURestoreSR(cpu_sr)) /* �ָ�ԭ���ж�״̬ */

/* �ھ������еǼǾ������� */
#define OSSetPrioRdy(prio)             			  \
{											      \
OSRdyTbl |= 0x01<<prio;	                          \
}		                                          
							          
/* �Ӿ�������ɾ������ */
#define OSDelPrioRdy(prio)             			  \
{												  \
OSRdyTbl &= ~(0x01<<prio); 						  \
}														   


/* �ھ������в��Ҹ��߼��ľ������� */
#define OSGetHighRdy()															\
{																				\
	for( OSNextTaskPrio = 0; 													\
	    (OSNextTaskPrio < OS_TASKS) && (!(OSRdyTbl & (0x01<<OSNextTaskPrio))); 	\
	    OSNextTaskPrio ++ );													\
	OSPrioHighRdy = OSNextTaskPrio;												\
}																														
	
	
///******************ϵͳ��������***************

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

/*���õĻ�ຯ������*/
INT32U OSCPUSaveSR(void);
void OSCPURestoreSR(INT32U cpu_sr);
void OSStartHighRdy(void);
void OS_TASK_SW(void);

#endif
