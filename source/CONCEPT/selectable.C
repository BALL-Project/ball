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
		throw()
		:	selected_(BALL_SELECTABLE_DEFAULT_SELECTION)
	{
	}

	Selectable::Selectable(const Selectable& selectable, bool)
		throw()
		:	selected_(selectable.selected_)
	{
	}

	Selectable::~Selectable()
		throw()
	{
	}

	void Selectable::dump(ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
    BALL_DUMP_HEADER(s, this, this)
 
		BALL_DUMP_DEPTH(s, depth);
		s << "  selected: " << selected_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}  

  void Selectable::write(PersistenceManager& pm) const
		throw()
  {
		pm.writePrimitive(selected_, "selected_");
	}

  bool Selectable::read(PersistenceManager& pm)
		throw()
  {
		return pm.readPrimitive(selected_, "selected_");
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/selectable.iC>
#	endif
	 
} // namespace BALL
