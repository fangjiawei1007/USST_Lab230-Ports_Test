#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "2416addr.h"
#include "2416lib.h"
#include "..\inc\nand.h"
#include "..\inc\def.h"
#include "..\inc\lcd.h"
#include "..\inc\systemconf.h"
#include "..\inc\communication.h"
#include "..\inc\function.h"
#include "..\inc\Rtc.h"
#include "..\inc\download.h"

#include "..\inc\encrypt.h"



extern INTERALMEMORY g_InteralMemory;

extern GLOBALVAR g_GlobalVar;

//extern  CurrentScrNum;	// g_InteralMemory.Word[PING_CTRL] = PreScrNum; �ᵼ���������
//extern  PreScrNum;
extern INT16U CurrentScrNum;
extern INT16U PreScrNum;

extern flag;

extern INT16U g_StarScr;
extern INT16U g_PlcCtrlNum;
extern char g_PlcCtrStr[5];




void Inn_Store_PassFlag_OnOrrOff(void)
{
	INT8U *PageBuff;
	int Block;

	//Programm the Configuration to the NandFlash.
	Block = Logical_To_Physical_Block(BLOCK_PasswordFlag);
	EraseBlock(Block);

	if(1 == g_InteralMemory.KeepBit[6]) //����
	{
		g_InteralMemory.KeepBit[0]=0xff;
		g_InteralMemory.KeepBit[1]=0xff;
		g_InteralMemory.KeepBit[2]=0xff;
		g_InteralMemory.KeepBit[3]=0xff;
	}
	else
	{
		g_InteralMemory.KeepBit[0]=1;
		g_InteralMemory.KeepBit[1]=1;
		g_InteralMemory.KeepBit[2]=1;
		g_InteralMemory.KeepBit[3]=1;
	}
	
	PageBuff = (INT8U *)&g_InteralMemory.KeepBit[0];		//Bit һ���ֽ�

	WritePage(Block,0,PageBuff);
}


void Inn_Store_Password_temp(void)
{
	int j;
	INT8U *PageBuff;
	int Block;	

	//Programm the Configuration to the NandFlash.
	Block = Logical_To_Physical_Block(BLOCK_Password);
	EraseBlock(Block);

//�����ʼ����222222
	for(j=0;j<5;j++)
	{
		g_InteralMemory.KeepWord[300+j*2]=222222 & 0xffff;
		g_InteralMemory.KeepWord[300+j*2+1]=222222>>16;
	}

	g_InteralMemory.KeepWord[320]=222222 & 0xffff;
	g_InteralMemory.KeepWord[321]=222222>>16;

	
	PageBuff = (INT8U *)&g_InteralMemory.KeepWord[300];		//KeepWord�����ֽ�

	WritePage(Block,0,PageBuff);
}




void Encrypt_Init()
{
//��ʾ#
	g_InteralMemory.Word[0]=0xffff;
	g_InteralMemory.Word[1]=0xffff;
//�����������
	g_InteralMemory.Word[400]=0;
	g_InteralMemory.Word[401]=0;
	g_InteralMemory.Word[402]=0;
	g_InteralMemory.Word[403]=0;
	g_InteralMemory.Word[404]=0;

	g_InteralMemory.Bit[202]=0;
	
	
	Inn_Load_Password();
	Inn_Load_PassFlag();


//	Inn_Store_PassFlag_Temp();			//!!!!!!!!!!!!!!!
//	Inn_Store_Password_temp();		//!!!!!!!!!!!!!!!


}



