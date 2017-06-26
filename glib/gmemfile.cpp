#include "stdglib.h"
#include "validate.h"
#include "glibconf.h"
#include "gmemfile.h"
#include "debug_new.h"

GMemoryFile::GMemoryFile()
{
	//WFCLTRACEINIT( TEXT( "GMemoryFile::GMemoryFile()" ) );
	WFC_VALIDATE_POINTER( this );

	m_FileLength	 = 0;
	m_Length	  = 0;
	m_MappedLength  = 0;
	m_MappedPointer = NULL;
	m_Pointer	 = NULL;
	Size		= 0;
	m_Attributes	 = FILE_ATTRIBUTE_NORMAL;
	m_TemplateFile  = NULL;

	SYSTEM_INFO si;
	::GetSystemInfo( &si );

	m_AllocationGranularity = si.dwAllocationGranularity;

	/*
	** 1999-04-18
	** Thanks go to Perry Rapp, (PRapp@itb.nawcsti.navy.mil) for 
	** finding that I wasn't initializing all members here. DOH!
	*/

	m_SecurityAttributes_p = (SECURITY_ATTRIBUTES *) NULL;
	m_SecurityDescriptor_p = (SECURITY_DESCRIPTOR *) NULL;
	m_MapHandle		= (HANDLE) INVALID_HANDLE_VALUE;
}

GMemoryFile::GMemoryFile( SIZE_T			 allocation_granularity,
				  SECURITY_ATTRIBUTES * security_attributes,
				  SECURITY_DESCRIPTOR * security_descriptor )
{
	//WFCLTRACEINIT( TEXT( "GMemoryFile::GMemoryFile( allocation_granularity )" ) );
	WFC_VALIDATE_POINTER( this );

#if defined( _DEBUG )
	SYSTEM_INFO _si;
	::GetSystemInfo( &_si );

	_ASSERTE( allocation_granularity == _si.dwAllocationGranularity );
#endif // _DEBUG

	m_FileLength	 = 0;
	m_Length	  = 0;
	m_MappedLength  = 0;
	m_MappedPointer = NULL;
	m_Pointer	 = NULL;
	Size		= 0;
	m_Attributes	 = FILE_ATTRIBUTE_NORMAL;
	m_TemplateFile  = NULL;

	m_AllocationGranularity = allocation_granularity;
	m_SecurityAttributes_p  = security_attributes;
	m_SecurityDescriptor_p  = security_descriptor;
	m_MapHandle		 = (HANDLE) INVALID_HANDLE_VALUE;
}

GMemoryFile::~GMemoryFile()
{
	//WFCLTRACEINIT( TEXT( "GMemoryFile::~GMemoryFile()" ) );
	WFC_VALIDATE_POINTER( this );

	Close();

	m_Uninitialize();

	m_AllocationGranularity = 0;
	m_FileLength		= 0;
	m_Length			 = 0;
	m_MapHandle		 = (HANDLE) INVALID_HANDLE_VALUE;
	m_MappedLength		 = 0;
	m_MappedPointer		= NULL;
	m_Pointer			= (void *) NULL;
	m_SecurityAttributes_p  = (SECURITY_ATTRIBUTES *) NULL;
	m_SecurityDescriptor_p  = (SECURITY_DESCRIPTOR *) NULL;
	Size			  = 0;
}

void GMemoryFile::Close( void )
{
	//WFCLTRACEINIT( TEXT( "GMemoryFile::Close()" ) );
	WFC_VALIDATE_POINTER( this );

	if ( m_MappedPointer != NULL )
	{
		if ( ::UnmapViewOfFile( m_MappedPointer ) == FALSE )
		{
			////WFCTRACEERROR( ::GetLastError() );
			//TRACE( TEXT( "UnmapViewOfFile() failed with the above error." ) );
		}

		m_MappedPointer = NULL;
		m_Pointer	 = NULL;
	}

	if ( m_MapHandle != (HANDLE) INVALID_HANDLE_VALUE )
	{
		if ( ::CloseHandle( m_MapHandle ) == FALSE )
		{
			//WFCTRACEERROR( ::GetLastError() );
			//TRACE( TEXT( "CloseHandle() failed with the above error." ) );
		}

		m_MapHandle = (HANDLE) INVALID_HANDLE_VALUE;
	}

	m_Filename.Empty();
	m_FileLength	= 0;
	m_Length	 = 0;
	m_MappedLength = 0;
	Size		  = 0;
}

BOOL GMemoryFile::Flush( void )
{
	//WFCLTRACEINIT( TEXT( "GMemoryFile::Flush()" ) );
	WFC_VALIDATE_POINTER( this );

	if ( ::FlushViewOfFile( m_MappedPointer, m_MappedLength ) == 0 )
	{
	//WFCTRACEERROR( ::GetLastError() );
	//TRACE( TEXT( "FlushViewOfFile() failed for the above reason." ) );
	return( FALSE );
	}

	return( TRUE );
}

