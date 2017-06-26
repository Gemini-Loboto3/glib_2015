/*
   Moon Reader

   Description: Module for iso management
   Last update: 8/11/2005
   Author: Gemini
*/

#include <stdafx.h>
#include "iso_img.h"

static void relocate(FILE *input, TOC_RECORD *record, int blocks)
{
	ULONG actual, jump;
	BYTE buffer[32];

	for(int i=0, counter=0; counter<blocks; i++)
	{
		fread(&record->lng_rec,1,1,input);
		jump=2352-ftell(input)%2352;
		if(record->lng_rec<1 || jump<=280)
		{
			counter++;
			if(counter<blocks)
			{
				jump=2352-ftell(input)%2352;
				fseek(input,ftell(input)+jump+24,SEEK_SET);
				fread(&record->lng_rec,1,1,input);
			}
			else break;
		}
		fread(&buffer,32,1,input);
		memcpy(&record->exd_lng_attr,buffer,32);
		fread(&record->file_id,record->lng_file_id,1,input);
		fread(&record->pad_fld,record->lng_rec-33-record->lng_file_id-10,1,input);
		fread(&record->sys_use,sizeof(record->sys_use),1,input);
		if(record[i].flags&0x2)		// is a directory
		{
			if(record[i].lng_file_id>0x01)
			{
				actual=ftell(input);
				fseek(input,record->ext_loc[0]*2352+24,SEEK_SET);
				//sprintf(folder,"%s",&record->file_id);
				//fprintf(output,"<-- %s\n",folder);
				//relocate(record->data_lng[0]/2048);
				fseek(input,actual,SEEK_SET);
				//fprintf(output,"-->\n");
			}
		}
		else						// is a file
		{
			strtok(record[i].file_id, ";");
			printf("%s\n",record[i].file_id);
			//fprintf(output,"%s, ",/*folder,*/record[i].file_id);
			//for(int j=0; j<10; j++) fprintf(output,"%.2X",record[i].sys_use[j]);
			//fprintf(output,"\n");
		}
	}
}

CIso::CIso()
{
}

CIso::CIso(LPCTSTR filename)
{
	OpenIso(filename);
}

CIso::~CIso()
{
}

// Open an handle to the iso file
int CIso::OpenIso(LPCTSTR filename)
{
	m_iso=_wfopen(filename,_T("rb+"));
	//if(!m_iso.Open(filename,CFile::modeReadWrite | CFile::shareDenyNone)) return(-1);
	if(!m_iso) return 0;
	ReadBoot();

	return 1;	// no errors occured, return 0
}

void CIso::BuildTree()
{
}

void CIso::BuildSector(BYTE sector[SECTOR_DATA], BYTE dest[SECTOR_FULL])
{
	BYTE buffer[SECTOR_ECD];

	memset(buffer,0,SECTOR_ECD);
	memcpy(&dest,&buffer,SECTOR_FULL);
}

// close the handle to the iso file
void CIso::CloseIso()
{
	//m_iso.Close();
	fclose(m_iso);
}

int CIso::ExtractFile(int entry, LPCTSTR filename)
{
	/*FILE *out;
	BYTE buffer[SECTOR_DATA];
	out=_wfopen(filename,_T("wb"));
	if(out==NULL) return 0;
	//m_iso.Seek(toc[entry].lba*2352,CFile::begin);
	for(int dim=0; dim<(int)toc[entry].size/2048; dim++)
	{
		ReadSectorData(buffer);
		fwrite(buffer,2048,1,out);
	}
	if(toc[entry].size%2048!=0)
	{
		m_iso.Seek(24,CFile::current);
		m_iso.Read(buffer,toc[entry].size%2048);
		fwrite(buffer,toc[entry].size%2048,1,out);
	}
	fclose(out);*/

	return 1;
}

BYTE* CIso::GetBoot()
{
	return((BYTE*)boot);
}

// return actual seek position
ULONG CIso::GetSeek()
{
	//return(m_iso.GetPosition());
	return ftell(m_iso);
}

