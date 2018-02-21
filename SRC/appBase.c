#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "def.h"
#include "2416addr.h"
#include "2416lib.h"
#include "option.h"
#include "LCD.h"
#include "mmu.h"
#include "Nand.h"
#include "..\inc\download.h"
#include "..\inc\systemconf.h"
#include "..\inc\systemmenu.h"
#include "..\inc\function.h"
#include "..\inc\jiema.h"
#include "..\inc\communication.h"
#include "..\inc\test.h"
#include "..\inc\Rtc.h"
#include "..\inc\encrypt.h"

#include "..\inc\appBase.h"



extern INTERALMEMORY g_InteralMemory;

//extern GLOBALVAR g_GlobalVar;

//extern INT16U PreScrNum;


unsigned int InPulseCount[7]={0};		//记录6路高速输入的脉冲数

int OutPulse_PreSet[4]={0};	// 3路脉冲输出的  脉冲预设值
unsigned int OutPulse_Counter[4]={0};	// 3路脉冲输出的  当前输出脉冲数

///Timer0 6-30*************************************************////
///变量定义
extern int Timer0_count,ShangXian_count,Timer0_SJ,number;
extern U32 flag_begin[NUMBER];
extern unsigned long int Tx_DQ,Tx_count_DQ,Tx_ShangXian_count_DQ;
extern U32 Time[NUMBER],Tx_0[NUMBER],Tx_count_0[NUMBER],Tx_ShangXian_count_0[NUMBER];
///***********************************************************************////

////启动软件定时器时调用
int Delay_Tx_Start(unsigned long int name, unsigned long int time)
{
  number = name; 
  Time[number] = time;
  
  if( flag_begin[number] == 0)
  {
    flag_begin[number] = 1;  
    Tx_0[number] = rTCNTO0;   
    Tx_count_0[number] = Timer0_count;
    Tx_ShangXian_count_0[number] = ShangXian_count;   
  }  
  return number;
}

////定时未到时清零，再次启动重新开始记
void Delay_Tx_Stop(unsigned int number) 
{
  flag_begin[number] = 0;
  Tx_0[number] = Tx_DQ;
  Tx_count_0[number] = Tx_count_DQ;
  Tx_ShangXian_count_0[number] = Tx_ShangXian_count_DQ;
}



///////////////////////////////////////////////////////////////////
////	高速脉冲输入X1 中断服务程序    ///////////////
////	InPulseCount[1] 为X1  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
void __irq PulseIn_1_Process(void)	
{
	/* int i;
	for(i=0; i<20; i++); */

	/* if((rGPFDAT >> 1) & 0x1)
	{ */
		InPulseCount[1]++;	
		x1_display ++;
	//}

	rEINTPEND=(1<<1);
	ClearPending(BIT_EINT1);
}
///////////////////////////////////////////////////////////////////
////	高速脉冲输入X2 中断服务程序    ///////////////
////	InPulseCount[2] 为X2  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
void __irq PulseIn_2_Process(void)	
{
	/* int i;
	for(i=0; i<20; i++);

	if((rGPFDAT >> 2) & 0x1)
	{ */
		InPulseCount[2]++;
		x2_display++;
	//}

	rEINTPEND=(1<<2);
	ClearPending(BIT_EINT2);
}
///////////////////////////////////////////////////////////////////
////	高速脉冲输入X3 中断服务程序    ///////////////
////	InPulseCount[3] 为X3  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
void __irq PulseIn_3_Process(void)	
{
	int i;
	for(i=0; i<20; i++);

	if((rGPFDAT >> 3) & 0x1)
	{
		InPulseCount[3]++;
	}

	rEINTPEND=(1<<3);
	ClearPending(BIT_EINT3);
}


//////////////////////////////////////////////////////////////////////
////	高速脉冲输入X4-X6  中断服务程序     //////////////
////	InPulseCount[4] 为X4  输入的脉冲数	//////////////////
////	InPulseCount[5] 为X5  输入的脉冲数	//////////////////
////	InPulseCount[6] 为X6  输入的脉冲数	//////////////////
/////////////////////////////////////////////////////////////////////
void __irq PulseIn_4_7_Process(void)
{
	int i;
	for(i=0; i<20; i++);

		// Eint4	// 高速输入X4
	if ( !( rEINTMASK >>4  & 0x1 )  && (rEINTPEND & (1<<4)) )//取消屏蔽标志+挂起有效
	{
		if((rGPFDAT >> 4) & 0x1)
		{
			InPulseCount[4]++;
		}

		rEINTPEND=(1<<4);		
	}
	// Eint5	// 高速输入X5
	else if ( !( rEINTMASK >>5  & 0x1 )  && (rEINTPEND & (1<<5)) )//取消屏蔽标志+挂起有效	
	
	{
		if((rGPFDAT >> 5) & 0x1)
		{
			InPulseCount[5]++;
		}

		rEINTPEND=(1<<5);		
	}
	// Eint6	// 高速输入X6
	else if ( !( rEINTMASK >>6  & 0x1 )  && (rEINTPEND & (1<<6)) )//取消屏蔽标志+挂起有效	
	
	{
		if((rGPFDAT >> 6) & 0x1)
		{
			InPulseCount[6]++;  //InPulseCount[n] 只定义了7个 InPulseCount[0]-----InPulseCount[6]
		}

		rEINTPEND=(1<<6);		
	}
	// Eint7	// 高速输入X7
	else if ( !( rEINTMASK >>7  & 0x1 )  && (rEINTPEND & (1<<7)) )//取消屏蔽标志+挂起有效	
	
	{
		if((rGPFDAT >> 7) & 0x1)
		{
		  /////////
		}
		rEINTPEND=(1<<7);		
	}
 
	ClearPending(BIT_EINT4_7);
}

