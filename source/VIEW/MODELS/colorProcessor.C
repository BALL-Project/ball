// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.C,v 1.31.2.10 2005/01/14 13:11:45 amoll Exp $
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
			:	UnaryProcessor<GeometricObject*>(),
				update_always_needed_(false)
		{
			clear();
		}

		ColorProcessor::ColorProcessor(const ColorProcessor& cp)
			throw()
			:	UnaryProcessor<GeometricObject*>(cp),
				default_color_(cp.default_color_),
				selection_color_(cp.selection_color_),
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
			selection_color_ = BALL_SELECTED_COLOR;
			last_composite_of_grid_ = 0;
			additional_grid_distance_ = 5.0;
		}

		void ColorProcessor::set(const ColorProcessor& cp)
			throw()
		{
			default_color_ = cp.default_color_;
			composites_ = cp.composites_;
			transparency_ = cp.transparency_;
			selection_color_ = cp.selection_color_;
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
			const Composite* composite = object->getComposite();

			Mesh* const mesh = dynamic_cast<Mesh*>(object);
			if (mesh != 0)
			{
				mesh->colorList.clear();
				if (composite == &composite_to_be_ignored_for_colorprocessors_ ||
						composites_ == 0)
				{
					mesh->colorList.push_back(default_color_);
					return Processor::CONTINUE;
				}

				if (composite == 0 ||
						composite != last_composite_of_grid_)
				{
					createAtomGrid_(composite);
				}

				colorMeshFromGrid_(*mesh);
				return Processor::CONTINUE;
			}

			ColorExtension2* const two_colored = dynamic_cast<ColorExtension2*>(object);

			if (composite == 0 ||
					composite == &composite_to_be_ignored_for_colorprocessors_)
			{
				object->setColor(default_color_); 
				if (two_colored != 0)
				{
					two_colored->setColor2(default_color_);
				}
				return Processor::CONTINUE;
			}

			if (two_colored == 0)
			{
				if (composite->isSelected())
				{
					object->setColor(selection_color_);
				}
				else
				{
					getColor(*composite, object->getColor()); 
				}
				return Processor::CONTINUE;
			}

			// ok, we have a two colored object
			const Bond* const bond = dynamic_cast<const Bond*>(composite);
			if (bond != 0)
			{
				const Atom* atom = bond->getFirstAtom();
				if (!atom->isSelected())
				{
					getColor(*atom, object->getColor());
				}
				else
				{
					object->setColor(selection_color_);
				}

				const Atom* atom2 = bond->getSecondAtom();
				if (!atom2->isSelected())
				{
					getColor(*atom2, two_colored->getColor2());
				}
				else
				{
					two_colored->setColor2(selection_color_);
				}
			}
			else
			{
				if (composite->isSelected())
				{
					object->setColor(selection_color_);
					two_colored->setColor2(selection_color_);
				}
				else
				{
 					getColor(*composite, object->getColor());
 					two_colored->setColor2(object->getColor());
				}
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

			const Vector3 diagonal = boxp.getUpper() - boxp.getLower();
			
			// grid spacing, tradeoff between speed and memory consumption
			float grid_spacing = 4.0;
			if (diagonal.getSquareLength() < 5000)
			{
				grid_spacing = 3.0;
			} 
			else if (diagonal.getSquareLength() > 100000)
			{
				// well this will be slower, but prevent locking machine by consuming all memory
				grid_spacing = 5.0;
			}
			
			atom_grid_ = AtomGrid(boxp.getLower() - Vector3(additional_grid_distance_),
														diagonal + Vector3(2 * additional_grid_distance_),
														grid_spacing); 
		 
			for (lit = atoms.begin(); lit != atoms.end(); lit++)
			{
				atom_grid_.insert((*lit)->getPosition(), *lit);
			}

			last_composite_of_grid_ = from_mesh;
		}

		void ColorProcessor::colorMeshFromGrid_(Mesh& mesh)
			throw()
		{
			if (atom_grid_.isEmpty()) return;
			
			mesh.colorList.resize(mesh.vertex.size());
			
			for (Position p = 0; p < mesh.vertex.size(); p++)
			{
				// make sure we found an atom
				const Atom* atom = getClosestItem_(mesh.vertex[p]);

				if (atom == 0)
				{
 					mesh.colorList[p] = default_color_;
				}
				else
				{
					if (atom->isSelected())
					{
						mesh.colorList[p] = selection_color_;
					}
					else
					{
 						getColor(*atom, mesh.colorList[p]);
					}
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
			selection_color_.setAlpha(255 - value);
		}

		void ColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			if (composite.isSelected())
			{
				color_to_be_set = selection_color_;
			}
			else
			{
				color_to_be_set = default_color_;
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

		void InterpolateColorProcessor::setTransparency(Size value)
			throw() 
		{ 
			ColorProcessor::setTransparency(value);
			min_color_.setAlpha(255 - value);
			max_color_.setAlpha(255 - value);
			min_min_color_.setAlpha(255 - value);
			max_max_color_.setAlpha(255 - value);
		}

		void InterpolateColorProcessor::interpolateColor(float value, ColorRGBA& color_to_be_set)
			throw()
		{
			if (value < min_value_)
			{
				color_to_be_set.set(min_min_color_);
				return;
			}
			
			if (value > max_value_) 
			{
				max_max_color_.set(max_max_color_);
				return;
			}

			float red1   = min_color_.getRed();
			float green1 = min_color_.getGreen();
			float blue1  = min_color_.getBlue();

			float red2   = max_color_.getRed();
			float green2 = max_color_.getGreen();
			float blue2  = max_color_.getBlue();

			value -= min_value_;

			color_to_be_set.set(red1 + (value * (red2 - red1)) 			/ max_value_,
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
