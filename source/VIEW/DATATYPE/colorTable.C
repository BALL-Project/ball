// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorTable.C,v 1.4.2.1 2003/01/07 13:23:18 anker Exp $

#include <BALL/VIEW/DATATYPE/colorTable.h>
#include <BALL/COMMON/rtti.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		ColorTable::ColorTable()
			throw()
			:	vector<ColorRGBA>(),
				color_number_(0),
				alpha_blending_(false),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.)
		{
		}

		ColorTable::ColorTable(Size color_number)
			throw()
			:	vector<ColorRGBA>(color_number),
				color_number_(color_number),
				alpha_blending_(false),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.)
		{
		}

		ColorTable::ColorTable(const ColorTable& color_table)
			throw()
			:	vector<ColorRGBA>(color_table),
				color_number_(color_table.color_number_),
				alpha_blending_(color_table.alpha_blending_),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.)
		{
		}

		ColorTable::ColorTable(Size size, const ColorRGBA& color, bool alpha_blending)
			throw()
			:	vector<ColorRGBA>(size, color),
				color_number_(size),
				alpha_blending_(alpha_blending),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.)
		{
		}

		ColorTable::ColorTable(const ColorTable& color_table, Index from, Index to, bool alpha_blending)
			throw()
			:	color_number_(to - from + 1),
				alpha_blending_(alpha_blending),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.)
		{
			clear();
			for (Index i = from; i <= to; i++)
			{
				push_back(color_table[i]);
			}
		}
		
		ColorTable::ColorTable(const ColorRGBA* array, Size array_size, bool alpha_blending)
			throw()
			: color_number_(array_size),
				alpha_blending_(alpha_blending),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.)
		{
			clear();
			for (Size i = 0; i < array_size; i++)
			{
				push_back(array[i]);
			}
		}
		

		ColorTable::~ColorTable()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " 
			 << RTTI::getName<ColorTable>() << endl;
			#endif 

			clear();
		}

		void ColorTable::setNumberOfColors(const Size color_number)
			throw()
		{
			color_number_ = color_number;
		}
			
		Size ColorTable::getNumberOfColors() const
			throw()
		{
			return color_number_;
		}

		void ColorTable::setAlphaBlending(const bool blending)
			throw()
		{
			alpha_blending_ = blending;
		}

		bool ColorTable::getAlphaBlending() const
			throw()
		{
			return alpha_blending_;
		}
				
		Size ColorTable::createTable()
			throw()
		{
			// TODO: let the user choose to go beyond linear interpolation!
			
			// we will build the color table in a temporary vector which we
			// will later copy into our own dataset
			vector<ColorRGBA> new_table(color_number_);

			Index old_number_of_colors = (Index)size();

			// we won't *reduce* the number of colors, so if we should, we
			// just return
			if (color_number_ < old_number_of_colors)
			{
				return old_number_of_colors;
			}
		
			// how many colors do we have to put between two of the old ones?
			Index number_of_interpolation_steps = (Index)floor((double)(color_number_ - old_number_of_colors) / (old_number_of_colors - 1));
	
			// adjust the number of colors so that there are no remainders after the interpolation
			if (color_number_ != (old_number_of_colors + number_of_interpolation_steps*(old_number_of_colors-1)))  
			{
				color_number_ = old_number_of_colors + (number_of_interpolation_steps*(old_number_of_colors-1));
			}
			
			ColorRGBA col1, col2;
			float pos;
			
			for (Index i=0; i<old_number_of_colors-1; i++)
			{
				col1 = (*this)[i];
				col2 = (*this)[i+1];

				new_table[i*(number_of_interpolation_steps+1)] = col1;
				
				for (Index j=1; j<=number_of_interpolation_steps; j++)
				{
					pos = (float)j/(float)(number_of_interpolation_steps+1);
					new_table[(i*(number_of_interpolation_steps+1))+j].setRed(pos*(float)col2.getRed() + (1.-pos)*(float)col1.getRed());
					new_table[(i*(number_of_interpolation_steps+1))+j].setGreen(pos*(float)col2.getGreen() + (1.-pos)*(float)col1.getGreen());
					new_table[(i*(number_of_interpolation_steps+1))+j].setBlue(pos*(float)col2.getBlue() + (1.-pos)*(float)col1.getBlue());

					if (alpha_blending_)
					{
						new_table[(i*(number_of_interpolation_steps+1))+j].setAlpha(pos*(float)col2.getAlpha() + (1.-pos)*(float)col1.getAlpha());
					}
				}
			}
			
			new_table[color_number_-1] = (*this)[old_number_of_colors-1]; 

			// This can probably done much faster...
			(*this).resize(color_number_);

			copy(new_table.begin(), new_table.end(), (*this).begin());
			
			return (color_number_);
		}
		
		void ColorTable::setMinMaxColors(ColorRGBA min, ColorRGBA max)
			throw()
		{
			min_color_ = min;
			max_color_ = max;
			has_min_max_colors_ = true;
		}
		
		void ColorTable::setRange(float min, float max)
			throw()
		{
			min_ = min;
			max_ = max;
		}

		ColorRGBA& ColorTable::map(float value)
			throw()
		{
			if (value < min_)
			{
				if (has_min_max_colors_)
				{
					return min_color_;
				}
				else
				{
					return (*this)[0];
				}
			}

			if (value > max_)
			{
				if (has_min_max_colors_)
				{
					return max_color_;
				}
				else
				{ 
					return (*this)[size() - 1];
				}
			}

			Index index = (Index)((value - min_) / (max_ - min_) * (size() - 1) + 0.5);

			return (*this)[index];
		}

		const ColorRGBA& ColorTable::map(float value) const
			throw()
		{
			if (value < min_)
			{
				if (has_min_max_colors_)
				{
					return min_color_;
				}
				else
				{
					return (*this)[0];
				}
			}

			if (value > max_)
			{
				if (has_min_max_colors_)
				{
					return max_color_;
				}
				else
				{
					return (*this)[size() - 1];
				}
			}

			Index index = (Index)((value - min_) / (max_ - min_) * (size() - 1) + 0.5);

			return (*this)[index];
		}
					
		void ColorTable::dump(ostream& s, Size /* depth */) const
		{
			for (Size i = 0; i < size(); i++)
			{
				s << operator[](i) << endl;
			}
		}

	} // namespace VIEW

} // namespace BALL
