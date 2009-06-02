// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: factory.h,v 1.10 2003/08/26 08:04:07 oliver Exp $
//

#ifndef MODEL_FACTORY_H
#define MODEL_FACTORY_H

#ifndef BALL_CONCEPT_FACTORY_H
#include <BALL/CONCEPT/factory.h>
#endif

#ifndef MODEL
#include <BALL/QSAR/Model.h>
#endif

#ifndef QSARH
#include <BALL/QSAR/QSARData.h>
#endif

namespace BALL
{
	namespace QSAR
	{
		//typedef Model* (*CreateMethod) (const QSARData& q);
		
		/** Factory class.
		The factory class is used to create objects of the various regression / classification Models it was parameterized with. */
		template <typename T>
		class ModelFactory
		{
			public:
			
			static T* create(const QSARData& q)
			{
				return new T(q);
			}
			
			static T* createKernel1(const QSARData& q, int k, double p1, double p2)
			{
				return new T(q,k,p1,p2);
			}
			
			static T* createKernel2(const QSARData& q, String s1, String s2)
			{
				return new T(q,s1,s2);
			}
		};
	}
}

#endif // QSAR_FACTORY_H
