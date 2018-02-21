
//void __irq PulseIn_0_Process(void);
void __irq PulseIn_1_Process(void);
//void __irq PulseIn_2_3_Process(void);

void InPulse1_Start(void);
void InPulse1_Stop(void);
void InPulse2_Start(void);
void InPulse2_Stop(void);
void InPulse3_Start(void);
void InPulse3_Stop(void);
void InPulse4_Start(void);
void InPulse4_Stop(void);
void InPulse5_Start(void);
void InPulse5_Stop(void);
void InPulse6_Start(void);
void InPulse6_Stop(void);
void InPulse7_Start(void);
void InPulse7_Stop(void); 
void InPulse8_Start(void);
void InPulse8_Stop(void); 
void InPulse9_Start(void);
void InPulse9_Stop(void); 
void InPulse10_Start(void);
void InPulse10_Stop(void); 
void InPulse11_Start(void);
void InPulse11_Stop(void); 
void InPulse12_Start(void);
void InPulse12_Stop(void); 
void InPulse13_Start(void);
void InPulse13_Stop(void); 
void InPulse14_Start(void);
void InPulse14_Stop(void); 
void InPulse15_Start(void);
void InPulse15_Stop(void);

void Pulse_In_Init(void);

//void __irq PulseOut_0_Process(void) ;
//void PulseOut_0_Start(unsigned int frequence, unsigned int pulse_PreSet);
//void PulseOut_0_Stop(void);
void __irq PulseOut_1_Process(void);

void Pulse_Out_Init(void);

void PulseOut_1_Start(unsigned int frequence, int pulse_PreSet);
void PulseOut_1_Stop(void);

void PulseOut_2_Start(unsigned int frequence, int pulse_PreSet);
void PulseOut_2_Stop(void);

void PulseOut_3_Start(unsigned int frequence, int pulse_PreSet);
void PulseOut_3_Stop(void);




void __irq DA0_10_Process(void);
void DA0_10_Start(float voltage);
void DA0_10_Stop(void);
void DA0_10_Init(void);



void X_In_Init(void);
void Y_Out_Init(void);		



unsigned char Get_X_Value(unsigned char X_num);
void Set_Y_Value(unsigned char Y_num, unsigned char Value);

////Timer0////
int Delay_Tx_Start(unsigned long int , unsigned long int);
void Delay_Tx_Stop(unsigned int);

#define	y1_start_button			g_InteralMemory.Bit[10]
#define	y1_fre					g_InteralMemory.Word[10]

#define	y2_start_button			g_InteralMemory.Bit[11]
#define	y2_fre					g_InteralMemory.Word[11]

#define	x1_qingling_button		g_InteralMemory.Bit[12]
#define	x2_qingling_button		g_InteralMemory.Bit[13]

#define	x1_display				g_InteralMemory.Word[12]
#define	x2_display				g_InteralMemory.Word[13]

#define	x1_fre_display			g_InteralMemory.Word[14]
#define	x2_fre_display			g_InteralMemory.Word[15]

#define	y1_display				g_InteralMemory.Word[16]
#define	y2_display				g_InteralMemory.Word[17]

#define	jidianqi_fre			g_InteralMemory.Word[18]
#define	jidianqi_num			g_InteralMemory.Word[19]
#define	jidianqi_qingling		g_InteralMemory.Bit[14]
#define	jidianqi_start			g_InteralMemory.Bit[15]