//////////////////////////////////
////��(һ�ϵ�Ͷ�����)
////////////////////////////////
void Inn_Load_Password(void)
{
	INT8U PageBuff[PAGE_SIZE];
	int Block,j;
	//INT16U temp;

	Block = Logical_To_Physical_Block(BLOCK_Password);

	ReadPage(Block,0,PageBuff);

	for(j=0;j<20;j++)  // P_KW300~P_KW339
	{
		g_InteralMemory.KeepWord[300+j*2]=(PageBuff[4*j+1]<<8)|PageBuff[4*j];
		g_InteralMemory.KeepWord[300+j*2+1]=(PageBuff[4*j+3]<<8)|PageBuff[4*j+2];
	}	
	
/////++++++++++++++++++++++++++++++++++++++++++
//���32λȫΪf ������ʼ����
	for(j=0;j<5;j++)
	{
		if((g_InteralMemory.KeepWord[300+j*2] & 0xffff)==0xffff 
			&&  (g_InteralMemory.KeepWord[300+j*2+1] & 0xffff)==0xffff)
		{
			g_InteralMemory.KeepWord[300+j*2]=222222 & 0xffff;
			g_InteralMemory.KeepWord[300+j*2+1]=222222>>16;
		}
	}
	for(j=0;j<3;j++)
	{
		if((g_InteralMemory.KeepWord[320+j*2] & 0xffff)==0xffff 
			&&  (g_InteralMemory.KeepWord[320+j*2+1] & 0xffff)==0xffff)
		{
			g_InteralMemory.KeepWord[320+j*2]=222222 & 0xffff;
			g_InteralMemory.KeepWord[320+j*2+1]=222222>>16;
		}
	}
	
/////++++++++++++++++++++++++++++++++++++++++++

}




void Inn_Load_PassFlag(void)
{
	INT8U PageBuff[PAGE_SIZE];
	int Block,j;
	//INT16U temp;

	Block = Logical_To_Physical_Block(BLOCK_PasswordFlag);

	ReadPage(Block,0,PageBuff);

	for(j=0;j<=8;j++)
	{
		g_InteralMemory.KeepBit[0+j]=PageBuff[j];
	}	

	if(0 != g_InteralMemory.KeepBit[7] && 1 != g_InteralMemory.KeepBit[7])
		g_InteralMemory.KeepBit[7] = 0; //��ʼ��

	if(0 != g_InteralMemory.KeepBit[8] && 1 != g_InteralMemory.KeepBit[8])
		g_InteralMemory.KeepBit[8] = 0; //��ʼ��
	
}


//////////////////////////////////
////д
////////////////////////////////
void Inn_Store_Password(void)
{
	INT8U *PageBuff;
	int Block;

	//Programm the Configuration to the NandFlash.
	Block = Logical_To_Physical_Block(BLOCK_Password);
	EraseBlock(Block);
	PageBuff = (INT8U *)&g_InteralMemory.KeepWord[300];		//KeepWord�����ֽ�

	WritePage(Block,0,PageBuff);
}


void Inn_Store_PassFlag(void)
{
	INT8U *PageBuff;
	int Block;

	//Programm the Configuration to the NandFlash.
	Block = Logical_To_Physical_Block(BLOCK_PasswordFlag);
	EraseBlock(Block);
	PageBuff = (INT8U *)&g_InteralMemory.KeepBit[0];		//Bit һ���ֽ�

	WritePage(Block,0,PageBuff);
}






//////////////////////////////////
////�ܲ���
////////////////////////////////
void Encrypt_Operation()
{
	//Read_DateTime();
	//Write_DateTime();
	RTC_SetTime();  // ��ť����
	RTC_ReadTime(); 


	TimeReach_Judge();
	TimeOUt_Screen();

	Time_Alarm();	

	encrypt();


	if(0 == g_InteralMemory.KeepBit[7]) //��ͣ��
	{
		if(1 == g_InteralMemory.KeepBit[6]) //��������
		{
			Inn_Store_PassFlag_OnOrrOff();	
			g_InteralMemory.KeepBit[7] = 1;
		}
	}
	if(1 == g_InteralMemory.KeepBit[7]) //������
	{
		if(0 == g_InteralMemory.KeepBit[6]) //ʹͣ��
		{
			Inn_Store_PassFlag_OnOrrOff();	
			g_InteralMemory.KeepBit[7] = 0;
		}
	}
	
	
	//��ʼ����
	if(g_InteralMemory.Bit[201]==1)
	{
		g_InteralMemory.Bit[201]=0;
		g_GlobalVar.Stop_Communication=0;
		g_InteralMemory.Word[PING_CTRL]=RUNSCR;//����������
	}
	

	//������󷵻�
	if(g_InteralMemory.Bit[204]==1)
	{
		g_InteralMemory.Bit[204] = 0;
		g_InteralMemory.Word[PING_CTRL] = PreScrNum;
		g_InteralMemory.Bit[205] = 0; //���  ����������־
	}
	
	
}









