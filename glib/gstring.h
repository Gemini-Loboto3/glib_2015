#pragma once
#include "glibconf.h"

#pragma warning( disable : 4710 4702 4172 4251 )

////////////////////////////////////////////////////////////////////////
#include <Oleauto.h>
#include <vector>

void PASCAL ASCII_to_UNICODE( LPCSTR ansi_string, LPWSTR unicode_string ) throw();
void PASCAL UNICODE_to_ASCII( LPCWSTR unicode_string, LPSTR ansi_string, int number_of_unicode_characters_to_convert = (-1), UINT code_page = CP_ACP ) throw();

template GLIB_CLASS std::wstring;
template GLIB_CLASS std::string;

class GLIB_CLASS GString
{
private:
#if defined( UNICODE )
	// UNICODE build
	std::wstring m_String;
#else // UNICODE
	// ASCII build
	std::string m_String;
#endif // UNICODE
	LPTSTR m_Buffer;

public:
	// Constructors
	GString();
	GString(const GString& source );
	GString(TCHAR character, int number_of_characters = 1 );

	// ANSI Strings
	GString(LPCSTR source);
	GString(LPCSTR ansi_string, int number_of_characters );

	// UNICODE Strings
	GString( LPCWSTR source );
	GString( LPCWSTR unicode_string, int number_of_characters );

	// Bytes that happend to be NULL terminated and considered to be an ANSI string
	GString( const unsigned char* ansi_string );
	~GString();

	enum GStringType
	{
		type_ascii,
		type_utf8,
		type_utf16,
		type_utf16b
	};

	// Methods
	inline BSTR AllocSysString( DWORD code_page = CP_ACP ) throw();

	inline void Append( LPCTSTR string_p ) throw();
	inline void Append( TCHAR text_character, int number_of_times ) throw();
	inline void Append( const GString& source ) throw();

	inline int Collate( LPCTSTR string_pointer ) const throw();
	inline int Compare( LPCTSTR string_p ) const throw();
	inline int CompareNoCase( LPCTSTR string_p ) const throw();

	inline void Copy( LPCSTR ascii_string_p, int number_of_characters = (-1), int beginning_at = 0 ) throw();
	inline void Copy( LPCWSTR string_p, int number_of_characters = (-1), int beginning_at = 0 ) throw();

	inline int Delete(int nIndex, int nCount=1) throw();
	inline void Empty( void ) throw();

	inline int Find( TCHAR character ) const throw();
	inline int Find( TCHAR character, int start_index ) const throw();
	inline int Find( LPCTSTR string_p ) const throw();
	inline int Find( LPCTSTR string_p, int start_index ) const throw();
	inline int FindOneOf( LPCTSTR string_p ) const throw();

	void Format(LPCTSTR pszFormat, ...) throw();

	inline TCHAR GetAt( int character_index ) const throw();
	LPTSTR GetBuffer( int minimum_buffer_length ) throw();
	LPTSTR GetBuffer() throw();
	inline BOOL GetEnvironmentVariable( LPCTSTR variable_name ) throw();
	inline int GetLength( void ) const throw();
	inline int GetSize( void ) const throw();

	inline int Insert( int string_index, TCHAR character_to_insert ) throw();
	inline int Insert( int string_index, LPCTSTR string_to_insert ) throw();

	inline BOOL IsEmpty( void ) const throw();

	inline GString Left( int count ) const throw();

	inline void MakeLower( void ) throw();
	inline void MakeReverse( void ) throw();
	inline void MakeUpper( void ) throw();

	inline GString Mid( int first, int count ) const throw();
	inline GString Mid( int first ) const throw();

	inline void ReleaseBuffer( int new_length = (-1) ) throw();

	inline int Remove(TCHAR character_to_remove ) throw();

	inline int Replace(TCHAR old_character, TCHAR new_character ) throw();
	inline int Replace(LPCTSTR old_string, LPCTSTR replacement_string ) throw();

	inline int ReverseFind(TCHAR character ) const throw();

	GString Right( int count ) const throw();

	inline void Set(LPVOID string, GStringType type) throw();
	inline void SetAt(int character_index, TCHAR character ) throw();
	inline void SetLength(int new_length ) throw();
	inline BSTR SetSysString(BSTR * b_string_p, DWORD code_page = CP_ACP ) const throw();

	inline GString SpanExcluding(LPCTSTR character_set ) const throw();
	inline GString SpanIncluding(LPCTSTR character_set ) const throw();

	inline void TrimLeft(void) throw();
	inline void TrimLeft(TCHAR character) throw();
	inline void TrimRight(void) throw();
	inline void TrimRight(TCHAR character) throw();

	// Operators
	inline operator LPCTSTR() const throw()
	{
		return m_String.c_str();
	}

	inline TCHAR operator[]( int array_index ) const throw()
	{
		return(GetAt(array_index));
	}

	inline GString& operator<<(const GString& str)
	{
		Append(str);
		return(*this);
	}

	inline GString& operator<<(TCHAR psz)
	{
		m_String.append(1,psz);
		return(*this);
	}

	inline const GString& operator = ( const GString& source ) throw()
	{
		Copy( source );
		return( *this );
	}

	inline const GString& operator = ( LPCSTR source ) throw()
	{
		Copy( source );
		return( *this );
	}

	inline const GString& operator = ( LPCWSTR source ) throw()
	{
		Copy( source );
		return( *this );
	}

	inline const GString& operator = ( const unsigned char * source ) throw()
	{
		Copy( (LPCSTR) source );
		return( *this );
	}

	inline const GString& operator = ( TCHAR character ) throw()
	{
		m_String.resize( 0 );
		m_String.append( 1, character );
		return( *this );
	}

