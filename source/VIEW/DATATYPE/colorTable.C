// $Id: colorTable.C,v 1.4.4.4 2002/10/18 14:47:37 amoll Exp $

#include <BALL/VIEW/DATATYPE/colorTable.h>

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
				min_(0.),
				max_(1.),
				has_min_max_colors_(false)
		{
		}

		ColorTable::ColorTable(Size color_number)
			throw()
			:	vector<ColorRGBA>(color_number),
				color_number_(color_number),
				alpha_blending_(false),
				min_(0.),
				max_(1.),
				has_min_max_colors_(false)
		{
		}

		ColorTable::ColorTable(const ColorTable& color_table, bool /* deep */)
			throw()
			:	vector<ColorRGBA>(color_table),
				color_number_(color_table.color_number_),
				alpha_blending_(color_table.alpha_blending_),
				min_(0.),
				max_(1.),
				has_min_max_colors_(false)
		{
		}

		ColorTable::ColorTable(Size size, const ColorRGBA& color, bool alpha_blending)
			throw()
			:	vector<ColorRGBA>(size, color),
				color_number_(size),
				alpha_blending_(alpha_blending),
				min_(0.),
				max_(1.),
				has_min_max_colors_(false)
		{
		}

		ColorTable::ColorTable(const ColorTable& color_table, Index from, Index to, bool alpha_blending)
			throw()
			: color_number_(to-from+1),
				alpha_blending_(alpha_blending),
				min_(0.),
				max_(1.),
				has_min_max_colors_(false)
		{
			clear();
			for (Index i = from; i <= to; i++)
			{
				push_back(color_table[i]);
			}
		}
		
		ColorTable::ColorTable(const ColorRGBA *array, Size array_size, bool alpha_blending)
			throw()
			: color_number_(array_size),
				alpha_blending_(alpha_blending),
				min_(0.),
				max_(1.),
				has_min_max_colors_(false)
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
			
		const Size ColorTable::getNumberOfColors() const
			throw()
		{
			return color_number_;
		}

		void ColorTable::setAlphaBlending(const bool blending)
			throw()
		{
			alpha_blending_ = blending;
		}

		const	bool ColorTable::getAlphaBlending() const
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
			Size old_colNum = (Size) size();

			// we won't *reduce* the number of colors, so if we should, we
			// just return
			if (color_number_ < old_colNum)
			{
				return old_colNum;
			}
		
			// how many colors do we have to put between two of the old ones?
			Size numInterpolSteps = (Size)floor((double)(color_number_ - old_colNum) / (old_colNum - 1));
	
			// adjust the number of colors so that there are no remainders after the interpolation
			if (color_number_ != (old_colNum + numInterpolSteps*(old_colNum-1)))  
			{
				color_number_ = old_colNum + (numInterpolSteps*(old_colNum-1));
			}
			
			ColorRGBA col1, col2;
			float pos;
			
			for (Index i=0; i<old_colNum-1; i++)
			{
				col1 = (*this)[i];
				col2 = (*this)[i+1];

				new_table[i*(numInterpolSteps+1)] = col1;
				
				for (Index j=1; j<=numInterpolSteps; j++)
				{
					pos = (float)j/(float)(numInterpolSteps+1);
					new_table[(i*(numInterpolSteps+1))+j].setRed(pos*(float)col2.getRed() + (1.-pos)*(float)col1.getRed());
					new_table[(i*(numInterpolSteps+1))+j].setGreen(pos*(float)col2.getGreen() + (1.-pos)*(float)col1.getGreen());
					new_table[(i*(numInterpolSteps+1))+j].setBlue(pos*(float)col2.getBlue() + (1.-pos)*(float)col1.getBlue());

					if (alpha_blending_)
					{
						new_table[(i*(numInterpolSteps+1))+j].setAlpha(pos*(float)col2.getAlpha() + (1.-pos)*(float)col1.getAlpha());
					}
				}
			}
			
			new_table[color_number_-1] = (*this)[old_colNum-1]; 

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

			Index index = (Index) rint((value - min_)/(max_-min_)*(size()-1));

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

			Index index = (Index) rint((value - min_)/(max_-min_)*(size()-1));

			return (*this)[index];
		}
					
		void ColorTable::dump(ostream& s, Size /* depth */) const
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

	} // namespace VIEW

} // namespace BALL
