// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.C,v 1.7 2003/10/18 11:24:20 amoll Exp $

#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/DATATYPE/colorExtension2.h>
#include <BALL/KERNEL/bond.h>

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

} } // namespaces
