#pragma once
#include "glibconf.h"
#include "gstring.h"

#define TYPE_DLG	0
#define TYPE_LIST	1

#define TEXTRET_EMPTY		0
#define TEXTRET_OK			1
#define TEXTRET_INVALID		2
#define TEXTRET_BROKEN		3		// unexpected end

class GLIB_CLASS CText
{
public:
	CText();
	CText(LPCTSTR filename);
	~CText();

	virtual void Create(int entries, int type);
	virtual void Deallocate();
	virtual int Open(LPCTSTR filename);
	virtual void Save(LPCTSTR filename);
	virtual void SetEntry(LPCTSTR string, LPCTSTR header, int entry);
	virtual void AddEntry(LPCTSTR string, LPCTSTR header);
	virtual bool IsEnd(LPCTSTR tag);	// override for extra end tags
	virtual bool IsList();

	virtual int GetCount();
	virtual int GetPadding();
	virtual GString GetString(int index);
	virtual GString GetHeader(int index);
	virtual void ReplaceString(LPCTSTR string, int index);
private:
	int type;					// 0=dialogue dump, 1=plain dump
	int counter;
	int padding;				// special parameter for type 1, ignore if 0
	GStringArray strings;
	GStringArray headers;
public:
	GString error;
};

int GetMarkupTag(GString &src, int in_pos, TCHAR close, GString &dest);
int SkipCommentLine(GString &src, int pos);
