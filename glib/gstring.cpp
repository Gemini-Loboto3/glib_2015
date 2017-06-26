// Include all of the windows header files that MFC would have included
#include "stdglib.h"
#include <string>
#include <vector>
#include <algorithm>
#include <crtdbg.h>
#include "validate.h"
#include "gstring.h"
#include "unicode.h"

inline GString::GString()
{
	 WFC_VALIDATE_POINTER( this );
	 m_Buffer = NULL;
}

inline GString::GString( const GString& source )
{
	WFC_VALIDATE_POINTER( this );

	m_Buffer = NULL;
	Copy( source );
}

inline GString::GString( TCHAR character, int number_of_characters)
{
	WFC_VALIDATE_POINTER( this );

	m_Buffer = NULL;
	m_String.append( number_of_characters, character );
}

// ANSI Strings
inline GString::GString(LPCSTR source)
{
	WFC_VALIDATE_POINTER( this );
	WFC_VALIDATE_POINTER_NULL_OK( source );

	m_Buffer = NULL;
	Copy( source );
}

inline GString::GString( LPCSTR ansi_string, int number_of_characters )
{
	m_Buffer = NULL;
	Copy( ansi_string, number_of_characters );
}

// UNICODE Strings
inline GString::GString( LPCWSTR source )
{
	WFC_VALIDATE_POINTER( this );
	WFC_VALIDATE_POINTER_NULL_OK( source );

	m_Buffer = NULL;
	Copy( source );
}

inline GString::GString( LPCWSTR unicode_string, int number_of_characters )
{
	m_Buffer = NULL;
	Copy( unicode_string, number_of_characters );
}

// Bytes that happend to be NULL terminated and considered to be an ANSI string
inline GString::GString( const unsigned char* ansi_string )
{
	m_Buffer = NULL;
	Copy( (LPCSTR) ansi_string );
}

inline GString::~GString()
{
	WFC_VALIDATE_POINTER( this );

	try
	{
		if ( m_Buffer != NULL )
		{
		   delete[] m_Buffer;
		   m_Buffer = NULL;
		}
	}
	catch(...)
	{
	}
}

// Methods
inline BSTR GString::AllocSysString( DWORD code_page) throw()
{
	WFC_VALIDATE_POINTER( this );

#if defined( _UNICODE ) || defined( OLE2ANSI )
	BSTR return_value = ::SysAllocStringLen( m_String.c_str(), (UINT) m_String.length() );
#else
	int length = ::MultiByteToWideChar( code_page,
									 0,
									 m_String.c_str(),
							   (int) m_String.length(),
									 NULL,
									 0 );

	BSTR return_value = ::SysAlloGStringLen( NULL, length );

	if ( return_value != NULL )
		::MultiByteToWideChar( code_page,
			0,
			m_String.c_str(),
			(int) m_String.length(),
			return_value,
			length );
	}
#endif

	return( return_value );
}

inline void GString::Append( LPCTSTR string_p ) throw()
{
	WFC_VALIDATE_POINTER( this );
	WFC_VALIDATE_POINTER_NULL_OK( string_p );

	try
	{
		m_String.append( string_p );
	}
	catch(...)
	{
	}
	
}

inline void GString::Append( TCHAR text_character, int number_of_times ) throw()
{
 WFC_VALIDATE_POINTER( this );

 try
 {
    int loop_index = 0;

    TCHAR temp_string[ 2 ];

    temp_string[ 0 ] = text_character;
    temp_string[ 1 ] = 0;

    while( loop_index < number_of_times )
    {
       m_String.append( temp_string );
       loop_index++;
    }
 }
 catch(...)
 {
 }
 
}

inline void GString::Append( const GString& source ) throw()
{
 WFC_VALIDATE_POINTER( this );

 try
 {
    m_String.append( (LPCTSTR) source.m_String.c_str() );
 }
 catch(...)
 {
 }
 
}

inline int GString::Collate( LPCTSTR string_pointer ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER_NULL_OK( string_pointer );

 try
 {
    return( ( string_pointer == NULL ) ? 1 : _tcscoll( m_String.c_str(), string_pointer ) );
 }
 catch(...)
 {
    return( 0 );
 }
 
}

