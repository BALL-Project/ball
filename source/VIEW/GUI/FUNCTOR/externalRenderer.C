// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: externalRenderer.C,v 1.3 2002/12/18 16:00:43 sturm Exp $

#include <BALL/VIEW/GUI/FUNCTOR/externalRenderer.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		ExternalRenderer::ExternalRenderer()
			throw()
			: UnaryProcessor<Composite>(),
				scene_(0)
		{
		}

		ExternalRenderer::~ExternalRenderer()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<ExternalRenderer>() << endl;
			#endif 

			destroy();
		}

		void ExternalRenderer::clear()
			throw()
		{
		}
		
		void ExternalRenderer::destroy()
			throw()
		{
		}

		void ExternalRenderer::setScene(const Scene& scene)
			throw()
		{
			scene_ = &scene;
		}
				
		bool ExternalRenderer::start()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				Log.info() << "start collect process ..." << endl;
      #endif

			clear();
			
			return true;
		}

		bool ExternalRenderer::finish()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG_PROCESSORS
				Log.info() << "Finished collect process..." << endl;
			#endif

			return true;
		}

		Processor::Result ExternalRenderer::operator() (Composite& composite)
			throw()
		{
			return Processor::CONTINUE;
		}
	}
}
