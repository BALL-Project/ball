// $Id: colorCalculator.C,v 1.2 1999/12/19 17:14:25 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/colorCalculator.h>

namespace BALL
{

	namespace MOLVIEW
	{

		ColorCalculator::ColorCalculator()
			:	Visitor<Atom>(),
				color_(),
				color_table_(const_cast<ColorTable*>((const ColorTable*)&RTTI<ElementColor>::getDefault()))
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
					<< " of class " << RTTI<ColorCalculator>::getName() << endl;
			#endif 

			destroy();
		}

		void 
		ColorCalculator::clear
			()
		{
			color_.set("000000FF");
			color_table_ = const_cast<ColorTable*>((const ColorTable*)&RTTI<ElementColor>::getDefault());
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

			if (RTTI<Atom>::isKindOf(composite) == true)
			{
				atom = RTTI<Atom>::castTo(composite);

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
			(ostream& s,
			 unsigned long depth) const
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
					 << ((color_table_ ==  (const ColorTable*)&RTTI<ElementColor>::getDefault())
										 ? "yes" : "no")
					 << endl;
					
			BALL_DUMP_DEPTH(s, depth);
			s << "is other colortable: " 
					 << ((color_table_ !=  (const ColorTable*)&RTTI<ElementColor>::getDefault())
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
