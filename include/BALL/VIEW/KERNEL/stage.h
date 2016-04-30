// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_STAGE_H
#define BALL_VIEW_KERNEL_STAGE_H

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_QUATERNION_H
#	include <BALL/MATHS/quaternion.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
# include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef  BALL_VIEW_KERNEL_REPRESENTATION_H
# include <BALL/VIEW/KERNEL/representation.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_VIEW_RENDERING_CAMERA_H
# include <BALL/VIEW/RENDERING/camera.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Light source is mainly used for Renderer classes (e.g. OpenGL and POVRay).
		 		Currently we support ambient, positional and directional light sources.
		 		The Position and direction of lights can be stored twofold:
				- Either with absolute room coordinates and a direction vector or \\
				- Relative to a Camera.
				In this case, the position and direction vector are stored as multiples of
				look right, look up and view vector.
				\ingroup  ViewKernelStage
		 */
		class BALL_VIEW_EXPORT LightSource
		{
			public:

			/// Enumeration of different types of lights
			enum Types
			{

			 	/**	Ambient light doesn't come from any particular direction. 
						All the objects in the scene will be lit up by the ambient light.
				*/
				AMBIENT = 0,

				/** Diffuse light is created the light source and is reflected off the surface 
						of any object in the scene. Any surface of an object that the light hits 
						directly will be very bright, and areas the light barely gets to will be darker.
				*/
				POSITIONAL,

				/**
				*/
				DIRECTIONAL
			};

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Constructor
			 */
			LightSource();

			/** Copy Constructor
			 */
			LightSource(const LightSource& light_source);

			/** Destructor
			 */
			virtual ~LightSource(){}

			//@}
			/**	@name	Accessors
			*/
			//@{

			/// Get position
			const Vector3& getPosition() const
				{ return position_;}

			/// Set position
			void setPosition(const Vector3& position)
				{ position_ = position; }

			/// Get the direction vector of the light
			const Vector3& getDirection() const
				{ return direction_;}

			/// Set the direction vector of the light
			void setDirection(const Vector3& direction)
				{ direction_ = direction;}
	
			/// Get the attenuation parameters of the light
			const Vector3& getAttenuation() const
				{ return attenuation_;}

			/// Set the attenuation parameters of the light
			void setAttenuation(const Vector3& attenuation)
				{ attenuation_ = attenuation;}


			/// Get the angle of the light cone
			const Angle& getAngle() const
				{ return angle_;}

			/// Set the angle of the light cone
			void setAngle(const Angle& angle)
				{ angle_ = angle;}
			
			/** Get the light intensity.
					0 is the minumum, 1 is the maximum.
			*/
			float getIntensity() const
				{ return intensity_;}

			/** Set the intensity.
					0 is the minumum, 1 is the maximum.
			*/
			void setIntensity(float intensity)
				{ intensity_ = intensity;}
				
			/** Get the color of the light.
			 		The alpha channel of the color is ignored.
			*/
			const ColorRGBA& getColor() const
				{ return color_;}

			/** Set the color of the light.
			 		The alpha channel of the color is ignored.
			*/
			void setColor(const ColorRGBA& color)
				{ color_ = color;}
			
			/** Get the type of the light.
			 		\see Types
			*/
			Index getType() const
				{ return type_;}

			/** Set the type of the light.
			 		\see Types
			*/
			void setType(Types type)
				{ type_ = type;}

			/// If set to true, the LightSource will move with the Camera
			void setRelativeToCamera(bool state)
				{ relative_ = state;}

			/// Test if a LightSource will move with the Camera
			bool isRelativeToCamera() const
				{ return relative_;}

			///
			LightSource& operator = (const LightSource& light);

			/// needed for MSVC, dont use it otherwise!
			bool operator < (const LightSource& light) const;

			//@}
			/**	@name Predicates
			*/
			//@{
			
			///
			bool operator == (const LightSource& light_source) const;

			//@}
			
			/** Internal value dump.
					Dump the current state of this instance to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			protected:

			//_ Position of the light source
			Vector3 		position_;

			//_ Direction of the light cone
			Vector3 		direction_;
			
			//_ Attenuation parameters of the light 
			Vector3 		attenuation_;

			//_
			Vector3     r_position_;

			//_
			Vector3  		r_direction_;

			//_ Angle of the light cone
			Angle 			angle_;

			//_ Intensity of the light
			float 			intensity_;

			//_ Color of the light
			ColorRGBA 	color_;

			//_ Enumeration of types for further usage
			Index 			type_;

			//_ Relative to camera
			bool 				relative_;
		};
				

		/** A Stage has a Camera, LightSources and a background color.
		 		It stores also the eye distance for the stereo view.
		 		Finally a flag can be set, so that a coordinate system will be shown.
				\ingroup ViewKernelStage
		*/
		class BALL_VIEW_EXPORT Stage
		{
			public:

			/** This class holds all material parameters passed on to a raytracer.
			 */
			class BALL_VIEW_EXPORT Material
				: public PersistentObject
			{
				public:

					BALL_CREATE(Material)

					Material();

					/** @name Persistence
					 */
					//@{

					/** Write a raytracing material to a persistent stream.
					 		@param pm the persistence manager
					 */
					virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const
						throw(Exception::GeneralException);

					/** Read a raytracing material from a persistent stream.
					 		@param pm the persistence manager
					*/
					virtual void persistentRead(PersistenceManager& pm)
						throw(Exception::GeneralException);

					//@}

					ColorRGBA ambient_color;
					float     ambient_intensity;

					ColorRGBA specular_color;
					float			specular_intensity;

					ColorRGBA reflective_color;
					float			reflective_intensity;

					float     shininess;	
					float     transparency;
			};

			// needed for backwards compatibilty of project files
			class BALL_VIEW_EXPORT RaytracingMaterial : public Material
			{
				public:

					BALL_CREATE(RaytracingMaterial)
			};

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor
			*/
			Stage();
			
			///	Copy constructor
			Stage(const Stage& stage);

			/// Destructor
			virtual ~Stage(){}

			/// Explicit default initialization.
			virtual void clear();

			//@}
			/**	@name	Accessors
			*/
			//@{
			
			/// Get the light sources (const)
			virtual const std::list<LightSource>& getLightSources() const
				{ return light_sources_;}

			/// Get a reference to the i-th light source
			virtual LightSource& getLightSource(Position i)
			{
				if (i >= light_sources_.size())
				{
					Exception::OutOfRange e(__FILE__, __LINE__);
					throw(e);
				}

				std::list<LightSource>::iterator l_it;

				Position current_light = 0;
				for (l_it = light_sources_.begin(); 
						(current_light < i) && (l_it != light_sources_.end()); 
						++l_it, ++current_light)
				{ }

				return *l_it;
			}
			/// Add a light source
			virtual void addLightSource(const LightSource& light_source);

			/// Remove a light source
			virtual void removeLightSource(const LightSource& light_source) ;

			///
			void clearLightSources();
			
			/// Get the camera
			virtual Camera& getCamera() 
				{ return camera_;}

			/// Get the camera (const)
			virtual const Camera& getCamera() const
				{ return camera_;}

			/** Set the camera of the stage
			 */
			virtual void setCamera(const Camera& camera)
				{ camera_ = camera;}

			/// Get the background color
			virtual const ColorRGBA& getBackgroundColor() const
				{ return background_color_;}

			/// Set the background color
			virtual void setBackgroundColor(const ColorRGBA& color)
				{ background_color_ = color;}
			
			/// Get the background color
			virtual const ColorRGBA& getInfoColor() const
				{ return info_color_;}

			/// Set the background color
			virtual void setInfoColor(const ColorRGBA& color)
				{ info_color_ = color;}

			/// Show coordinate system
			void showCoordinateSystem(bool state)
				{ show_coordinate_system_ = state;}

			/// Shows coordinate system
			bool coordinateSystemEnabled() const
				{ return show_coordinate_system_;}

			/// Set the eye distance for the stereo view
			void setEyeDistance(float value) 
				{ eye_distance_ = value;}

			/// Get the eye distance for the stereo view
			float getEyeDistance() const
				{ return eye_distance_;}
				
			/// Set the focal distance for the stereo view
			void setFocalDistance(float value) 
				{ focal_distance_ = value;}

			/// Get the focal distance for the stereo view
			float getFocalDistance() const
				{ return focal_distance_;}

			/// Settings for side by side stereo side swapping
			void setSwapSideBySideStereo(bool state)
				{ swap_side_by_side_stereo_ = state;}

			/// Get settings for side by side stereo side swapping
			bool swapSideBySideStereo() const
				{ return swap_side_by_side_stereo_;}

			///
			float getFogIntensity() const
				{ return fog_intensity_;}

			///
			void setFogIntensity(float value)
				{ fog_intensity_ = value;}

			//@}
			/**	@name Predicates
			*/
			//@{
			
			///
			bool operator == (const Stage& stage) const;

			/** Calculate coordiantes relative to the position of the camera in units of 
			 		right_vector, look_up_vector and view_vector.
					This is done by calculating the normals to three planes, spaned by these three vectors.
					This method is e.g. used to store the coordinates of the relative light sources in the INIFile,
					or in the LightSettings dialog.
					@return Vector3(times right_vector, times look_up_vector, times view_vector)
			*/
			Vector3 calculateRelativeCoordinates(Vector3 pos) const;

			/** Calculate absolute room coordinates from relative coordinates.
			 		@see calculateRelativeCoordinates
			*/
			Vector3 calculateAbsoluteCoordinates(Vector3 pos) const;

			//@}
			
			/** Internal value dump.
					Dump the current state of this instance to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			/// Gives access to the default material parameters
			Material& getMaterial() { return material_; }

			/// Gives access to the default material parameters, const version
			const Material& getMaterial() const { return material_; }

			protected:

			//_
			ColorRGBA 					background_color_;

			//_
			ColorRGBA 					info_color_;

			//_
			std::list<LightSource> light_sources_;

			//_
			Camera 						 	camera_;

			//_
			bool 								show_coordinate_system_;

			//_
			float 							fog_intensity_;

			//_
			float 							eye_distance_;
			
			//_
			float 							focal_distance_;

			//_
			bool 							swap_side_by_side_stereo_;

			// the current default materials used for raytracing
			Material  material_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_STAGE_H
