// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorMap.C,v 1.2.18.1 2007/03/25 21:56:34 oliver Exp $
//

#include <BALL/VIEW/DATATYPE/colorMap.h>
#include <BALL/MATHS/common.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		ColorMap::ColorMap()
			:	vector<ColorRGBA>(),
				color_number_(0),
				alpha_blending_(false),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.),
				interpolation_boundaries_()
		{
		}

		ColorMap::ColorMap(Size color_number)
			:	vector<ColorRGBA>(color_number),
				color_number_(color_number),
				alpha_blending_(false),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.),
				interpolation_boundaries_()
		{
		}

		ColorMap::ColorMap(const ColorMap& color_Map)
			:	vector<ColorRGBA>(color_Map),
				color_number_(color_Map.color_number_),
				alpha_blending_(color_Map.alpha_blending_),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.),
				interpolation_boundaries_(color_Map.interpolation_boundaries_)
		{
		}

		ColorMap::ColorMap(Size size, const ColorRGBA& color, bool alpha_blending)
			:	vector<ColorRGBA>(size, color),
				color_number_(size),
				alpha_blending_(alpha_blending),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.),
				interpolation_boundaries_()
		{
		}

		ColorMap::ColorMap(const ColorMap& color_Map, Index from, Index to, bool alpha_blending)
			:	color_number_(to - from + 1),
				alpha_blending_(alpha_blending),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.),
				interpolation_boundaries_()
		{
			clear();
			for (Index i = from; i <= to; i++)
			{
				push_back(color_Map[i]);
			}
		}
		
		ColorMap::ColorMap(const ColorRGBA* array, Size array_size, bool alpha_blending)
			: color_number_(array_size),
				alpha_blending_(alpha_blending),
				min_color_(),
				max_color_(),
				has_min_max_colors_(false),
				min_(0.),
				max_(1.),
				interpolation_boundaries_()
		{
			clear();
			for (Size i = 0; i < array_size; i++)
			{
				push_back(array[i]);
			}
		}
		

		ColorMap::~ColorMap()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this << " of class ColorMap" << endl;
			#endif 

			clear();
		}

		void ColorMap::setNumberOfColors(Size color_number)
		{
			color_number_ = color_number;
		}
			
		Size ColorMap::getNumberOfColors() const
		{
			return color_number_;
		}
		
		void ColorMap::setBaseColors(const ColorRGBA* color_array, Size array_size)
		{
			clear();
			for (Size i = 0; i < array_size; i++)
			{
				push_back(color_array[i]);
			}
		}	

		void ColorMap::setAlphaBlending(bool blending)
		{
			alpha_blending_ = blending;
		}

		bool ColorMap::getAlphaBlending() const
		{
			return alpha_blending_;
		}
				
		Size ColorMap::createMap()
		{
			// TODO: let the user choose to go beyond linear interpolation!
			
			if (size() == 1) // this makes no sense... so just return
				return 1;
			
			// we won't *reduce* the number of colors, so if we should, we just return
			Size old_number_of_colors = size();
			if (old_number_of_colors > color_number_) return old_number_of_colors;
	
			// let's see if we are given the vector of interpolation points to use 
			if (interpolation_boundaries_.size() != old_number_of_colors)
			{
				interpolation_boundaries_.clear();
				// nope. we just create them evenly distributed
				float step_size = 1./(old_number_of_colors - 1.);
				float step = 0;
				for (Size i =0; i<old_number_of_colors; i++)
				{
					interpolation_boundaries_.push_back(Vector4(std::min(step, (float)1.)));
					step += step_size;
				}
			}
				
			// we will build the color Map in a temporary vector which we
			// will later copy into our own dataset
			vector<ColorRGBA> new_map;
			new_map.reserve(color_number_);

			// fill the color vector. we iterate with x from 0 to 1 and compute
			// the interpolated vectors
			float step_size = 1./(color_number_ - 1);
			float x = step_size;
			
			int current_bound_x = 0;
			int current_bound_y = 0;
			int current_bound_z = 0;
			int current_bound_a = 0;

			new_map.push_back((*this)[0]);
			for (Size i=1; i < color_number_ - 1; i++)
			{
				if (x > interpolation_boundaries_[current_bound_x+1].x)
					current_bound_x++;
				if (x > interpolation_boundaries_[current_bound_y+1].y)
					current_bound_y++;
				if (x > interpolation_boundaries_[current_bound_z+1].z)
					current_bound_z++;
				if (x > interpolation_boundaries_[current_bound_a+1].h)
					current_bound_a++;


				float red_left    = (*this)[current_bound_x].getRed();
				float red_right   = (*this)[current_bound_x+1].getRed();

				float green_left  = (*this)[current_bound_y].getGreen();
				float green_right = (*this)[current_bound_y+1].getGreen();
				 
				float blue_left   = (*this)[current_bound_z].getBlue();
				float blue_right  = (*this)[current_bound_z+1].getBlue();

				float alpha_left  = (*this)[current_bound_z].getAlpha();
				float alpha_right = (*this)[current_bound_z+1].getAlpha();

				Vector4 pos1;
				Vector4 pos2;

				pos1.x = interpolation_boundaries_[current_bound_x].x;
				pos2.x = interpolation_boundaries_[current_bound_x+1].x;

				pos1.y = interpolation_boundaries_[current_bound_y].y;
				pos2.y = interpolation_boundaries_[current_bound_y+1].y;

				pos1.z = interpolation_boundaries_[current_bound_z].z;
				pos2.z = interpolation_boundaries_[current_bound_z+1].z;

				pos1.h = interpolation_boundaries_[current_bound_a].h;
				pos2.h = interpolation_boundaries_[current_bound_a+1].h;

				float color_r, color_g, color_b, color_a;

				if (pos2.x - pos1.x > 0)
				{
					color_r	= (red_right - red_left) / (pos2.x - pos1.x) * (x - pos1.x) + red_left;
				} else {
					color_r = red_left;
				}

				if (pos2.y - pos1.y > 0)
				{
					color_g = (green_right - green_left) / (pos2.y - pos1.y) * (x - pos1.y) + green_left;
				} else {
					color_g = green_left;
				}

				if (pos2.z - pos1.z > 0)
				{
					color_b = (blue_right - blue_left) / (pos2.z - pos1.z) * (x - pos1.z) + blue_left;
				} else {
					color_b = blue_left;
				}
				
				if (alpha_blending_)
				{
					if (pos2.h - pos1.h > 0)
					{
						color_a = (alpha_right - alpha_left) / (pos2.h - pos1.h) * (x - pos1.h) + alpha_left;
					} else
						color_a = alpha_left;
				}
				else
					color_a = 1.;

				color_r = std::max((float)0., std::min(color_r, (float)1.));
				color_g = std::max((float)0., std::min(color_g, (float)1.));
				color_b = std::max((float)0., std::min(color_b, (float)1.));
				color_a = std::max((float)0., std::min(color_a, (float)1.));

				new_map.push_back(ColorRGBA(color_r, color_g, color_b, color_a));
				x += step_size;
			}
			
			new_map.push_back((*this)[old_number_of_colors-1]); 

			// This can probably done much faster...
			(*this).resize(new_map.size());
			copy(new_map.begin(), new_map.end(), (*this).begin());
		
			return new_map.size();
		}
		
		void ColorMap::setMinMaxColors(ColorRGBA min, ColorRGBA max)
		{
			min_color_ = min;
			max_color_ = max;
			has_min_max_colors_ = true;
		}
		
		void ColorMap::setRange(float min, float max)
		{
			min_ = min;
			max_ = max;
		}

		ColorRGBA& ColorMap::map(float value)
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
					
		bool ColorMap::setInterpolationBoundaries(const vector<Vector4>& boundaries)
		{
			if (boundaries.size() == size())
			{
				interpolation_boundaries_ = boundaries;

				return true;
			}
			
			return false;	
		}

		void ColorMap::createMapJet(const Size color_number)
		{
			clear();
			setNumberOfColors(color_number);

			ColorRGBA base_colors[6] = { ColorRGBA(0., 0., 0.5, 1.),
																	 ColorRGBA(0., 0., 1., 1.),
																	 ColorRGBA(1., 1., 1., 1.),
																	 ColorRGBA(1., 1., 0., 1.),
																	 ColorRGBA(0.5, 0., 0., 1.),
																	 ColorRGBA(0.5, 0., 0., 1.) };

			std::vector<Vector4> interpolation;
			interpolation.push_back(Vector4(0., 0., 0., 0.));
			interpolation.push_back(Vector4(0.35, 0.125, 0.11, 1./5.));
			interpolation.push_back(Vector4(0.66, 0.375, 0.34, 2./5.));
			interpolation.push_back(Vector4(0.89, 0.64, 0.65, 3./5.));
			interpolation.push_back(Vector4(1., 0.91, 1., 4./5.));
			interpolation.push_back(Vector4(1., 1., 1., 1.));

			setBaseColors(base_colors, 6);
			setInterpolationBoundaries(interpolation);

			createMap();
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