BOOL GMemoryFile::FromHandle( HANDLE file_handle, UINT open_flags, ULONGLONG beginning_at_offset, SIZE_T number_of_bytes_to_map, void * desired_address )
{
	//WFCLTRACEINIT( TEXT( "GMemoryFile::FromHandle()" ) );
	WFC_VALIDATE_POINTER( this );

	BOOL return_value = FALSE;

	return_value = m_MapTheFile( file_handle, open_flags, beginning_at_offset, number_of_bytes_to_map, desired_address );

	return( return_value );
}

void GMemoryFile::GetFilename( GString& filename ) const
{
	//WFCLTRACEINIT( TEXT( "GMemoryFile::GetFilename()" ) );
	WFC_VALIDATE_POINTER( this );

	filename = m_Filename;
}

void GMemoryFile::m_Initialize( void )
{
	//WFCLTRACEINIT( TEXT( "GMemoryFile::m_Initialize()" ) );
	WFC_VALIDATE_POINTER( this );

	_ASSERTE( m_SecurityAttributes_p == NULL );
	_ASSERTE( m_SecurityDescriptor_p == NULL );
	_ASSERTE( m_MapHandle		== (HANDLE) INVALID_HANDLE_VALUE );

	m_Attributes	 = FILE_ATTRIBUTE_NORMAL;
	m_TemplateFile  = NULL;

	try
	{
	m_SecurityAttributes_p = new SECURITY_ATTRIBUTES;
	}
	catch(...)
	{
	m_SecurityAttributes_p = NULL;
	}

	if ( m_SecurityAttributes_p == NULL )
	{
	return;
	}

	try
	{
	m_SecurityDescriptor_p = new SECURITY_DESCRIPTOR;
	}
	catch(...)
	{
	m_SecurityDescriptor_p = NULL;
	}

	if ( m_SecurityDescriptor_p == NULL )
	{
	delete m_SecurityAttributes_p;
	m_SecurityAttributes_p = NULL;

	return;
	}

	if ( ::InitializeSecurityDescriptor( m_SecurityDescriptor_p, SECURITY_DESCRIPTOR_REVISION ) == FALSE )
	{
	//WFCTRACEERROR( ::GetLastError() );
	//WFCTRACE( TEXT( "InitializeSecurityDescriptor() failed with the above error." ) );

	delete m_SecurityAttributes_p;
	m_SecurityAttributes_p = NULL;

	delete m_SecurityDescriptor_p;
	m_SecurityDescriptor_p = NULL;

	return;
	}

	if ( ::SetSecurityDescriptorDacl( m_SecurityDescriptor_p, TRUE, NULL, FALSE ) == FALSE )
	{
	//WFCTRACEERROR( ::GetLastError() );
	//WFCTRACE( TEXT( "SetSecurityDescriptorDacl() failed with the above error." ) );

	delete m_SecurityAttributes_p;
	m_SecurityAttributes_p = NULL;

	delete m_SecurityDescriptor_p;
	m_SecurityDescriptor_p = NULL;

	return;
	}

	m_SecurityAttributes_p->nLength		  = sizeof( SECURITY_ATTRIBUTES );
	m_SecurityAttributes_p->lpSecurityDescriptor = m_SecurityDescriptor_p;
	m_SecurityAttributes_p->bInheritHandle	 = TRUE;
}

