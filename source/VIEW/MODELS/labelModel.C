// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/MODELS/labelModel.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/residue.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		LabelModel::LabelModel()
			: ModelProcessor(),
				font_("Helvetica", 12),
				color_(),
				mode_(ALL_ITEMS),
				nr_objects_(0),
				center_(),
				center_processor_()
		{
		}

		LabelModel::LabelModel(const LabelModel& model)
			: ModelProcessor(model),
				font_(model.font_),
				color_(model.color_),
				mode_(model.mode_),
				nr_objects_(0),
				center_(),
				center_processor_()
		{
		}

		LabelModel::~LabelModel()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << this << " of class LabelModel" << std::endl;
			#endif 
		}

		Processor::Result LabelModel::operator() (Composite &composite)
		{
			if (mode_ == ONE_LABEL)
			{
				if (RTTI::isKindOf<Atom>(composite)) 
				{
					center_ += (dynamic_cast<Atom*>(&composite))->getPosition();
					nr_objects_ ++;
				}

				return Processor::CONTINUE;
			}


			// create a label for this composite?
			bool do_it = false;

			if (mode_ == ALL_ITEMS)
			{
				do_it = true;
			}
			else if (mode_ == ALL_ATOMS)
			{
				do_it = RTTI::isKindOf<Atom>(composite);
			}
			else if (mode_ == ALL_RESIDUES)
			{
				do_it = RTTI::isKindOf<Residue>(composite);
			}

			if (!do_it) return Processor::CONTINUE;

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

			return Processor::CONTINUE;
		}

		bool LabelModel::createGeometricObjects() 
		{
			if (mode_ != ONE_LABEL || nr_objects_ == 0) return true;

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
