
// os.c
// Runs on LM4F120/TM4C123/MSP432
// Lab 3 starter file.
// Daniel Valvano
// March 24, 2016

#include "Userlibs.h"
#include "os.h"


void StartOS(void);         // function definitions in osasm.s

#define NUMTHREADS  6       // maximum number of threads
#define NUMPERIODIC 2      // maximum number of periodic threads
#define STACKSIZE   200      // number of 32-bit words in stack per thread


struct tcb{
  int32_t *sp;       // pointer to stack (valid for threads not running
  struct tcb *next;  // linked-list pointer
  int32_t *Blocked;  // nonzero if blocked
	uint32_t Sleep;     // nonzero if this thread is sleeping
};
typedef struct tcb tcbType;

tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
int32_t Stacks[NUMTHREADS][STACKSIZE];

void SetInitialStack(int i)
{  
	//Itachi implemetation
  tcbs[i].sp = &Stacks[i][STACKSIZE-16]; // thread stack pointer
  Stacks[i][STACKSIZE-1] = 0x01000000;   // thumb bit
  Stacks[i][STACKSIZE-3] = 0x14141414;   // R14
  Stacks[i][STACKSIZE-4] = 0x12121212;   // R12
  Stacks[i][STACKSIZE-5] = 0x03030303;   // R3
  Stacks[i][STACKSIZE-6] = 0x02020202;   // R2
  Stacks[i][STACKSIZE-7] = 0x01010101;   // R1
  Stacks[i][STACKSIZE-8] = 0x00000000;   // R0
  Stacks[i][STACKSIZE-9] = 0x11111111;   // R11
  Stacks[i][STACKSIZE-10] = 0x10101010;  // R10
  Stacks[i][STACKSIZE-11] = 0x09090909;  // R9
  Stacks[i][STACKSIZE-12] = 0x08080808;  // R8
  Stacks[i][STACKSIZE-13] = 0x07070707;  // R7
  Stacks[i][STACKSIZE-14] = 0x06060606;  // R6
  Stacks[i][STACKSIZE-15] = 0x05050505;  // R5
  Stacks[i][STACKSIZE-16] = 0x04040404;  // R4
}

//******** OS_AddThreads ***************
// Add six main threads to the scheduler
// Inputs: function pointers to six void/void main threads
// Outputs: 1 if successful, 0 if this thread can not be added
// This function will only be called once, after OS_Init and before OS_Launch
int OS_AddThreads(void(*thread0)(void),
                  void(*thread1)(void),
                  void(*thread2)(void),
                  void(*thread3)(void),
                  void(*thread4)(void),
                  void(*thread5)(void))
{
  // **similar to Lab 2. initialize as not blocked, not sleeping****
	// initialize TCB circular list
	// initialize RunPt
	// initialize four stacks, including initial PC
	
  //Itachi implemetation
  IntMasterDisable();
	//Linked-lists
  tcbs[0].next = &tcbs[1]; // 0 points to 1
  tcbs[1].next = &tcbs[2]; // 1 points to 2
  tcbs[2].next = &tcbs[3]; // 2 points to 3
	tcbs[3].next = &tcbs[4]; // 3 points to 4			
  tcbs[4].next = &tcbs[5]; // 4 points to 5
	tcbs[5].next = &tcbs[0]; // 5 points to 0
	//Blocked & Sleep initilize
	int i;
	for(i=0;i<NUMTHREADS;i++)
	{
		tcbs[i].Blocked=0;
		tcbs[i].Sleep=0;
	}
  SetInitialStack(0); Stacks[0][STACKSIZE-2] = (int32_t)(thread0); // PC
  SetInitialStack(1); Stacks[1][STACKSIZE-2] = (int32_t)(thread1); // PC
  SetInitialStack(2); Stacks[2][STACKSIZE-2] = (int32_t)(thread2); // PC
	SetInitialStack(3); Stacks[3][STACKSIZE-2] = (int32_t)(thread3); // PC
  SetInitialStack(4); Stacks[4][STACKSIZE-2] = (int32_t)(thread4); // PC
  SetInitialStack(5); Stacks[5][STACKSIZE-2] = (int32_t)(thread5); // PC
  RunPt = &tcbs[0];       // thread 0 will run first
  IntMasterEnable();
  return 1;               // successful
}

//******** OS_Launch ***************
// Start the scheduler, enable interrupts
// Inputs: number of clock cycles for each time slice
// Outputs: none (does not return)
// Errors: theTimeSlice must be less than 16,777,216
void OS_Launch(uint32_t theTimeSlice){
  HWREG(NVIC_ST_CTRL)&=~(0x07);                 //Disable Systick Timer during initialization
  HWREG(NVIC_ST_RELOAD)= theTimeSlice - 1;			//Reload value
  HWREG(NVIC_SYS_PRI3)= (HWREG(NVIC_SYS_PRI3)&~(0xE0000000))|0xE0000000;	//Priority 7
  HWREG(NVIC_ST_CURRENT)=0;         						//Write any value to clear the counter
  HWREG(NVIC_ST_CTRL)|=0x07;          					//Enable Systick Timer and ARM for Systick interrupt
  StartOS();                   // start on the first task
}
// runs every ms
void Scheduler(void) 		// every time slice
{ 
 	RunPt = RunPt->next;    	 // run next thread not blocked
	while( (RunPt->Blocked!=0) || (RunPt->Sleep!=0) )      // skip if blocked or sleep
	{
		RunPt = RunPt->next;
	}
	//Sleeping handler
	int i;
	for(i=0;i<NUMTHREADS;i++)
	{
			if(tcbs[i].Sleep>0)
			{
				tcbs[i].Sleep--;
			}
	}
}

