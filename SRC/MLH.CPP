#include	<windows.h>
#include	<stdio.h>
#include	"le.h"

extern	HANDLE	NonOverWriteCreateFile(LPCSTR);
extern	void	decode_nbp(unsigned char *,char *,unsigned long);
extern	void	decode_vas(unsigned char *,char *);
		void	decode_mlh(unsigned char *,char *);



void	decode_mlh(unsigned char *src,char *_name)
{
	HANDLE			hbin;
	char			name[0x20];
	char			drive[MAX_PATH];
	char			dir[MAX_PATH];
	char			fname[MAX_PATH];
	char			ext[MAX_PATH];
	unsigned char	*ITB=src+	*(unsigned long *)(src+0x28);	//Item Table
	unsigned long	 IMX=		*(unsigned long *)(src+0x24);	//Item NO. MAX
	unsigned long	 INO;										//Item NO.
	unsigned long	tmp;

	for(INO=0;INO!=IMX;INO++){
//		printf("ITB=%08X\n",(unsigned long)(ITB - src));

		printf("%s\n",ITB);
		sprintf(name,"%s",ITB);

/*		_splitpath(name,drive,dir,fname,ext);
		if(	strcmp(_strlwr(ext),".nbp")==0)
			decode_nbp(src+	*(unsigned long *)(ITB+0x18),name
				,*(unsigned long *)(ITB+0x10));
		if(	strcmp(_strlwr(ext),".vas")==0)
			decode_vas(src+	*(unsigned long *)(ITB+0x18),name
//				,*(unsigned long *)(ITB+0x10));
																);
*/

		hbin=NonOverWriteCreateFile((LPCSTR)name);
		if(hbin==(HANDLE)-1){
			printf("\tError\tcan't create file\n");
			exit(-1);	}

		printf("%08X - %08X\n",	*(unsigned long *)(ITB+0x18),
								*(unsigned long *)(ITB+0x10)	);

		WriteFile(hbin,src+	*(unsigned long *)(ITB+0x18),
							*(unsigned long *)(ITB+0x10),&tmp,NULL);
		CloseHandle(hbin);

		ITB=src+			*(unsigned long *)(ITB+0x1c);		//Item Table
	}
}
