// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.C,v 1.2 2003/08/26 09:18:39 oliver Exp $

#include <BALL/VIEW/GUI/FUNCTOR/colorProcessor.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		ColorProcessor::ColorProcessor()
			throw()
			:	UnaryProcessor<GeometricObject*>()
		{
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

		void ColorProcessor::get(ColorProcessor& color_Processor) const
			throw()
		{
			color_Processor.set(*this);
		}

		void ColorProcessor::swap(ColorProcessor& color_Processor)
			throw()
		{
			color_map_.swap(color_Processor.color_map_);
			color_.swap(color_Processor.color_);
			default_color_.swap(color_Processor.default_color_);
		}


		Processor::Result ColorProcessor::operator() (const Composite* composite)
		{
			const Atom* atom = dynamic_cast<const Atom*>(composite);

			if (atom != 0)
			{
				const String key = calculateKey(*atom);
				ColorMap::ConstIterator map_it = color_map_.find(key);
				if (map_it == color_map_.end())
				{
					color_ = default_color_;
				} 
				else 
				{
					color_ = map_it->second;
				}
			} 
			else 
			{
				color_ = default_color_;
			}

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

	} // namespace VIEW
} // namespace BALL
