#include	<windows.h>
#include	<stdio.h>
#include	"le.h"

extern	HANDLE	NonOverWriteCreateFile(LPCSTR);
		void	decode_nmd(unsigned char *,char *,unsigned long);



void	decode_nmd(unsigned char *src,char *name,unsigned long srcs)
{
	char				tfn[0x100];
	unsigned long		s;
	unsigned long		sadd;
	unsigned long		slen;
	unsigned long		scrr;

	unsigned long		d;
	unsigned char		*dst;
	unsigned char		flug;
	unsigned long		dsts;
	unsigned long		tul1;

	HANDLE				hbin;

	dsts=*(unsigned long *)src;
	dst=new unsigned char[dsts+0x800];
	memset(dst,0x00,dsts+0x800);



	s=4;	d=0;
	for(;s<srcs;){
		flug=*(src+s);	s++;
//		printf("F:%02X\n",flug);

		for(tul1=0;tul1<8;tul1++){
			if(flug&1){
//				printf("\tu\tP:%08X X:%03X Y:%03X V:%02X\n",
//					d,(d-0x60)%dx,(d-0x60)/dx,*(src+s));
				*(dst+d)=*(src+s);
				d++;			s++;
			}else{
				slen=3+(0x0f& *(src+s+1));			//fix
				sadd=(((*(src+s+1))<<4)&0xf00)|(*(src+s));
				scrr=(d-sadd-0x12)&0xfffff000;
				if(scrr&0x80000000)	scrr=0;

//				printf("\tc\tP:%08X X:%03X Y:%03X D:%03X L:%02X S:%08X\n",
//					d,(d-0x60)%dx,(d-0x60)/dx,sadd,slen,scrr+sadd+0x12+slen);

//				if(d<=(scrr+sadd+0x12+slen))
				if(d<(scrr+sadd+0x12+slen))
					memset(dst+d,*(dst+scrr+sadd+0x12),slen);
				else{
					memcpy(dst+d,dst+scrr+sadd+0x12,slen);
				}
					d+=slen;	s+=2;
			}
			flug>>=1;
			if(d>=dsts)	goto	pass1_end;
			if(s>=srcs)	goto	pass1_end;
		}
	}
pass1_end:

	sprintf(tfn,"%s.bin",name);
	hbin=NonOverWriteCreateFile((LPCSTR)tfn);
	if(hbin==(HANDLE)-1){
		printf("\tError\tcan't create dst file\n");
		exit(-1);									}
	WriteFile(hbin,dst,dsts,&tul1,NULL);
	CloseHandle(hbin);

	delete[]	dst;
}
