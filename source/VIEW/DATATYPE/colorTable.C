// $Id: colorTable.C,v 1.4 1999/12/30 18:05:44 oliver Exp $

#include <BALL/VIEW/DATATYPE/colorTable.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorTable::ColorTable()
			:	vector<ColorRGBA>()
		{
		}

		ColorTable::ColorTable(const ColorTable& color_table, bool /* deep */)
			:	vector<ColorRGBA>(color_table)
		{
		}

		ColorTable::ColorTable(Size size, const ColorRGBA& color)
			:	vector<ColorRGBA>(size, color)
		{
		}

		ColorTable::ColorTable(const ColorTable& color_table, Index from, Index to)
		{
			clear();
			for (Index i = from; i <= to; i++)
			{
				push_back(color_table[i]);
			}
		}
		
		ColorTable::ColorTable
			(const ColorRGBA *array, Size array_size)
		{
			clear();
			for (Size i = 0; i < array_size; i++)
			{
				push_back(array[i]);
			}
		}
		

		ColorTable::~ColorTable()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
			 << RTTI::getName<ColorTable>() << endl;
			#endif 

			clear();
		}

		void ColorTable::dump
			(ostream& s, Size /* depth */) const
		{
			for (Size i = 0; i < size(); i++)
			{
				s << operator[](i) << endl;
			}
		}

		void ColorTable::read(istream & /*s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ColorTable::write(ostream & /*s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorTable.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
