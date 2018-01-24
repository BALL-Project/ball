#ifndef CLIPPINGPLANES_H
#define CLIPPINGPLANES_H

#include <Eigen/Geometry>

#include <vector>


/**
 * @brief Manages and stores clipping planes in world space.
 *
 * There is a fixed number of clipping planes which may be stored. MAX_CLIPPING_PLANES is set to 12.
 * The first 6 clipping planes may be used for frustum clipping and can be created by calling
 * createFrustumClippingPlanes(). The other 6 planes can be set using addClippingPlane() or
 * setClippingPlane().
 *
 * Planes are handled using the coefficients a, b, c, d which describe the plane equation
 * ax+by+cz+d=0.
 **/
class ClippingPlanes
{


	public:

		/**
		 * @brief Maximum number of clipping planes the class can hold.
		 **/
		const static unsigned int MAX_CLIPPING_PLANES = 12;

		/**
		 * @brief Default constructor.
		 *
		 * Calles clear() to initialize the planes.
		 *
		 **/
		ClippingPlanes();

		/**
		 * @brief Copy constructor.
		 *
		 * @param clippin_planes The clipping planes to copy.
		 **/
		ClippingPlanes(const ClippingPlanes& clippin_planes);


		/**
		 * @brief Destructor.
		 *
		 **/
		virtual ~ClippingPlanes();

		/**
		 * @brief Assignment operator.
		 *
		 * @param clipping_planes The clipping planes to assign.
		 * @return ClippingPlanes& Returns this.
		 **/
		ClippingPlanes& operator = (const ClippingPlanes& clipping_planes);

		/**
		 * @brief Prints the current status of the clipping planes to std::cout.
		 *
		 * @return void
		 **/
		void dump();

		/**
		 * @brief Add a single clipping plane.
		 *
		 * The plane is set to num_planes_ array index. This is the highest set array index + 1.
		 *
		 * @param plane The plane to be set.
		 * @return bool Returns the success of this operation.
		 **/
		bool addClippingPlane(const Eigen::Vector4f& plane);

		/**
		 * @brief Add a single clipping plane.
		 *
		 * The plane is set to num_planes_ array index. This is the highest set array index + 1.
		 *
		 * @param a Coefficient a of the plane equation ax+by+cz+d=0.
		 * @param b Coefficient b of the plane equation ax+by+cz+d=0.
		 * @param c Coefficient c of the plane equation ax+by+cz+d=0.
		 * @param d Coefficient d of the plane equation ax+by+cz+d=0.
		 * @return bool Returns the success of this operation.
		 **/
		bool addClippingPlane(float a, float b, float c, float d);

		/**
		 * @brief Add multiple clipping planes.
		 *
		 *Plane i is set to num_planes_ + i array index. This is the highest set array index + 1 + i.
		 *
		 * @param clippingPlanes The clipping planes to be set.
		 * @return bool Returns the success of this operation.
		 **/
		bool addClippingPlanes(const std::vector< Eigen::Vector4f>& clippingPlanes);

		/**
		 * @brief Add a single clipping plane to index i.
		 *
		 * @param i Array index the plane is set to.
		 * @param plane The plane to be set.
		 * @return bool Returns the success of this operation.
		 **/
		bool setClippingPlane(unsigned int i, const Eigen::Vector4f& plane);

		/**
		 * @brief Add a single clipping plane to index i.
		 *
		 * @param i Array index the plane is set to.
		 * @param a Coefficient a of the plane equation ax+by+cz+d=0.
		 * @param b Coefficient b of the plane equation ax+by+cz+d=0.
		 * @param c Coefficient c of the plane equation ax+by+cz+d=0.
		 * @param d Coefficient d of the plane equation ax+by+cz+d=0.
		 * @return bool Returns the success of this operation.
		 **/
		bool setClippingPlane(unsigned int i, float a, float b, float c, float d);

		/**
		 * @brief Returns the highest used array index + 1.
		 *
		 * @return int The highest used array index + 1.
		 **/
		int countClippingPlanes();

		/**
		 * @brief Create the frustum clipping planes.
		 *
		 * The planes are set to index 0-5. They are extracted from the view-projection matrix using the
		 * properties of clip space.
		 *
		 * @param view_matrix The view matrix.
		 * @param projection_matrix The projection matrix.
		 * @return void
		 **/
		void createFrustumClippingPlanes(const Eigen::Affine3f& view_matrix, const Eigen::Matrix4f& projection_matrix);

		/**
		 * @brief Resets all data.
		 *
		 * All clipping planes are set to a plane infront of the near plane. Thus, no clipping will occur.
		 *
		 * @return bool Success of this operation.
		 **/
		bool clear();

		/**
		 * @brief Returns the clipping planes.
		 *
		 * This is an array of MAX_CLIPPING_PLANES * 4 float values.
		 *
		 * @return float* Array of float values describing the clipping planes.
		 **/
		float* getClippingPlanes();

		/**
		 * @brief Returns one clipping plane.
		 *
		 * This is an array of 4 float values describing clipping plane i.
		 *
		 * @param i The index of the clipping plane to be returned.
		 * @return float* Array of float values describing the clipping plane.
		 **/
		float* getClippingPlane(int i);


	private:

		/**
		 * @brief Normalizes a plane equation.
		 *
		 * Plane equation consists of 4 floats a, b, c, d which fullfill the following equation:
		 * ax + by + cz + d = 0. This is normalized by dividing a, b, c, d by sqrt(a^2 + b^2 + c^2).
		 *
		 * @param plane Pointer to the first element of an array containing 4 floats.
		 * @return void
		 **/
		void normalize_(float* plane);

		/**
		 * @brief Number of planes.
		 *
		 * The highest set array index + 1 or 0 if nothing is set.
		 * This implies that this value is the array index which should be set next.
		 **/
		int num_planes_;

		/**
		 * @brief True, if the first 6 planes are used for the frustum planes and are initialized.
		 **/
		bool frustum_planes_set_;

		/**
		 * @brief Array which stores the planes.
		 *
		 * Index i, i+1, i+2, i+3 for i%4 = 0 correspond to one plane (4 floats for one plane beginning at
		 * index 0).
		 **/
		float planes_[MAX_CLIPPING_PLANES * 4];

};

#endif// CLIPPINGPLANES_H