inline int GString::Compare( LPCTSTR string_p ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER_NULL_OK( string_p );

 try
 {
    if ( string_p != NULL )
    {
       return( m_String.compare( string_p ) );
    }
    else
    {
       // We are greater than nothing (NULL)
       return( 1 );
    }
 }
 catch(...)
 {
    return( 0 );
 }
 
}

inline int GString::CompareNoCase( LPCTSTR string_p ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER_NULL_OK( string_p );

 try
 {
    if ( string_p != NULL )
    {
       return( _tcsicmp( m_String.c_str(), string_p ) );
    }
    else
    {
       // We are greater than nothing (NULL)
       return( 1 );
    }
 }
 catch(...)
 {
    return( 0 );
 }
 
}

inline void GString::Copy( LPCSTR ascii_string_p, int number_of_characters, int beginning_at) throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER_NULL_OK( ascii_string_p );
#if defined( UNICODE )
 // We are a UNICODE build trying to copy an ASCII string, this is easy...
 try
 {
    m_String.resize( 0 );

    if ( ascii_string_p == NULL )
    {
       return;
    }

    if ( number_of_characters < 0 ) // used to be -1 but that opened us up to errors
    {
       int loop_index   = beginning_at;
       int string_index = 0;

       size_t string_size = strlen( ascii_string_p );

       if ( loop_index >= (int)strlen( ascii_string_p ) )
       {
          return;
       }

       _ASSERTE( loop_index <= (int)string_size );

       string_size -= loop_index;

       m_String.resize( string_size );

       while( ascii_string_p[ loop_index ] != 0 )
       {
          m_String.at( string_index ) = (TCHAR)(ascii_string_p[ loop_index ]&0xFF);
          loop_index++;
          string_index++;
       }
    }
    else
    {
       // Only append a finite number of characters...

       int loop_index   = 0;
       int string_index = 0;

       if ( beginning_at > 0 )
       {
          if ( beginning_at >= (int)strlen( ascii_string_p ) )
          {
             return;
          }
       }

       m_String.resize( number_of_characters );

       while( (int) loop_index < number_of_characters )
       {
          m_String.at( string_index ) = ascii_string_p[ beginning_at + loop_index ];
          loop_index++;
          string_index++;
       }
    }
 }
 catch(...)
 {
 }
 
#else // UNICODE

 // We are an ASCII build copying an ascii string, we don't need to do anything special
 try
 {
    m_String.resize( 0 );

    if ( ascii_string_p == NULL )
    {
       return;
    }

    if ( number_of_characters == (-1) )
    {
       m_String.append( &ascii_string_p[ beginning_at ] );
    }
    else
    {
       // Only append a finite number of characters...

       int loop_index = 0;

       while( loop_index < number_of_characters )
       {
          Append( ascii_string_p[ beginning_at + loop_index ] );
          loop_index++;
       }
    }
 }
 catch(...)
 {
 }
 
#endif // UNICODE
}

inline void GString::Copy( LPCWSTR string_p, int number_of_characters, int beginning_at) throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER_NULL_OK( string_p );

#if defined( UNICODE )

 // We're already UNICODE so we don't need to do anything special
 try
 {
    m_String.resize( 0 );

    if ( string_p == NULL )
    {
       return;
    }

    if ( number_of_characters == (-1) )
    {
       m_String.append( &string_p[ beginning_at ] );
    }
    else
    {
       // Only append a finite number of characters...

       int loop_index = 0;

       while( (int) loop_index < number_of_characters )
       {
          Append( string_p[ beginning_at + loop_index ] );
          loop_index++;
       }
    }
 }
 catch(...)
 {
 }
 
