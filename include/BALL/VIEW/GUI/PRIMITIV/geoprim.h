// $Id: geoprim.h,v 1.2 2000/12/12 16:15:42 oliver Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV__H
#define BALL_VIEW_GUI_PRIMITIV__H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_COLOR_H
#	include <BALL/VIEW/color.h>
#endif

#ifndef BALL_VIEW_DLISTMAN_H
#	include <BALL/VIEW/dlistman.h>
#endif

//#ifndef BALL_VIEW_EXPORT_H
//#	include <BALL/VIEW/extract.h>
//#endif

#ifndef BALL_VIEW_GEOOBJ_H
#	include <BALL/VIEW/geoobj.h>
#endif

#ifndef BALL_VIEW_NAMESPACE_H
#	include <BALL/VIEW/namespace.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/* forwards */
		class Sphere;
		class Tube;
		class Cone;
		class Box;

		// ----------------------------------------------------------------------------
		// Class Sphere
		// ----------------------------------------------------------------------------

		class Sphere
			: public ColorExtension,
				public Point,
				public Radius,
				public Resolution,
				public GeometricObject
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			Sphere();
		
			Sphere(const Sphere& sphere);

			Sphere(const Sphere& sphere, bool deep);
		
			Sphere(const GeometricObject& geometric_object);

			virtual ~Sphere()
				throw();
			//@}


			/**	@name	Debugging and Diagnostics
			*/
			//@{
			virtual void dump(std::ostream& s) const
				throw();
			//@}

			

			protected:

			virtual bool draw();

			virtual bool extract();

			virtual GeometricObject *getGeometricObject();
		};


		// ----------------------------------------------------------------------------
		// Class Tube
		// ----------------------------------------------------------------------------

		class Tube
			: public ColorExtension,
			  public OpenCloseExtension,
				public Point2,
				public Radius,
				public Resolution,
				public GeometricObject
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Tube();

			Tube(const Tube& tube);

			Tube(const Tube& tube, bool deep = true);
		
			Tube(const Cone& cone);

			Tube(const Box& box);

			Tube(const GeometricObject& geometric_object);

			virtual ~Tube();
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual void dump(std::ostream& s) const;
			//@}

			/**	@name	Accessors
			*/
			//@{
			Real getSquareLength() const;

			Real getLength() const;

			bool getSquareLength(Real& square_length) const;

			bool getLength(Real& length) const;
			//@}


			protected:

			virtual bool draw();

			virtual bool extract();

			virtual GeometricObject *getGeometricObject();
		};


		// ----------------------------------------------------------------------------
		// Class Cone
		// ----------------------------------------------------------------------------

		class Cone
			: public ColorExtension,
				public OpenCloseExtension,
				public Point2,
				public Radius,
				public Resolution,
				public GeometricObject
		{
			public:
			
			/**	@name	Constructors and Destructors
			*/
			//@{
				
			Cone();

			Cone(const Cone& cone);
		
			Cone(const Cone& cone, bool deep = true);
		
			Cone(const Tube& tube);

			Cone(const Box& box);

			Cone(const GeometricObject& geometric_object);

			virtual ~Cone();
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			virtual void dump(std::ostream& s) const;
			//@}

			/**	@name	Accessors
			*/
			//@{
			Real getSquareLength() const;
	 
			Real getLength() const;
		
			bool getSquareLength(Real& square_length) const;

			bool getLength(Real& length) const;
			//@}
			

		protected:

			virtual bool draw();

			virtual bool extract();

			virtual GeometricObject *getGeometricObject();
		};

		// ----------------------------------------------------------------------------
		// Class Box
		// ----------------------------------------------------------------------------

		class Box
			: public ColorExtension,
				public Point2,
				public GeometricObject
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Box();

			Box(const Box& box, bool deep = true);

			Box(const Tube& tube);

			Box(const Cone& cone);

			Box(const GeometricObject& geometric_object);

			virtual ~Box();
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{
			virtual void dump(std::ostream& s) const;
			//@}

			/**	@name	Accessors
			*/
			//@{

			Real getSquareDiameter() const;

			Real getDiameter() const;
		
			bool getSquareDiameter(Real& square_diameter) const;

			bool getDiameter(Real& diameter) const;
			//@}

			

			protected:

			virtual bool draw();

			virtual bool extract();

			virtual GeometricObject* getGeometricObject();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/geoprim.iC>
#		endif

	} // namespace VIEW
	
} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GEOPRIM_H
