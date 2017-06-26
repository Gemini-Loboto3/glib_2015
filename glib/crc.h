#pragma once
#include "glibconf.h"

GLIB_API ULONG GetCrc32(BYTE *inout, int size);
GLIB_API ULONG GetChecksum(BYTE *inout, int size);