//////////////////////////////////
////����
////////////////////////////////
void encrypt()
{
////����Ա����


if(g_InteralMemory.Bit[603]==1)  //ȡ��
	{
	
		g_InteralMemory.Bit[603]=0;
		RefreshButtonUp();		
		DisplaySystemScr(0);
		
	
	}
	

	if(g_InteralMemory.Bit[150]==1)
	{
		g_InteralMemory.Bit[150]=0;
		if (g_InteralMemory.KeepWord[300]==g_InteralMemory.Word[150] 
			&&  g_InteralMemory.KeepWord[301]==g_InteralMemory.Word[151])
		{
			g_InteralMemory.Word[PING_CTRL]=65;//����ͳ����
			g_InteralMemory.Word[150]=0;
			g_InteralMemory.Word[151]=0;

			g_InteralMemory.Word[400]=0;
		}
		else	
		{
			g_InteralMemory.Word[PING_CTRL]=75;//��������
			g_InteralMemory.Word[400]++;
		}
//����������󳬹�5��
		if(g_InteralMemory.Word[400]>=5)
		{
			g_InteralMemory.Word[PING_CTRL]=80;
		}		
	}


////��һ�ι�������򸽼���
	if(g_InteralMemory.Bit[155]==1)
	{
		g_InteralMemory.Bit[155]=0;
		if (g_InteralMemory.KeepWord[302]==g_InteralMemory.Word[190] 
			&&  g_InteralMemory.KeepWord[303]==g_InteralMemory.Word[191])
		{
			g_InteralMemory.KeepBit[0]=1;
			Inn_Store_PassFlag();
		
			g_InteralMemory.Word[PING_CTRL]=RUNSCR;//����ʼ����
			g_InteralMemory.Bit[207] = 0; //���   �����������뻭���־
			
			g_InteralMemory.Word[190]=0;
			g_InteralMemory.Word[191]=0;

			g_InteralMemory.Word[401]=0;
		}
		else
		{
			if (g_InteralMemory.KeepWord[308]==g_InteralMemory.Word[192] 
				&&  g_InteralMemory.KeepWord[309]==g_InteralMemory.Word[193]
				&&  g_InteralMemory.KeepBit[3]!=1)
			{
				g_InteralMemory.KeepBit[3]=1;
				Inn_Store_PassFlag();
				Time_Delay_1();
		
				g_InteralMemory.Word[PING_CTRL]=RUNSCR;//����ʼ����
				g_InteralMemory.Bit[207] = 0; //���   �����������뻭���־
				
				g_InteralMemory.Word[192]=0;
				g_InteralMemory.Word[193]=0;

				g_InteralMemory.Word[401]=0;
			}
			else		
			{
				g_InteralMemory.Word[PING_CTRL]=75;//��������
				g_InteralMemory.Bit[205] = 1; //����������־
				g_InteralMemory.Word[401]++;
			}
//����������󳬹�5��
			if(g_InteralMemory.Word[401]>=5)
			{
				g_InteralMemory.Word[PING_CTRL]=80;
			}		
		}
	}

////�ڶ��ι�������򸽼���
	if(g_InteralMemory.Bit[156]==1)
	{
		g_InteralMemory.Bit[156]=0;
		if (g_InteralMemory.KeepWord[304]==g_InteralMemory.Word[194] 
			&&  g_InteralMemory.KeepWord[305]==g_InteralMemory.Word[195])
		{
			g_InteralMemory.KeepBit[1]=1;
			Inn_Store_PassFlag();
		
			g_InteralMemory.Word[PING_CTRL]=RUNSCR;//����ʼ����
			g_InteralMemory.Bit[207] = 0; //���   �����������뻭���־

			g_InteralMemory.Word[194]=0;
			g_InteralMemory.Word[195]=0;

			g_InteralMemory.Word[402]=0;
		}
		else
		{
			if (g_InteralMemory.KeepWord[308]==g_InteralMemory.Word[196] 
				&&  g_InteralMemory.KeepWord[309]==g_InteralMemory.Word[197]
				&&  g_InteralMemory.KeepBit[3]!=1)
			{
				g_InteralMemory.KeepBit[3]=1;
				Inn_Store_PassFlag();
				Time_Delay_2();
				
				g_InteralMemory.Word[PING_CTRL]=RUNSCR;//����ʼ����
				g_InteralMemory.Bit[207] = 0; //���   �����������뻭���־

				g_InteralMemory.Word[196]=0;
				g_InteralMemory.Word[197]=0;

				g_InteralMemory.Word[402]=0;
			}
			else		
			{
				g_InteralMemory.Word[PING_CTRL]=75;//��������
				g_InteralMemory.Bit[205] = 1; //����������־
				g_InteralMemory.Word[402]++;
			}
//����������󳬹�5��
			if(g_InteralMemory.Word[402]>=5)
			{
				g_InteralMemory.Word[PING_CTRL]=80;
			}		
		}
	}

////�����ι�������򸽼���
	if(g_InteralMemory.Bit[157]==1)
	{
		g_InteralMemory.Bit[157]=0;
		if (g_InteralMemory.KeepWord[306]==g_InteralMemory.Word[198] 
			&&  g_InteralMemory.KeepWord[307]==g_InteralMemory.Word[199])
		{
			g_InteralMemory.KeepBit[2]=1;
			Inn_Store_PassFlag();
		
			g_InteralMemory.Word[PING_CTRL]=RUNSCR;//����ʼ����
			g_InteralMemory.Bit[207] = 0; //���   �����������뻭���־

			g_InteralMemory.Word[198]=0;
			g_InteralMemory.Word[199]=0;

			g_InteralMemory.Word[403]=0;
		}
		else
		{
			if (g_InteralMemory.KeepWord[308]==g_InteralMemory.Word[200] 
				&&  g_InteralMemory.KeepWord[309]==g_InteralMemory.Word[201] 
				&&  g_InteralMemory.KeepBit[3]!=1)
			{
				g_InteralMemory.KeepBit[3]=1;
				Inn_Store_PassFlag();
				Time_Delay_3();
		
				g_InteralMemory.Word[PING_CTRL]=RUNSCR;//����ʼ����
				g_InteralMemory.Bit[207] = 0; //���   �����������뻭���־

				g_InteralMemory.Word[200]=0;
				g_InteralMemory.Word[201]=0;

				g_InteralMemory.Word[403]=0;
			}
			else		
			{
				g_InteralMemory.Word[PING_CTRL]=75;//��������
				g_InteralMemory.Bit[205] = 1; //����������־
				g_InteralMemory.Word[403]++;
			}
//����������󳬹�5��
			if(g_InteralMemory.Word[403]>=5)
			{
				g_InteralMemory.Word[PING_CTRL]=80;
			}		
		}
	}






/////�޸Ĺ���Ա����	
	if(g_InteralMemory.Bit[151]==1)
	{
		g_InteralMemory.Bit[151]=0;
		if(g_InteralMemory.KeepWord[300]==g_InteralMemory.Word[168] 
			&&  g_InteralMemory.KeepWord[301]==g_InteralMemory.Word[169] )
		{
			if (g_InteralMemory.Word[170]==g_InteralMemory.Word[172] 
				&& g_InteralMemory.Word[171]==g_InteralMemory.Word[173])
			{
				if(g_InteralMemory.Word[170]!=0 || g_InteralMemory.Word[171]!=0)
				{
					g_InteralMemory.KeepWord[300]=g_InteralMemory.Word[170];
					g_InteralMemory.KeepWord[301]=g_InteralMemory.Word[171];
					Inn_Store_Password();
					g_InteralMemory.Word[PING_CTRL]=65;
					g_InteralMemory.Word[168]=0;
					g_InteralMemory.Word[169]=0;
					g_InteralMemory.Word[170]=0;
					g_InteralMemory.Word[171]=0;
					g_InteralMemory.Word[172]=0;
					g_InteralMemory.Word[173]=0;
				}
			}
			else
				g_InteralMemory.Word[PING_CTRL]=77;//�����Ƚϴ���	
		}		
		else
		{
			g_InteralMemory.Word[PING_CTRL]=75;//����������
		}
	}
	
	


/////���������趨
	if(g_InteralMemory.Bit[152]==1)
	{
		g_InteralMemory.Bit[152]=0;
		Inn_Store_Password();
		g_InteralMemory.Word[PING_CTRL]=65;		//����ͳ����
	}

	
/////��������ʱ���趨	
	if(g_InteralMemory.Bit[153]==1)	
	{
		g_InteralMemory.Bit[153]=0;
		
		g_InteralMemory.Word[PING_CTRL]=65;//����ͳ����
		Inn_Store_Password();					
	}

}
		




