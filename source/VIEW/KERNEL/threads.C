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

	if (rebuild_ && rep_->getModelProcessor() != 0)
	{
		Representation::CompositeSet::Iterator it = rep_->getComposites().begin();
		for (; it!= rep_->getComposites().end(); it++)
		{
			(const_cast<Composite*>(*it))->apply(*rep_->getModelProcessor());
		}
	}

	rep_->getGeometricObjects() = rep_->getModelProcessor()->getGeometricObjects();

	if (rep_->getColorProcessor() != 0) 
	{
		// make sure, that the atom grid is recomputed for meshes
		if (rebuild_) rep_->getColorProcessor()->setComposites(&rep_->getComposites());
		rep_->getColorProcessor()->setTransparency(rep_->getTransparency());
		rep_->getGeometricObjects().apply(*rep_->getColorProcessor());
	}
}

} }

#endif //Thread support
