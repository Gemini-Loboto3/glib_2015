#include "stdglib.h"
#include "Unicode.h"

//#include "winldap.h"
//#pragma comment(lib, "wldap32.lib")

int align(int val, int align)
{
	return Align(val,align);
}

static int GetFileSize(FILE *file)
{
	int f_cur=ftell(file);
	fseek(file,0,SEEK_END);
	int size=ftell(file);
	fseek(file,f_cur,SEEK_SET);
	return size;
}

int StoreFile(LPCTSTR filename, BYTE* &buffer)
{
#if 1
	FILE *file;
#else
	GFile file;
#endif
	buffer=NULL;

#if 1
	file=_tfopen(filename,_T("rb"));
	if(!file) return 0;
	int f_size=GetFileSize(file);
#else
	file.Open(filename,GFile::modeRead | GFile::shareDenyNone);
	if(file.GetHandle()==INVALID_HANDLE_VALUE) return 0;
	int f_size=(int)file.GetLength();
#endif

	if(f_size>0)
	{
		buffer=new BYTE[f_size];
#if 1
		fread(buffer,f_size,1,file);
#else
		file.Read(buffer,f_size);
#endif
	}
#if 1
	fclose(file);
#else
	file.Close();
#endif

	return f_size;
}

void FlushFile(LPCTSTR filename, BYTE *data, int size)
{
	//CFile file(filename, CFile::modeCreate | CFile::modeWrite);
	FILE *file=_tfopen(filename,_T("wb"));
	//file.Write(data,size);
	fwrite(data,size,1,file);
	//file.Close();
	fclose(file);
}

int FileExists(LPCTSTR filename)
{
	if(_taccess(filename,0)!=-1) return 1;
	return 0;
}

int UnicodeToUtf8(LPCWSTR string, char* &dest)
{
	//int strlen=LdapUnicodeToUTF8(string,(int)string.GetLength(),dest,0);
	//dest=(char*)new BYTE[strlen];
	//LdapUnicodeToUTF8(string,(int)string.GetLength(),dest,strlen);
	//return(strlen);
	int length=WideCharToMultiByte(CP_UTF8,0,string,-1,NULL,0,(const char*)NULL, (BOOL*)0);
	dest=new char[length];
	WideCharToMultiByte(CP_UTF8,0,string,-1,dest,length,(const char*)NULL,(BOOL*)0);

	return length-1;
}

int Utf8ToUnicode(wchar_t* &dest, char* string)
{
	//int len=LdapUTF8ToUnicode(string,(int)strlen(string),dest,0);
	//dest=(TCHAR*)new TCHAR[len+1];
	//dest[len]=_T('\0');
	//LdapUTF8ToUnicode(string,(int)strlen(string),dest,len);
	//return(len);

	unsigned length = MultiByteToWideChar(CP_UTF8, 0, string, -1, 0, 0);
	dest = new wchar_t[length];
	MultiByteToWideChar(CP_UTF8, 0, string, -1, dest, length);

	return length-1;
}

void WriteUtf8(GString str, GFile &file)
{
	char *dest;
	int strlen=UnicodeToUtf8((LPCWSTR)str,dest);
	file.Write(dest,strlen);
	delete[] dest;
}

void WriteUtf8(GString str, FILE *file)
{
	char *dest;
	int strlen=UnicodeToUtf8((LPCWSTR)str,dest);
	fwrite(dest,strlen,1,file);
	delete[] dest;
}

__inline void WriteBOM(GFile &file)
{
	file.Write("\xEF\xBB\xBF",3);
}

__inline void WriteBOM(FILE *file)
{
	fwrite("\xEF\xBB\xBF",3,1,file);
}

__inline void WriteBOM(u8* buffer)
{
	memcpy(buffer,"\xEF\xBB\xBF",3);
}

// converts to utf16
int StoreAsciiText(LPCTSTR filename, TCHAR* &buffer)
{
	FILE *file=_tfopen(filename,_T("rb+"));
	// error in file
	if(!file) return 0;

	int buf_size=GetFileSize(file);
	BYTE *temp=new BYTE[buf_size];
	fread(temp,buf_size,1,file);
	fclose(file);

	buffer=new TCHAR[buf_size+1];
	buffer[buf_size]=0; // end NULL char
	for(int i=0; i<buf_size; i++) buffer[i]=temp[i];
	delete[] temp;

	// successful
	return 1;
}

int StoreUtf8Text(LPCTSTR filename, TCHAR* &buffer)
{
	FILE *file=_tfopen(filename,_T("rb+"));
	// error in file or BOM
	if(!file) return 0;
	if(!ReadBOM(file))
	{
		//fseek(file,0,SEEK_SET);
		fclose(file);
		return StoreAsciiText(filename,buffer);
	}

	int buf_size=GetFileSize(file);
	BYTE *temp=(BYTE*)new BYTE[buf_size+1];
	fread(temp,buf_size,1,file);
	fclose(file);

	temp[buf_size]=0; // end NULL char
	Utf8ToUnicode(buffer,(char*)temp);
	delete[] temp;

	// successful
	return 1;
}

int ReadLine(TCHAR *data, GString &line)
{
	int size=(int)wcslen(data);
	for(int i=0; i<size; i++)
	{
		if(data[i]==0x0D)
		{
			if(data[i+1]==0x0A) return(i+2);
			else return(i+1);
		}
		line.Append(data[i]);
	}
	return -1;	// end of file reached
}

int ReadBOM(GFile &file)
{
	BYTE dest[3];
	file.Read(dest,3);

	return ReadBOM(dest);
}

int ReadBOM(FILE *file)
{
	BYTE dest[3];
	fread(dest,3,1,file);

	return ReadBOM(dest);
}

int ReadBOM(u8* buffer)
{
	if(memcmp(buffer,"\xEF\xBB\xBF",3)==0) return 1;
	return 0;
}

void CreateFileList(LPCTSTR folder, LPCTSTR filter, GStringArray &dest)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError;
	GString DirSpec, message;
	DirSpec.Format(_T("%s\\%s"),folder,filter);

	// Find the first file in the directory.
	hFind = FindFirstFile(DirSpec, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) return;
	else
	{
		if(!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
			dest.Add(FindFileData.cFileName);
		// List all the other files in the directory.
		while(FindNextFile(hFind,&FindFileData)!=0)
		{
			if(!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
				dest.Add(FindFileData.cFileName);
		}

		dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES) return; // End of list
	}
}

//////////////////////////////////////////
CBufferFile::CBufferFile()
{
	buffer=NULL;
}

CBufferFile::CBufferFile(LPCTSTR filename)
{
	buffer=NULL;
	StoreFile(filename);
}

CBufferFile::~CBufferFile()
{
	Reset();
}

int CBufferFile::StoreFile(LPCTSTR filename)
{
	Reset();
	size=::StoreFile(filename,buffer);

	return size;
}

void CBufferFile::Reset()
{
	if(buffer) {delete[] buffer;buffer=NULL;}
}
