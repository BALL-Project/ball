// $Id: colorCalculator.C,v 1.4 1999/12/30 18:05:37 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/colorCalculator.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		ColorCalculator::ColorCalculator()
			:	Visitor<Atom>(),
				color_(),
				color_table_(const_cast<ColorTable*>((const ColorTable*)&RTTI::getDefault<ElementColor>()))
		{
		}

		ColorCalculator::ColorCalculator
			(const ColorCalculator& color_calculator, bool /* deep */)
			:	Visitor<Atom>(color_calculator),
				color_(),
				color_table_(color_calculator.color_table_)
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

		void 
		ColorCalculator::clear
			()
		{
			color_.set("000000FF");
			color_table_ = const_cast<ColorTable*>((const ColorTable*)&RTTI::getDefault<ElementColor>());
		}

		void 
		ColorCalculator::destroy
			()
		{
			clear();
		}

		void 
		ColorCalculator::set
			(const ColorCalculator& color_calculator,
			 bool /* deep */)
		{
			color_table_ = color_calculator.color_table_;
		}

		ColorCalculator& 
		ColorCalculator::operator =
			(const ColorCalculator& color_calculator)
		{
			set(color_calculator);

			return *this;
		}

		void 
		ColorCalculator::get
			(ColorCalculator& color_calculator,
			 bool deep) const
		{
			color_calculator.set(*this, deep);
		}

		void 
		ColorCalculator::swap
			(ColorCalculator& color_calculator)
		{
			ColorTable *__pColorTable = color_table_;
			color_table_ = color_calculator.color_table_;
			color_calculator.color_table_ = __pColorTable;
		}

		void ColorCalculator::visit(Atom &atom)
		{
			color_ = color_table_->operator[]((atom.getElement()).getAtomicNumber());
		}

		void 
		ColorCalculator::visit
			(Composite &composite)
		{
			Atom *atom = 0;

			if (RTTI::isKindOf<Atom>(composite) == true)
			{
				atom = RTTI::castTo<Atom>(composite);

				color_ = color_table_->operator[]((atom->getElement()).getAtomicNumber());
			}
		}

		bool 
		ColorCalculator::isValid
			() const
		{
			return (bool)(color_.isValid() == true);
		}

		void 
		ColorCalculator::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "color: " << color_ << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "color table *: " << (void *)color_table_ << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "is default element colortable: " 
					 << ((color_table_ ==  (const ColorTable*)&RTTI::getDefault<ElementColor>())
										 ? "yes" : "no")
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "is other colortable: " 
					 << ((color_table_ !=  (const ColorTable*)&RTTI::getDefault<ElementColor>())
										 ? "yes" : "no")
					 << endl;
					
			color_table_->dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/colorCalculator.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