//////////////////////////////////////////////////////////////////////
////	高速脉冲输入X8-X23  中断服务程序     //////////////
/////////////////////////////////////////////////////////////////////
void __irq PulseIn_8_23_Process(void)   // 高速输入X8-x10   
{	 
	 if ( !( rEINTMASK >>8  & 0x1 )  && (rEINTPEND & (1<<8)) ) //取消屏蔽+挂起有效
	
	{	
	    /*if((rGPGDAT >> 0) & 0x1) //判断沿触发
	    {
	      //////////
	      
	    }*/
		
		rEINTPEND=(1<<8);
		 
	}
	else if ( !( rEINTMASK >>9  & 0x1 )  && (rEINTPEND & (1<<9)) ) //取消屏蔽+挂起有效   
	{ 
		//////////
		rEINTPEND=(1<<9);
	}
	else if ( !( rEINTMASK >>10  & 0x1 )  && (rEINTPEND & (1<<10)) ) //取消屏蔽+挂起有效  
	{
		////////
		rEINTPEND=(1<<10);
	}
	else if ( !( rEINTMASK >>11  & 0x1 )  && (rEINTPEND & (1<<11)) ) //取消屏蔽+挂起有效  
	{
		////////
		rEINTPEND=(1<<11);
	}
	else if ( !( rEINTMASK >>12  & 0x1 )  && (rEINTPEND & (1<<12)) ) //取消屏蔽+挂起有效  
	{
		////////
		rEINTPEND=(1<<12);
	}
	else if ( !( rEINTMASK >>13  & 0x1 )  && (rEINTPEND & (1<<13)) ) //取消屏蔽+挂起有效  
	{
		////////
		rEINTPEND=(1<<13);
	}
	else if ( !( rEINTMASK >>14  & 0x1 )  && (rEINTPEND & (1<<14)) ) //取消屏蔽+挂起有效  
	{
		////////
		rEINTPEND=(1<<14);
	} 
	else if ( !( rEINTMASK >>15  & 0x1 )  && (rEINTPEND & (1<<15)) ) //取消屏蔽+挂起有效  
	{
		////////
		rEINTPEND=(1<<15);
	}
	
	
	
	ClearPending(BIT_EINT8_23);

}







