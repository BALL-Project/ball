// $Id: JCAMPFile.C,v 1.7 2001/08/01 01:04:14 oliver Exp $

#include <BALL/FORMAT/JCAMPFile.h>

namespace BALL
{

	JCAMPFile::JCAMPFile( const String& name, OpenMode open_mode) 
		throw(Exception::FileNotFound)
		: File(name, open_mode), 
			buffer_(0)
	{
	}

	JCAMPFile::JCAMPFile(const JCAMPFile& file) 
		throw(Exception::FileNotFound)
		: File(file), 
			buffer_(0)
	{
	}

	JCAMPFile::~JCAMPFile()
		throw()
	{
		if (buffer_ != 0)
		{
			delete[] (buffer_);
		}
	}

	const Size JCAMPFile::MAX_LENGTH_ = 4096;

	bool JCAMPFile::nextLine_()
	{
		if (!buffer_)
			{
				buffer_ = new char[MAX_LENGTH_];
			};

		if ( getline( buffer_, MAX_LENGTH_ ) )
		{
			line_.set( buffer_ );
			return ( true );
		};

		return( false );
	}

	void JCAMPFile::read()
	{
		// Position line_index = 0, title_index=0, i;

		// First I try to read the title. Only parameters appearing *after* the
		// title are used.
		while ( nextLine_() )
		{
			if (line_.find( "##TITLE=", 0 ) != string::npos )
			{
				title_ = line_.after( "=" );
				break;
			 };
		 };

		while (nextLine_())
		{
			if (line_.has( '=' ))
			{
				parameters_[String (line_.before( "=" )).after( "$" )] = atof( line_.after( "=" ).c_str() );
			};
		};
	}

	String JCAMPFile::title()
	{
		return (title_);
	}

	double JCAMPFile::parameter( const String& name ) const
	{
		return (parameters_[name]);
	}

	bool JCAMPFile::has( const String& name ) const
	{
		return (parameters_.has(name));
	}
}