//////////////////////////////////
////����ʱ���Ƴ�
////////////////////////////////
void Time_Delay_1()
{
	INT16U year,month,day;
	int password,total;

	password=(g_InteralMemory.KeepWord[308] & 0xffff) | (g_InteralMemory.KeepWord[308+1]<<16);
	total=password/100000%10+password/10000%10+password/1000%10
		+password/100%10+password/10%10+password%10;
	total=total%3;
	
//��һ�ε���ʱ��
	day=g_InteralMemory.KeepWord[312];
	month=g_InteralMemory.KeepWord[311]+total+1;
	year=g_InteralMemory.KeepWord[310];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[310]=year;
	g_InteralMemory.KeepWord[311]=month;
	g_InteralMemory.KeepWord[312]=day;

//�ڶ��ε���ʱ��
	day=g_InteralMemory.KeepWord[315];
	month=g_InteralMemory.KeepWord[314]+total+1;
	year=g_InteralMemory.KeepWord[313];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[313]=year;
	g_InteralMemory.KeepWord[314]=month;
	g_InteralMemory.KeepWord[315]=day;

//�����ε���ʱ��
	day=g_InteralMemory.KeepWord[318];
	month=g_InteralMemory.KeepWord[317]+total+1;
	year=g_InteralMemory.KeepWord[316];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[316]=year;
	g_InteralMemory.KeepWord[317]=month;
	g_InteralMemory.KeepWord[318]=day;


	Inn_Store_Password();
	
}


