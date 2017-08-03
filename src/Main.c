//-------------------------------------------------------------------------
// �ļ�����MAIN.C
// ���ܣ�  ����ARM�����е�΢�Ͳ���ϵͳ
// ��ע��  
//		By Lisuwei
//-------------------------------------------------------------------------
#include "Includes.h" 

///******************������***************///
void Task0	(void);
void Task1	(void);
void Task2	(void);
void Task3	(void);

///**************�����������ȼ�*************///
#define PrioTask0   		 0
#define PrioTask1        	 1
#define PrioTask2        	 2
#define PrioTask3        	 3

///******************�����ջ��С����***********///
#define StackSizeTask0       512
#define StackSizeTask1       512
#define StackSizeTask2       512
#define StackSizeTask3       512

///******************���������ջ***************///
INT32U StackTask0[StackSizeTask0];				
INT32U StackTask1[StackSizeTask1];				
INT32U StackTask2[StackSizeTask2];			   
INT32U StackTask3[StackSizeTask3];

///******************��ຯ������***************///
void OS_TASK_SW(void);

///******************ȫ�ֱ���***************///


/************************************************************************************************************************
�������ƣ�  main
����ԭ�ͣ�	int main(void)
�������ܣ�	������
��ڲ�����	��
���ڲ�����	��
�й�˵����	
************************************************************************************************************************/
int Main(void)
{
	TargetInit();  //��ʼ��Ŀ���
	OSInit();      //��ʼ������ϵͳ   
	
	OSTaskCreate(Task0,&StackTask0[StackSizeTask0 - 1],PrioTask0); // ����һ������
	Uart_Printf("Ready to start OS\n");
	OSStart(); 	   //���в���ϵͳ
	
	return 0;      //���򲻻��������� 
}



/************************************************************************************************************************
�������ƣ�  Task0
����ԭ�ͣ�	void Task0(void)
�������ܣ�	����0
��ڲ�����	��
���ڲ�����	��
�й�˵����		
************************************************************************************************************************/
void Task0(void)
{
	TargetStart();	//�����ж���������������ϵͳ��Ӳ����ʱ���ж�
	Uart_Printf("Start OS\n");
	// ������������
	OSTaskCreate(Task1,&StackTask1[StackSizeTask1 - 1],PrioTask1);
	OSTaskCreate(Task2,&StackTask2[StackSizeTask2 - 1],PrioTask2);
	OSTaskCreate(Task3,&StackTask3[StackSizeTask2 - 1],PrioTask3);
	
	while(1)
	{
		OS_ENTER_CRITICAL();
		Uart_Printf("Task0\n");
		OS_EXIT_CRITICAL();
		OSTimeDly(100);		//1������һ��
	}
}

/************************************************************************************************************************
�������ƣ�	Task1
����ԭ�ͣ�	void Task1(void)
�������ܣ�	����1
��ڲ�����	��
���ڲ�����	��
�й�˵����		
************************************************************************************************************************/
void Task1(void)
{
	
	while(1)
	{
		OS_ENTER_CRITICAL();
		Uart_Printf("Task1\n");
		OS_EXIT_CRITICAL();
		OSTimeDly(300);	//3������һ��
	}
}

/************************************************************************************************************************
�������ƣ�	Task2
����ԭ�ͣ�	void Task2(void)
�������ܣ�	����2
��ڲ�����	��
���ڲ�����	��
�й�˵����	
************************************************************************************************************************/
void Task2(void)
{

	while(1)
	{	
		OS_ENTER_CRITICAL();
		Uart_Printf("Task2\n");
		OS_EXIT_CRITICAL();
		OSTaskSuspend(PrioTask2);  // ʹ�Լ��������״̬ 
	}
}


/************************************************************************************************************************
�������ƣ�	Task3
����ԭ�ͣ�	void Task3(void)
�������ܣ�	����3
��ڲ�����	��
���ڲ�����	��
�й�˵����	
************************************************************************************************************************/
void Task3(void)
{

	while(1)
	{	
		OS_ENTER_CRITICAL();
		Uart_Printf("Resume Task2\n");
		OS_EXIT_CRITICAL();
	    OSTaskResume(PrioTask2);     // �ָ�����2 
		OSTimeDly(800);
	}
}

