// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.C,v 1.4 2003/10/15 13:59:06 amoll Exp $

#include <BALL/VIEW/MODELS/colorProcessor.h>

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
		color_map_(color_Processor.color_map_),
		default_color_(color_Processor.default_color_),
		color_(color_Processor.color_)
{
}

ColorProcessor::~ColorProcessor()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class " << RTTI::getName<ColorProcessor>() << std::endl;
	#endif 
}

void ColorProcessor::clear()
	throw()
{
	color_.set("FF0000FF");
	default_color_.set("FF0000FF");
	color_map_.clear();
}

void ColorProcessor::set(const ColorProcessor& color_Processor)
	throw()
{
	color_map_ = color_Processor.color_map_;
	default_color_ = color_Processor.default_color_;
	color_ = color_Processor.color_;
}


const ColorProcessor& ColorProcessor::operator = (const ColorProcessor& color_Processor)
	throw()
{
	set(color_Processor);
	return *this;
}


void ColorProcessor::swap(ColorProcessor& color_Processor)
	throw()
{
	color_map_.swap(color_Processor.color_map_);
	color_.swap(color_Processor.color_);
	default_color_.swap(color_Processor.default_color_);
}


Processor::Result ColorProcessor::operator() (const Composite* /*composite*/)
{
	color_ = default_color_;
	return Processor::CONTINUE;
}


void ColorProcessor::dump(ostream& s, Size depth) const
	throw()
{
	BALL_DUMP_STREAM_PREFIX(s);
	
	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	BALL_DUMP_DEPTH(s, depth);
	s << "color: " << color_ << endl;
			
	BALL_DUMP_DEPTH(s, depth);
	s << "default_color: " << default_color_ << endl;
			
	BALL_DUMP_DEPTH(s, depth);
	s << "color map: " << endl;
	color_map_.dump(s, depth + 2)
			
	BALL_DUMP_STREAM_SUFFIX(s);
}

Processor::Result ColorProcessor::operator() (GeometricObject*& object)
{
	if (object->getComposite() == 0)
	{
		object->setColor(default_color_); 
	}
	else
	{
		operator () (object->getComposite());
		object->setColor(color_); 
	}
	return Processor::CONTINUE;
}

void ColorProcessor::getColor(const String& key)
{
	if (color_map_.has(key))
	{
		color_ = color_map_[key];
	}
	else
	{
		color_ = default_color_;
	}
}

} } // namespaces
