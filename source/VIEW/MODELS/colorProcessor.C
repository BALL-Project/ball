// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.C,v 1.15 2003/10/26 10:29:01 oliver Exp $
//

#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/DATATYPE/colorExtension2.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/DATATYPE/list.h>
#include <BALL/STRUCTURE/geometricProperties.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		ColorProcessor::ColorProcessor()
			throw()
			:	UnaryProcessor<GeometricObject*>()
		{
			clear();
		}

		ColorProcessor::ColorProcessor(const ColorProcessor& color_Processor)
			throw()
			:	UnaryProcessor<GeometricObject*>(color_Processor),
				default_color_(color_Processor.default_color_)
		{
		}

		ColorProcessor::~ColorProcessor()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<ColorProcessor>() << std::endl;
			#endif 
		}

		void ColorProcessor::clear()
			throw()
		{
			default_color_.set("FF0000FF");
			composites_ = 0;
			clearAtomGrid();
		}

		void ColorProcessor::set(const ColorProcessor& color_Processor)
			throw()
		{
			default_color_ = color_Processor.default_color_;
			composites_ = color_Processor.composites_;
		}


		const ColorProcessor& ColorProcessor::operator = (const ColorProcessor& color_Processor)
			throw()
		{
			set(color_Processor);
			return *this;
		}


		void ColorProcessor::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "default_color: " << default_color_ << endl;
					
			BALL_DUMP_STREAM_SUFFIX(s);
		}

		Processor::Result ColorProcessor::operator() (GeometricObject*& object)
		{
			if (RTTI::isKindOf<Mesh> (*object))
			{
				Mesh* mesh = dynamic_cast<Mesh*>(object);
				mesh->colorList.clear();
				if (composites_ == 0)
				{
					mesh->colorList.push_back(default_color_);
					return Processor::CONTINUE;
				}

				if (!atom_grid_created_)
				{
					createAtomGrid_();
				}

				colorMeshFromGrid_(*mesh);
			}
			

			if (object->getComposite() == 0)
			{
				object->setColor(default_color_); 
				if (RTTI::isKindOf<ColorExtension2>(*object))
				{
					ColorExtension2* two_colored = dynamic_cast<ColorExtension2*>(object);
					two_colored->setColor2(default_color_);
				}
				return Processor::CONTINUE;
			}
			
			if (!RTTI::isKindOf<ColorExtension2>(*object))
			{
				object->setColor(getColor(object->getComposite())); 
				return Processor::CONTINUE;
			}

			// ok, we have a two colored object
			ColorExtension2* two_colored = dynamic_cast<ColorExtension2*>(object);
			if (RTTI::isKindOf<Bond>(*object->getComposite()))
			{
				Bond* bond = (Bond*) object->getComposite();
				object->setColor(getColor(bond->getFirstAtom()));
				two_colored->setColor2(getColor(bond->getSecondAtom()));
			}
			else
			{
				ColorRGBA color = getColor(object->getComposite());
				object->setColor(color); 
				two_colored->setColor2(color);
			}
			return Processor::CONTINUE;
		}

		void ColorProcessor::createAtomGrid_()
			throw()
		{
			atom_grid_.clear();
			if (composites_ == 0)
			{
				return;
			}
			List<const Atom*> atoms;
			CompositeSet::ConstIterator it = composites_->begin();
			for(; it != composites_->end(); it++)
			{
				if (RTTI::isKindOf<AtomContainer>(**it))
				{
					AtomIterator ait;
					AtomContainer* acont = (AtomContainer*)(*it);
					BALL_FOREACH_ATOM(*acont, ait)
					{
						atoms.push_back(&*ait);
					}
				}
				else if (RTTI::isKindOf<Atom>(**it))
				{
					const Atom* atom = dynamic_cast<const Atom*> (*it);
					atoms.push_back(atom);
				}
			}

			BoundingBoxProcessor boxp;
			boxp.start();
			List<const Atom*>::Iterator lit = atoms.begin();
			for(;lit != atoms.end(); lit++)
			{
				boxp.operator() (*(Atom*)*lit);
			}
			boxp.finish();

			Vector3 diagonal = boxp.getUpper() - boxp.getLower();
			atom_grid_ = AtomGrid(boxp.getLower() - Vector3(6.0, 6.0, 6.0), 
														diagonal + Vector3(6.0, 6.0, 6.0),
														5.0); // spacing, increase this, it the grid consumes too much memory
		 
			for (lit = atoms.begin(); lit != atoms.end(); lit++)
			{
				atom_grid_.insert((*lit)->getPosition(), *lit);
			}

			atom_grid_created_ = true;
		}

		void ColorProcessor::colorMeshFromGrid_(Mesh& mesh)
			throw()
		{
			if (!atom_grid_created_ || atom_grid_.isEmpty())
			{
				return;
			}
			mesh.colorList.clear();
			std::vector<Vector3>::iterator sit = mesh.vertex.begin();
			for(; sit != mesh.vertex.end(); sit++)
			{
				const Atom* atom = *atom_grid_.getClosestItem(*sit, 1);

				if (atom == 0)
				{
					mesh.colorList.push_back(ColorRGBA(0.0, 0.0, 1.0));
				}
				else
				{
					mesh.colorList.push_back(getColor(atom));
				}
			}
		}

		void ColorProcessor::setComposites(const CompositeSet* composites)
			throw() 
		{ 
			composites_ = composites;
			clearAtomGrid();
		}

		void ColorProcessor::clearAtomGrid()
			throw()
		{
			atom_grid_.clear();
			atom_grid_created_ = false;
		}

		//////////////////////////////////////////////////////////////////////
		InterpolateColorProcessor::InterpolateColorProcessor()
			: ColorProcessor(),
				min_color_(ColorRGBA(0,0,1.0)),
				max_color_(ColorRGBA(1.0,1.0,0)),
				min_min_color_(ColorRGBA(1.0,1.0,1.0)),
				max_max_color_(ColorRGBA(1.0,0.0,0)),
				max_value_(1),
				min_value_(0)
		{
			default_color_ = ColorRGBA(1.0,1.0,1.0);
		}

		ColorRGBA InterpolateColorProcessor::interpolateColor(float value)
		{
			if (value < min_value_) return min_min_color_;
			if (value > max_value_) return max_max_color_;

			float red1   = min_color_.getRed();
			float green1 = min_color_.getGreen();
			float blue1  = min_color_.getBlue();

			float red2   = max_color_.getRed();
			float green2 = max_color_.getGreen();
			float blue2  = max_color_.getBlue();

			value -= min_value_;

			return ColorRGBA(red1 + (value * (red2 - red1)) 			/ max_value_,
											 green1 + (value * (green2 - green1))	/ max_value_,
											 blue1 + (value * (blue2 - blue1)) 		/ max_value_);
		}

		void InterpolateColorProcessor::setMinColor(const ColorRGBA& color)
			throw()
		{
			min_color_ = color;
		}

		void InterpolateColorProcessor::setMaxColor(const ColorRGBA& color)
			throw()
		{
			max_color_ = color;
		}

		const ColorRGBA& InterpolateColorProcessor::getMinColor() const
			throw()
		{
			return min_color_;
		}

		const ColorRGBA& InterpolateColorProcessor::getMaxColor() const
			throw()
		{
			return max_color_;
		}

		float InterpolateColorProcessor::getMaxValue() const
			throw()
		{
			return max_value_;
		}

		void InterpolateColorProcessor::setMaxValue(float value)
			throw()
		{
			max_value_ = value;
		}

		void InterpolateColorProcessor::setMinMinColor(const ColorRGBA& color)
			throw()
		{
			min_min_color_ = color;
		}

		void InterpolateColorProcessor::setMaxMaxColor(const ColorRGBA& color)
			throw()
		{
			max_max_color_ = color;
		}

		const ColorRGBA& InterpolateColorProcessor::getMinMinColor() const
			throw()
		{
			return min_min_color_;
		}

		const ColorRGBA& InterpolateColorProcessor::getMaxMaxColor() const
			throw()
		{
			return max_max_color_;
		}

	} // namespace VIEW

 } // namespace BALL