void Time_Delay_2()
{
	INT16U year,month,day;
	int password,total;

	password=(g_InteralMemory.KeepWord[308] & 0xffff) | (g_InteralMemory.KeepWord[308+1]<<16);
	total=password/100000%10+password/10000%10+password/1000%10
		+password/100%10+password/10%10+password%10;
	total=total%3;
	
//�ڶ��ε���ʱ��
	day=g_InteralMemory.KeepWord[315];
	month=g_InteralMemory.KeepWord[314]+total+1;
	year=g_InteralMemory.KeepWord[313];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[313]=year;
	g_InteralMemory.KeepWord[314]=month;
	g_InteralMemory.KeepWord[315]=day;

//�����ε���ʱ��
	day=g_InteralMemory.KeepWord[318];
	month=g_InteralMemory.KeepWord[317]+total+1;
	year=g_InteralMemory.KeepWord[316];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[316]=year;
	g_InteralMemory.KeepWord[317]=month;
	g_InteralMemory.KeepWord[318]=day;


	Inn_Store_Password();
	
}


void Time_Delay_3()
{
	INT16U year,month,day;
	int password,total;

	password=(g_InteralMemory.KeepWord[308] & 0xffff) | (g_InteralMemory.KeepWord[308+1]<<16);
	total=password/100000%10+password/10000%10+password/1000%10
		+password/100%10+password/10%10+password%10;
	total=total%3;
	
//�����ε���ʱ��
	day=g_InteralMemory.KeepWord[318];
	month=g_InteralMemory.KeepWord[317]+total+1;
	year=g_InteralMemory.KeepWord[316];
	if(month>12)
	{
		month=month-12;
		year++;
	}	
	g_InteralMemory.KeepWord[316]=year;
	g_InteralMemory.KeepWord[317]=month;
	g_InteralMemory.KeepWord[318]=day;


	Inn_Store_Password();
	
}






