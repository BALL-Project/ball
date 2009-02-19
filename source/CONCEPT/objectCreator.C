// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: objectCreator.C,v 1.1.4.1 2007/03/25 22:00:06 oliver Exp $

#include <BALL/CONCEPT/objectCreator.h>

using namespace std;

namespace BALL
{
  
	ObjectCreator::ObjectCreator()
			
		:
		init_(false),
		pm_()
	{
	}

	ObjectCreator::~ObjectCreator()
			
	{
		#ifdef BALL_DEBUG
			cout << "Destructing object " << (void *)this 
				<< " of class " << RTTI::getName<ObjectCreator>() << endl;
		#endif 
	}

	void ObjectCreator::clear()
			
	{
	}


	void ObjectCreator::initPersistenceManager(TextPersistenceManager & /* pm */)
			
	{
	}

	Composite *ObjectCreator::convertObject(PersistentObject & /* po */)
			
	{
		return (Composite *)0;
	}

#ifdef BALL_HAS_ASIO
	Composite *ObjectCreator::operator() (TCPIOStream& tcp_iostream)
			
	{
		// initialize the PersistenceManager only one times
		if (init_ == false)
		{
			init_ = true;

			initPersistenceManager(pm_);
		}

		// read persistent object from stream
		pm_.setIstream(tcp_iostream);
		PersistentObject*	po = pm_.readObject();
		
		// convert the object
		return convertObject(*po);
	}
#endif

} // namespace BALL
