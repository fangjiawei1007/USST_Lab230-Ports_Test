
void Test_In_Init(void);	// IN 5~24 ��20 ·
void Test_Out_Init(void);		// OUT 3~16 ��14 ·
void Test_Init(void);

void Test_HighSpeedIn_Init(void);	// IN 1~4   4·��������
void Test_HighSpeedIn_Stop(void);
//static void __irq Eint2Int(void);
//static void __irq Eint3Int(void);
//static void __irq Eint4_7Int(void);

void Test_HighSpeedOut_Init(void);	// OUT 1~2   2·�������
void HS_Out1_Start(void);
void HS_Out1_Stop(void);
void HS_Out2_Start(void);
void HS_Out2_Stop(void);
void __irq Timer1_HS_Out1(void); // ��ͨ��ʾ����ȷ��Ƶ��
void __irq Timer0_HS_Out2(void); // ��ͨ��ʾ����ȷ��Ƶ��

void Test_DA_Init(void);
void DA_Start(void);
void DA_Stop(void);
void __irq Timer3_DA(void); // ��ͨ��ʾ����ȷ��Ƶ��


void Test_GetIn(void);
void Test_SetOut(void);
void Test_Run(void);



