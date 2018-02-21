#ifndef __DEF_H__
#define __DEF_H__

#define U32 unsigned int
#define UINT32 unsigned int
#define INT32U unsigned int
#define INT32	int
#define S32 int

#define U16 unsigned short
#define UINT16 unsigned short
#define INT16U unsigned short
#define INT16	short int
#define S16 short int

#define U8  unsigned char
#define UINT8  unsigned char
#define INT8U  unsigned char
#define INT8 char
#define	S8  char

#define	BYTE	char
#define	WORD 	short
#define	DWORD	int
#define	UINT	U32
//#define	LPSTR	U8 *		

typedef unsigned char BOOL;
//typedef unsigned char   BYTE;
//typedef BYTE *LPBYTE;


#define TRUE 	1   
#define FALSE 	0
#define OK		1
#define FAIL		0


////2015-6-26--Timer0/////
#define Timer0_count_SX   600000
#define NUMBER    50
/*
#define	SIZE_1K		0x00000400
#define	SIZE_2K		0x00000800
#define	SIZE_4K		0x00001000
#define	SIZE_8K		0x00002000
#define	SIZE_16K	0x00004000
#define	SIZE_32K	0x00008000
#define	SIZE_64K	0x00010000
#define	SIZE_128K	0x00020000
#define	SIZE_256K	0x00040000
#define	SIZE_512K	0x00080000
#define	SIZE_1M		0x00100000
#define	SIZE_2M		0x00200000
#define	SIZE_4M		0x00400000
#define	SIZE_8M		0x00800000
#define	SIZE_16M	0x01000000
#define	SIZE_32M	0x02000000
#define	SIZE_64M	0x04000000
#define	SIZE_128M	0x08000000
#define	SIZE_256M	0x10000000
#define	SIZE_512M	0x20000000
#define	SIZE_1G		0x40000000
#define	SIZE_2G		0x80000000

#define	ENTER_KEY	0x0d
#define	BACK_KEY	0x08
#define BEEP_KEY	0x07
#define UP_KEY		0x41
#define DOWN_KEY	0x42
#define RIGHT_KEY	0x43
#define LEFT_KEY		0x44
#define HOME_KEY	0x48
#define END_KEY		0x4b
#define	ESC_KEY	0x1b


#define NAND_SB_PAGE_SIZE_BYTES	(0x200)		// 1 Page = 0x200 (512 Bytes)
#define NAND_SB_BLOCK_SIZE_BYTES	(0x4000)	// 1 Block = 16 KB
#define NAND_SB_PAGES_PER_BLOCK	(NAND_SB_BLOCK_SIZE_BYTES / NAND_SB_PAGE_SIZE_BYTES)	// 32-pages

#define NAND_LB_PAGE_SIZE_BYTES	(0x800)		// 1 Page = 0x800 (2048 Bytes)
#define NAND_LB_BLOCK_SIZE_BYTES	(0x20000)	// 1 Block = 128 KB
#define NAND_LB_PAGES_PER_BLOCK	(NAND_LB_BLOCK_SIZE_BYTES / NAND_LB_PAGE_SIZE_BYTES)	// 64-pages


#define NAND_COPY_PAGE_OFFSET   (2*((bLARGEBLOCK==TRUE)?NAND_LB_PAGES_PER_BLOCK:NAND_SB_PAGES_PER_BLOCK))

#define LOAD_ADDRESS_PHYSICAL   0x30038000
#define LOAD_SIZE_BYTES		 0x00040000
#define LOAD_SIZE_PAGES		 (LOAD_SIZE_BYTES / ((bLARGEBLOCK==TRUE)?NAND_LB_PAGE_SIZE_BYTES:NAND_SB_PAGE_SIZE_BYTES))
*/


/*
typedef struct {

	//unsigned long boot_key;
	unsigned char Os_Auto_Flag;
	char boot_params[256];

	//
}EnvParams;
static EnvParams Env;
*/


