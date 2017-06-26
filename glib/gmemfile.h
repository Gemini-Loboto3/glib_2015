#pragma once
#include "gstring.h"
#include "gfile.h"

#define WFC_DOESNT_THROW_EXCEPTIONS throw()

class GLIB_CLASS GMemoryFile
{
private:
	// Don't allow canonical behavior
	GMemoryFile( const GMemoryFile& ) {};
	const GMemoryFile& operator=( const GMemoryFile& ) { return( *this ); };
	
protected:
	SECURITY_ATTRIBUTES * m_SecurityAttributes_p;
	SECURITY_DESCRIPTOR * m_SecurityDescriptor_p;

	HANDLE m_MapHandle;

	void * m_MappedPointer;
	void * m_Pointer;

	GString m_Filename;

	SIZE_T m_AllocationGranularity;
	SIZE_T m_Length;
	SIZE_T m_MappedLength;

	ULONGLONG m_FileLength;

	DWORD  m_Attributes;
	HANDLE m_TemplateFile;

	virtual void m_Initialize( void ) WFC_DOESNT_THROW_EXCEPTIONS;
	virtual BOOL m_MapTheFile( HANDLE file_handle, UINT open_flags, ULONGLONG begining_at_offset, SIZE_T number_of_bytes_to_map, void * desired_pointer ) WFC_DOESNT_THROW_EXCEPTIONS;
	virtual void m_Uninitialize( void ) WFC_DOESNT_THROW_EXCEPTIONS;

   public:

	// Construction

	GMemoryFile();
	GMemoryFile(SIZE_T allocation_granularity,
		SECURITY_ATTRIBUTES * security_attributes = NULL,
		SECURITY_DESCRIPTOR * security_descriptor = NULL );

	virtual ~GMemoryFile();

	// Properties

	SIZE_T Size;

	// Methods

	virtual void	Close( void ) WFC_DOESNT_THROW_EXCEPTIONS;
	virtual BOOL	Flush( void ) WFC_DOESNT_THROW_EXCEPTIONS;
	virtual BOOL	FromHandle( HANDLE file_handle,
		UINT open_flags		   = GFile::modeRead,
		ULONGLONG beginning_at	  = 0,
		SIZE_T number_of_bytes_to_map = 0,
		void * desired_address	  = NULL ) WFC_DOESNT_THROW_EXCEPTIONS;
	inline ULONGLONG GetFileLength( void ) const WFC_DOESNT_THROW_EXCEPTIONS { return( m_FileLength ); }
	virtual void	GetFilename( GString& filename ) const WFC_DOESNT_THROW_EXCEPTIONS;
	inline  SIZE_T	GetLength( void ) const WFC_DOESNT_THROW_EXCEPTIONS { return( m_Length ); };
	inline  void *	GetPointer( void ) const WFC_DOESNT_THROW_EXCEPTIONS { return( m_Pointer ); };
	virtual BOOL	Open( LPCTSTR filename,
		UINT open_flags	= GFile::modeRead,
		ULONGLONG beginning_at = 0,
		SIZE_T number_of_bytes_to_map = 0,
		void * desired_address = NULL ) WFC_DOESNT_THROW_EXCEPTIONS;

	// New for Release 50, full control of CreateFile!

	inline virtual DWORD  GetAttributes( void ) const WFC_DOESNT_THROW_EXCEPTIONS { return( m_Attributes ); }
	inline virtual void   SetAttributes( DWORD attributes ) WFC_DOESNT_THROW_EXCEPTIONS { m_Attributes = attributes; }

	inline virtual HANDLE GetTemplateFile( void ) const WFC_DOESNT_THROW_EXCEPTIONS { return( m_TemplateFile ); }
	inline virtual void   SetTemplateFile( HANDLE file_handle ) WFC_DOESNT_THROW_EXCEPTIONS { m_TemplateFile = file_handle; };

	// Operators

	inline operator void * ( void ) const WFC_DOESNT_THROW_EXCEPTIONS { return( m_Pointer ); };
};