#else // UNICODE

 // We are ASCII build, we need to convert ASCII to UNICODE
 try
 {
    m_String.resize( 0 );

    if ( string_p == NULL )
    {
       return;
    }

    int number_of_characters_to_copy = 0;

    if ( number_of_characters == (-1) )
    {
       number_of_characters_to_copy = wcslen( string_p );
    }
    else
    {
       number_of_characters_to_copy = number_of_characters;
    }

    number_of_characters_to_copy -= beginning_at;

    char  * ascii_string = NULL;

    if ( number_of_characters_to_copy <= 0 )
    {
       return;
    }

    try
    {
       ascii_string = new char[ ( number_of_characters_to_copy + 1 ) * sizeof( WCHAR ) ];
    }
    catch
    {
       ascii_string = NULL;
    }
    

    if ( ascii_string == NULL )
    {
       Empty();
       return;
    }

    UNICODE_to_ASCII( &string_p[ beginning_at ], ascii_string, number_of_characters_to_copy );
    ascii_string[ number_of_characters_to_copy ] = 0x00;

    m_String.append( ascii_string );

    delete[] ascii_string;
 }
 catch
 {
 }
 
#endif // UNICODE
}

int GString::Delete(int nIndex, int nCount) throw()
{
	m_String.erase(m_String.begin()+nIndex,m_String.begin()+nIndex+nCount);
	return GetSize();
}

void GString::Empty( void ) throw()
{
 WFC_VALIDATE_POINTER( this );
 m_String.resize( 0 );
}

int GString::Find( TCHAR character ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 return( (int) m_String.find( character ) );
}

int GString::Find( TCHAR character, int start_index ) const throw()
{
 WFC_VALIDATE_POINTER( this );

 if ( start_index >= (int)m_String.length() )
 {
    return( -1 );
 }

 if ( start_index == 0 )
 {
    return( (int)m_String.find( character ) );
 }

 // STL's find is whacko. Put some sense back into it.
 // This is a hack until I can decipher what STL is attempting to do

 int found_at = Mid( start_index ).Find( character );

 if ( found_at >= 0 )
 {
    return( start_index + found_at );
 }

 return( -1 );
}

inline int GString::Find( LPCTSTR string_p ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER_NULL_OK( string_p );

 if ( string_p != NULL )
 {
    return( (int)m_String.find( string_p ) );
 }
 else
 {
    return( -1 );
 }
}

inline int GString::Find( LPCTSTR string_p, int start_index ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER_NULL_OK( string_p );

 if ( string_p != NULL )
 {
    return( (int) m_String.find( string_p, start_index ) );
 }
 else
 {
    return( -1 );
 }
}

inline int GString::FindOneOf( LPCTSTR string_p ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER_NULL_OK( string_p );

 if ( string_p != NULL )
 {
    return( (int)m_String.find_first_of( string_p ) );
 }
 else
 {
    return( -1 );
 }
}

inline TCHAR GString::GetAt( int character_index ) const throw()
{
	WFC_VALIDATE_POINTER( this );
	if(character_index>=GetSize()) return 0;

	return(m_String.at(character_index));
}

LPTSTR GString::GetBuffer( int minimum_buffer_length ) throw()
{
 WFC_VALIDATE_POINTER( this );

 try
 {
    if ( m_Buffer != NULL )
    {
       delete[] m_Buffer;
       m_Buffer = NULL;
    }

    try
    {
       m_Buffer = new TCHAR[ minimum_buffer_length + 1 ];
    }
    catch(...)
    {
       m_Buffer = NULL;
    }
    

    if ( m_Buffer == NULL )
    {
       return( NULL );
    }

    m_Buffer[ minimum_buffer_length ] = 0; // NULL terminate the string

    // Copy the contents of our current string into the new buffer

    if ( minimum_buffer_length < (int)m_String.length() )
    {
       // The currently held string is longer than the minimum buffer length
       memcpy( m_Buffer, m_String.data(), minimum_buffer_length * sizeof( TCHAR ) );
    }
    else
    {
       memcpy( m_Buffer, m_String.data(), m_String.length() * sizeof( TCHAR ) );
    }

    return( m_Buffer );
 }
 catch(...)
 {
    return( NULL );
 }
}

LPTSTR GString::GetBuffer() throw()
{
	return GetBuffer(GetLength());
}