	inline const GString& operator += ( const GString& source ) throw()
	{
		Append( source );
		return( *this );
	}

	inline const GString& operator += ( TCHAR character ) throw()
	{
		m_String.append( 1, character );
		return( *this );
	}

	inline friend GString operator + ( const GString& string1, const GString& string2 ) throw()
	{
		GString return_value( string1 );
		return_value.Append( string2 );
		return( return_value );
	}

	inline friend GString operator + ( const GString& string1, TCHAR character ) throw()
	{
		GString return_value( string1 );
		return_value.Append( character, 1 );
		return( return_value );
	}

	inline friend GString operator + ( TCHAR character, const GString& string2 ) throw()
	{
		GString return_value;
		return_value.Append( character, 1 );
		return_value.Append( string2 );
		return( return_value );
	}

	inline friend GString operator + ( const GString& string1, LPCTSTR string2 ) throw()
	{
		GString return_value( string1 );
		return_value.Append( string2 );
		return( return_value );
	}

	inline friend GString operator + ( LPCTSTR string1, const GString& string2 ) throw()
	{
		GString return_value( string1 );
		return_value.Append( string2 );
		return( return_value );
	}
};

////////////////////////////////////////////////////////////////////////
template GLIB_CLASS std::vector<GString>;

class GLIB_CLASS GStringArray
{
protected:
	std::vector<GString> m_Array;
public:
	inline GStringArray(){};
	inline GStringArray( const GStringArray& source );
	inline ~GStringArray(){};

	int Add(const GString& new_element) throw();
	int Add(LPCTSTR new_element) throw();
	void Append(const GStringArray& source) throw();
	void Copy(const GStringArray& source) throw();
	GString& ElementAt( int array_index ) throw();
	void FreeExtra( void ) throw();
	GString GetAt( int array_index ) const throw();
	int GetSize( void ) const throw();
	int GetCount( void ) const throw();
	int GetUpperBound( void ) const throw();
	void InsertAt(int array_index, const GString& new_element, int number_of_times_to_insert_it = 1) throw();
	void InsertAt(int array_index, GStringArray * array_p) throw();
	void RemoveAll(void) throw();
	void RemoveAt( int array_index, int number_of_elements_to_remove = 1 ) throw();
	void SetSize( int new_size, int x = 32 ) throw();
	void SetAt(int array_index, const GString& new_element) throw();
	void SetAtGrow(int nIndex, const GString& new_element) throw();

	const GString operator[]( int array_index ) const throw()
	{
		return(GetAt(array_index));
	}

	GString& operator[]( int array_index ) throw()
	{
		return(ElementAt(array_index));
	}

	inline GStringArray& operator=( const GStringArray& source ) throw()
	{
		Copy( source );
		return( *this );
	}
};

inline bool operator==( const GString& s1, const GString& s2 ) throw()
{
   return( ( s1.Compare( s2 ) == 0 ) ? true : false );
}

inline bool operator==( const GString& s1, LPCTSTR s2 ) throw()
{
   return( ( s1.Compare( s2 ) == 0 ) ? true : false );
}

inline bool operator==( LPCTSTR s1, const GString& s2 ) throw()
{
   return( ( s2.Compare( s1 ) == 0 ) ? true : false );
}

inline bool operator!=( const GString& s1, const GString& s2 ) throw()
{
   return( ( s1.Compare( s2 ) == 0 ) ? false : true );
}

inline bool operator!=( const GString& s1, LPCTSTR s2 ) throw()
{
   return( ( s1.Compare( s2 ) == 0 ) ? false : true );
}

inline bool operator!=( LPCTSTR s1, const GString& s2 ) throw()
{
   return( ( s2.Compare( s1 ) == 0 ) ? false : true );
}

inline bool operator<( const GString& s1, const GString& s2 ) throw()
{
   return( ( s1.Compare( s2 ) < 0 ) ? true : false );
}

inline bool operator<( const GString& s1, LPCTSTR s2 ) throw()
{
   return( ( s1.Compare( s2 ) < 0 ) ? true : false );
}

inline bool operator<( LPCTSTR s1, const GString& s2 ) throw()
{
   return( ( s2.Compare( s1 ) > 0 ) ? true : false );
}

inline bool operator>( const GString& s1, const GString& s2 ) throw()
{
   return( ( s1.Compare( s2 ) > 0 ) ? true : false );
}

inline bool operator>( const GString& s1, LPCTSTR s2 ) throw()
{
   return( ( s1.Compare( s2 ) > 0 ) ? true : false );
}

inline bool operator>( LPCTSTR s1, const GString& s2 ) throw()
{
   return( ( s2.Compare( s1 ) < 0 ) ? true : false );
}

inline bool operator<=( const GString& s1, const GString& s2 ) throw()
{
   return( ( s1.Compare( s2 ) <= 0 ) ? true : false );
}

inline bool operator<=( const GString& s1, LPCTSTR s2 ) throw()
{
   return( ( s1.Compare( s2 ) <= 0 ) ? true : false );
}

inline bool operator<=( LPCTSTR s1, const GString& s2 ) throw()
{
   return( ( s2.Compare( s1 ) >= 0 ) ? true : false );
}

inline bool operator>=( const GString& s1, const GString& s2 ) throw()
{
   return( ( s1.Compare( s2 ) >= 0 ) ? true : false );
}

inline bool operator>=( const GString& s1, LPCTSTR s2 ) throw()
{
   return( ( s1.Compare( s2 ) >= 0 ) ? true : false );
}

inline bool operator>=( LPCTSTR s1, const GString& s2 ) throw()
{
   return( ( s2.Compare( s1 ) <= 0 ) ? true : false );
}
