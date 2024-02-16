///////////////////////////////////////////////////////////////////////////////
//	PS2 ������100% converter
//		NFP format
//			050331
//				by	AZUCO
///////////////////////////////////////////////////////////////////////////////

#include	<windows.h>
#include	<stdio.h>

extern	void	decode_bin(HANDLE,unsigned long,unsigned long,char *);



/*****����*****NFP �t�H�[�}�b�g************************************************
0x0000	�V�O�l�`��
0x0034	�A�C�e����
0x0038	�A�C�e���e�[�u���J�n�ʒu(byte) 
0x003c	��ԍŏ��̃A�C�e���̎��̊J�n�ʒu(byte) 

�A�C�e���e�[�u��
0x0000	8.3�t�@�C����(null) 
0x000c	�t�@�C���̎��ۂ̒���(byte) 
0x0018 �t�@�C���J�n�ʒu(byte) 
0x001c �t�@�C���̒���(0x800 align byte) 
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