inline BOOL GString::GetEnvironmentVariable( LPCTSTR variable_name ) throw()
{
 TCHAR temp_string[ 4096 ];

 DWORD number_of_characters = ::GetEnvironmentVariable( variable_name, temp_string, (DWORD) ( DIMENSION_OF( temp_string ) - 1 ) );

 if ( number_of_characters > ( DIMENSION_OF( temp_string ) - 1 ) )
 {
    TCHAR * buffer = new TCHAR[ number_of_characters * 2 ];

    number_of_characters = ::GetEnvironmentVariable( variable_name, buffer, (DWORD) ( ( number_of_characters * 2 ) - 1 ) );

    buffer[ number_of_characters ] = 0x00;
    Copy( buffer );
    delete[] buffer;
 }
 else
 {
    temp_string[ number_of_characters ] = 0x00;
    Copy( temp_string );
 }

 return( ( number_of_characters > 0 ) ? TRUE : FALSE );
}

inline int GString::GetLength( void ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 return( (int)m_String.length() );
}

inline int GString::GetSize( void ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 return( (int)m_String.length() );
}

inline int GString::Insert( int string_index, TCHAR character_to_insert ) throw()
{
 WFC_VALIDATE_POINTER( this );

 int string_length = (int)m_String.length();

 if ( string_index >= string_length )
 {
    Append( character_to_insert );
 }
 else
 {
    if ( string_index < 0 )
    {
       string_index = 0;
    }

    m_String.insert( string_index, 1, character_to_insert );
 }

 return( (int)m_String.length() );
}

inline int GString::Insert( int string_index, LPCTSTR string_to_insert ) throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER_NULL_OK( string_to_insert );

 if ( string_to_insert != NULL )
 {
    int string_length =(int) m_String.length();

    if ( string_index >= string_length )
    {
       Append( string_to_insert );
    }
    else
    {
       if ( string_index < 0 )
       {
          string_index = 0;
       }

       m_String.insert( string_index, string_to_insert );
    }
 }

 return( (int)m_String.length() );
}

inline BOOL GString::IsEmpty( void ) const throw()
{
	WFC_VALIDATE_POINTER( this );
	return( m_String.empty() ? TRUE : FALSE );
}

inline GString GString::Left( int count ) const throw()
{
	WFC_VALIDATE_POINTER( this );

	GString return_value;

	return_value.m_String = m_String.substr( 0, count );

	return( return_value );
}

inline void GString::MakeLower( void ) throw()
{
	WFC_VALIDATE_POINTER( this );

	std::vector< TCHAR > character_array( m_String.data(), m_String.data() + m_String.length() );
	std::transform( character_array.begin(), character_array.end(), character_array.begin(), tolower );

#if defined( UNICODE) 
	// UNICODE build
	Copy( std::wstring( character_array.begin(), character_array.end() ).c_str() );
#else // UNICODE
	// ASCII build
	Copy( std::string( character_array.begin(), character_array.end() ).c_str() );
#endif // UNICODE
}

inline void GString::MakeReverse( void ) throw()
{
	WFC_VALIDATE_POINTER( this );

	if ( m_String.length() == 0 ) return;

	int last_character_index = (int)m_String.length() - 1;
	int loop_index           = 0;

	TCHAR character_to_move = 0;

	while( loop_index < last_character_index )
	{
		character_to_move = m_String.at( loop_index );
		m_String.at( loop_index ) = m_String.at( last_character_index );
		m_String.at( last_character_index ) = character_to_move;
		loop_index++;
		last_character_index--;
	}
}

inline void GString::MakeUpper( void ) throw()
{
	WFC_VALIDATE_POINTER( this );

	std::vector< TCHAR > character_array( m_String.data(), m_String.data() + m_String.length() );
	std::transform( character_array.begin(), character_array.end(), character_array.begin(), toupper );
#if defined( UNICODE) 
	// UNICODE build
	Copy( std::wstring( character_array.begin(), character_array.end() ).c_str() );
#else // UNICODE
	// ASCII build
	Copy( std::string( character_array.begin(), character_array.end() ).c_str() );
#endif // UNICODE
}

inline GString GString::Mid( int first, int count ) const throw()
{
 WFC_VALIDATE_POINTER( this );

 GString return_value;

 return_value.m_String = m_String.substr( first, count );

 return( return_value );
}

inline GString GString::Mid( int first ) const throw()
{
 WFC_VALIDATE_POINTER( this );

 if ( first == 0 )
 {
    return( GString( *this ) );
 }

 // "ABCD1234" Mid( 2 ) should return "CD1234"
 GString return_value;

 return_value.m_String = m_String.substr( first, m_String.length() - first );

 return( return_value );
}

