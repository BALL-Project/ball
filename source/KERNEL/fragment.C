// $Id: fragment.C,v 1.5 2000/04/17 13:51:33 amoll Exp $

#include <BALL/KERNEL/fragment.h>

using namespace std;

namespace BALL 
{

	Fragment::Fragment()
		:	BaseFragment()
	{
	}
		
	Fragment::Fragment(const Fragment& fragment, bool deep)
		: BaseFragment(fragment, deep)
	{
	}
		
	Fragment::Fragment(const String& name)
		: BaseFragment(name)
	{
	}

	Fragment::~Fragment()
	{
		destroy();
	}

	void Fragment::persistentWrite(PersistenceManager& pm, const char* name) const
  {
    pm.writeObjectHeader(this, name);
      BaseFragment::persistentWrite(pm);
		pm.writeObjectTrailer(name);
	}

  void Fragment::persistentRead(PersistenceManager& pm)
  {    
		pm.checkObjectHeader(RTTI::getStreamName<BaseFragment>());
			BaseFragment::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}
	
	Fragment& Fragment::operator =(const Fragment& fragment)
	{
		BaseFragment::operator =(fragment);

		return *this;
	}
			
	void Fragment::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BaseFragment::dump(s, depth);
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
