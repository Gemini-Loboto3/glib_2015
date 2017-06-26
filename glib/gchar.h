#pragma once
#include "glibconf.h"
#include "types.h"

class GLIB_CLASS GChar
{
public:
	GChar();

private:
	GChar(char ch);
	GChar(u16 ch);

	u16 ucs;
};

inline GChar::GChar() : ucs(0) {}