inline void GString::ReleaseBuffer(int new_length) throw()
{
	WFC_VALIDATE_POINTER(this);
	try
	{
		if(m_Buffer==NULL) return;
		if(new_length<0)
		{
			// NULL terminated string buffer
			Copy( GString( m_Buffer ) );
		}
		else
		{
			m_String.resize(0); // Empty the string
			m_String.assign(m_Buffer,new_length);
		}
		delete[] m_Buffer;
		m_Buffer = NULL;
	}
	catch(...)
	{
	}
}

// 2000-12-05
// Thanks go to Matt Foltz for noticing that I forgot to implement Remove()
inline int GString::Remove( TCHAR character_to_remove ) throw()
{
 WFC_VALIDATE_POINTER( this );

 int string_length = (int)m_String.length();

 if ( string_length == 0 )
 {
    return( 0 );
 }

 GString new_string;

 int loop_index = 0;

 TCHAR character_to_test = 0;

 while( loop_index < string_length )
 {
    character_to_test = m_String.at( loop_index );

    if ( character_to_test != character_to_remove )
    {
       new_string += character_to_test;
    }

    loop_index++;
 }

 loop_index = string_length - new_string.GetLength();

 Copy( new_string );

 return( loop_index );
}

inline int GString::Replace( TCHAR old_character, TCHAR new_character ) throw()
{
 WFC_VALIDATE_POINTER( this );

 int number_of_replacements = 0;
 int string_index           = 0;
 int string_length          = (int)m_String.length();

 while( string_index < string_length )
 {
    if ( m_String.at( string_index ) == old_character )
    {
       m_String.at( string_index ) = new_character;
       number_of_replacements++;
    }

    string_index++;
 }

 return( number_of_replacements );
}

inline int GString::Replace( LPCTSTR old_string, LPCTSTR replacement_string ) throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER( old_string );
 WFC_VALIDATE_POINTER_NULL_OK( replacement_string );

 int number_of_replacements = 0;

 try
 {
    int old_string_length = (int)_tcslen( old_string );

    if ( old_string_length == 0 )
    {
       return( 0 );
    }

    GString new_string;

    int found_at = 0;

    found_at = Find( old_string );

    while( found_at != (-1) )
    {
       new_string += Left( found_at );

       if ( replacement_string != NULL )
       {
          new_string += replacement_string;
       }

       Copy( Right( (int)m_String.length() - ( found_at + old_string_length ) ) );

       number_of_replacements++;

       found_at = Find( old_string );
    }

    // The old string is no longer found, pick up any data left in the string

    if ( number_of_replacements > 0 )
    {
       new_string += *this;
       Copy( new_string );
    }
 }
 catch(...)
 {
 }
 

 return( number_of_replacements );
}

inline int GString::ReverseFind( TCHAR character ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 return( (int)m_String.rfind( character ) );
}

GString GString::Right( int count ) const throw()
{
 WFC_VALIDATE_POINTER( this );

 GString return_value;

 if ( (int)m_String.length() > count )
 {
    return_value.m_String = m_String.substr( m_String.length() - count );
 }
 else
 {
    // 2000-08-05
    // Thanks go to Tony Lam (tony@digitcom.net), I was not handling counts
    // were as long as or longer than the string the same way MFC was. DOH!

    return_value.m_String = m_String;
 }

 return( return_value );
}

inline void GString::Set(LPVOID string, GStringType type) throw()
{
	WFC_VALIDATE_POINTER(this);
	LPWSTR temp=NULL, temp_s;
	int slen;

	switch(type)
	{
	case type_ascii:
		// set directly
		Copy((LPCSTR)string);
		break;
	case type_utf8:
		// convert to unicode
		Utf8ToUnicode(temp,(char*)string);
		// set converted string
		Copy((LPCWSTR)temp);
		break;
	case type_utf16:
		// set directly
		Copy((LPCWSTR)string);
		break;
	case type_utf16b:
		// cast for smaller code
		temp_s=(LPWSTR)string;
		// allocate copy
		slen=(int)_tcslen(temp_s);
		temp=new WCHAR[slen+1];
		// swap bytes
		for(int i=0; i<=slen; i++) temp[i]=(temp_s[i]>>8)|(temp_s[i]<<8);
		// set swapped string
		Copy((LPCWSTR)temp);
	}
	if(temp) delete[] temp;
}

