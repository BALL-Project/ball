// $Id: ballAndStickModel.h,v 1.7 2000/12/12 16:16:52 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H
#define BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
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

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>
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
			: public AtomBondModelBaseProcessor
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			AddBallAndStickModel();

			AddBallAndStickModel
				(const AddBallAndStickModel& add_ball_and_stick_model, bool deep = true);

			virtual ~AddBallAndStickModel()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
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
			
			virtual Processor::Result operator() (Composite& composite);
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
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
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
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/ballAndStickModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H
