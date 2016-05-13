#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/MATHS/plane3.h>
#include <BALL/MATHS/analyticalGeometry.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

		LightSource::LightSource()
			:	position_(),
				direction_(0, 0, -1),
				attenuation_(1., 0., 0.),
				angle_(10),
				intensity_(0.8),
				color_(255, 255, 255, 255),
				type_(1),
				relative_(true)	
		{
		}

		LightSource::LightSource(const LightSource& light_source)
			: position_(light_source.position_),
				direction_(light_source.direction_),
				attenuation_(light_source.attenuation_),
				angle_(light_source.angle_),
				intensity_(light_source.intensity_),
				color_(light_source.color_),
				type_(light_source.type_),
				relative_(light_source.relative_)
		{
		}

		LightSource& LightSource::operator = (const LightSource& light) 
		{
				position_ = light.position_;
			 direction_ = light.direction_;
		 attenuation_ = light.attenuation_; 
					 angle_ = light.angle_,
			 intensity_ = light.intensity_;
					 color_ = light.color_,
						type_ = light.type_;
				relative_ = light.relative_;

			return *this;
		}


		bool LightSource::operator == (const LightSource& light_source) const
		{
			return position_ 		== light_source.position_ 	 &&
						 direction_ 	== light_source.direction_ 	 &&
						 attenuation_ == light_source.attenuation_ &&
						 angle_ 			== light_source.angle_ 		 	 &&
						 intensity_ 	== light_source.intensity_ 	 &&
						 color_ 			== light_source.color_ 			 &&
						 type_ 				== light_source.type_ 			 &&
						 relative_ 		== light_source.relative_;
		}
		
		bool LightSource::operator < (const LightSource& light) const
		{
			return this < &light;
		}


		void LightSource::dump(std::ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Position : " << position_ << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Direction : " << direction_<< std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Attenuation : " << attenuation_<< std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Angle : " << angle_ << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Intensity : " << intensity_ << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Color : " << color_ << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Type : " << type_ << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Relative : " << relative_ << std::endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		Stage::Material::Material()
			: PersistentObject(),
				ambient_color(ColorRGBA(255, 255, 255, 255)),
				ambient_intensity(0.),
				specular_color(ColorRGBA(255, 255, 255, 255)),
				specular_intensity(1.),
				reflective_color(ColorRGBA(255, 255, 255, 255)),
				reflective_intensity(0.),
				shininess(60.),
				transparency(0.)
		{
		}

		void Stage::Material::persistentWrite(PersistenceManager& pm, const char* name) const
			throw(Exception::GeneralException)
		{
			pm.writeObjectHeader(this, name);
				// ambient
				pm.writePrimitive((String)ambient_color, "ambient_color_");
				pm.writePrimitive(ambient_intensity,     "ambient_intensity_");
				// specular
				pm.writePrimitive((String)specular_color, "specular_color_");
				pm.writePrimitive(specular_intensity,     "specular_intensity_");
				// reflective
				pm.writePrimitive((String)reflective_color, "reflective_color_");
				pm.writePrimitive(reflective_intensity,     "reflective_intensity_");
				
				pm.writePrimitive(shininess,    "shininess_");
				pm.writePrimitive(transparency, "transparency_");
			pm.writeObjectTrailer(name);
		}
		
		void Stage::Material::persistentRead(PersistenceManager& pm)
			throw(Exception::GeneralException)
		{
			String color;

			// ambient
			pm.readPrimitive(color, "ambient_color_");
			ambient_color.set(color);

			pm.readPrimitive(ambient_intensity,     "ambient_intensity_");
			
			// specular
			pm.readPrimitive(color, "specular_color_");
			specular_color.set(color);

			pm.readPrimitive(specular_intensity,     "specular_intensity_");

			// reflective
			pm.readPrimitive(color, "reflective_color_");
			reflective_color.set(color);

			pm.readPrimitive(reflective_intensity,     "reflective_intensity_");
			
			pm.readPrimitive(shininess,    "shininess_");
			pm.readPrimitive(transparency, "transparency_");
		}		
		
		Stage::Stage()
			: background_color_(ColorRGBA(1.,1.,1.,1.)),
				info_color_(0, 0, 255, 255),
				light_sources_(),
				camera_(),
				show_coordinate_system_(false),
				fog_intensity_(0),
				eye_distance_(2.0),
				focal_distance_(40),
				swap_side_by_side_stereo_(false)
		{}

		Stage::Stage(const Stage& stage)
			: background_color_(stage.background_color_),
				light_sources_(stage.light_sources_),
				camera_(stage.camera_),
				show_coordinate_system_(false),
				fog_intensity_(stage.fog_intensity_),
				eye_distance_(stage.eye_distance_),
				focal_distance_(stage.focal_distance_),
				swap_side_by_side_stereo_(stage.swap_side_by_side_stereo_),
				material_(stage.material_)
		{
		}

		void Stage::clear()
		{
			background_color_.clear();
			light_sources_.clear();
			camera_ = Camera();
			show_coordinate_system_ = false;
			eye_distance_ = 2.0;
			focal_distance_ = 40;
			swap_side_by_side_stereo_ = false;
			fog_intensity_ = 0;
			material_ = Material();
		}

		bool Stage::operator == (const Stage& stage) const
		{
			return light_sources_ 					== stage.light_sources_ 		&&
						 camera_ 					 				== stage.camera_ 						&&
						 background_color_ 				== stage.background_color_ 	&&
						 show_coordinate_system_ 	== stage.show_coordinate_system_ &&
						 eye_distance_ 						== stage.eye_distance_ 			&&
						 focal_distance_ 					== stage.focal_distance_ 		&&
						 swap_side_by_side_stereo_== stage.swap_side_by_side_stereo_;
		}

		void Stage::dump(std::ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Light sources: -------------------------------" << endl;

			list<LightSource>::const_iterator it = light_sources_.begin();
			for (; it != light_sources_.end(); it++)
			{
				it->dump(s, depth + 1);
			}

			BALL_DUMP_DEPTH(s, depth);
			s << "Camera: ---------------------------------------" << endl;
			camera_.dump(s, depth);

			BALL_DUMP_DEPTH(s, depth);
			s << "Background color:  " << background_color_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Show coordinate system:  " << show_coordinate_system_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Fog intensity:  " << fog_intensity_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Eye distance :  " << eye_distance_<< endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Focal width:  " << focal_distance_<< endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Swap side by side:  " << swap_side_by_side_stereo_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		Vector3 Stage::calculateRelativeCoordinates(Vector3 pos) const
		{
			// relative in units of right_vector , look_up_vector , view_vector
			// by calculating the normals to three planes
			
			// make sure the three planes are far enough, to be always on one side of them
			const float d = 1000000.0;

			try
			{
				Vector3 dr(camera_.getRightVector() * d);

				Vector3 dv(camera_.getViewVector());
				dv.normalize();
				dv *= d;

				Vector3 du(camera_.getLookUpVector() * d);

				// calculate the planes
				const Plane3 plane_rv(dr, dr);
				const Plane3 plane_uv(du, du);
				const Plane3 plane_vv(dv, dv);

				// distance of the destion of the light source from the three planes
				Vector3 result(
					GetDistance(plane_rv, pos),
					GetDistance(plane_uv, pos),
					GetDistance(plane_vv, pos));
				result -= Vector3(d);

				return -result;
			}
			catch(...)
			{
				Log.error() << "Could not calculate relative light coordinates, degenerated view vectors." << std::endl;
			}

			return Vector3(1.0);
		}

		Vector3 Stage::calculateAbsoluteCoordinates(Vector3 pos) const
		{
			try
			{
				Vector3 dv(camera_.getViewVector());
				dv.normalize();

				return (pos.x * camera_.getRightVector() + 
							  pos.y * camera_.getLookUpVector() +
								pos.z * dv);
			}
			catch(...)
			{
				Log.error() << "Could not calculate absolute light coordinates, degenerated view vectors." << std::endl;
			}

			return Vector3(1.0);
		}

		void Stage::addLightSource(const LightSource& light_source)
		{
			light_sources_.push_back(light_source);
		}

		void Stage::removeLightSource(const LightSource& light_source)
		{
			list<LightSource>::iterator it = light_sources_.begin();
			for (; it != light_sources_.end(); it++)
			{
				if (&*it == &light_source) 
				{
					light_sources_.erase(it);
					return;
				}
			}
		}

		void Stage::clearLightSources()
		{
			light_sources_.clear();
		}

	} // namespace VIEW
} // namespace BALL
