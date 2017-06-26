#include "GChar.h"

GChar::GChar(char ch)
{
	ucs=u16(ch);
}

GChar::GChar(u16 ch)
{
	ucs=ch;
}
