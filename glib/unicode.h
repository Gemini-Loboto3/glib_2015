#pragma once
#include "glibconf.h"
#include "gstring.h"
#include "gfile.h"
#include "types.h"

// u32
#define GetPtrULong(x)		(*(u32*)(x))
#define GetPtrU32(x)		GetPtrULong(x)
// u16
#define GetPtrUShort(x)		(*(u16*)(x))
#define GetPtrU16(x)		GetPtrUShort(x)
// s32
#define GetPtrLong(x)		(*(u32*)(x))
#define GetPtrS32(x)		GetPtrLong(x)
// s16
#define GetPtrShort(x)		(*(s16*)(x))
#define GetPtrS16(x)		GetPtrShort(x)

#define Align(val,align)	((((val-1)/align)+1)*align)

GLIB_API int align(int val, int align);

#define GetPtrULongB(x)		(x[3]|(x[2]<<8)|(x[1]<<16)|(x[0]<<24))
#define GetPtrUShortB(x)	(x[1]|(x[0]<<8))

GLIB_API int StoreFile(LPCTSTR filename, BYTE* &buffer);
GLIB_API void FlushFile(LPCTSTR filename, BYTE *data, int size);
GLIB_API int FileExists(LPCTSTR filename);

GLIB_API int UnicodeToUtf8(LPCWSTR string, char* &dest);
GLIB_API int Utf8ToUnicode(wchar_t* &dest, char* string);
GLIB_API void WriteUtf8(GString str, GFile &file);
GLIB_API void WriteUtf8(GString str, FILE *file);
GLIB_API void WriteBOM(GFile &file);
GLIB_API void WriteBOM(FILE *file);
GLIB_API void WriteBOM(u8* buffer);
GLIB_API int StoreAsciiText(LPCTSTR filename, TCHAR* &buffer);
GLIB_API int StoreUtf8Text(LPCTSTR filename, TCHAR* &buffer);

GLIB_API int ReadLine(TCHAR *data, GString &line);
GLIB_API int ReadBOM(GFile &file);
GLIB_API int ReadBOM(FILE *file);
GLIB_API int ReadBOM(u8* buffer);
GLIB_API void CreateFileList(LPCTSTR folder, LPCTSTR filter, GStringArray &dest);

class GLIB_CLASS CBufferFile
{
public:
	CBufferFile();
	CBufferFile(LPCTSTR filename);
	~CBufferFile();

	void* GetBuffer() {return buffer;};
	int GetSize() {return size;};
	void Reset();
	int StoreFile(LPCTSTR filename);

private:
	u8* buffer;
	int size;
};