//////////////////////////////////
////�����ж�
////////////////////////////////
void TimeReach_Judge()
{
//��һ�ε���
	if(g_InteralMemory.Word[330]>g_InteralMemory.KeepWord[310])
	{
		g_GlobalVar.TimeReach_1=1;
	}
	else
	{
		if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[310]
			&& g_InteralMemory.Word[331]>g_InteralMemory.KeepWord[311])
		{
			g_GlobalVar.TimeReach_1=1;
		}
		else
		{
			if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[310]
				&& g_InteralMemory.Word[331]==g_InteralMemory.KeepWord[311]
				&& g_InteralMemory.Word[332]>=g_InteralMemory.KeepWord[312])
			{
				g_GlobalVar.TimeReach_1=1;
			}
			else
			{
				g_GlobalVar.TimeReach_1=0;
			}
		}
	}

//�ڶ��ε���
	if(g_InteralMemory.Word[330]>g_InteralMemory.KeepWord[313])
	{
		g_GlobalVar.TimeReach_2=1;
	}
	else
	{
		if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[313]
			&& g_InteralMemory.Word[331]>g_InteralMemory.KeepWord[314])
		{
			g_GlobalVar.TimeReach_2=1;
		}
		else
		{
			if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[313]
				&& g_InteralMemory.Word[331]==g_InteralMemory.KeepWord[314]
				&& g_InteralMemory.Word[332]>=g_InteralMemory.KeepWord[315])
			{
				g_GlobalVar.TimeReach_2=1;
			}
			else
			{
				g_GlobalVar.TimeReach_2=0;
			}
		}
	}

//�����ε���
	if(g_InteralMemory.Word[330]>g_InteralMemory.KeepWord[316])
	{
		g_GlobalVar.TimeReach_3=1;
	}
	else
	{
		if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[316]
			&& g_InteralMemory.Word[331]>g_InteralMemory.KeepWord[317])
		{
			g_GlobalVar.TimeReach_3=1;
		}
		else
		{
			if(g_InteralMemory.Word[330]==g_InteralMemory.KeepWord[316]
				&& g_InteralMemory.Word[331]==g_InteralMemory.KeepWord[317]
				&& g_InteralMemory.Word[332]>=g_InteralMemory.KeepWord[318])
			{
				g_GlobalVar.TimeReach_3=1;
			}
			else
			{
				g_GlobalVar.TimeReach_3=0;
			}
		}
	}


}




