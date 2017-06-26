#pragma once
#include "glibconf.h"

// Sjis<->Unicode conversion
typedef struct SJIS_CONVERSION
{
	TCHAR unicode;
	USHORT sjis;
} SJIS_CONVERSION;

TCHAR GLIB_API SjisToUnicode(TCHAR sjis);
USHORT GLIB_API UnicodeToSjis(TCHAR unicode);
