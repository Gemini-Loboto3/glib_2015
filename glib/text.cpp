#include "stdglib.h"
#include "text.h"
#include "unicode.h"

CText::CText()
{
}

CText::CText(LPCTSTR filename)
{
	Open(filename);
}

CText::~CText()
{
}

void CText::Deallocate()
{
	strings.RemoveAll();
	headers.RemoveAll();
	counter=0;
}

int CText::Open(LPCTSTR filename)
{
	GString line;
	TCHAR *txt=NULL;
	padding=0;
	int ptr, index;
	Deallocate();

	try
	{
		// store text
		if(!StoreUtf8Text(filename,txt))
			throw TEXTRET_INVALID;
		// read header
		int seek=ReadLine(txt,line);
		if(line.GetAt(0)!=_T('<'))
			throw TEXTRET_INVALID;

		if(swscanf(line,_T("<%d text entries, %d>"),&counter,&padding)!=2)
			swscanf(line,_T("<%d text entries>"),&counter);
		if(counter==0)
		{
			//delete[] txt;
			type=0;
			//return TEXTRET_EMPTY;	// empty file
			throw TEXTRET_EMPTY;
		}

		// look for <Text block %d, %X> tag to determine type
		line.Empty();
		ReadLine(txt+seek,line);
		if(swscanf(line,_T("<Text block %d, %X>"),&index,&ptr)==2) type=0;	// dialogue detected
		if(swscanf(line,_T("<Text entry %d>"),&index)==1) type=0;
		else type=1;	// plain text detected

		for(int i=0; i<counter; i++)
		{
			// read dialogue block header
			if(type==TYPE_DLG)
			{
				line.Empty();
				seek+=ReadLine(txt+seek,line);
				headers.Add(line);
			}

			GString string;
			for(bool found=false; found==false;)
			{
				line.Empty();

				int ret=ReadLine(txt+seek,line);
				if(ret==-1)
				{
					//delete[] txt;
					//return TEXTRET_BROKEN;
					throw TEXTRET_BROKEN;
				}
				seek+=ret;
				string+=line;
				// skip all comments
				if(line[0]==_T('/') && line[1]==_T('/'))	// found a comment
				{
					string+=_T("\r\n");
					continue;
				}
				// search for <End> and <Call X> tags
				for(int j=0; j<(int)line.GetLength(); j++)
				{
					if(line.GetAt(j)==_T('<'))
					{
						int jump=(int)line.Find(_T('>'),j);
						GString tag;
						for(int k=0; k<=jump-j; k++) tag+=line.GetAt(k+j);
						j=jump;

						int dummy;
						// break on <End> or <Call X> found
						if(wcscmp(tag,_T("<End>"))==0 || swscanf(tag,_T("<Call %d>"),&dummy)==1 || IsEnd(tag))
						{
							found=true;
							break;
						}
					}
				}
				if(found==false) string+=_T("\r\n");
			}
			strings.Add(string);

			// read dialogue block footer
			if(type==TYPE_DLG)
			{
				seek+=ReadLine(txt+seek,line);	// skip <End of block>
				seek+=ReadLine(txt+seek,line);	// skip blank line
			}
		}
		delete[] txt;
	}
	catch(int e)
	{
		if(txt) delete[] txt;

		switch(e)
		{
		case TEXTRET_EMPTY:
			error=_T("This text file is empty.");
			break;
		case TEXTRET_INVALID:
			type=-1;
			error=_T("Invalid format.");
			break;
		case TEXTRET_BROKEN:
			error.Format(_T("Unexpected end (found %d strings, expected %d)."),strings.GetCount(),counter);
			break;
		}

#ifdef _DEBUG
#include "validate.h"
		GString out;
		out.Format(_T("Error on file %s: %s\n"),filename,(LPCTSTR)error);
		TRACE(out);
#endif
		return 0;
	}
	return 1;	// file read successfully
}

void CText::Save(LPCTSTR filename)
{
	GString tmp;
	FILE *dump;

	//dump.Open(filename,CFile::modeCreate | CFile::modeWrite);
	dump=_tfopen(filename,_T("wb"));
	WriteBOM(dump);
	if(padding==0) tmp.Format(_T("<%d text entries>\r\n"),counter);
	else tmp.Format(_T("<%d text entries, %d>\r\n"),counter,padding);
	WriteUtf8(tmp,dump);

	for(int i=0; i<counter; i++)
	{
		// write dialogue block header
		if(type==0)
		{
//			tmp.Format(_T("<Text block %d, %X>\r\n"),i,pointers[i]);
			tmp=headers[i]+_T("\r\n");
			WriteUtf8(tmp,dump);
		}

		tmp=strings[i]+_T("\r\n");
		WriteUtf8(tmp,dump);
		// write dialogue block footer
		if(type==0)
		{
			tmp=_T("<End of Block>\r\n\r\n");
			WriteUtf8(tmp,dump);
		}
	}
	//dump.Close();
	fclose(dump);
}

bool CText::IsEnd(LPCTSTR tag)
{
	return false;
}

bool CText::IsList()
{
	return type==TYPE_LIST ? true : false;
}

int CText::GetCount()
{
	return counter;
}

int CText::GetPadding()
{
	return padding;
}

GString CText::GetString(int index)
{
	return strings[index];
}

GString CText::GetHeader(int index)
{
	return headers[index];
}

void CText::ReplaceString(LPCTSTR string, int index)
{
	strings[index]=string;
}

void CText::Create(int entries, int type)
{
	this->type=type;
	strings.SetSize(entries);
	headers.SetSize(entries);
	counter=entries;
	padding=0;
}

void CText::AddEntry(LPCTSTR string, LPCTSTR header)
{
	strings.Add(string);
	headers.Add(header);
	counter++;
}

void CText::SetEntry(LPCTSTR string, LPCTSTR header, int entry)
{
	strings[entry]=string;
	if(header) headers[entry]=header;
}

int GetMarkupTag(GString &src, int in_pos, TCHAR close, GString &dest)
{
	dest.Empty();
	for(int add=0; ; add++)
	{
		dest+=src.GetAt(in_pos+add);
		if(src.GetAt(in_pos+add)==close) return add;
	}
}

int SkipCommentLine(GString &src, int pos)
{
	int j;
	// skip line entirely
	for(j=0; ; j++) if(src[pos+j]==_T('\r')) break;
	if(src[pos+j+1]==_T('\n')) j++;

	return j+1;
}
