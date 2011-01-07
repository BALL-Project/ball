#ifndef BALL_VIEW_RENDERING_CAMERA_H
#define BALL_VIEW_RENDERING_CAMERA_H

#ifndef BALL_COMMON_GLOBAL_H
	#include <BALL/COMMON/global.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
	#include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_QUATERNION_H
	#include <BALL/MATHS/quaternion.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Camera with viewpoint, a look at point and an up-vector.
				\ingroup ViewKernelStage
		 */
		class BALL_VIEW_EXPORT Camera
		{
			public:
				/// Enumeration of different projection modes
				enum ProjectionMode
				{
					PERSPECTIVE = 0,
					ORTHOGRAPHIC
				};

				/**	@name	Constructors and Destructors
				*/
				//@{

				/// Constructor
				Camera();

				/// Copy Constructor
				Camera(const Camera& camera);

				Camera(const Vector3& view_point, const Vector3& look_at, const Vector3& look_up_vector, const ProjectionMode& mode = PERSPECTIVE);

				/// Destructor
				virtual ~Camera(){}

				///
				Camera& operator = (const Camera& camera);

				//@}
				/**	@name	Accessors
				*/
				//@{

				/// Move the camera along the right vector
				void moveRight(float translation)
					{ view_point_ += right_vector_*translation; look_at_ += right_vector_*translation; }

				/// Move the camera along the up vector
				void moveUp(float translation)
					{ view_point_ += look_up_vector_*translation; look_at_ += look_up_vector_*translation; }

				/// Move the camera along the view vector
				void moveForward(float translation)
				{
					Vector3 normal_view_vector(view_vector_);
					normal_view_vector.normalize();
					view_point_ += normal_view_vector*translation;
					look_at_ += normal_view_vector*translation;
				}

				/// Get the position of the camera
				const Vector3& getViewPoint() const
					{ return view_point_;}

				/// Set the position of the camera
				void setViewPoint(const Vector3& view_point)
					{ view_point_ = view_point; calculateVectors_();}

				/// Get the direction of the camera
				const Vector3& getLookAtPosition() const
					{ return look_at_;}

				/// Set the direction of the camera
				void setLookAtPosition(const Vector3& look_at)
					{ look_at_ = look_at; calculateVectors_();}

				/// Get the look up vector
				const Vector3& getLookUpVector() const
					{ return look_up_vector_;}

				/// Set the look up vector and compute the new right vector
				void setLookUpVector(const Vector3& look_up_vector)
					{ look_up_vector_ = look_up_vector; calculateVectors_();}

				/// Rotate up and right around the view vector
				void rotateAboutView(float degree);

				/// Get the distance between the view point and the look at point
				float getDistance() const
					{ return view_point_.getDistance(look_at_);}

				/// Get the view vector
				Vector3 getViewVector() const
					{ return view_vector_;}

				/// Get an vector orthogonal to the viewing vector and showing to the right
				Vector3 getRightVector() const
					{ return right_vector_;}

				/// Translate the view point and the point the camera is looking to by a given vector
				void translate(const Vector3& v);

				/// Convert the given vector from camera system to cartesian coordinates
				Vector3 convertCameraToSceneCoordinates(const Vector3& v);

				/// Rotate the camera
				void rotate(const Quaternion& q, const Vector3& origin);

				/// Rotate the camera
				void rotate(const Matrix4x4& mat, const Vector3& origin);

				/// Reset Camera to standard values
				virtual void clear()
					{ *this = Camera();}

				/// Set the projection mode
				void setProjectionMode(ProjectionMode const& mode)
				{ projection_mode_ = mode; }

				/// Get the projection mode
				ProjectionMode getProjectionMode() const
				{ return projection_mode_; }

				///
				String toString() const;

				///
				bool readFromString(const String& data);

				//@}
				/**	@name Predicates
				*/
				//@{

				///
				bool operator == (const Camera& camera) const;

				/// Needed for MSVC
				bool operator < (const Camera& camera) const;

				//@}

				/** Internal value dump.
						Dump the current state of this instance to
						the output ostream <b> s</b> with dumping depth <b> depth</b>.
						\param   s output stream
						\param   depth the dumping depth
				*/
				virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			protected:
				//_
				void calculateVectors_();

				//_
				Vector3 view_point_;

				//_
				Vector3 look_at_;

				//_
				Vector3 look_up_vector_;

				/*_ The viewing vector.
						Stored only for better performance in the scene.
				*/
				Vector3 view_vector_;

				/*_ The orthogonal vector to the viewing vector, which is showing to the right.
						Stored only for better performance in the scene.
				*/
				Vector3 right_vector_;

				/* The projection mode used by this camera.
				 */
				ProjectionMode    projection_mode_;
		};

	} // namespace VIEW
} // namespace BALL

#endif //BALL_VIEW_RENDERING_CAMERA_H