/////////////////////////////////////////////////////////////////////////
#define BLACK 0x0000
//#define WHITE ( (0x1f<<11) | (0x3f<<5) | (0x1f) )
#define WHITE ( 0xffff )
#define RED ( (0x1f<<11) | (0x00<<5) | (0x00) )
#define GREEN ( (0x00<<11) | (0x3f<<5) | (0x00) )
#define BLUE ( (0x00<<11) | (0x00<<5) | (0x1f) )
#define GREEN_BLUE ( (0x00<<11) | (0x3f<<5) | (0x1f) )
//#define GREEN_BLUE ( 0x00 | 0xff | 0xff )



#define HZK8x8     0
#define HZK16x16   1
#define HZK24x24   2

#define RAISE 		0
#define INSERT 		1

#define RefreshNum     100

#define DOWNLOAD 	-1
#define FP0 		0
#define NANO 		1
#define FX 		2

#define VERSION1		0x02						 //
#define VERSION2		0x06
#define VERSION3		0x03

#define PAGE_SIZE		2048

#define PI  3.1415926535

#define	TSP_SAMPLE_NUM		6 	// 4
#define TSP_INVALIDLIMIT		40	//40   10   20
#define FILTER_LIMIT 			25

#define SAMPLE_TIMES			20	// 20

#define KEYBOARDOFFSET 		100

#define RECT    1
#define CIRCLE  0



#define NAND_SCR_INDEX		21	// ����������ϵͳ��Ϣ
#define NAND_SYS_CONF		22	// ��0ҳ������ϵͳ���ã��������ã�
#define NAND_DATA_SAVE	23	// ������ΪKeepBit��KeepWord��
#define NAND_SCR_INFO		24	// �߼���4�鿪ʼ�Ż�����Ϣ


//#define  PING_CTRL	2			//��������
//#define  PING_STATE	3			//��״̬��



#define ERROR_NUM				3000	// 485ͨѶʱ��ֻҪ400�Ϳ��Խ��յ�
										//ͨѶ�ȴ�����


#define NandFlashID		0xecf1

#define SJF_Ram_Base	0x32000000	
#define SJF_Ram_END	0x32400000	// 4M Ram ��


#define VERSION "Ver3.0.0 2013-05-01"


#define UART_PrintStr		0		// �Ƿ񴮿����������Ϣ
									// 1 ��������������ʲ����޸�								
#define Nand_SJF		(UART_PrintStr)	// SJF ��Ҫͨ��DNW , DNW �õĴ��ڴ�ӡ
									//  1:  ����SJF ��д����
									// 0: ������SJF ��д����

#define Encrypt				1
#define ProductTest			!Encrypt		// 0		//(!Encrypt)

#define Rotate180			0


#define	LW						1		// ��ֵ��ʾ��ť���߿�


//���봦��070601
#define  STARSCR_ENCRYPT	71	//���ȵ��ں���ת�Ļ���
#define  RUNSCR		1			//�趨ʱ��󰴿�ʼ���к����������
#define  PING_CTRL	100			//��������
#define  PING_STATE	101			//��״̬��
#define  ENCRYPT_STARTSCR	61	//�������ÿ�ʼ����
#define  ENCRYPT_ENDSCR		81	//�������ý�������


#define	BLOCK_Password			500		// ��500�������
#define	BLOCK_PasswordFlag		501		// ��501��������־
//#define	BLOCK_ErrorList			502		// ��502�������б�


#define BEEP_DELAY				3500000	// 500000   3500000  //TJ 2015-6-27����Ϊ3500 ԭ��ֵΪ3500000
#define DELAY_TIME_RTU			5000	// RTU ͨѶ��ʱʱ��10000

//#define SCR_RELATIVE			30		// ��Ӣ�Ļ���������


#define DELAY_TIME				10000	// ��ʱʱ��5000(�����)				50000  20000
#define DELAY_TIME_TRAN		10000	// ��Ƶ����ʱʱ��50000(�����)	50000  20000


#endif /*__DEF_H__*/

