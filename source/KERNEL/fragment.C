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
		setBeingDestroyed_();  // v2.2 H2a (D69): before destroy()'s teardown
		destroy();
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
