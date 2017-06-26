#pragma once

#if ! defined( DIMENSION_OF )
#define DIMENSION_OF( argument ) ( sizeof( argument ) / sizeof( *( argument ) ) )
#endif

////////////////////////////////////////////////////////////////////////
extern char glb_TraceTxtBuf [256];

#ifdef _DEBUG
  #define TRACE(txt)		OutputDebugString (txt)

  #define TRACE1(fmt,parm1)	\
	{	_stprintf(glb_TraceTxtBuf, fmt, parm1); \
		OutputDebugString (glb_TraceTxtBuf); }

  #define TRACE2(fmt,parm1,parm2)	\
	{	_stprintf(glb_TraceTxtBuf, fmt, parm1, parm2); \
		OutputDebugString (glb_TraceTxtBuf); }
#else
	#define TRACE(txt)				((void)0)
	#define TRACE1(fmt,parm1)		((void)0)
	#define TRACE2(fmt,parm1,parm2)	((void)0)
#endif // _DEBUG


////////////////////////////////////////////////////////////////////////
#if ! defined( WFC_VALIDATE_POINTER )
#if defined( _W64 )
// 64-bit version
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_POINTER( _pointer ) _ASSERTE( (SIZE_T) _pointer != NULL ); \
_ASSERTE( (SIZE_T) _pointer != 0xABABABAB ); \
_ASSERTE( (SIZE_T) _pointer != 0xBAADF00D ); \
_ASSERTE( (SIZE_T) _pointer != 0xFEEEFEEE ); \
_ASSERTE( (SIZE_T) _pointer != 0xCCCCCCCC ); \
_ASSERTE( (SIZE_T) _pointer != 0xCDCDCDCD ); \
_ASSERTE( (SIZE_T) _pointer != 0xDDDDDDDD ); \
_ASSERTE( (SIZE_T) _pointer != 0xFDFDFDFD ); \
_ASSERTE( (SIZE_T) _pointer != 0xABABABABABABABAB ); \
_ASSERTE( (SIZE_T) _pointer != 0xBAADF00DBAADF00D ); \
_ASSERTE( (SIZE_T) _pointer != 0xFEEEFEEEFEEEFEEE ); \
_ASSERTE( (SIZE_T) _pointer != 0xCCCCCCCCCCCCCCCC ); \
_ASSERTE( (SIZE_T) _pointer != 0xCDCDCDCDCDCDCDCD ); \
_ASSERTE( (SIZE_T) _pointer != 0xDDDDDDDDDDDDDDDD ); \
_ASSERTE( (SIZE_T) _pointer != 0xFDFDFDFDFDFDFDFD )
#else // _DEBUG
#define WFC_VALIDATE_POINTER( _pointer )
#endif // _DEBUG
#else // _W64
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_POINTER( _pointer ) _ASSERTE( (SIZE_T)_pointer != NULL ); \
_ASSERTE( (SIZE_T) _pointer != 0xABABABAB ); \
_ASSERTE( (SIZE_T) _pointer != 0xBAADF00D ); \
_ASSERTE( (SIZE_T) _pointer != 0xFEEEFEEE ); \
_ASSERTE( (SIZE_T) _pointer != 0xCCCCCCCC ); \
_ASSERTE( (SIZE_T) _pointer != 0xCDCDCDCD ); \
_ASSERTE( (SIZE_T) _pointer != 0xDDDDDDDD ); \
_ASSERTE( (SIZE_T) _pointer != 0xFDFDFDFD )
#else // _DEBUG
#define WFC_VALIDATE_POINTER( _pointer )
#endif // _DEBUG
#endif // _W64
#endif // WFC_VALIDATE_POINTER

#if ! defined( WFC_VALIDATE_POINTER_NULL_OK )
#if defined( _W64 )
// 64-bit version
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer ) \
_ASSERTE( (SIZE_T) _pointer != 0xABABABAB ); \
_ASSERTE( (SIZE_T) _pointer != 0xBAADF00D ); \
_ASSERTE( (SIZE_T) _pointer != 0xFEEEFEEE ); \
_ASSERTE( (SIZE_T) _pointer != 0xCCCCCCCC ); \
_ASSERTE( (SIZE_T) _pointer != 0xCDCDCDCD ); \
_ASSERTE( (SIZE_T) _pointer != 0xDDDDDDDD ); \
_ASSERTE( (SIZE_T) _pointer != 0xFDFDFDFD ); \
_ASSERTE( (SIZE_T) _pointer != 0xABABABABABABABAB ); \
_ASSERTE( (SIZE_T) _pointer != 0xBAADF00DBAADF00D ); \
_ASSERTE( (SIZE_T) _pointer != 0xFEEEFEEEFEEEFEEE ); \
_ASSERTE( (SIZE_T) _pointer != 0xCCCCCCCCCCCCCCCC ); \
_ASSERTE( (SIZE_T) _pointer != 0xCDCDCDCDCDCDCDCD ); \
_ASSERTE( (SIZE_T) _pointer != 0xDDDDDDDDDDDDDDDD ); \
_ASSERTE( (SIZE_T) _pointer != 0xFDFDFDFDFDFDFDFD )
#else // _DEBUG
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer )
#endif // _DEBUG
#else // _W64
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer ) \
_ASSERTE( (SIZE_T) _pointer != 0xABABABAB ); \
_ASSERTE( (SIZE_T) _pointer != 0xBAADF00D ); \
_ASSERTE( (SIZE_T) _pointer != 0xFEEEFEEE ); \
_ASSERTE( (SIZE_T) _pointer != 0xCCCCCCCC ); \
_ASSERTE( (SIZE_T) _pointer != 0xCDCDCDCD ); \
_ASSERTE( (SIZE_T) _pointer != 0xDDDDDDDD ); \
_ASSERTE( (SIZE_T) _pointer != 0xFDFDFDFD )
#else // _DEBUG
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer )
#endif // _DEBUG
#endif // _W64
#endif // WFC_VALIDATE_POINTER_NULL_OK

#if ! defined( WFC_VALIDATE_POINTER_NULL_OK )
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer ) \
_ASSERTE( (DWORD) _pointer != 0xABABABAB ); \
_ASSERTE( (DWORD) _pointer != 0xBAADF00D ); \
_ASSERTE( (DWORD) _pointer != 0xFEEEFEEE ); \
_ASSERTE( (DWORD) _pointer != 0xCCCCCCCC ); \
_ASSERTE( (DWORD) _pointer != 0xCDCDCDCD ); \
_ASSERTE( (DWORD) _pointer != 0xDDDDDDDD ); \
_ASSERTE( (DWORD) _pointer != 0xFDFDFDFD )
#else // _DEBUG
#define WFC_VALIDATE_POINTER_NULL_OK( _pointer )
#endif // _DEBUG
#endif // WFC_VALIDATE_POINTER_NULL_OK

#if ! defined( WFC_VALIDATE_BOOL )
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_BOOL( _BOOLean_value ) \
_ASSERTE( _BOOLean_value == TRUE || _BOOLean_value == FALSE )
#else // _DEBUG
#define WFC_VALIDATE_BOOL( _BOOLean_value )
#endif // _DEBUG
#endif // WFC_VALIDATE_BOOL

#if ! defined( WFC_VALIDATE_bool )
#if defined( _DEBUG ) && defined( _ASSERTE )
#define WFC_VALIDATE_bool( _BOOLean_value ) \
_ASSERTE( _BOOLean_value == true || _BOOLean_value == false )
#else // _DEBUG
#define WFC_VALIDATE_bool( _BOOLean_value )
#endif // _DEBUG
#endif // WFC_VALIDATE_bool
