// $Id: twoColoredTube.h,v 1.6 2000/12/22 19:12:14 amoll Exp $

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
#define BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION2_H
#	include <BALL/VIEW/KERNEL/colorExtension2.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_BASETUBE_H
#	include <BALL/VIEW/PRIMITIV/basetube.h>
#endif


namespace BALL
{
	
	namespace MOLVIEW
	{


		using VIEW::BaseTube;
		using VIEW::ColorExtension2;

		/**
		*/
		class TwoColoredTube
			: public BaseTube,
				public ColorExtension2
		{
			public:

			/**	@name	Constructors and Destructors	
			*/
			//@{

			TwoColoredTube();

			TwoColoredTube
				(const TwoColoredTube& two_colored_tube, bool deep = true);

			TwoColoredTube
				(const VIEW::GeometricObject& geometric_object);

			virtual ~TwoColoredTube()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}


			/**	@name	Assignment
			*/
			//@{

			void set(const TwoColoredTube& two_colored_tube, bool deep = true);

			const TwoColoredTube& operator = (const TwoColoredTube& two_colored_tube);

			void get(TwoColoredTube& two_colored_tube, bool deep = true) const;

			void swap(TwoColoredTube& two_colored_tube);
			//@}

			/**	@name	Accessors
			*/
			//@{

			Vector3 getMiddleVertex();
			//@}


			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const
				throw();

			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s)
				throw();

			virtual void write(std::ostream& s) const
				throw();
			//@}


			
			protected:

			virtual bool extract()
				throw();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