inline void GString::SetAt(int character_index, TCHAR character ) throw()
{
	WFC_VALIDATE_POINTER( this );
	_ASSERTE(character_index<(int)m_String.length());

	if(character_index<(int)m_String.length()) m_String.at(character_index)=character;
}

inline void GString::SetLength(int new_length ) throw()
{
 WFC_VALIDATE_POINTER(this);

 if ( new_length < (int)m_String.length() )
 {
    Copy( Left( new_length ) );
 }
 else if ( new_length > (int)m_String.length() )
 {
    while( new_length > (int)m_String.length() )
    {
       Append( (TCHAR) ' ' );
    }
 }
}

inline BSTR GString::SetSysString( BSTR * b_string_p, DWORD code_page) const throw()
{
 WFC_VALIDATE_POINTER( this );

#if defined( _UNICODE ) || defined( OLE2ANSI )

 ::SysReAllocStringLen( b_string_p, m_String.c_str(),(UINT)  m_String.length() );

#else

 int length = ::MultiByteToWideChar( code_page,
                                     0,
                                     m_String.c_str(),
                               (int) m_String.length(),
                                     NULL,
                                     0 );

 ::SysReAlloGStringLen( b_string_p, NULL, length );

 ::MultiByteToWideChar( code_page,
                        0,
                        m_String.c_str(),
                  (int) m_String.length(),
                       *b_string_p,
                        length );

#endif

 return( *b_string_p );
}

inline GString GString::SpanExcluding( LPCTSTR character_set ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER_NULL_OK( character_set );

 if ( character_set != NULL && m_String.length() > 0 )
 {
    return( Left( (int)::_tcscspn( m_String.c_str(), character_set ) ) );
 }
 else
 {
    GString return_value;
    return( return_value );
 }
}

inline GString GString::SpanIncluding( LPCTSTR character_set ) const throw()
{
 WFC_VALIDATE_POINTER( this );
 WFC_VALIDATE_POINTER_NULL_OK( character_set );

 if ( character_set != NULL )
 {
    return( Left( (int)::_tcsspn( m_String.c_str(), character_set ) ) );
 }
 else
 {
    GString return_value;
    return( return_value );
 }
}

inline void GString::TrimLeft( void ) throw()
{
 WFC_VALIDATE_POINTER( this );

 int string_length = (int)m_String.length();

 if ( string_length > 0 )
 {
    int number_of_elements_to_erase = 0;

    while( number_of_elements_to_erase < string_length &&
          _istspace( m_String.at( number_of_elements_to_erase ) ) )
    {
       number_of_elements_to_erase++;
    }

    if ( number_of_elements_to_erase >= string_length )
    {
       m_String.resize( 0 );
    }
    else
    {
       m_String.erase( 0, number_of_elements_to_erase );
    }
 }
}

inline void GString::TrimLeft( TCHAR character )
{
 WFC_VALIDATE_POINTER( this );

 int string_length = (int)m_String.length();

 if ( string_length > 0 )
 {
    int number_of_elements_to_erase = 0;

    while( number_of_elements_to_erase < string_length &&
           character == m_String.at( number_of_elements_to_erase ) )
    {
       number_of_elements_to_erase++;
    }

    if ( number_of_elements_to_erase >= string_length )
    {
       m_String.resize( 0 );
    }
    else
    {
        m_String.erase( 0, number_of_elements_to_erase );
    }
 }
}

inline void GString::TrimRight( void )
{
 WFC_VALIDATE_POINTER( this );

 if ( m_String.length() <= 0 )
 {
    return;
 }

 while( _istspace( m_String.at( m_String.length() - 1 ) ) )
 {
    m_String.resize( m_String.length() - 1 );

    if ( m_String.length() <= 0 )
    {
       return;
    }
 }
}

inline void GString::TrimRight( TCHAR character )
{
 WFC_VALIDATE_POINTER( this );

 if ( m_String.length() <= 0 )
 {
    return;
 }

 while( character == m_String.at( m_String.length() - 1 ) )
 {
    m_String.erase( m_String.length() - 1, 1 );

    if ( m_String.length() <= 0 )
    {
       return;
    }
 }
}

