#ifndef PRESENTABALLVIEW_H
#define PRESENTABALLVIEW_H

#include <BALL/VIEW/WIDGETS/HTMLView.h>

#include <QSignalMapper>

namespace BALL
{
	namespace VIEW
	{
		class PresentaBALLSettings;

		class PresentaBALLSignal : public QObject
		{
			Q_OBJECT

			public:
				PresentaBALLSignal(QObject* parent = nullptr): QObject(parent) { }

			Q_SIGNALS:
				void actionSignal(int i);
				void messageSignal(int i, int j);
		};

		class PresentaBALLView : public HTMLView, public ModularWidget
		{
			Q_OBJECT
			BALL_EMBEDDABLE(PresentaBALLView, ModularWidget)

			public:
				PresentaBALLView(PresentaBALLSettings* settings, QWidget* parent = nullptr, const char* name = nullptr);
				virtual ~PresentaBALLView();

				virtual void onNotify(Message* message);
				virtual void applyPreferences();

			protected:
				void contextMenuEvent(QContextMenuEvent* evt);
			
			private:
				PresentaBALLSignal* signal_;
				QWebChannel* channel_;
				QSignalMapper* signalMapper_;
				QString index_html_;
				PresentaBALLSettings* settings_;
		};
	}
}

#endif // PRESENTABALLVIEW_H
