#include <BALL/VIEW/RENDERING/camera.h>

#include <BALL/VIEW/KERNEL/common.h>

#include <iostream>

namespace BALL
{
	namespace VIEW
	{
		Camera::Camera()
			: view_point_(0, 0, 0),
				look_at_(0, 0, -1),
				look_up_vector_(0, -1, 0),
				projection_mode_(PERSPECTIVE)
		{
			calculateVectors_();
		}

		Camera::Camera(const Camera& camera)
			: view_point_(camera.view_point_),
				look_at_(camera.look_at_),
				look_up_vector_(camera.look_up_vector_),
				view_vector_(camera.view_vector_),
				right_vector_(camera.right_vector_),
				projection_mode_(camera.projection_mode_)
		{
		}

		Camera::Camera(const Vector3& view_point, const Vector3& look_at, const Vector3& look_up_vector, const ProjectionMode& mode)
			: view_point_(view_point),
				look_at_(look_at),
				look_up_vector_(look_up_vector),
				projection_mode_(mode)

		{
			calculateVectors_();
		}

		Camera& Camera::operator = (const Camera& camera)
		{
					 view_point_ = camera.view_point_;
							look_at_ = camera.look_at_;
			 look_up_vector_ = camera.look_up_vector_;
					view_vector_ = camera.view_vector_;
				 right_vector_ = camera.right_vector_;
			projection_mode_ = camera.projection_mode_;

			return *this;
		}

		bool Camera::operator < (const Camera& camera) const
		{
			return this < &camera;
		}


		bool Camera::operator == (const Camera& camera) const
		{
			return view_point_      == camera.view_point_     &&
			       look_at_         == camera.look_at_        &&
			       look_up_vector_  == camera.look_up_vector_ &&
			       projection_mode_ == camera.projection_mode_;
		}

		void Camera::calculateVectors_()
		{
			view_vector_  = look_at_ - view_point_;

			if (look_up_vector_.isZero())
				look_up_vector_.set(0,1,0);
			else
			{
				Vector3 normalized_view(view_vector_);
				normalized_view.normalize();

				look_up_vector_ -= normalized_view*(normalized_view*look_up_vector_);
				look_up_vector_.normalize();
			}

			right_vector_ = look_up_vector_ % view_vector_;
			right_vector_ *= -1.0;

			if (!right_vector_.isZero()) right_vector_.normalize();
			else right_vector_.set(1,0,0);
		}

		void Camera::dump(std::ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Viewpoint : " << view_point_ << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Look at : " << look_at_ << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Look up vector: " << look_up_vector_ << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Projection mode: " << ((projection_mode_ == PERSPECTIVE) ? "perspective" : "orthographic") << std::endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		String Camera::toString() const
		{
			// NOTE: we do *not* store the projection mode when we write the current camera.
			//       Instead, it is extracted from the stage settings dialog. If we would store
			//       it here, we would break backwards compatibility severely.
			return vector3ToString(view_point_)     + " " +
						 vector3ToString(look_at_)        + " " +
						 vector3ToString(look_up_vector_);
		}

		bool Camera::readFromString(const String& data)
		{
			vector<String> fields;
			if (data.split(fields) < 3) return false;

			Vector3 results[3];
			for (Position p = 0; p < 3; p++)
			{
				if (!stringToVector3(fields[p], results[p])) return false;
			}

			view_point_     = results[0];
			look_at_        = results[1];
			look_up_vector_ = results[2];

			calculateVectors_();

			return true;
		}

		void Camera::rotate(const Quaternion& q, const Vector3& origin)
		{
			translate(-origin);
			Matrix4x4  m;
			q.getRotationMatrix(m);

			view_point_     = m*view_point_;
			look_at_        = m*look_at_;
			look_up_vector_ = m*look_up_vector_;

			calculateVectors_();
			translate(origin);
		}

		void Camera::rotate(const Matrix4x4& mat, const Vector3& origin)
		{
			translate(-origin);

			view_point_     = mat*view_point_;
			look_at_        = mat*look_at_;
			look_up_vector_ = mat*look_up_vector_;

			calculateVectors_();
			translate(origin);
		}

		void Camera::rotateAboutView(float degree)
		{
			Matrix4x4 m;
			m.setRotation(Angle(-degree, false), view_vector_);

			// setLookUp takes care of normalization and the right vector for us...
			setLookUpVector(m * look_up_vector_);
		}

		void Camera::translate(const Vector3& v)
		{ 
			view_point_ += v; 
			look_at_ += v; 
			calculateVectors_();
		}

		Vector3 Camera::convertCameraToSceneCoordinates(const Vector3& v)
		{
			Vector3 vv = view_vector_;
			float length = vv.getLength();
			if (!Maths::isZero(length)) vv /= length;

			return v.x * right_vector_ +
				     v.y * look_up_vector_ -
						 v.z * vv;
		}

	} // namespace VIEW
} // namespace BALL
