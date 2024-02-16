#include	<windows.h>
#include	<stdio.h>
#include	"le.h"

extern	HANDLE	NonOverWriteCreateFile(LPCSTR);
		void	decode_nbp(			unsigned char *,
									char *,
									unsigned long		);
extern	void	decode_palette_8bit(unsigned char *);
extern	void	tim2_uncompress(	unsigned char *,
									unsigned char *,
									unsigned long,
									unsigned long		);



void	decode_nbp(
	unsigned char		*src,
	char				*name,
	unsigned long		ss				)
{	unsigned long		dx,dy;	//dst
	unsigned long		bx,by;	//bmp
	unsigned long		tl;				//tmp
	unsigned char		*dst;			//dst work
	unsigned char		*org;			//dst original
	unsigned char		*pal;			//src palette
	unsigned char		*bmp;
	char				tfn[0x100];
	HANDLE				hbmp;
	BITMAPFILEHEADER	bf;
	BITMAPINFOHEADER	bi;

	dst=new unsigned char[(*(unsigned long *)src)+0x1000];
	org=dst;
	memset(dst,0x00,(*(unsigned long *)src)+0x1000);
	memset(&bf,0x00,sizeof(BITMAPFILEHEADER));
	memset(&bi,0x00,sizeof(BITMAPINFOHEADER));

	tim2_uncompress(src+4,dst,ss,*(unsigned long *)src);

nest_nbp:
				dx= *(unsigned short *)(dst+0x58);
	if(dx&0x7)	dx=(dx+8)&0xfffffff8;
				dy= *(unsigned long  *)(dst+0x54)/dx;
	if(dy==0)	dy=(*(unsigned long  *)(dst+0x28)-0x60)/dx;

	pal=dst+*(unsigned long *)(dst+0x24);
	if(pal==dst){									//paletteless ?
		printf("paletteless nbp\n");
		goto	escape_bmp;			}

	for(bx=0;bx<0x400;bx+=4){						//pal•ÏŠ·
		tl		=ler4(pal+bx);
		*(pal+bx+0)	=(unsigned char)(tl>> 8);		//B
		*(pal+bx+1)	=(unsigned char)(tl>>16);		//G
		*(pal+bx+2)	=(unsigned char)(tl>>24);		//R
		*(pal+bx+3)	=(unsigned char)(tl>> 0);	}	//X
	decode_palette_8bit(pal);						//ps2 pal•ÏŠ·

	sprintf(tfn,"%s.bmp",name);
	hbmp=NonOverWriteCreateFile((LPCSTR)tfn);
	if(hbmp==(HANDLE)-1){
		printf("\tError\tcan't create bmp file\n");
		exit(-1);									}

	*((char *)&bf.bfType+0)='B';
	*((char *)&bf.bfType+1)='M';
	bf.bfSize=			sizeof(BITMAPFILEHEADER)+
						sizeof(BITMAPINFOHEADER)+
						dx*dy;								//image
	bf.bfOffBits=		sizeof(BITMAPFILEHEADER)+
						sizeof(BITMAPINFOHEADER);
	bi.biSize=			sizeof(BITMAPINFOHEADER);
	bi.biWidth=			dx;
	bi.biHeight=		dy;
	bi.biPlanes=		1;
	bi.biBitCount=		32;
	WriteFile(hbmp,&bf,sizeof(BITMAPFILEHEADER),&tl,NULL);
	WriteFile(hbmp,&bi,sizeof(BITMAPINFOHEADER),&tl,NULL);

	bmp=new unsigned char[dx*dy*4];
	for(by=0;by<dy;by++)
		for(bx=0;bx<dx;bx++)
			memcpy(bmp+by*dx*4+bx*4,pal+*(dst+0x60+(dy-by-1)*dx+bx)*4,4);
	WriteFile(hbmp,bmp,dx*dy*4,&tl,NULL);
	CloseHandle(hbmp);
	delete[]	bmp;
escape_bmp:

	if(		 *(unsigned long *)(dst+0x18)!=0){		//next nbp pointer
		dst+=*(unsigned long *)(dst+0x18);
		goto	nest_nbp;					}
	delete[]	org;								//original dst pointer
}
