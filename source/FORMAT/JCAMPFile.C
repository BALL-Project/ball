// $Id: JCAMPFile.C,v 1.1 2000/09/14 12:23:44 oliver Exp $

#include <BALL/FORMAT/JCAMPFILE.H>

namespace BALL
{

	BrukerParameter::BrukerParameter( const String& name, OpenMode open_mode = IN ) : File( name, open_mode ), buffer_(0)
	{
	}

	BrukerParameter::BrukerParameter( const BrukerParameter& file ) : File( file ), buffer_(0)
	{
	}

	BrukerParameter::~BrukerParameter()
	{
		if (buffer_)
			delete[] (buffer_);
	}

	const Size BrukerParameter::MAX_LENGTH_ = 4096;

	bool BrukerParameter::nextLine_()
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

	void BrukerParameter::read()
	{
		Position line_index = 0, title_index=0, i;

		// Zuerst versuchen wir, den Titel einzulesen.
		// Es werden nur Parameter betrachtet, die *nach* dem Titel in der Datei auftauchen.
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

	String BrukerParameter::title()
	{
		return (title_);
	}

	double BrukerParameter::parameter( const String& name )
	{
		return (parameters_[name]);
	}

	bool BrukerParameter::has( const String& name )
	{
		return (parameters_.has(name));
	}
}
