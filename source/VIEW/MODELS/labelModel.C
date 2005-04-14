// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: labelModel.C,v 1.1.2.1 2005/04/14 10:30:08 amoll Exp $
//

#include <BALL/VIEW/MODELS/labelModel.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/KERNEL/atom.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		LabelModel::LabelModel()
			throw()
			: ModelProcessor(),
				font_("Helvetica", 12),
				color_(),
				many_labels_(true),
				nr_objects_(0),
				center_()

		{
		}

		LabelModel::LabelModel(const LabelModel& model)
			throw()
			: ModelProcessor(model),
				font_(model.font_),
				color_(model.color_),
				many_labels_(model.many_labels_),
				nr_objects_(0)
		{
		}

		LabelModel::~LabelModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << this << " of class LabelModel" << std::endl;
			#endif 
		}

		Processor::Result LabelModel::operator() (Composite &composite)
		{
			if (many_labels_)
			{
				center_processor_.start();
				composite.apply(center_processor_);
				// create Label and Representation
				Label* label = new Label;
				label->setText(text_);
				label->setColor(color_);
				label->setVertex(center_processor_.getCenter());
				label->setFont(font_);
				label->setComposite(&composite);
				geometric_objects_.push_back(label);
			}
			else
			{
				if (RTTI::isKindOf<Atom>(composite)) 
				{
					center_ += (dynamic_cast<Atom*>(&composite))->getPosition();
					nr_objects_ ++;
				}
			}

			return Processor::CONTINUE;
		}

		bool LabelModel::createGeometricObjects() 
			throw()
		{
			if (many_labels_ || nr_objects_ == 0) return true;

			Label* label = new Label;
			label->setText(text_);
			label->setColor(color_);
			center_ /= nr_objects_;
			label->setVertex(center_);
			label->setFont(font_);
			geometric_objects_.push_back(label);

			nr_objects_ = 0;

			return true;
		}

	} // namespace VIEW
} // namespace BALL
