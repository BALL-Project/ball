#ifndef BALL_VIEW_WIDGETS_SDWIDGET_H
#define BALL_VIEW_WIDGETS_SDWIDGET_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#include <BALL/KERNEL/system.h>

#include <QtWidgets/QWidget>

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
				SDWidget(const System& system, QWidget *parent = 0);
	
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
				void plot(const System& system, bool create_sd = true);

				///
				System& getSystem() {return system_;};
				
				///
				const System& getSystem() const {return system_;};

				///
				void setSystem(const System& system) {system_ = system;};

				///
				void clear();

				QSize sizeHint() const;

			protected slots:
				void exportImage_();

			protected:
				void setup_();
				void paintEvent(QPaintEvent *event);

				void renderSD_(QPaintDevice* paint_device);
				void drawFrame_();

				QPointF getEndpoint_(QRectF& character_boundary, QPointF from, QPointF to, bool character_is_from);
				System system_;

				bool resize_to_parent_;

				Vector3 upper_;
				Vector3 lower_;
		};

	}
}

#endif
