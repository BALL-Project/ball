// $Id: selectable.C,v 1.5 2000/12/09 21:56:04 amoll Exp $

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

  ostream& operator << (ostream& s, const Selectable& selectable)
		throw()
  {
		s << selectable.selected_;
		return s;
	}

  istream& operator >> (istream& s, Selectable& selectable)
		throw()
  {
    s >> selectable.selected_;
		return s;
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
