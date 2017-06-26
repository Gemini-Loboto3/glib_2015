#pragma once
#include "gstring.h"

#define BOOT_SIZE		38

#define SECTOR_FORM2	2336
#define SECTOR_ECD		2448
#define SECTOR_FULL		2352
#define SECTOR_DATA		2048
#define SECTOR_SYNC		16
#define SECTOR_SUB		4
#define SECTOR_TIME		4
#define SECTOR_BOOT		SECTOR_FULL*BOOT_SIZE

typedef struct TOC_RECORD
{
	char lng_rec;			// length for this entry
	char exd_lng_attr;		// extended length attribute
	ULONG ext_loc[2];		// lba
	ULONG data_lng[2];		// size
	BYTE rec_date[7];		// date
	char flags;
	char file_unt_size;
	char itl_gap_size;
	char vol_seq_num[4];
	char lng_file_id;		// file id length
	char file_id[15];		// name
	char pad_fld[20];
	char sys_use[10];
} TOC_RECORD;

typedef struct FILE_ATTRIBUTES
{
	ULONG next;
	TCHAR folder[11];
} FILE_ATTRIBUTES;

class CIso
{
public:
	CIso();
	CIso(LPCTSTR filename);
	~CIso();

	int OpenIso(LPCTSTR filename);
	void CloseIso();
	ULONG GetSeek(void);

	void BuildSector(BYTE sector[SECTOR_DATA], BYTE dest[SECTOR_FULL]);
	int ExtractFile(int entry, LPCTSTR filename);
	BYTE* GetBoot();
	void Read(void* dest, int size);
	void ReadBoot();
	void ReadForm2(BYTE *data);
	void ReadForm2(BYTE *data, int sectors);
	void ReadSectorData(BYTE *data);
	void ReadSectorData(ULONG sector, BYTE *data);
	void ReadSectors(BYTE *dest, int count);
	void ReadSectors(BYTE *dest, ULONG sector, int count);
	void ReplaceFile(ULONG index, BYTE *buffer, ULONG size);
	void SeekToSector(ULONG sector);
	void WriteSector(BYTE *sector);

	void BuildTree();

public:
	FILE* m_iso;
	//CIsoLib m_isolib;

	BYTE boot[SECTOR_BOOT];	// boot sectors
};

class CFileEntry
{
public:
	CFileEntry();
	~CFileEntry();

	GString name;
	ULONG size;
	ULONG pos;
	char rec_date[7];
	char flags;

	CFileEntry *next;	// null terminated
};
