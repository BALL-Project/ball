#include <BALL/VIEW/KERNEL/stage.h>

namespace BALL
{
	namespace VIEW
	{
		using std::endl;

LightSource::LightSource()
	throw()
	:	position_(),
		direction_(0, 0, -1),
		angle_(180),
		intensity_(1),
		color_(255, 255, 255, 255),
		type_(0),
		relative_(true)	
{
}

LightSource::LightSource(const LightSource& light_source)
	throw()
	: position_(light_source.position_),
		direction_(light_source.direction_),
		angle_(light_source.angle_),
		intensity_(light_source.intensity_),
		color_(light_source.color_),
		type_(light_source.type_),
		relative_(light_source.relative_)
{
}


bool LightSource::operator == (const LightSource& light_source) const
	throw()
{
	return position_ 		== light_source.position_ 	&&
				 direction_ 	== light_source.direction_ 	&&
				 angle_ 			== light_source.angle_ 		 	&&
				 intensity_ 	== light_source.intensity_ 	&&
				 color_ 			== light_source.color_ 			&&
				 type_ 				== light_source.type_ 			&&
				 relative_ 		== light_source.relative_;
}

void LightSource::dump(std::ostream& s, Size depth) const
	throw()
{
	BALL_DUMP_STREAM_PREFIX(s);
	
	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	BALL_DUMP_DEPTH(s, depth);
	s << "Position : " << position_ << endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "Direction : " << direction_<< endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "Angle : " << angle_ << endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "Intensity : " << intensity_ << endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "Color : " << color_ << endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "Type : " << type_ << endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "Relative : " << relative_ << endl;

	BALL_DUMP_STREAM_SUFFIX(s);
}

void LightSource::rotate(const Quaternion& q)
	throw()
{
	Matrix4x4  m;
	q.getRotationMatrix(m);

	position_  = m*position_;
	direction_ = m*direction_;
}

Camera::Camera()
	throw()
	: view_point_(0, 0, 0),
	 	look_at_(0, 0, -1),
		look_up_vector_(0, -1, 0)
{
	calculateVectors_();
}

Camera::Camera(const Camera& camera)
	throw()
	: view_point_(camera.view_point_),
		look_at_(camera.look_at_),
		look_up_vector_(camera.look_up_vector_),
		view_vector_(camera.view_vector_),
		right_vector_(camera.right_vector_)
{
}

Camera::Camera(const Vector3& view_point, const Vector3& look_at, const Vector3& look_up_vector)
	throw()
	: view_point_(view_point),
		look_at_(look_at),
		look_up_vector_(look_up_vector)
{
	calculateVectors_();
}

bool Camera::operator == (const Camera& camera) const
	throw()
{
	return 	view_point_ 		== camera.view_point_ &&
				 	look_at_ 				== camera.look_at_ 		&&
					look_up_vector_ == camera.look_up_vector_;
}

void Camera::calculateVectors_()
	throw()
{
	view_vector_  = look_at_ - view_point_;
	right_vector_ = look_up_vector_ % view_vector_;
	if (!right_vector_.isZero())	right_vector_.normalize();
	else right_vector_.set(1,0,0);
}


void Camera::dump(std::ostream& s, Size depth) const
	throw()
{
	BALL_DUMP_STREAM_PREFIX(s);
	
	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	BALL_DUMP_DEPTH(s, depth);
	s << "Viewpoint : " << view_point_ << endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "Look at : " << look_at_ << endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "Look up vector: " << look_up_vector_ << endl;

	BALL_DUMP_STREAM_SUFFIX(s);
}

void Camera::rotate(const Quaternion& q)
	throw()
{
	Matrix4x4  m;
	q.getRotationMatrix(m);

	view_point_     = m*view_point_;
	look_at_        = m*look_at_;
	look_up_vector_ = m*look_up_vector_;

	calculateVectors_();
}

Stage::Stage()
	throw()
	: background_color_(),
		light_sources_(),
		clipping_planes_(),
		camera_(),
		show_coordinate_system_(false),
		eye_distance_(2.0),
		focal_distance_(40),
		swap_side_by_side_stereo_(false)
{}

ClippingPlane::ClippingPlane()
	throw()
	: Representation(),
		translation(),
		plane_vector(),
		hidden(false)
{
}

ClippingPlane::ClippingPlane(const ClippingPlane& plane)
	throw()
	: Representation(plane),
		translation(plane.translation),
		plane_vector(plane.plane_vector),
		hidden(plane.hidden)
{
}


bool ClippingPlane::operator == (const ClippingPlane& plane) const
	throw()
{
	return plane_vector == plane.plane_vector &&
				 translation  == plane.translation;
}

ClippingPlane& ClippingPlane::operator = (const ClippingPlane& plane)
	throw()
{
	Representation::operator = (plane);
	plane_vector = plane.plane_vector;
	translation = plane.translation;
	hidden = plane.hidden;
	return *this;
}

Stage::Stage(const Stage& stage)
	throw()
	: background_color_(stage.background_color_),
		light_sources_(stage.light_sources_),
		clipping_planes_(stage.clipping_planes_),
		camera_(stage.camera_),
		show_coordinate_system_(false),
		eye_distance_(stage.eye_distance_),
		focal_distance_(stage.focal_distance_),
		swap_side_by_side_stereo_(stage.swap_side_by_side_stereo_)
{
}

void Stage::clear()
	throw()
{
	background_color_.clear();
	light_sources_.clear();
	clipping_planes_.clear();
	camera_ = Camera();
	show_coordinate_system_ = false;
	eye_distance_ = 2.0;
	focal_distance_ = 40;
	swap_side_by_side_stereo_ = false;
}

void Stage::removeLightSource(const LightSource& light_source)
	throw()
{
	List<LightSource>::Iterator it = light_sources_.begin();
	for (; it != light_sources_.end(); it++)
	{
		if (&*it == &light_source) 
		{
			light_sources_.erase(it);
			return;
		}
	}
}

bool Stage::operator == (const Stage& stage) const
	throw()
{
	return light_sources_ 					== stage.light_sources_ 		&&
				 clipping_planes_ 				== stage.clipping_planes_   &&
				 camera_ 					 				== stage.camera_ 						&&
				 background_color_ 				== stage.background_color_ 	&&
				 show_coordinate_system_ 	== stage.show_coordinate_system_ &&
				 eye_distance_ 						== stage.eye_distance_ 			&&
				 focal_distance_ 					== stage.focal_distance_ 		&&
				 swap_side_by_side_stereo_== stage.swap_side_by_side_stereo_;
}



void Stage::dump(std::ostream& s, Size depth) const
	throw()
{
	BALL_DUMP_STREAM_PREFIX(s);
	
	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	BALL_DUMP_DEPTH(s, depth);
	s << "Light sources: -------------------------------" << endl;

	List<LightSource>::ConstIterator it = light_sources_.begin();
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
	s << "Eye distance :  " << eye_distance_<< endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "Focal width:  " << focal_distance_<< endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "Swap side by side:  " << swap_side_by_side_stereo_ << endl;

	BALL_DUMP_STREAM_SUFFIX(s);
}

void Stage::rotate(const Quaternion& q)
	throw()
{
	camera_.rotate(q);

	List<LightSource>::Iterator it = light_sources_.begin();
	for (; it != light_sources_.end(); it++)
	{
		if (it->isRelativeToCamera()) 
		{
			it->rotate(q);
		}
	}	
}

void Stage::translate(const Vector3& v3)
	throw()
{
	camera_.translate(v3);

	List<LightSource>::Iterator it = light_sources_.begin();
	for (; it != light_sources_.end(); it++)
	{
		if (it->isRelativeToCamera()) 
		{
			it->translate(v3);
		}
	}
}

void Stage::moveCameraTo(const Camera& camera)
	throw()
{
	setCamera(camera);

	List<LightSource>::Iterator it = light_sources_.begin();
	for (; it != light_sources_.end(); it++)
	{
		if (it->isRelativeToCamera())
		{
			it->setPosition(camera_.getViewPoint() - (camera_.getViewVector() * 100));
			it->setDirection(camera_.getLookAtPosition());
		}
	}
}

} } // namespaces
