// $Id: selectable.C,v 1.1 1999/08/26 08:02:35 oliver Exp $

#include <BALL/CONCEPT/selectable.h>
#include <BALL/CONCEPT/persistenceManager.h>

namespace BALL {

	Selectable::Selectable(void)
		:	selected_(BALL_SELECTABLE_DEFAULT_SELECTION)
	{
	}

	Selectable::Selectable(const Selectable& selectable, bool)
		:	selected_(selectable.selected_)
	{
	}

	Selectable::~Selectable(void)
	{
	}

	void Selectable::dump(ostream &s, unsigned long depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);

		BALL_DUMP_DEPTH(s, depth);
		s << "  selectable: " << selected_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}  

  ostream& operator << (ostream& s, const Selectable& selectable)
  {
		s << selectable.selected_;

		return s;
	}

  istream& operator >> (istream& s, Selectable& selectable)
  {
    s >> selectable.selected_;

		return s;
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
