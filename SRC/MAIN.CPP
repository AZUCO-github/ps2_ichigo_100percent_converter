///////////////////////////////////////////////////////////////////////////////
//	PS2 いちご100% converter
//		050331
//			by	AZUCO
///////////////////////////////////////////////////////////////////////////////

#include	<windows.h>
#include	<stdio.h>

extern	void	decode_nfp(HANDLE);
extern	void	decode_vas(unsigned char *,char *);
extern	void	decode_mlh(unsigned char *,char *);
extern	void	decode_nbp(unsigned char *,char *,unsigned long);
extern	void	decode_nmd(unsigned char *,char *,unsigned long);
extern	void	overlay_bmp(unsigned char *,char *);



void	main(void)
{	char			file[_MAX_PATH];
	char			*tfpa;
	char			*tfpb;
	HANDLE			hndl;
	unsigned char	*buff=0;
	unsigned long	size;
	char			drive[MAX_PATH];
	char			dir[MAX_PATH];
	char			fname[MAX_PATH];
	char			ext[MAX_PATH];
	unsigned long	tmp;

	printf("PS2 Ichigo 100%% Converter\n");
	printf("\t\tProg. By AZUCO\n\n");



	strcpy(file,GetCommandLine());				//コマンドラインをコピーする

	tfpa=strrchr(file,' ');		//一番最後のスペース以降をファイル名と見なす
	tfpa++;

	tfpb=strrchr(tfpa,'"');		//ダブルクオートありの場合
	if(tfpb!=NULL)	*tfpb=0;

	tfpb=strrchr(tfpa,'>');		//リダイレクトの場合
	if(tfpb!=NULL)	*tfpb=0;



	tfpb=&file[0];
loop:
	if(*tfpa=='\\'){	*(tfpb+0)='\\';
						*(tfpb+1)='\\';
						tfpa++;
						tfpb+=2;
						goto loop;		}
	if(*tfpa>=0x20){	*tfpb=*tfpa;
						tfpa++;
						tfpb++;
						goto loop;		}
						*tfpb=0;



//strcpy(file,"E:\\TMP\\I100\\ETC\\K.NBP");

	hndl=CreateFile(file,
					GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(hndl==(HANDLE)-1){
		printf("Satuki <data file>\n");
		printf("\tError\tsource file not found\n");
		exit(-1);	}

	_splitpath(file,drive,dir,fname,ext);
	if(	strcmp(_strlwr(ext),".nfp")==0)		decode_nfp(hndl);
	else{
		size=GetFileSize(hndl,NULL);
		buff=new unsigned char[size];
		ReadFile(hndl,buff,size,&tmp,NULL);	}
	CloseHandle(hndl);

	if(	strcmp(_strlwr(ext),".vas")==0)		decode_vas(buff,file);
	if(	strcmp(_strlwr(ext),".mlh")==0)		decode_mlh(buff,file);
	if(	strcmp(_strlwr(ext),".nbp")==0)		decode_nbp(buff,file,size);
	if(	strcmp(_strlwr(ext),".nmd")==0)		decode_nmd(buff,file,size);
	if(	strcmp(_strlwr(ext),".bmp")==0)		overlay_bmp(buff,file);
	if(buff!=0)	delete[]	buff;

	
	CloseHandle(hndl);
}
