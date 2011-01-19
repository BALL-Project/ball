// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/fragment.h>

using namespace::std;
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
	
	Fragment& Fragment::operator = (const Fragment& fragment)
	{
		AtomContainer::operator =(fragment);
		return *this;
	}

	bool Fragment::operator == (const Fragment& fragment) const
	{
		return(AtomContainer::operator ==(fragment));
	}

	bool Fragment::operator != (const Fragment& fragment) const
	{
		return !(*this == fragment);
	}
			
	void Fragment::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s)
		
		AtomContainer::dump(s, depth);
		// just to avoid these damned compiler warnings 
		// (dump_indent_depth_ was declared but never referenced)
		if (dump_indent_depth_ == 0) {};
		
		BALL_DUMP_STREAM_SUFFIX(s)
	}

} // namespace BALL
