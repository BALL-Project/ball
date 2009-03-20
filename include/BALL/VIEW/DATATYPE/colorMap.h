// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorMap.h,v 1.2.18.1 2007/03/25 21:25:40 oliver Exp $
//

#ifndef BALL_VIEW_DATATYPE_COLORMAP_H
#define BALL_VIEW_DATATYPE_COLORMAP_H

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
#	include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_MATHS_VECTOR4_H
# include <BALL/MATHS/vector4.h>
#endif

#include <vector>

namespace BALL 
{
	namespace VIEW 
	{
		/** This class is used to implement a color Map that can be used to
		 *  map a floating point value into a color.
		\ingroup ViewDatatypeFormat
		*/
		class BALL_VIEW_EXPORT ColorMap
			: public vector<ColorRGBA>
		{
			public:

			BALL_CREATE(ColorMap)

			/**	@name	Constructors and Destructors 
			*/	
			//@{

			/**	Default constructor.
					Create an empty instance of ColorMap.
			*/
			ColorMap();

			/** Alternative constructor.
			 *  Create an instance of ColorMap.
			 *  \param color_number The number of colors the Map will contain.
			 */
			ColorMap(Size color_number);
			
			/**	Copy constructor.
					Create a copy of a ColorMap object.
					\param	color_Map the color Map to be copied
			*/
			ColorMap(const ColorMap& color_Map);

			/** Detailed constructor.
			 * 	\param size the number of elements in the color Map
			 * 	\param color an array of colors, used to initialize the Map
			 * 	\param alpha_blending decides whether the alpha channel should be interpolated between colors
			*/
			ColorMap(Size size, const ColorRGBA& color, bool alpha_blending=false);

			/**
			*/
			ColorMap(const ColorMap& color_Map, Index from, Index to, bool alpha_blending=false);

			/**
			*/
			ColorMap(const ColorRGBA* color_array, Size array_size, bool alpha_blending=false);

			/**	Destructor.
			*/
			virtual ~ColorMap();
			//@}
			
			/** @name Accessors
			 */
			//@{
			
			/// Create a colormap similar to the "jet" map of matlab
			void createMapJet(const Size color_number);

			/// Set the base colors for the interpolation
			void setBaseColors(const ColorRGBA* color_array, Size array_size);

			/// Set the number of colors this Map should contain after interpolation.
			void setNumberOfColors(const Size color_number);

			/// Return the number of elements in our color Map.
			Size getNumberOfColors() const;

			/// Decides if we should interpolate the alpha channel as well
			void setAlphaBlending(bool blending);

			/// Returns true if the alpha channel is interpolated between colors, false otherwise.
			bool getAlphaBlending() const;

			/** Build the Map, i.e. interpolate between the colors to obtain the desired number of colors. 
			 *  Returns the actual size of the Map after interpolation.
			 */	
			Size createMap();
			
			/** Sets the colors that are used for values below min and above max.
			 */
			void setMinMaxColors(ColorRGBA min, ColorRGBA max);

			/** Sets the range used for the mapping.
			 */
			void setRange(float min, float max);
		
			/** Maps value into the color Map.
			 */
			ColorRGBA& map(float value);

			/** Maps value into the color Map. (const method)
			 */
			const ColorRGBA& map(float value) const;
			
			/** Set the interpolation boundary points. This allows different slopes in
			 *  the interpolation of the R, G, B, and A components. The color vector must
			 *  be initialized prior to this function. If the number of colors does not
			 *  match the number of interpolation points we return false and do nothing.
			 */
			bool setInterpolationBoundaries(const vector<Vector4>& boundaries);

			//@}
			/**	@name	Debugging and Diagnostics
			*/	
			//@{

			/**	Dump the content to an ostream
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			protected:

			Size 			color_number_;
			bool			alpha_blending_;
			ColorRGBA min_color_;
			ColorRGBA	max_color_;
			bool			has_min_max_colors_;
			float			min_;
			float			max_;

			vector<Vector4> interpolation_boundaries_; 
		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DATATYPE_COLORMAP_H
