// $Id: fragment.C,v 1.1 1999/08/26 08:02:34 oliver Exp $

#include <BALL/KERNEL/fragment.h>

namespace BALL 
{

	Fragment::Fragment(void)
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

	Fragment::~Fragment(void)
	{
		destroy();
	}

	void Fragment::clear(void)
	{
		BaseFragment::clear();
	}
		
	void Fragment::destroy(void)
	{
		BaseFragment::destroy();
	}

	void Fragment::persistentWrite(PersistenceManager& pm, const char* name) const
  {
    pm.writeObjectHeader(this, name);
      BaseFragment::persistentWrite(pm);
		pm.writeObjectTrailer(name);
	}

  void Fragment::persistentRead(PersistenceManager& pm)
  {    
		pm.checkObjectHeader(RTTI<BaseFragment>::getStreamName());
			BaseFragment::persistentRead(pm);
    pm.checkObjectTrailer(0);
	}
	
	Fragment& Fragment::operator =(const Fragment& fragment)
	{
		BaseFragment::operator =(fragment);

		return *this;
	}
			
	bool Fragment::isValid(void) const
	{ 
		return BaseFragment::isValid();
	}

	void Fragment::dump(ostream& s, unsigned long depth) const
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