void GString::Format(LPCTSTR pszFormat, ...) throw()
{
	va_list args;
	int len;
	TCHAR* buffer;

	va_start(args,pszFormat);
	len=_vsctprintf(pszFormat,args)+1;
	buffer=(TCHAR*)malloc(len*sizeof(TCHAR));
	_vstprintf(buffer,pszFormat,args);
	m_String=buffer;
	free(buffer);
}

////////////////////////////////////////////////////////////////////////////////
GStringArray::GStringArray( const GStringArray& source ) throw()
{
	Copy(source);
}

int GStringArray::Add(const GString& new_element) throw()
{
	m_Array.push_back(new_element);
	return(GetUpperBound());
}

int GStringArray::Add(LPCTSTR new_element) throw()
{
	return Add(GString(new_element));
}

void GStringArray::Append(const GStringArray& source) throw()
{
	int loop_index=0;

	while(loop_index<source.GetSize())
	{
		m_Array.push_back( source.m_Array.at( loop_index ) );
		loop_index++;
	}
}

inline void GStringArray::Copy( const GStringArray& source ) throw()
{
 m_Array.clear();

 int loop_index            = 0;
 int number_of_array_items = source.GetSize();

 while( loop_index < number_of_array_items )
 {
    m_Array.push_back( source.m_Array.at( loop_index ) );
    loop_index++;
 }
}

GString& GStringArray::ElementAt( int array_index ) throw()
{
	if ( array_index >= 0 && (int) array_index <= GetUpperBound() )
		return( m_Array.at( array_index ) );

 // This is bad
	return GString(_T(""));
}

inline void GStringArray::FreeExtra( void ) throw()
{
	m_Array.resize(m_Array.size());
}

inline GString GStringArray::GetAt( int array_index ) const throw()
{
	if(array_index>=0 && (int)array_index<=GetUpperBound()) return(m_Array.at(array_index));

	// This is bad
	GString temporary_local_variable;

	return( temporary_local_variable );
}

inline int GStringArray::GetSize(void) const throw()
{
	return((int)m_Array.size());
}

inline int GStringArray::GetCount(void) const throw()
{
	return((int)m_Array.size());
}

inline int GStringArray::GetUpperBound( void ) const throw()
{
	return((int)m_Array.size()-1);
}

inline void GStringArray::InsertAt( int array_index, const GString& new_element, int number_of_times_to_insert_it) throw()
{
 if ( number_of_times_to_insert_it == 0 )
 {
    return;
 }

 if ( array_index > ( (int)m_Array.size() - 1 ) )
 {
    size_t loop_index = 0;

    while( (int)loop_index < number_of_times_to_insert_it )
    {
       m_Array.push_back( new_element );
       loop_index++;
    }

    return;
 }

 m_Array.insert( m_Array.begin() + array_index, number_of_times_to_insert_it, new_element );
}

inline void GStringArray::InsertAt( int array_index, GStringArray * array_p ) throw()
{
	if ( array_p != NULL )
		m_Array.insert( m_Array.begin() + array_index, array_p->m_Array.begin(), array_p->m_Array.end() );
}

void GStringArray::RemoveAll( void ) throw()
{
	m_Array.clear();
}

void GStringArray::RemoveAt( int array_index, int number_of_elements_to_remove) throw()
{
	if ( number_of_elements_to_remove < 1 )
		return;

	if ( array_index >=(int)m_Array.size() )
		return;

	m_Array.erase( m_Array.begin() + array_index, m_Array.begin() + array_index + number_of_elements_to_remove );
}

inline void GStringArray::SetSize( int new_size, int x) throw()
{
	m_Array.resize( new_size );
}

inline void GStringArray::SetAt(int array_index, const GString& new_element) throw()
{
	if(array_index>=0 && (int)array_index<=GetUpperBound()) m_Array.at( array_index ) = new_element;
}

inline void GStringArray::SetAtGrow(int nIndex, const GString& new_element) throw()
{
	if(nIndex>GetUpperBound()) SetSize(nIndex+1);
	SetAt(nIndex,new_element);
}