BOOL GMemoryFile::m_MapTheFile( HANDLE file_handle, UINT open_flags, ULONGLONG beginning_at_offset, SIZE_T number_of_bytes_to_map, void * desired_address )
{
	//WFCLTRACEINIT( TEXT( "GMemoryFile::m_MapTheFile()" ) );
	WFC_VALIDATE_POINTER( this );

	if ( m_Pointer != NULL )
	{
	Close();
	}

	if ( file_handle == NULL || file_handle == INVALID_HANDLE_VALUE )
	{
	return( FALSE );
	}

	DWORD protections = PAGE_READWRITE;
	DWORD access_mode = FILE_MAP_WRITE;

	if ( open_flags == GFile::modeRead )
	{
	//WFCTRACE( TEXT( "User wants read-only access." ) );
	protections = PAGE_READONLY;
	access_mode = FILE_MAP_READ;
	}

	if ( ( open_flags & GFile::modeWrite ) || ( open_flags & GFile::modeReadWrite ) )
	{
	//TRACE( TEXT( "User wants Read/Write access." ) );
	protections = PAGE_READWRITE;
	access_mode = FILE_MAP_WRITE;
	}

	// Let's adjust number_of_bytes_to_map if we're using an offset

	LARGE_INTEGER file_length = { 0, 0 };

	if ( ::GetFileSizeEx( file_handle, &file_length ) == FALSE )
	{
	//WFCTRACEERROR( ::GetLastError() );
	//WFCTRACE( TEXT( "Can't get file size because of above error." ) );
	return( FALSE );
	}

	m_FileLength = file_length.QuadPart;

	if ( beginning_at_offset > 0 )
	{
	if ( beginning_at_offset > m_FileLength )
	{
		//WFCTRACE( TEXT( "You want to start mapping a file after the end of the file." ) );
		return( FALSE );
	}

	if ( number_of_bytes_to_map == 0 )
	{
		// user has specified default length, since the user also
		// specified an offset, we need to compute how many bytes
		// are really gonna be mapped.

		number_of_bytes_to_map = static_cast< SIZE_T >( m_FileLength - beginning_at_offset );
	}
	}
	else
	{
	if ( number_of_bytes_to_map == 0 )
	{
		number_of_bytes_to_map = static_cast< SIZE_T >( m_FileLength );
	}
	}

	m_MapHandle = ::CreateFileMapping( file_handle,
						  m_SecurityAttributes_p,
						  protections,
						  0, // Microsoft isn't very clear about these parameters, always use zero
						  0, // Microsoft isn't very clear about these parameters, always use zero
						  NULL );

	if ( m_MapHandle == NULL )
	{
	//WFCTRACEERROR( ::GetLastError() );
	//WFCTRACE( TEXT( "CreateFileMapping() failed with the above error." ) );

	m_MapHandle = (HANDLE) INVALID_HANDLE_VALUE;
	return( FALSE );
	}

	ULARGE_INTEGER large_file_offset;

	large_file_offset.QuadPart = beginning_at_offset;

	if ( ( large_file_offset.QuadPart % m_AllocationGranularity ) != 0 )
	{
	// The user wants to start mapping at a point in the file that doesn't fall
	// on a memory allocation boundary (usually a 64KB boundary). That means we
	// must do some creative programming to give the user what they asked for.

	large_file_offset.QuadPart -= ( large_file_offset.QuadPart % m_AllocationGranularity );
	}

	m_MappedLength = number_of_bytes_to_map + static_cast< SIZE_T >( beginning_at_offset - large_file_offset.QuadPart );

	m_MappedPointer = ::MapViewOfFileEx( m_MapHandle, 
							 access_mode,
							 large_file_offset.HighPart,
							 large_file_offset.LowPart,
							 m_MappedLength,
							 desired_address );

	if ( m_MappedPointer == NULL )
	{
	//WFCTRACEERROR( ::GetLastError() );
	//WFCTRACE( TEXT( "MapViewOfFileEx() failed with the above error." ) );

	if ( ::CloseHandle( m_MapHandle ) == FALSE )
	{
		//WFCTRACEERROR( ::GetLastError() );
		//WFCTRACE( TEXT( "CloseHandle() failed with the aboce error." ) );
	}

	m_MapHandle = (HANDLE) INVALID_HANDLE_VALUE;

	Close();

	return( FALSE );
	}

	m_Filename.Empty();
	m_Length	= number_of_bytes_to_map;
	Size	 = number_of_bytes_to_map;

	char * temp_pointer = (char *) m_MappedPointer;

	m_Pointer = &temp_pointer[ m_MappedLength - number_of_bytes_to_map ];

	return( TRUE );
}

void GMemoryFile::m_Uninitialize( void )
{
	//WFCLTRACEINIT( TEXT( "GMemoryFile::m_Uninitialize()" ) );
	WFC_VALIDATE_POINTER( this );

	if ( m_SecurityAttributes_p != NULL )
	{
	delete m_SecurityAttributes_p;
	m_SecurityAttributes_p = NULL;
	}

	if ( m_SecurityDescriptor_p != NULL )
	{
	delete m_SecurityDescriptor_p;
	m_SecurityDescriptor_p = NULL;
	}
}

BOOL GMemoryFile::Open( LPCTSTR filename, UINT open_flags, ULONGLONG beginning_at_offset, SIZE_T number_of_bytes_to_map, void * desired_address )
{
	//WFCLTRACEINIT( TEXT( "GMemoryFile::Open()" ) );
	WFC_VALIDATE_POINTER( this );
	WFC_VALIDATE_POINTER( filename );

	if ( m_Pointer != NULL )
	{
	Close();
	}

	if ( filename == NULL )
	{
	return( FALSE );
	}

	GFile file;

	//file.SetAttributes( GetAttributes() );
	//file.SetTemplateFile( GetTemplateFile() );

	// The user passed us a pointer, don't trust it
	try
	{
		if ( file.Open( filename, open_flags ) == FALSE )
		{
			//WFCTRACE( TEXT( "Can't open file" ) );
			return( FALSE );
		}

		if ( m_MapTheFile( (HANDLE) file.m_hFile, open_flags, beginning_at_offset, number_of_bytes_to_map, desired_address ) == FALSE )
		{
			//WFCTRACE( TEXT( "Can't map the file." ) );
			return( FALSE );
		}

		m_Filename = filename;

		return( TRUE );
	}
	catch(...)
	{
	}

	return( FALSE );
}

// End of source
