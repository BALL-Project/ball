#ifndef BALL_VIEW_KERNEL_THREADS_H
#define BALL_VIEW_KERNEL_THREADS_H

#ifndef BALL_COMMON_H
	#include <BALL/common.h>
#endif

#ifdef BALL_QT_HAS_THREADS

#ifndef BALL_VIEW_KERNEL_REPRESENTATION_H
 #include <BALL/VIEW/KERNEL/representation.h>
#endif

#include <qthread.h>
#include <qmutex.h>

namespace BALL
{
	namespace VIEW
	{

		///
		class UpdateRepresentationThread
			: public QThread
		{
			public:

				///
				UpdateRepresentationThread()
					throw();

				///
				virtual void run();

				///
				void setRepresentation(Representation& rep)
					throw() { rep_ = &rep;}

				///
				void setRebuild(bool rebuild)
					throw(){ rebuild_ = rebuild;}

			protected:

				Representation* rep_;

				bool rebuild_;
		};

	}
}
#endif //BALL_QT_HAS_THREADS
#endif //BALL_VIEW_KERNEL_THREADS_H
