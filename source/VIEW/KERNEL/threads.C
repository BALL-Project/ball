// Do nothing, if no thread support

#include <BALL/VIEW/KERNEL/threads.h>
#ifdef BALL_QT_HAS_THREADS

#include <BALL/VIEW/MODELS/modelProcessor.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>

namespace BALL
{
	namespace VIEW
	{

UpdateRepresentationThread::UpdateRepresentationThread()
	throw() 
	: QThread(),
		rep_(0)
{
}


void UpdateRepresentationThread::run()
{
	if (rep_ == 0) return;
	rep_->update_(rebuild_);
}

} }

#endif //Thread support
