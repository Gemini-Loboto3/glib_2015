#pragma once
#include "gstring.h"

GLIB_API GString base64_encode(BYTE* , unsigned int len);
GLIB_API void base64_decode(GString encoded_string, BYTE *ret);