void CIso::ReadBoot()
{
	ReadSectors(boot,0,BOOT_SIZE);
}

// read user data from the actual sector
void CIso::ReadSectorData(BYTE *data)
{
	BYTE sector[SECTOR_FULL];
	//m_iso.Read(sector,SECTOR_FULL);
	fread(sector,SECTOR_FULL,1,m_iso);
	memcpy(data,sector+24,SECTOR_DATA);
}

void CIso::ReadForm2(BYTE *data)
{
	BYTE sector[SECTOR_FULL];
	//m_iso.Read(sector,SECTOR_FULL);
	fread(sector,SECTOR_FULL,1,m_iso);
	memcpy(data,sector+16,SECTOR_FORM2);
}

void CIso::ReadForm2(BYTE *data, int sectors)
{
	BYTE sector[SECTOR_FULL];
	for(int i=0; i<sectors; i++)
	{
		//m_iso.Read(sector,SECTOR_FULL);
		fread(sector,SECTOR_FULL,1,m_iso);
		memcpy(data+i*SECTOR_FORM2,sector+16,SECTOR_FORM2);
	}
}

// read user data from a specified sector
void CIso::ReadSectorData(ULONG sector, BYTE *data)
{
	BYTE full[SECTOR_FULL];
	SeekToSector(sector);
	//m_iso.Read(full,SECTOR_FULL);
	fread(full,SECTOR_FULL,1,m_iso);
	memcpy(data,full+24,SECTOR_DATA);
}

void CIso::ReadSectors(BYTE *dest, ULONG sector, int count)
{
	SeekToSector(sector);
	//m_iso.Read(dest,count*SECTOR_FULL);
	fread(dest,SECTOR_FULL,count,m_iso);
}

void CIso::ReadSectors(BYTE *dest, int count)
{
	//m_iso.Read(dest,count*SECTOR_FULL);
	fread(dest,SECTOR_FULL,count,m_iso);
}

void CIso::Read(void* dest, int size)
{
	int loop=size/2048;
	int rest=size%2048;
	BYTE *dest_ptr=(BYTE*)dest;
	for(int i=0; i<loop; i++, dest_ptr+=SECTOR_DATA) ReadSectorData(dest_ptr);
	if(rest!=0)
	{
		BYTE temp[SECTOR_DATA];
		ReadSectorData(temp);
		memcpy(dest_ptr,temp,rest);
	}
}

void CIso::ReplaceFile(ULONG index, BYTE *buffer, ULONG size)
{
	/*int loop, rest;
	ULONG lba=toc[index].lba;
	BYTE data[2048]={0}, sector[2352];
	if(size>toc[index].size) size=toc[index].size;

	loop=size/2048;
	rest=size%2048;
	SeekToSector(toc[index].lba);
	for(int i=0; i<loop; i++)
	{
		m_isolib.EncodeMode2Form1(buffer+i*2048,lba,sector,8);
		m_iso.Write(&sector,2352);
		lba++;
	}
	if(rest)
	{
		memcpy(data,buffer+i*2048,rest);
		m_isolib.EncodeMode2Form1(data,lba,sector,0x89);
		m_iso.Write(&sector,2352);
	}*/
}

// seek to the beginning of a specified sector
void CIso::SeekToSector(ULONG sector)
{
	//m_iso.Seek(sector*SECTOR_FULL,CFile::begin);
	fseek(m_iso,SECTOR_FULL*sector,SEEK_SET);
}

void CIso::WriteSector(BYTE *sector)
{
	//m_iso.Write(sector,SECTOR_FULL);
	fwrite(sector,SECTOR_FULL,1,m_iso);
}

///////////////////////////////////////////////////////////
CFileEntry::CFileEntry()
{
	size=0;
	pos=0;
	ZeroMemory(rec_date,sizeof(rec_date));
	next=NULL;	// end of the list
}

///////////////////////////////////////////////////////////