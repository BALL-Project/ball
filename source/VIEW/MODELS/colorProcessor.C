// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.C,v 1.9 2003/10/19 14:09:48 amoll Exp $

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
	atom_grid_.clear();
	atom_grid_created_ = false;
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

	atom_grid_ = AtomGrid(boxp.getLower() - Vector3(10,10,10), 
												(boxp.getUpper() - boxp.getLower()) + Vector3(10,10,10), 5);
 
	for(lit = atoms.begin(); lit != atoms.end(); lit++)
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
	vector<Vector3>::iterator sit = mesh.vertex.begin();
	for(; sit != mesh.vertex.end(); sit++)
	{
		AtomBox* box = atom_grid_.getBox(*sit);
		float distance = 9999999;
		const Atom* atom = 0;
		
		List<AtomBox*> boxes = getNeighbourBoxes_(*box);
		List<AtomBox*>::Iterator boxes_it = boxes.begin();
		for (; boxes_it != boxes.end(); boxes_it++)
		{
			AtomBox::DataIterator hit = (*boxes_it)->beginData();
			for (;hit != box->endData(); hit++)
			{
				float new_dist = ((*hit)->getPosition() - *sit).getSquareLength();
				if (new_dist < distance)
				{
					atom = *hit;
					distance = new_dist;
				}
			}
		}

		if (atom == 0)
		{
			mesh.colorList.push_back(ColorRGBA(0,.0,1));
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
	atom_grid_created_ = false;
	atom_grid_.clear();
}


List<ColorProcessor::AtomBox*> ColorProcessor::getNeighbourBoxes_(ColorProcessor::AtomBox& box)
	throw()
{
	List<ColorProcessor::AtomBox*> box_list;
	Position x, y, z;
	atom_grid_.getIndices(box, x, y, z);
	for (Index xi = -1; xi <= 1; xi++)
	{
		for (Index yi = -1; yi <= 1; yi++)
		{
			for (Index zi = -1; zi <= 1; zi++)
			{
				AtomBox* box_ptr = atom_grid_.getBox(x+xi, y+yi, z+zi);	
				if (box_ptr != 0 && !box_ptr->isEmpty())
				{
					box_list.push_back(box_ptr);
				}
			}
		}
	}
	return box_list;
}


} } // namespaces
