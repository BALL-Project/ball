// $Id: ballAndStickModel.h,v 1.3 1999/12/28 18:38:05 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H
#define BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_FOREACH_H
#	include <BALL/KERNEL/forEach.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#	include <BALL/VIEW/PRIMITIV/sphere.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#	include <BALL/VIEW/PRIMITIV/tube.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModel.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.h>
#endif


namespace BALL
{
	
	namespace MOLVIEW
	{

		using VIEW::Sphere;
		using VIEW::Tube;

		/**
		*/
		class AddBallAndStickModel
			: public BaseModelProcessor
		{
			public:

			enum ErrorCode
			{
				ERROR__NONE                             = -1,
				ERROR__UNKNOWN                          = 0,
				ERROR__NOT_IMPLEMENTED                  = 1,
				ERROR__CANNOT_CREATE_SPHERE             = 2,
				ERROR__CANNOT_CREATE_TUBE               = 3,
				ERROR__CANNOT_CREATE_2CTUBE             = 4,
				ERROR_BALL_RADIUS_LOWER_OR_EQUAL_ZERO  = 5,
				ERROR__STICK_RADIUS_LOWER_OR_EQUAL_ZERO = 6,


				NUMBER_OF_BALLANDSTICKMODEL_ERRORS
			};

			/**	@name	Constructors and Destructors
			*/
			//@{

			AddBallAndStickModel();

			AddBallAndStickModel
				(const AddBallAndStickModel& add_ball_and_stick_model, bool deep = true);

			virtual ~AddBallAndStickModel();

			virtual void clear();

			virtual void destroy();
			//@}


			/**	@name	Assignment
			*/
			//@{

			void set
				(const AddBallAndStickModel& add_ball_and_stick_model, bool deep = true);

			AddBallAndStickModel& operator =
				(const AddBallAndStickModel& add_ball_and_stick_model);

			void get
				(AddBallAndStickModel& add_ball_and_stick_model, bool deep = true) const;

			void swap(AddBallAndStickModel& add_ball_and_stick_model);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setBallRadius(const Real radius);

			Real getBallRadius() const;

			void getBallRadius(Real& radius);


			void setStickRadius(const Real radius);

			Real getStickRadius() const;

			void getStickRadius(Real& radius);


			void enableBallAndStickModel();

			void enableStickModel();
			//@}

			/**	@name	Processor-Related Methods
			*/
			//@{

			virtual bool start();
			
			virtual bool finish();
			
			virtual Processor::Result operator() (Atom& atom);
			//@}

			/**	@name	Predicates
			*/
			//@{

			bool isBallAndStickModel() const;

			bool isStickModel() const;
			//@}


			/**	@name	Debugging and Diagnostics
			*/
			//@{
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}


			
			private:

			virtual Sphere* createSphere_();

			virtual Tube* createTube_();

			virtual TwoColoredTube* createTwoColoredTube_();

			Real ball_radius_;
			Real stick_radius_;
		
			bool ball_and_stick_;

			List<Atom*> used_atoms_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/ballAndStickModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H
