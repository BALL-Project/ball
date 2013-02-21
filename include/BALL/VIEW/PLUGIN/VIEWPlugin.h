#ifndef BALL_VIEW_PLUGIN_VIEWPLUGIN_H
#define BALL_VIEW_PLUGIN_VIEWPLUGIN_H

#include <BALL/VIEW/KERNEL/preferencesEntry.h>

#include <QtCore/QString>
#include <QtCore/QtPlugin>

#include <QtGui/QDialog>

class QPixmap;

namespace BALL
{
	namespace VIEW
	{
		class ConfigDialog : public QDialog, public PreferencesEntry
		{
			public:
				ConfigDialog(QWidget* parent, Qt::WindowFlags f)
					: QDialog(parent, f)
				{
				}

				virtual ~ConfigDialog() {};
		};

		class VIEWPlugin
		{
			public:
				virtual ~VIEWPlugin() {}

				virtual QString getName() const = 0;
				virtual QString getDescription() const = 0;
				virtual const QPixmap* getIcon() const = 0;

				virtual ConfigDialog* getConfigDialog() = 0;

				virtual bool isActive() = 0;
		};

	}
}

Q_DECLARE_INTERFACE(BALL::VIEW::VIEWPlugin, "org.ball-project.Plugin.VIEWPlugin/1.0")

#endif // BALL_VIEW_PLUGIN_VIEWPLUGIN_H