//////////////////////////////////
////�������ڻ���
////////////////////////////////
void TimeOUt_Screen()
{
	if(g_GlobalVar.Stop_Communication==0)
	{
		if(g_InteralMemory.KeepBit[2]!=1)		//û��ȷ���������
		{
			//if(g_GlobalVar.TimeReach_3==1 
			//	&& g_InteralMemory.Word[PING_STATE]!=75  
			//	&&  g_InteralMemory.Word[PING_STATE]!=80)		
			//�Ҳ��ڵ�����ʾ��������������
			if((g_GlobalVar.TimeReach_3==1)
				&& (1 != g_InteralMemory.Bit[203])  
				&&  (1 != g_InteralMemory.Bit[205]))
			{	
				WriteWord(g_PlcCtrlNum,g_PlcCtrStr,64);	//�����ε���
				g_InteralMemory.Bit[207] = 1; //�����������뻭���־
				
/*			
				if(g_StarScr==STARSCR_ENCRYPT)
					g_InteralMemory.Word[PING_CTRL]=64;	//�����ε���
				else
					WriteWord(g_PlcCtrlNum,g_PlcCtrStr,64);	
*/				
/*		
				g_InteralMemory.Word[PING_CTRL]=64;	//�����ε���
				if(g_InteralMemory.Word[PING_CTRL] != CurrentScrNum)
				{
					PreScrNum = CurrentScrNum;
					CurrentScrNum = g_InteralMemory.Word[PING_CTRL];
				}
*/				
			}
		}
		if(g_InteralMemory.KeepBit[1]!=1)		//û��ȷ���������
		{
			//if(g_GlobalVar.TimeReach_2==1 
			//	&& g_InteralMemory.Word[PING_STATE]!=75  &&  g_InteralMemory.Word[PING_STATE]!=80)
			if((g_GlobalVar.TimeReach_2==1)
				&& (1 != g_InteralMemory.Bit[203])  
				&&  (1 != g_InteralMemory.Bit[205]))
			{			
				WriteWord(g_PlcCtrlNum,g_PlcCtrStr,63);		//�ڶ��ε���				
				g_InteralMemory.Bit[207] = 1; //�����������뻭���־
			}
		}
		if(g_InteralMemory.KeepBit[0]!=1)		//û��ȷ���������
		{
			//if(g_GlobalVar.TimeReach_1==1 
			//	&& g_InteralMemory.Word[PING_STATE]!=75 &&  g_InteralMemory.Word[PING_STATE]!=80)
			if((g_GlobalVar.TimeReach_1==1)
				&& (1 != g_InteralMemory.Bit[203])  
				&&  (1 != g_InteralMemory.Bit[205]))
			{			
				WriteWord(g_PlcCtrlNum,g_PlcCtrStr,62);	//��һ�ε���
				g_InteralMemory.Bit[207] = 1; //�����������뻭���־
			}
		}
	}
}



