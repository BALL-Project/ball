// $Id: colorCalculator.C,v 1.5 2000/01/11 20:15:49 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/colorCalculator.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		ColorCalculator::ColorCalculator()
			:	Visitor<Atom>()
		{
		}

		ColorCalculator::ColorCalculator
			(const ColorCalculator& color_calculator, bool /* deep */)
			:	Visitor<Atom>(color_calculator),
  			color_map_(color_calculator.color_map_),
				default_color_(color_calculator.default_color_),
				color_(color_calculator.color_)
		{
		}

		ColorCalculator::~ColorCalculator()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<ColorCalculator>() << endl;
			#endif 

			destroy();
		}

		void ColorCalculator::clear()
		{
			color_.set("FF0000FF");
			default_color_.set("FF0000FF");
			color_map_.clear();
		}

		void ColorCalculator::destroy()
		{
			clear();
		}

		void ColorCalculator::set
			(const ColorCalculator& color_calculator,
			 bool /* deep */)
		{
			color_map_ = color_calculator.color_map_;
			default_color_ = color_calculator.default_color_;
			color_ = color_calculator.color_;
		}

		ColorCalculator& 	ColorCalculator::operator =
			(const ColorCalculator& color_calculator)
		{
			set(color_calculator);

			return *this;
		}

		void ColorCalculator::get
			(ColorCalculator& color_calculator, bool deep) const
		{
			color_calculator.set(*this, deep);
		}

		void ColorCalculator::swap
			(ColorCalculator& color_calculator)
		{
			color_map_.swap(color_calculator.color_map_);
			color_.swap(color_calculator.color_);
			default_color_.swap(color_calculator.default_color_);
		}

		void ColorCalculator::visit(Atom& atom)
		{
			String key = calculateKey(atom);
			ColorMap::ConstIterator map_it = color_map_.find(key);
			if (map_it == color_map_.end())
			{
				color_ = default_color_;
			} else {
				color_ = map_it->second;
			}
		}

		void ColorCalculator::visit
			(Composite& composite)
		{
			Atom* atom = dynamic_cast<Atom*>(&composite);

			if (atom != 0)
			{
				visit(*atom);
			} else {
				color_ = default_color_;
			}
		}

		bool ColorCalculator::isValid() const
		{
			return true;
		}

		void ColorCalculator::dump
			(ostream& s, Size depth) const
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

		const String& ColorCalculator::calculateKey(const Atom& atom) const
		{
			return atom.getElement().getSymbol();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/colorCalculator.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
