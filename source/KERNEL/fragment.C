// $Id: fragment.C,v 1.7 2000/12/11 21:14:48 oliver Exp $

#include <BALL/KERNEL/fragment.h>

using namespace std;

namespace BALL 
{

	Fragment::Fragment()
		:	AtomContainer()
	{
	}
		
	Fragment::Fragment(const Fragment& fragment, bool deep)
		: AtomContainer(fragment, deep)
	{
	}
		
	Fragment::Fragment(const String& name)
		: AtomContainer(name)
	{
	}

	Fragment::~Fragment()
		throw()
	{
		destroy();
	}

	void Fragment::persistentWrite(PersistenceManager& pm, const char* name) const
  {
    pm.writeObjectHeader(this, name);
      AtomContainer::persistentWrite(pm);
		pm.writeObjectTrailer(name);
	}

  void Fragment::persistentRead(PersistenceManager& pm)
  {    
		pm.checkObjectHeader(RTTI::getStreamName<AtomContainer>());
			AtomContainer::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}
	
	Fragment& Fragment::operator =(const Fragment& fragment)
	{
		AtomContainer::operator =(fragment);

		return *this;
	}
			
	void Fragment::dump(ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		AtomContainer::dump(s, depth);
		// just to avoid these damned compiler warnings 
		// (dump_indent_depth_ was declared but never referenced)
		if (dump_indent_depth_ == 0) ;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void Fragment::read(istream&  /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Fragment::write(ostream& /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

} // namespace BALL
