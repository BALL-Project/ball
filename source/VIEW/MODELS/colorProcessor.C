// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.C,v 1.31.2.1 2004/12/27 17:44:18 amoll Exp $
//

#include <BALL/VIEW/MODELS/colorProcessor.h>

#include <BALL/COMMON/limits.h>
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

		ColorProcessor::ColorProcessor(const ColorProcessor& cp)
			throw()
			:	UnaryProcessor<GeometricObject*>(cp),
				default_color_(cp.default_color_),
				transparency_(0)
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
			transparency_ = 0;
			composites_ = 0;
			clearAtomGrid();
		}

		void ColorProcessor::set(const ColorProcessor& cp)
			throw()
		{
			default_color_ = cp.default_color_;
			composites_ = cp.composites_;
			transparency_ = cp.transparency_;
		}


		const ColorProcessor& ColorProcessor::operator = (const ColorProcessor& cp)
			throw()
		{
			set(cp);
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

				if (!atom_grid_created_ || mesh->getComposite() != 0)
				{
					const Composite* c = mesh->getComposite();
					createAtomGrid_(c);
				}

				colorMeshFromGrid_(*mesh);

				return Processor::CONTINUE;
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

		void ColorProcessor::createAtomGrid_(const Composite* from_mesh)
			throw()
		{
			atom_grid_.clear();
			if (composites_ == 0 && from_mesh == 0)
			{
				return;
			}
			
			List<const Atom*> atoms;

			if (from_mesh == 0)
			{
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
			}
			else 
			{
				// composite from mesh
				if (RTTI::isKindOf<AtomContainer>(*from_mesh))
				{
					AtomIterator ait;
					AtomContainer* acont = (AtomContainer*)(from_mesh);
					BALL_FOREACH_ATOM(*acont, ait)
					{
						atoms.push_back(&*ait);
					}
				}
				else if (RTTI::isKindOf<Atom>(*from_mesh))
				{
					const Atom* atom = dynamic_cast<const Atom*> (from_mesh);
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
			atom_grid_ = AtomGrid(boxp.getLower() - Vector3(5, 5, 5), 
														diagonal + Vector3(10, 10,10),
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
				// make sure we found an atom
				const Atom* atom = getClosestItem_(*sit);

				if (atom == 0)
				{
					mesh.colorList.push_back(default_color_);
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

		void ColorProcessor::setDefaultColor(const ColorRGBA& color)
			throw()
		{
			default_color_ = color;
			default_color_.setAlpha(255 - transparency_);
		}

		void ColorProcessor::setTransparency(Size value)
			throw() 
		{ 
			transparency_ = value;
			default_color_.setAlpha(255 - value);
		}

		ColorRGBA ColorProcessor::getColor(const Composite* composite)
		{
			if (composite == 0) return default_color_;

			if (composite->isSelected())
			{
				ColorRGBA color(BALL_SELECTED_COLOR);
				color.setAlpha(255 - transparency_);
				return color;
			}
			else
			{
				return default_color_;
			}
		}


		const Atom* ColorProcessor::getClosestItem_(const Vector3& point) const
			throw()
		{
			const HashGridBox3<const Atom*>* box = atom_grid_.getBox(point);
			if (!box)
			{
				return 0;
			}

			Position x, y, z;
			atom_grid_.getIndices(*box, x, y, z);

			const Atom* const* item = 0;
			float distance = Limits<float>::max();
			List<HashGridBox3<const Atom*>* > box_list;
			Size dist = 1;
			// iterator over neighbour boxes
			for (Index xi = -(Index)dist; xi <= (Index)dist; xi++)
			{
				for (Index yi = -(Index)dist; yi <= (Index)dist; yi++)
				{
					for (Index zi = -(Index)dist; zi <= (Index)dist; zi++)
					{
						// iterate over all data items
						const HashGridBox3<const Atom*>* box_ptr = atom_grid_.getBox(x+xi, y+yi, z+zi);	
						if (box_ptr != 0 && !box_ptr->isEmpty())
						{
							HashGridBox3<const Atom*>::ConstDataIterator hit = box_ptr->beginData();
							for (;hit != box_ptr->endData(); hit++)
							{
								// this is not 
								float radius = (*hit)->getElement().getVanDerWaalsRadius();
								if (model_type_ == MODEL_SA_SURFACE)
								{
									radius += 2;
								}
								if (radius <= 0.0) radius = 1;
								// avoid calculation of the square roots
								float new_dist = ((*hit)->getPosition() - point).getSquareLength() - radius * radius;
								if (new_dist < distance)
								{
									item = &*hit;
									distance = new_dist;
								}
							}
						}
					}
				}
			}

			if (item == 0) return 0;
			
			return *item;
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
											 blue1 + (value * (blue2 - blue1)) 		/ max_value_,
											 255 - transparency_);
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

		float InterpolateColorProcessor::getMinValue() const
			throw()
		{
			return min_value_;
		}

		void InterpolateColorProcessor::setMinValue(float value)
			throw()
		{
			min_value_ = value;
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

		bool InterpolateColorProcessor::start()
		{
			min_min_color_.setAlpha(255 - transparency_);
			min_color_.setAlpha(255 - transparency_);
			max_max_color_.setAlpha(255 - transparency_);
			max_color_.setAlpha(255 - transparency_);
			default_color_.setAlpha(255 - transparency_);

			return true;
		}

	} // namespace VIEW
 } // namespace BALL