///////////////////////////////////////////////////////////////
////	高速脉冲输入X1  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse1_Start(void)
{
	rEINTPEND=(1<<1);
	ClearPending(BIT_EINT1);
	
	rEINTMASK &= ~(1<<1);
	rINTMSK1 &= ~BIT_EINT1;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X1  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse1_Stop(void)
{
	rEINTMASK |=(1<<1);
	rINTMSK1 |=BIT_EINT1;
	
	rEINTPEND=(1<<1);
	ClearPending(BIT_EINT1);
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X2  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse2_Start(void)
{
	rEINTPEND=(1<<2);
	ClearPending(BIT_EINT2);
	
	rEINTMASK &= ~(1<<2);
	rINTMSK1 &= ~BIT_EINT2;
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X2  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse2_Stop(void)
{
	rEINTMASK |=(1<<2);
	rINTMSK1 |=BIT_EINT2;
	
	rEINTPEND=(1<<2);
	ClearPending(BIT_EINT2);
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X3  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse3_Start(void)
{
	rEINTPEND=(1<<3);
	ClearPending(BIT_EINT3);

	rEINTMASK &= ~(1<<3);
	rINTMSK1 &= ~BIT_EINT3;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X3  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse3_Stop(void)
{
	rEINTMASK |=(1<<3);
	rINTMSK1 |=BIT_EINT3;
	
	rEINTPEND=(1<<3);
	ClearPending(BIT_EINT3);
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X4  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse4_Start(void)
{
	rEINTPEND=(1<<4);
	ClearPending(BIT_EINT4_7);
	
	rEINTMASK &= ~(1<<4);
	rINTMSK1 &= ~BIT_EINT4_7;
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X4  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse4_Stop(void)
{
	rEINTMASK |=(1<<4);
	//rINTMSK1 |=BIT_EINT4_7;
	
	rEINTPEND=(1<<4);
	ClearPending(BIT_EINT4_7);
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X5  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse5_Start(void)
{
	rEINTPEND=(1<<5);
	ClearPending(BIT_EINT4_7);
	
	rEINTMASK &= ~(1<<5);
	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X5  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse5_Stop(void)
{
	rEINTMASK |=(1<<5);
	//rINTMSK1 |=BIT_EINT4_7;
	
	
	rEINTPEND=(1<<5);
	ClearPending(BIT_EINT4_7);
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X6  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse6_Start(void)
{
	rEINTPEND=(1<<6);
	ClearPending(BIT_EINT4_7);
	
	rEINTMASK &= ~(1<<6);
	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X6  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse6_Stop(void)
{
	rEINTMASK |=(1<<6);
	//rINTMSK1 |=BIT_EINT4_7;
	
	rEINTPEND=(1<<6);
	ClearPending(BIT_EINT4_7);
}


///////////////////////////////////////////////////////////////
////	高速脉冲输入X7  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse7_Start(void)
{
	rEINTPEND=(1<<7);
	ClearPending(BIT_EINT4_7);
	
	rEINTMASK &= ~(1<<7);
	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X7  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse7_Stop(void)
{
	rEINTMASK |=(1<<7);
	//rINTMSK1 |=BIT_EINT4_7;
	rEINTPEND=(1<<7);
	ClearPending(BIT_EINT4_7);
	
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X8  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse8_Start(void)
{  
	rEINTPEND=(1<<8);
	ClearPending(BIT_EINT8_23);
	 
	rEINTMASK &= ~(1<<8);
	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X8  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse8_Stop(void)
{
	rEINTMASK |=(1<<8);
	//rINTMSK1 |=BIT_EINT8_23;
	
	rEINTPEND=(1<<8);
	ClearPending(BIT_EINT8_23);
	
}


///////////////////////////////////////////////////////////////
////	高速脉冲输入X9  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse9_Start(void)
{
	rEINTPEND=(1<<9);
	ClearPending(BIT_EINT8_23);
	
	rEINTMASK &= ~(1<<9);
	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X9  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse9_Stop(void)
{
	rEINTMASK |=(1<<9);
	//rINTMSK1 |=BIT_EINT8_23;
	
	rEINTPEND=(1<<9);
	ClearPending(BIT_EINT8_23);
	
}
 
///////////////////////////////////////////////////////////////
////	高速脉冲输入X10  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse10_Start(void)
{
	rEINTPEND=(1<<10);
	ClearPending(BIT_EINT8_23);
	
	rEINTMASK &= ~(1<<10);
	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X10  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse10_Stop(void)
{
	rEINTMASK |=(1<<10);
	//rINTMSK1 |=BIT_EINT8_23;
	
	rEINTPEND=(1<<10);
	ClearPending(BIT_EINT8_23);
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X11  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse11_Start(void)
{
	rEINTPEND=(1<<11);
	ClearPending(BIT_EINT8_23);
	
	rEINTMASK &= ~(1<<11);
	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X11  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse11_Stop(void)
{
	rEINTMASK |=(1<<11);
	//rINTMSK1 |=BIT_EINT8_23;
	
	rEINTPEND=(1<<11);
	ClearPending(BIT_EINT8_23);
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X12  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse12_Start(void)
{
	rEINTPEND=(1<<12);
	ClearPending(BIT_EINT8_23);
	
	rEINTMASK &= ~(1<<12);
	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X12  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse12_Stop(void)
{
	rEINTMASK |=(1<<12);
	//rINTMSK1 |=BIT_EINT8_23;
	
	rEINTPEND=(1<<12);
	ClearPending(BIT_EINT8_23);
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X13  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse13_Start(void)
{
	rEINTPEND=(1<<13);
	ClearPending(BIT_EINT8_23);
	
	rEINTMASK &= ~(1<<13);
	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X13  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse13_Stop(void)
{
	rEINTMASK |=(1<<13);
	//rINTMSK1 |=BIT_EINT8_23;
	
	rEINTPEND=(1<<13);
	ClearPending(BIT_EINT8_23);
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X14  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse14_Start(void)
{
	rEINTPEND=(1<<14);
	ClearPending(BIT_EINT8_23);
	
	rEINTMASK &= ~(1<<14);
	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X14  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse14_Stop(void)
{
	rEINTMASK |=(1<<14); 
	//rINTMSK1 |=BIT_EINT8_23;
	
	rEINTPEND=(1<<14);
	ClearPending(BIT_EINT8_23);
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X15  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse15_Start(void)
{
	rEINTPEND=(1<<15);
	ClearPending(BIT_EINT8_23);
	
	rEINTMASK &= ~(1<<15);
	rINTMSK1 &= ~BIT_EINT8_23;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X15  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse15_Stop(void)
{
	rEINTMASK |=(1<<15);
	//rINTMSK1 |=BIT_EINT8_23;
	
	rEINTPEND=(1<<15);
	ClearPending(BIT_EINT8_23);
	
} 
 
 



/////////////////////////////////////////////////
//////	高速脉冲输入初始化     //////////////
//////	IN 1~6   6路高速输入	IN7     //////////////
////////////////////////////////////////////////
void Pulse_In_Init(void)	
{
	DWORD tmp;
	
	//set GPF1~7 as EINT	x1-x7
	// tmp = rGPFCON & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14));
	// rGPFCON = tmp | (0x2<<2) | (0x2<<4) | (0x2<<6) | (0x2<<8) | (0x2<<10) | (0x2<<12) | (0x2<<14);	
	//GPF1~7 up down disable x1-x7
	rGPFUDP &=  (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14));

	
	
	//////	IN 8~15   8路普通输入	    //////////////
	//	GPG 0 1 2---up down disable---  X9 X10 X11 X12 X13 X14 X15
	
    rGPGUDP &= (~(0x3<< 0)) & (~(0x3<< 2))& (~(0x3<< 4)) & (~(0x3<< 6))& (~(0x3<< 8))& (~(0x3<< 10))& (~(0x3<< 12))& (~(0x3<< 14));

	//	set GPG  1 2 3 4 5 6 7 as EINT --- X9 X10 X11 X12 X13 X14 X15
	
	////(TJ)配置X8~X15为EINT
	// tmp = rGPGCON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12))& (~(0x3<< 14));
	// rGPGCON = tmp | (0x2<<0) | (0x2<<2) | (0x2<<4) | (0x2<<6) | (0x2<<8) | (0x2<<10) | (0x2<<12)| (0x2<<14);		
	
	
	
	
	
	
	//GPF1~6  x1-x6 上升沿触发
	rEXTINT0 = (rEXTINT0 & (~(0x7<<4))) | (0x4<<4);		// Eint1	Rising edge triggered
	rEXTINT0 = (rEXTINT0 & (~(0x7<<8))) | (0x4<<8);		// Eint2	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<12))) | (0x4<<12);	// Eint3	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<16))) | (0x4<<16);	// Eint4	Rising edge triggered
	rEXTINT0 = (rEXTINT0 & (~(0x7<<20))) | (0x4<<20);	// Eint5	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<24))) | (0x4<<24);	// Eint6	Rising edge triggered	
	/*
	//GPF1~6  x1-x6 下降沿触发
	rEXTINT0 = (rEXTINT0 & (~(0x7<<4))) | (0x2<<4);		// Eint1	Falling edge triggered
	rEXTINT0 = (rEXTINT0 & (~(0x7<<8))) | (0x2<<8);		// Eint2	Falling edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<12))) | (0x2<<12);	// Eint3	Falling edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<16))) | (0x2<<16);	// Eint4	Falling edge triggered
	rEXTINT0 = (rEXTINT0 & (~(0x7<<20))) | (0x2<<20);	// Eint5	Falling edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<24))) | (0x2<<24);	// Eint6	Falling edge triggered	
	*/
	/*
	//GPF1~6  x1-x6 双边沿触发
	rEXTINT0 = (rEXTINT0 & (~(0x7<<4))) | (0x6<<4);		// Eint1	Both edge triggered
	rEXTINT0 = (rEXTINT0 & (~(0x7<<8))) | (0x6<<8);		// Eint2	Both edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<12))) | (0x6<<12);	// Eint3	Both edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<16))) | (0x6<<16);	// Eint4	Both edge triggered
	rEXTINT0 = (rEXTINT0 & (~(0x7<<20))) | (0x6<<20);	// Eint5	Both edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<24))) | (0x6<<24);	// Eint6	Both edge triggered	
	*/
	//GPF7 
	//rEXTINT0 = (rEXTINT0 & (~(0x7<<28))) | (0x2<<28);	// Eint7	Falling edge triggered
	rEXTINT0 = (rEXTINT0 & (~(0x7<<28))) | (0x4<<28);	// Eint7	Rising edge triggered
	//rEXTINT0 = (rEXTINT0 & (~(0x7<<28))) | (0x6<<28);	// Eint7	Both edge triggered
	/*
	//GPG1~8   x8-x15 下降沿触发  
	rEXTINT1 = (rEXTINT1 & (~(0x7<<0))) | (0x2<<0);	        // Eint8	Falling edge triggered	////5-25(要使用则需先配置X8功能)6-14 配置大盘信号X8为双边沿触发
	rEXTINT1 = (rEXTINT1 & (~(0x7<<4))) | (0x2<<4);	        // Eint9	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<8))) | (0x2<<8);	        // Eint10	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<12))) | (0x2<<12);	    // Eint11	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<16))) | (0x2<<16);	    // Eint12	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<20))) | (0x2<<20);	    // Eint13	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<24))) | (0x2<<24);	    // Eint14	Falling edge triggered		
	rEXTINT1 = (rEXTINT1 & (~(0x7<<28))) | (0x2<<28);	    // Eint15	Falling edge triggered
	*/
	//GPG1~8   x8-x15 上升沿触发
	rEXTINT1 = (rEXTINT1 & (~(0x7<<0))) | (0x4<<0);	        // Eint8	Falling edge triggered	////5-25(要使用则需先配置X8功能)6-14 配置大盘信号X8为双边沿触发
	rEXTINT1 = (rEXTINT1 & (~(0x7<<4))) | (0x4<<4);	        // Eint9	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<8))) | (0x4<<8);	        // Eint10	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<12))) | (0x4<<12);	    // Eint11	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<16))) | (0x4<<16);	    // Eint12	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<20))) | (0x4<<20);	    // Eint13	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<24))) | (0x4<<24);	    // Eint14	Falling edge triggered		
	rEXTINT1 = (rEXTINT1 & (~(0x7<<28))) | (0x4<<28);	    // Eint15	Falling edge triggered
	/*
	//GPG1~8   x8-x15 双边沿触发
	rEXTINT1 = (rEXTINT1 & (~(0x7<<0))) | (0x6<<0);	        // Eint8	Falling edge triggered	////5-25(要使用则需先配置X8功能)6-14 配置大盘信号X8为双边沿触发
	rEXTINT1 = (rEXTINT1 & (~(0x7<<4))) | (0x6<<4);	        // Eint9	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<8))) | (0x6<<8);	        // Eint10	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<12))) | (0x6<<12);	    // Eint11	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<16))) | (0x6<<16);	    // Eint12	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<20))) | (0x6<<20);	    // Eint13	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<24))) | (0x2<<24);	    // Eint14	Falling edge triggered		
	rEXTINT1 = (rEXTINT1 & (~(0x7<<28))) | (0x2<<28);	    // Eint15	Falling edge triggered
	*/


	pISR_EINT1= (U32)PulseIn_1_Process;		// X1
	pISR_EINT2= (U32)PulseIn_2_Process;		// X2
	pISR_EINT3= (U32)PulseIn_3_Process;		// X3
	pISR_EINT4_7= (U32)PulseIn_4_7_Process;	// X4_6


	rEINTPEND = 0xffffff;

	rSRCPND1 |= BIT_EINT1|BIT_EINT2|BIT_EINT3|BIT_EINT4_7; //to clear the previous pending states
	rINTPND1 |= BIT_EINT1|BIT_EINT2|BIT_EINT3|BIT_EINT4_7;

	//rEINTMASK &= ~((1<<2)|(1<<3)|(1<<4)|(1<<6));
	//rINTMSK1 &= ~(BIT_EINT2|BIT_EINT3|BIT_EINT4_7);
	 
	InPulse1_Stop();
	InPulse2_Stop();
	InPulse3_Stop();
	InPulse4_Stop();
	InPulse5_Stop();
	InPulse6_Stop();
	
	/**********************//**********************//**********************//**********************/
	//      注意！！！上面的外部中断寄存器只配置x1-x6  之后的x7-x15要用中断形式，需提前配置       //
	/**********************//**********************//**********************//**********************/
	
	InPulse7_Stop();
	InPulse8_Stop();
	InPulse9_Stop();
	InPulse10_Stop();
	InPulse11_Stop();
	InPulse12_Stop();
	InPulse13_Stop();
	InPulse14_Stop();
	InPulse15_Stop();
	
	/*------------------x7-x15中断示例------------------*/
	/*
	
	//	GPF1~7 up down disable----x1~x7
	
	rGPFUDP &=  (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14)); 
	
	//	set GPF1~7 as EINT -----x1~x7
	
	tmp = rGPFCON & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12))& (~(0x3<< 14));
	rGPFCON = tmp | (0x2<<2) | (0x2<<4) | (0x2<<6) | (0x2<<8) | (0x2<<10) | (0x2<<12)| (0x2<<14);		
   
   
    
    //	GPG 0 1 2---up down disable---X8 X9 X10
	
    tmp = rGPGUDP & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4));
	rGPGUDP  = tmp | (0x0<<0)| (0x0<<2)| (0x0<<4) ;
		
	//	set GPG 0 1 2 as EINT ---X8 X9 X10
	tmp = rGPGCON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4));
	rGPGCON = tmp | (0x2<<0)| (0x2<<2)| (0x2<<4) ;  
	
	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<28))) | (0x2<<28);	// Eint7	Falling edge triggered	
	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<0))) | (0x2<<0);	    // Eint8	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<4))) | (0x2<<4);	    // Eint9	Falling edge triggered	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<8))) | (0x2<<8);	    // Eint10	Falling edge triggered	
	
	pISR_EINT8_23= (U32)PulseIn_8_23_Process;	// X8 X9 X10
	
	
	*/
	
	 
	
	
}





///////////////////////////////////////////////////////////////////////////
////	Y1 脉冲发送中断服务程序////////
///////////////////////////////////////////////////////////////////////////
void __irq PulseOut_1_Process(void)
{
	OutPulse_Counter[1]++;
	rGPBDAT ^= (1<<1);
	y1_display = OutPulse_Counter[1]/2;
	//if(OutPulse_Counter[1] >= OutPulse_PreSet[1])
	if  ((  OutPulse_PreSet[1] > 0   )&&(OutPulse_Counter[1]/2 >= OutPulse_PreSet[1]))
	/////OutPulse_PreSet[1]<0用于无限发脉冲
	{
		PulseOut_1_Stop();
	}
	ClearPending((U32)BIT_TIMER1);
	
}



///////////////////////////////////////////////////////////////////////////
////	Y1 脉冲输出启动//////////////////////////////
////	 每发送一段脉冲 PulseOut_1_Start 只需启动一次//////
////	frequence: 脉冲频率///////////////////////////////
////	pulse_PreSet: 发送脉冲数 ///////////////////////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_1_Start(unsigned int frequence, int pulse_PreSet)
{
	DWORD tmp;
	
	if(0 == pulse_PreSet)
	{
		PulseOut_1_Stop();
		return;
	}
	else
	{
		OutPulse_PreSet[1] = pulse_PreSet;
		OutPulse_Counter[1] = 0;	// 确保每次启动PulseOut_1_Start ，发送脉冲数 pulse_PreSet
	}
	
	if(0 == frequence)
	{
		frequence = 1;
		PulseOut_1_Stop();
		return;
	}
	
	rTCNTB1= 300300/frequence;	// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}
	
	rTCMPB1 = rTCNTB1/2;
	
	rSRCPND1 = rSRCPND1 | ((U32)0x1<<11);   //清空定时器1源请求
    rINTPND1 = rINTPND1 | ((U32)0x1<<11);   //清空定时器1中断请求
   
	
	rINTMSK1 &=~(BIT_TIMER1);
	tmp = rTCON & (~(0xf << 8));	// dead zone Disable
	rTCON = tmp | (2 << 8);		/* update TCVNTB0, stop					*/
	rTCON = tmp | (9 << 8);		/* interval mode,  start				*/	
}

///////////////////////////////////////////////////////////////////////////
////	Y1 脉冲发送停止////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_1_Stop(void)
{
	//DWORD tmp;

	rINTMSK1 |= BIT_TIMER1;
	rTCON &= ~(1 << 8);			/* Timer1, stop							*/

	//OutPulse_Counter[1] = 0;

}


///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲发送中断服务程序////////
///////////////////////////////////////////////////////////////////////////
void __irq PulseOut_2_Process(void)
{
	OutPulse_Counter[2]++;
	rGPBDAT ^= (1<<2);
	y2_display = OutPulse_Counter[2]/2;
	//if(OutPulse_Counter[2] >= OutPulse_PreSet[2])
	if  ((  OutPulse_PreSet[2] > 0   )&&(OutPulse_Counter[2]/2 >= OutPulse_PreSet[2]))
	//OutPulse_PreSet[2]<0用于无限发脉冲
	
	{
		PulseOut_2_Stop();
	}

	ClearPending((U32)BIT_TIMER2);

}



///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲输出启动//////////////////////////////
////	 每发送一段脉冲 PulseOut_2_Start 只需启动一次//////
////	frequence: 脉冲频率///////////////////////////////
////	pulse_PreSet: 发送脉冲数 ///////////////////////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_2_Start(unsigned int frequence, int pulse_PreSet)
{
	DWORD tmp;
	
	if(0 == pulse_PreSet)
	{
		PulseOut_2_Stop();
		return;
	}
	else
	{
		OutPulse_PreSet[2] = pulse_PreSet;
		OutPulse_Counter[2] = 0;	// 确保每次启动PulseOut_2_Start ，发送脉冲数 pulse_PreSet
	}
	
	if(0 == frequence)
	{
		frequence = 1;
		PulseOut_2_Stop();
		return;
	}
	
	rTCNTB2= 300300/frequence;	// // Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}
	rTCMPB2 = rTCNTB2/2;
	
	rSRCPND1 = rSRCPND1 | ((U32)0x1<<12);   //清空定时器2源请求
    rINTPND1 = rINTPND1 | ((U32)0x1<<12);    //清空定时器2中断请求
  
	
	rINTMSK1 &=~(BIT_TIMER2);
	tmp = rTCON & (~(0xf << 12));	// dead zone Disable
	rTCON = tmp | (2 << 12)	;/* update TCVNTB0, stop					*/
	rTCON = tmp | (9 << 12)	;/* interval mode,  start				*/	
}

///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲发送停止////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_2_Stop(void)
{
	//DWORD tmp;

	rINTMSK1 |= BIT_TIMER2;
	rTCON &= ~(1 << 12);		/* Timer2, stop							*/

	//OutPulse_Counter[2] = 0;

	
}


/////////////高速Y3未配置////////////////////
///////////////////////////////////////////////////////////////////////////
////	Y3 脉冲发送中断服务程序////////
///////////////////////////////////////////////////////////////////////////
void __irq PulseOut_3_Process(void)
{
	
	OutPulse_Counter[3]++; 
	
	//if(OutPulse_Counter[3] >= OutPulse_PreSet[3])
	if  ((  OutPulse_PreSet[3] > 0   )&&(OutPulse_Counter[3] >= OutPulse_PreSet[3]))
	//OutPulse_PreSet[3]<0用于无限发脉冲
	{	
		
		OutPulse_Counter[3]=0;
		PulseOut_3_Stop();
	 
	}
  	ClearPending((U32)BIT_TIMER3);

}
///////////////////////////////////////////////////////////////////////////
////	Y3 脉冲输出启动//////////////////////////////
////	 每发送一段脉冲 PulseOut_3_Start 只需启动一次//////
////	frequence: 脉冲频率///////////////////////////////
////	pulse_PreSet: 发送脉冲数 ///////////////////////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_3_Start(unsigned int frequence, int pulse_PreSet)
{
	DWORD tmp;
	
	if(0 == pulse_PreSet)
	{
		PulseOut_3_Stop();
		return;
	}
	else
	{
		OutPulse_PreSet[3] = pulse_PreSet;
		OutPulse_Counter[3] = 0;	// 确保每次启动PulseOut_3_Start ，发送脉冲数 pulse_PreSet
	}
	
	if(0 == frequence)
	{
		frequence = 1;
		PulseOut_3_Stop();
		return;
	}
	
	rTCNTB3= 300300/frequence;	// // Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}
	rTCMPB3 = rTCNTB3/2;
	
	rSRCPND1 = rSRCPND1 | ((U32)0x1<<13);   //清空定时器3源请求
	rINTPND1 = rINTPND1 | ((U32)0x1<<13);    //清空定时器3中断请求
  
	
	rINTMSK1 &=~(BIT_TIMER3);
	tmp = rTCON & (~(0xf << 16));	// dead zone Disable
	rTCON = tmp | (2 << 16);		//update TCVNTB0, stop				
	rTCON = tmp | (9 << 16);		// interval mode,  start
}

///////////////////////////////////////////////////////////////////////////
////	Y3 脉冲发送停止////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_3_Stop(void)
{
	//DWORD tmp;

	rINTMSK1 |= BIT_TIMER3;
	rTCON &= ~(1 << 16);		// Timer3, stop		

	//OutPulse_Counter[2] = 0;

	
}


/////////////////////////////////////////////////
//////	高速脉冲输出初始化     //////////////
//////	OUT 1~3   3路高速输出 //////////////
////////////////////////////////////////////////
void Pulse_Out_Init()	
{
	DWORD tmp;
	
    //set GPB1 2 3 as TOUT
	//tmp = rGPBCON & (~(0x3<< 2)) & (~(0x3<< 4))& (~(0x3<< 6));
	//rGPBCON = tmp | (0x2<<2) | (0x2<<4) | (0x2<<6);		
	
	//set GPB1 2 3 as 普通输出Output
	tmp = rGPBCON & (~(0x3<< 2)) & (~(0x3<< 4))& (~(0x3<< 6));
	rGPBCON = tmp | (0x1<<2) | (0x1<<4) | (0x1<<6);	

	
	    
	
	// Timer1 Initialize	----HS Out1
	pISR_TIMER1 = (int)PulseOut_1_Process;	// Timer ISR for HS Out1
	rTCFG0 &= ~(0xff << 0); 
	rTCFG0 |= (110 << 0); 	// Dead zone=0, Prescaler0=10(0xff)
	rTCFG1 &= ~(0xf << 4); 
	rTCFG1 |= (0x0 << 4); 	// Interrupt, Mux0=1/2
	//rTCNTB1 = 30;    //30.27273	// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	100K
	//rTCMPB1 = 15;
	
	 
	
	
	PulseOut_1_Stop();
	

	
	
	// Timer2 Initialize	----HS Out2
	pISR_TIMER2 = (int)PulseOut_2_Process;	// Timer ISR for HS Out2
	rTCFG0 &= ~(0xff << 8); 
	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   注意注意注意！！！不能变 Timer 2 3 4 共用
	rTCFG1 &= ~(0xf << 8); 
	rTCFG1 |= (0x0 << 8); 	// Interrupt, Mux0=1/2
	
	
 	
	PulseOut_2_Stop(); 
	
	
	
	
	

	// Timer3 Initialize	----HS Out3
 	pISR_TIMER3 = (int)PulseOut_3_Process;	// Timer ISR for HS Out3
	rTCFG0 &= ~(0xff << 8); 
	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   注意注意注意！！！不能变 Timer 2 3 4 共用
	rTCFG1 &= ~(0xf << 12); 
 	rTCFG1 |= (0x0 << 12); 	// Interrupt, Mux0=1/2
 	
 	PulseOut_3_Stop();

}






//////////////////////////////////////////////////////////////////////
////	DA 输出中断服务程序///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void __irq DA0_10_Process(void) // 可通过示波器确定频率
{
	if(0xffff == (g_InteralMemory.Word[32] & 0xffff))
	{
		g_InteralMemory.Word[32] = 0;
		g_InteralMemory.Word[33]++;
	}
	else
		g_InteralMemory.Word[32]++;

	ClearPending((U32)BIT_TIMER3);
}


//////////////////////////////////////////////////////////////////////
////	DA 输出启动(只需启动一次)//////////////////////////
////	voltage: 输出电压(0.000~10.000V )  小数点后3 位//////////
//////////////////////////////////////////////////////////////////////
void DA0_10_Start(float voltage)
{
	DWORD tmp;

	if(voltage>10*1000)
		voltage = 10*1000;
	else if(voltage<0)
		voltage = 0;
	
	rTCNTB3= 300;	
	//rTCMPB3 = (rTCNTB3*g_InteralMemory.Word[30])/(10000*1.326);// 最大10V , 小数点后3 位10*1000
	rTCMPB3 = (rTCNTB3*voltage)/(10*1000*1.326);// 最大10V , 小数点后3 位10*1000
	if(rTCMPB3 == rTCNTB3)
		rTCMPB3 -= 1;
	
	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (0x2<<6);	//set GPB3 as TOUT	

	rINTMSK1 &=~(BIT_TIMER3);
	tmp = rTCON & (~(0xf << 16));	// dead zone Disable
	rTCON = tmp | (2 << 16);		/* update TCVNTB3, stop					*/
	rTCON = tmp | (9 << 16);		/* interval mode,  start				*/	
}

//////////////////////////////////////////////////////////////////////
////	DA 输出停止//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void DA0_10_Stop(void)
{
	DWORD tmp;

	rTCNTB3= 300;	
	rINTMSK1 |= BIT_TIMER3;
	rTCON &= ~(1 << 16);			/* Timer3, stop							*/

	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (1<<6);	//set GPB3 as OUT	
	rGPBDAT &= ~(1<<3);
}


//////////////////////////////////////////////////////////////////////
////	DA 输出初始化///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void DA0_10_Init(void)
{
	DWORD tmp;

	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (0x2<<6);	//set GPB3 as TOUT	

	// Timer3 Initialize	----DA
	pISR_TIMER3 = (int)DA0_10_Process;	// Timer ISR for DA
	rTCFG0 &= ~(0xff << 8); 
	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   Timer 2 3 4 共用
	rTCFG1 &= ~(0xf << 12); 
	rTCFG1 |= (0x0 << 12); 	// Interrupt, Mux0=1/2
	rTCNTB3 = 300;    			// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	1K
	rTCMPB3 = 150;
	DA0_10_Stop();
}



/////////////////////////////////////////////////
//////	普通输入初始化     //////////////
//////	IN7~IN15 共9 路		//////////////
////////////////////////////////////////////////
void X_In_Init(void)	
{
	DWORD tmp;
	
	tmp = rGPFCON & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14));
	rGPFCON = tmp | (0x0<<2) | (0x0<<4) | (0x0<<6) | (0x0<<8) | (0x0<<10) | (0x0<<12) | (0x0<<14);	

	tmp = rGPGCON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12))& (~(0x3<< 14));
	rGPGCON = tmp | (0x0<<0) | (0x0<<2) | (0x0<<4) | (0x0<<6) | (0x0<<8) | (0x0<<10) | (0x0<<12)| (0x0<<14);
}



/////////////////////////////////////////////////
//////	普通输出初始化     //////////////
//////	Y2~Y15 共14 路	 //////////////
////////////////////////////////////////////////
void Y_Out_Init(void)		
{
	DWORD tmp;

	tmp = rGPECON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14));
	
	
	rGPECON = tmp | (0x1<<0) | (0x1<<2) | (0x1<<4) | (0x1<<6) | (0x1<<8) | (0x1<<10) | (0x1<<12) | (0x1<<14);	
	// 输入输出都反向
	rGPEDAT |= (1<<0);	// OUT4		
	rGPEDAT |= (1<<1);	// OUT5		
	rGPEDAT |= (1<<2);	// OUT6		
	rGPEDAT |= (1<<3);	// OUT7		
	rGPEDAT |= (1<<4);	// OUT8		
	rGPEDAT |= (1<<5);	// OUT9		
	rGPEDAT |= (1<<6);	// OUT10	
	rGPEDAT |= (1<<7);	// OUT11	


}





////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////	输入   IN7-IN15//////////////////////
////	返回: 端口状态
////////////////////////////////////////////////////////////////
unsigned char Get_X_Value(unsigned char X_num)
{
	unsigned char X_value=0xff;

	switch(X_num)
	{	
		case 1:
			X_value = (rGPFDAT >> 1) & 0x1;	// X1
			break;
		case 2:
			X_value = (rGPFDAT >> 2) & 0x1;	// X2
			break;
		case 3:
			X_value = (rGPFDAT >> 3) & 0x1;	// X3
			break;
		case 4:
			X_value = (rGPFDAT >> 4) & 0x1;	// X4
			break;
		case 5:
			X_value = (rGPFDAT >> 5) & 0x1;	// X5
			break;
		case 6:
			X_value = (rGPFDAT >> 6) & 0x1;	// X6
			break;
		case 7:
			X_value = (rGPFDAT >> 7) & 0x1;	// X7
			break;
		case 8:
			X_value = (rGPGDAT >> 0) & 0x1;	// X8	
			break;
		case 9:
			X_value = (rGPGDAT >> 1) & 0x1;	// X9
			break;
		case 10:
			X_value = (rGPGDAT >> 2) & 0x1;	// X10
			break;
		case 11:
			X_value = (rGPGDAT >> 3) & 0x1;	// X11
			break;
		case 12:
			X_value = (rGPGDAT >> 4) & 0x1;	// X12
			break;
		case 13:
			X_value = (rGPGDAT >> 5) & 0x1;	// X13
			break;
		case 14:
			X_value = (rGPGDAT >> 6) & 0x1;	// X14
			break;
		case 15:
			X_value = (rGPGDAT >> 7) & 0x1;	// X15	
	}

	return X_value;// xuzhiqin tixing xiugai
}


////////////////////////////////////////////////////////////////
////	输出   Y2~Y15    /////////////////////////
////	Y_num:2~15		Value:   0或1    		//////
////////////////////////////////////////////////////////////////
void Set_Y_Value(unsigned char Y_num, unsigned char Value)
{
	Value = !Value;// xuzhiqin tixing xiugai
	
	switch(Y_num)
	{
	/*case 1:
		(Value) ? (rGPBDAT |= (1<<1)) : (rGPBDAT &= ~(1<<1));	// Y1	需先配置引脚功能为Output
		break;
	case 2:
		(Value) ? (rGPBDAT |= (1<<2)) : (rGPBDAT &= ~(1<<2));	// Y2	需先配置引脚功能为Output
		break;
	case 3:
		(Value) ? (rGPBDAT |= (1<<3)) : (rGPBDAT &= ~(1<<3));	// Y3	需先配置引脚功能为Output
		break;*/
		
	case 4:
		(Value) ? (rGPEDAT |= (1<<0)) : (rGPEDAT &= ~(1<<0));	// Y4	
		break;
	case 5:
		(Value) ? (rGPEDAT |= (1<<1)) : (rGPEDAT &= ~(1<<1));	// Y5	
		break;
	case 6:
		(Value) ? (rGPEDAT |= (1<<2)) : (rGPEDAT &= ~(1<<2));	// Y6	
		break;
	case 7:
		(Value) ? (rGPEDAT |= (1<<3)) : (rGPEDAT &= ~(1<<3));	// Y7	
		break;
	case 8:
		(Value) ? (rGPEDAT |= (1<<4)) : (rGPEDAT &= ~(1<<4));	// Y8	
		break;
	case 9:
		(Value) ? (rGPEDAT |= (1<<5)) : (rGPEDAT &= ~(1<<5));	// Y9	
		break;
	case 10:
		(Value) ? (rGPEDAT |= (1<<6)) : (rGPEDAT &= ~(1<<6));	// Y10	
		break;
	case 11:
		(Value) ? (rGPEDAT |= (1<<7)) : (rGPEDAT &= ~(1<<7));	// Y11	
		break;
	}

	
}