//******** OS_Suspend ***************
// Called by main thread to cooperatively suspend operation
// Inputs: none
// Outputs: none
// Will be run again depending on sleep/block status
void OS_Suspend(void)
{
  HWREG(NVIC_ST_CURRENT)=0;         						//Write any value to clear the counter
	HWREG(NVIC_INT_CTRL) = 0x04000000; 						//Trigger SysTick, next thread gets a full time slice
}

// ******** OS_Sleep ************
// place this thread into a dormant state
// input:  number of msec to sleep
// output: none
// OS_Sleep(0) implements cooperative multitasking
void OS_Sleep(uint32_t sleepTime)
{
	// set sleep parameter in TCB
	// suspend, stops running
	//Itachi implementation
	IntMasterDisable();		//Start critical section
	RunPt->Sleep=sleepTime;
	IntMasterEnable();			//End Critical section
	OS_Suspend();
}

// ******** OS_InitSemaphore ************
// Initialize counting semaphore
// Inputs:  pointer to a semaphore
//          initial value of semaphore
// Outputs: none
void OS_InitSemaphore(int32_t *semaPt, int32_t value){
	//Itachi Solution
  *semaPt = value;
}

// ******** OS_Wait ************
// Decrement semaphore and block if less than zero
// Lab2 spinlock (does not suspend while spinning)
// Lab3 block if less than zero
// Inputs:  pointer to a counting semaphore
// Outputs: none
void OS_Wait(int32_t *semaPt)
{
	//Itachi Solution
	IntMasterDisable();
	*semaPt-=1;
	if( *semaPt <0)
	{
		RunPt->Blocked=semaPt;	//Store the reason why its blocked
		IntMasterEnable();	
		OS_Suspend();
	}
	IntMasterEnable();
}

// ******** OS_Signal ************
// Increment semaphore
// Lab2 spinlock
// Lab3 wakeup blocked thread if appropriate
// Inputs:  pointer to a counting semaphore
// Outputs: none
void OS_Signal(int32_t *semaPt)
{
	//Itachi Solution
	tcbType *search_tcbs_pt;
	IntMasterDisable();
	*semaPt +=1;
	if( *semaPt<=0) //at least one blocked thread
	{
		search_tcbs_pt=RunPt->next;
		while(search_tcbs_pt->Blocked!=semaPt)
		{
			search_tcbs_pt=search_tcbs_pt->next;
		}
		search_tcbs_pt->Blocked=0;	//unlock
	}
	IntMasterEnable();
}

#define FSIZE 10    // can be any size
uint32_t PutI;      // index of where to put next
uint32_t GetI;      // index of where to get next
uint32_t Fifo[FSIZE];
int32_t CurrentSize;// 0 means FIFO empty, FSIZE means full
uint32_t LostData;  // number of lost pieces of data

// ******** OS_FIFO_Init ************
// Initialize FIFO.  
// One event thread producer, one main thread consumer
// Inputs:  none
// Outputs: none
void OS_FIFO_Init(void)
{
	//Itachi Implementation
	PutI=GetI=0;
	OS_InitSemaphore(&CurrentSize,0);
}

// ******** OS_FIFO_Put ************
// Put an entry in the FIFO.  
// Exactly one event thread puts,
// do not block or spin if full
// Inputs:  data to be stored
// Outputs: 0 if successful, -1 if the FIFO is full
int OS_FIFO_Put(uint32_t data)
{
	//Itachi Implementation
	if(CurrentSize==FSIZE)	//FIFO Full
	{
		return -1;
	}
	Fifo[PutI]=data;
	PutI=(PutI+1)%FSIZE;
	OS_Signal(&CurrentSize);
  return 0;   // success
}

// ******** OS_FIFO_Get ************
// Get an entry from the FIFO.   
// Exactly one main thread get,
// do block if empty
// Inputs:  none
// Outputs: data retrieved
uint32_t OS_FIFO_Get(void)
{
	uint32_t data;
	//Itachi Implementation
	OS_Wait(&CurrentSize);
	data=Fifo[GetI];
	GetI=(GetI+1)%FSIZE;
  return data;
}

void StartOS(void)
{
    __asm(/*" LDR     R0, =RunPt\n"*/
          " LDR     R2, [R0]\n"
          " LDR     SP, [R2]\n"
          " POP     {R4-R11}\n"
          " POP     {R0-R3}\n"
          " POP     {R12}\n"
          " ADD     SP,SP,#4\n"
          " POP     {LR}\n"
          " ADD     SP,SP,#4\n"
          " CPSIE   I\n"
          " BX      LR");

}



