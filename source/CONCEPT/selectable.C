// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: selectable.C,v 1.7 2002/12/12 10:05:40 oliver Exp $

#include <BALL/CONCEPT/selectable.h>
#include <BALL/CONCEPT/persistenceManager.h>

using namespace std;

namespace BALL 
{

	Selectable::Selectable()
		
		:	selected_(BALL_SELECTABLE_DEFAULT_SELECTION)
	{
	}

	Selectable::Selectable(const Selectable& selectable, bool)
		
		:	selected_(selectable.selected_)
	{
	}

	Selectable::~Selectable()
		
	{
	}

	void Selectable::dump(ostream& s, Size depth) const
		
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
    BALL_DUMP_HEADER(s, this, this)
 
		BALL_DUMP_DEPTH(s, depth);
		s << "  selected: " << selected_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}  

  void Selectable::write(PersistenceManager& pm) const
		
  {
		pm.writePrimitive(selected_, "selected_");
	}

  bool Selectable::read(PersistenceManager& pm)
		
  {
		return pm.readPrimitive(selected_, "selected_");
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/selectable.iC>
#	endif
	 
} // namespace BALL
