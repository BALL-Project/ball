// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: fragment.C,v 1.12 2002/02/27 12:21:23 sturm Exp $

#include <BALL/KERNEL/fragment.h>

using namespace std;

namespace BALL 
{

	Fragment::Fragment()
		throw()
		:	AtomContainer()
	{
	}
		
	Fragment::Fragment(const Fragment& fragment, bool deep)
		throw()
		: AtomContainer(fragment, deep)
	{
	}
		
	Fragment::Fragment(const String& name)
		throw()
		: AtomContainer(name)
	{
	}

	Fragment::~Fragment()
		throw()
	{
		destroy();
	}

	void Fragment::persistentWrite(PersistenceManager& pm, const char* name) const
		throw(Exception::GeneralException)
  {
    pm.writeObjectHeader(this, name);
      AtomContainer::persistentWrite(pm);
		pm.writeObjectTrailer(name);
	}

  void Fragment::persistentRead(PersistenceManager& pm)
		throw(Exception::GeneralException)
  {    
		pm.checkObjectHeader(RTTI::getStreamName<AtomContainer>());
			AtomContainer::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}
	
	const Fragment& Fragment::operator =(const Fragment& fragment)
		throw()
	{
		AtomContainer::operator =(fragment);
		return *this;
	}

	bool Fragment::operator == (const Fragment& fragment) const
		throw()
	{
		return(AtomContainer::operator ==(fragment));
	}

	bool Fragment::operator != (const Fragment& fragment) const
		throw()
	{
		return !(*this == fragment);
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

} // namespace BALL
