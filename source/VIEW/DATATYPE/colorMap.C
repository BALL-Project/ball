// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorMap.C,v 1.1.2.1 2005/11/16 13:29:27 amoll Exp $
//

#include <BALL/VIEW/DATATYPE/colorMap.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		ColorMap::ColorMap()
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

		ColorMap::ColorMap(Size color_number)
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

		ColorMap::ColorMap(const ColorMap& color_Map)
			throw()
			:	vector<ColorRGBA>(color_Map),
				color_number_(color_Map.color_number_),
				alpha_blending_(color_Map.alpha_blending_),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.)
		{
		}

		ColorMap::ColorMap(Size size, const ColorRGBA& color, bool alpha_blending)
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

		ColorMap::ColorMap(const ColorMap& color_Map, Index from, Index to, bool alpha_blending)
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
				push_back(color_Map[i]);
			}
		}
		
		ColorMap::ColorMap(const ColorRGBA* array, Size array_size, bool alpha_blending)
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
		

		ColorMap::~ColorMap()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this << " of class ColorMap" << endl;
			#endif 

			clear();
		}

		void ColorMap::setNumberOfColors(Size color_number)
			throw()
		{
			color_number_ = color_number;
		}
			
		Size ColorMap::getNumberOfColors() const
			throw()
		{
			return color_number_;
		}
		
		void ColorMap::setBaseColors(const ColorRGBA* color_array, Size array_size)
			throw()
		{
			clear();
			for (Size i = 0; i < array_size; i++)
			{
				push_back(color_array[i]);
			}
		}	

		void ColorMap::setAlphaBlending(bool blending)
			throw()
		{
			alpha_blending_ = blending;
		}

		bool ColorMap::getAlphaBlending() const
			throw()
		{
			return alpha_blending_;
		}
				
		Size ColorMap::createMap()
			throw()
		{
			// TODO: let the user choose to go beyond linear interpolation!
				
			// we won't *reduce* the number of colors, so if we should, we just return
			Size old_number_of_colors = size();
			if (color_number_ < old_number_of_colors)
			{
				return old_number_of_colors;
			}
			
			// we will build the color Map in a temporary vector which we
			// will later copy into our own dataset
			vector<ColorRGBA> new_Map(color_number_);

			// how many colors do we have to put between two of the old ones?
			Index number_of_interpolation_steps = (Index)floor(
					(double)(color_number_ - old_number_of_colors) / (old_number_of_colors - 1));
	
			// adjust the number of colors so that there are no remainders after the interpolation
			color_number_ = old_number_of_colors + (number_of_interpolation_steps*(old_number_of_colors-1));
			
			ColorRGBA col1, col2;
			double pos; // had to use double here, instead of float...
			
			for (Size i=0; i< old_number_of_colors-1; i++)
			{
				col1 = (*this)[i];
				col2 = (*this)[i+1];

				new_Map[i*(number_of_interpolation_steps+1)] = col1;
				
				for (Index j=1; j<=number_of_interpolation_steps; j++)
				{
					pos = (float)j/(float)(number_of_interpolation_steps+1);
					new_Map[(i*(number_of_interpolation_steps+1))+j].setRed(
							pos*(float)col2.getRed() + (1.-pos)*(float)col1.getRed());
					new_Map[(i*(number_of_interpolation_steps+1))+j].setGreen(
							pos*(float)col2.getGreen() + (1.-pos)*(float)col1.getGreen());
					new_Map[(i*(number_of_interpolation_steps+1))+j].setBlue(
							pos*(float)col2.getBlue() + (1.-pos)*(float)col1.getBlue());

					if (alpha_blending_)
					{
						new_Map[(i*(number_of_interpolation_steps+1))+j].setAlpha(
								pos*(float)col2.getAlpha() + (1.-pos)*(float)col1.getAlpha());
					}
				}
			}
			
			new_Map[color_number_-1] = (*this)[old_number_of_colors-1]; 

			// This can probably done much faster...
			(*this).resize(new_Map.size());

			copy(new_Map.begin(), new_Map.end(), (*this).begin());
		
			// fix an off-by-one error:
			// the last color doesnt get set when the number of colors is even
			if (color_number_ != size())
			{
				(*this)[size()-1] = (*this)[size()-2];
			}
			return color_number_;
		}
		
		void ColorMap::setMinMaxColors(ColorRGBA min, ColorRGBA max)
			throw()
		{
			min_color_ = min;
			max_color_ = max;
			has_min_max_colors_ = true;
		}
		
		void ColorMap::setRange(float min, float max)
			throw()
		{
			min_ = min;
			max_ = max;
		}

		ColorRGBA& ColorMap::map(float value)
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

		const ColorRGBA& ColorMap::map(float value) const
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
					
		void ColorMap::dump(ostream& s, Size /* depth */) const
		{
			for (Size i = 0; i < size(); i++)
			{
				s << operator[](i) << endl;
			}
		}

	} // namespace VIEW
} // namespace BALL
