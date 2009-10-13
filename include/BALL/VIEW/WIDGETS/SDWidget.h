#ifndef BALL_VIEW_WIDGETS_SDWIDGET_H
#define BALL_VIEW_WIDGETS_SDWIDGET_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#include <BALL/KERNEL/system.h>
#include <QtGui/qwidget.h>

namespace BALL
{
	namespace VIEW
	{

		///
		class BALL_VIEW_EXPORT SDWidget 
			: public QWidget
		{
			Q_OBJECT

			public:
		
				/** @name Constant Definitions
				*/
				//@{
				/// Option names
				struct BALL_VIEW_EXPORT Option
				{		
					/**	show hydrogen atoms.
					*/
		 			static const char* SHOW_HYDROGENS;
				};

				/// Default values for options
				struct BALL_VIEW_EXPORT Default
				{
					static const bool SHOW_HYDROGENS;
				};
			
				//@}

				///
				SDWidget(QWidget *parent = 0, bool show_hydrogens = false);

				///
				SDWidget(const System& system, QWidget *parent = 0, bool resize_to_parent = true);
	
      	///
      	virtual ~SDWidget();

				/** @name Public Attributes
				*/
				//@{
				/// options
				Options options;

				/** Resets the options to default values.
				*/
				void setDefaultOptions();
				//@}

				///
				void plot(const System& system, bool clear = true, bool create_sd = true);

				///
				System& getSystem() {return system_;};
				
				///
				const System& getSystem() const {return system_;};

				///
				void setSystem(const System& system) {system_ = system;};

				///
				void clear();

			protected:
				void paintEvent(QPaintEvent *event);

				QPointF getEndpoint_(QRectF& character_boundary, QPointF from, QPointF to);
				System system_;

				bool resize_to_parent_;
				bool clear_;
		};

	}
}

#endif