//////////////////////////////////
////������ʾ
////////////////////////////////
void Time_Alarm()
{
	INT16U year,month,day;

	if(g_GlobalVar.Time_Alarm_One==0)
	{
		if(g_InteralMemory.Bit[202]==1)
		{
//			g_InteralMemory.Word[100]=60;
			g_InteralMemory.Word[PING_CTRL] = PreScrNum;
			g_InteralMemory.Bit[203] = 0;	 //���    ������ʾ�����־
			g_GlobalVar.Time_Alarm_One=1;

			g_GlobalVar.Time_Alarm_State = 0;
		}
	}

	if(g_InteralMemory.KeepBit[0]!=1 && g_InteralMemory.Bit[202]!=1)	// ��һ��û����
	{
		year=g_InteralMemory.Word[330];
		month=g_InteralMemory.Word[331];
		day=g_InteralMemory.Word[332];

		day=day+10;
		if(day>28)
		{
			day=day-28;
			month++;
		}
		if(month>12)
		{
			month=month-12;
			year++;
		}	
		
		if(year>g_InteralMemory.KeepWord[310])
		{
			g_InteralMemory.Word[PING_CTRL]=81;
			g_InteralMemory.Bit[203] = 1; //������ʾ�����־
			g_GlobalVar.Time_Alarm_State = 1;

			g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[310];
			g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[311];
			g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[312];
		}
		else
		{
			if(year==g_InteralMemory.KeepWord[310]
				&& month>g_InteralMemory.KeepWord[311])
			{
				g_InteralMemory.Word[PING_CTRL]=81;
				g_InteralMemory.Bit[203] = 1; //������ʾ�����־
				g_GlobalVar.Time_Alarm_State = 1;

				g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[310];
				g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[311];
				g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[312];
			}
			else
			{
				if(year==g_InteralMemory.KeepWord[310]
					&& month==g_InteralMemory.KeepWord[311]
					&& day>=g_InteralMemory.KeepWord[312])
				{
					g_InteralMemory.Word[PING_CTRL]=81;
					g_InteralMemory.Bit[203] = 1; //������ʾ�����־
					g_GlobalVar.Time_Alarm_State = 1;

					g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[310];
					g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[311];
					g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[312];
				}
			}
		}
	}
	else
	{
		if(g_InteralMemory.KeepBit[1]!=1 && g_InteralMemory.Bit[202]!=1)	// �ڶ���û����
		{
			year=g_InteralMemory.Word[330];
			month=g_InteralMemory.Word[331];
			day=g_InteralMemory.Word[332];

			day=day+10;
			if(day>28)
			{
				day=day-28;
				month++;
			}
			if(month>12)
			{
				month=month-12;
				year++;
			}	
			
			if(year>g_InteralMemory.KeepWord[313])
			{
				g_InteralMemory.Word[PING_CTRL]=81;
				g_InteralMemory.Bit[203] = 1; //������ʾ�����־
				g_GlobalVar.Time_Alarm_State = 1;

				g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[313];
				g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[314];
				g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[315];
			}
			else
			{
				if(year==g_InteralMemory.KeepWord[313]
					&& month>g_InteralMemory.KeepWord[314])
				{
					g_InteralMemory.Word[PING_CTRL]=81;
					g_InteralMemory.Bit[203] = 1; //������ʾ�����־
					g_GlobalVar.Time_Alarm_State = 1;

					g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[313];
					g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[314];
					g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[315];
				}
				else
				{
					if(year==g_InteralMemory.KeepWord[313]
						&& month==g_InteralMemory.KeepWord[314]
						&& day>=g_InteralMemory.KeepWord[315])
					{
						g_InteralMemory.Word[PING_CTRL]=81;
						g_InteralMemory.Bit[203] = 1; //������ʾ�����־
						g_GlobalVar.Time_Alarm_State = 1;

						g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[313];
						g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[314];
						g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[315];
					}
				}
			}
		}
		else
		{
			if(g_InteralMemory.KeepBit[2]!=1 && g_InteralMemory.Bit[202]!=1)	// ������û����
			{
				year=g_InteralMemory.Word[330];
				month=g_InteralMemory.Word[331];
				day=g_InteralMemory.Word[332];

				day=day+10;
				if(day>28)
				{
					day=day-28;
					month++;
				}
				if(month>12)
				{
					month=month-12;
					year++;
				}	
				
				if(year>g_InteralMemory.KeepWord[316])
				{
					g_InteralMemory.Word[PING_CTRL]=81;
					g_InteralMemory.Bit[203] = 1; //������ʾ�����־
					g_GlobalVar.Time_Alarm_State = 1;

					g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[316];
					g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[317];
					g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[318];
				}
				else
				{
					if(year==g_InteralMemory.KeepWord[316]
						&& month>g_InteralMemory.KeepWord[317])
					{
						g_InteralMemory.Word[PING_CTRL]=81;
						g_InteralMemory.Bit[203] = 1; //������ʾ�����־
						g_GlobalVar.Time_Alarm_State = 1;

						g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[316];
						g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[317];
						g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[318];
					}
					else
					{
						if(year==g_InteralMemory.KeepWord[316]
							&& month==g_InteralMemory.KeepWord[317]
							&& day>=g_InteralMemory.KeepWord[318])
						{
							g_InteralMemory.Word[PING_CTRL]=81;
							g_InteralMemory.Bit[203] = 1; //������ʾ�����־
							g_GlobalVar.Time_Alarm_State = 1;

							g_InteralMemory.Word[350]=g_InteralMemory.KeepWord[316];
							g_InteralMemory.Word[351]=g_InteralMemory.KeepWord[317];
							g_InteralMemory.Word[352]=g_InteralMemory.KeepWord[318];
						}
					}
				}
			}

		}

	}

}



