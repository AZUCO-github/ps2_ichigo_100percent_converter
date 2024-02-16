///////////////////////////////////////////////////////////////////////////////
//	PS2 いちご100% converter
//		NFP format
//			050331
//				by	AZUCO
///////////////////////////////////////////////////////////////////////////////

#include	<windows.h>
#include	<stdio.h>

extern	void	decode_bin(HANDLE,unsigned long,unsigned long,char *);



/*****メモ*****NFP フォーマット************************************************
0x0000	シグネチャ
0x0034	アイテム数
0x0038	アイテムテーブル開始位置(byte) 
0x003c	一番最初のアイテムの実体開始位置(byte) 

アイテムテーブル
0x0000	8.3ファイル名(null) 
0x000c	ファイルの実際の長さ(byte) 
0x0018 ファイル開始位置(byte) 
0x001c ファイルの長さ(0x800 align byte) 
******************************************************************************/



void	decode_nfp(HANDLE hndl)
{
	char			ITB[0x20];	//Item Table Buffer
	unsigned long	IMX;		//Item MAX NO.
	unsigned long	ITA;		//Item Table Addr
	unsigned long	INO;		//Item NO.
	unsigned long	IDA;		//Item Data Address
	unsigned long	ILN;		//Item LeNgth
	unsigned long	tmp;

	SetFilePointer(hndl,0x20,0,FILE_BEGIN);
	ReadFile(hndl,ITB,0x20,&tmp,NULL);
	IMX=*(unsigned long*)(ITB+0x0014);
	ITA=*(unsigned long*)(ITB+0x0018);

	for(INO=0;IMX!=INO;INO++){
		SetFilePointer(hndl,ITA+INO*0x20,0,FILE_BEGIN);
		ReadFile(hndl,ITB,0x20,&tmp,NULL);
		IDA=*(unsigned long*)(ITB+0x18);
		ILN=*(unsigned long*)(ITB+0x0c);
		*(ITB+0x0c)=0;						//file name + null

printf("No.%04X %08X %08X %s\n",INO,IDA,ILN,ITB);
		decode_bin(hndl,IDA,ILN,ITB);
		goto CONV_DONE;

CONV_DONE:
		NULL;
	}
}
