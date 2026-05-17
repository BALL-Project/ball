// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/workspaceManager.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/COMMON/logStream.h>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QByteArray>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>
// Qt 6: QAction moved from QtWidgets to QtGui — see Qt 6 porting guide.
#include <QtGui/QAction>
#include <QtGui/QCursor>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QStatusBar>

namespace BALL
{
	namespace VIEW
	{
		// =======================================================================
		// WorkspaceManager
		// =======================================================================

		WorkspaceManager& WorkspaceManager::instance()
		{
			// Function-local static: thread-safe lazy init in C++17, lives for
			// the lifetime of the process. We deliberately don't attach to a
			// QApplication parent — the singleton outlives the QApplication
			// during shutdown (Qt 6 destroys QApplication children before main
			// returns, which would crash any late-shutdown callers).
			static WorkspaceManager s_instance;
			return s_instance;
		}

		WorkspaceManager::WorkspaceManager()
			: QObject(nullptr),
			  current_preset_(Default)
		{
		}

		WorkspaceManager::~WorkspaceManager() = default;

		WorkspaceManager::Preset WorkspaceManager::currentPreset() const
		{
			return current_preset_;
		}

		void WorkspaceManager::setCurrentPreset(Preset preset)
		{
			if (current_preset_ == preset) return;
			current_preset_ = preset;
			Q_EMIT presetChanged(preset);
		}

		QString WorkspaceManager::presetDisplayName(Preset preset) const
		{
			switch (preset)
			{
				case Default:     return tr("Default");
				case Classic:     return tr("Classic");
				case Focused:     return tr("Focused");
				case UserDefined: return tr("User Defined");
			}
			return QString();
		}

		QString WorkspaceManager::builtInPresetPath(Preset preset) const
		{
			QString filename;
			switch (preset)
			{
				case Default:     filename = "default.layout"; break;
				case Classic:     filename = "classic.layout"; break;
				case Focused:     filename = "focused.layout"; break;
				case UserDefined: return QString();  // not built-in
			}

			BALL::Path path;
			// BALL::Path::find returns the absolute filesystem path of a data
			// file relative to BALL_DATA_PATH, scanning every directory listed
			// in the path. Returns empty string if not found.
			const BALL::String rel = BALL::String("BALLView/workspaces/") + BALL::String(filename.toStdString().c_str());
			BALL::String found = path.find(rel);
			if (found.empty())
			{
				return QString();
			}
			return QString::fromStdString(found);
		}

		namespace
		{
			// Map an area string from JSON to Qt's dock-area enum.
			Qt::DockWidgetArea parseDockArea(const QString& area)
			{
				const QString a = area.toLower();
				if (a == "left")   return Qt::LeftDockWidgetArea;
				if (a == "right")  return Qt::RightDockWidgetArea;
				if (a == "top")    return Qt::TopDockWidgetArea;
				if (a == "bottom") return Qt::BottomDockWidgetArea;
				return Qt::NoDockWidgetArea;
			}
		}

		bool WorkspaceManager::apply(Preset preset, QMainWindow* main)
		{
			if (main == nullptr) return false;
			if (preset == UserDefined) return false;  // use load(name) instead

			const QString path = builtInPresetPath(preset);
			if (path.isEmpty())
			{
				BALL::Log.warn() << "WorkspaceManager: built-in preset descriptor not found for preset "
				                 << static_cast<int>(preset) << " (BALL_DATA_PATH misconfigured?)"
				                 << std::endl;
				return false;
			}

			QFile f(path);
			if (!f.open(QIODevice::ReadOnly))
			{
				BALL::Log.warn() << "WorkspaceManager: could not open " << path.toStdString() << std::endl;
				return false;
			}
			const QByteArray bytes = f.readAll();
			f.close();

			QJsonParseError err;
			const QJsonDocument doc = QJsonDocument::fromJson(bytes, &err);
			if (doc.isNull() || !doc.isObject())
			{
				BALL::Log.warn() << "WorkspaceManager: malformed JSON in " << path.toStdString()
				                 << ": " << err.errorString().toStdString() << std::endl;
				return false;
			}

			const QJsonObject root = doc.object();
			const QJsonArray docks = root.value("docks").toArray();
			const bool toolbarsVisible = root.value("toolbarsVisible").toBool(true);

			// Walk the dock descriptor list and place each registered dock.
			QList<QDockWidget*> placed_docks;
			QList<int> placed_sizes;
			bool placed_any = false;

			// Track docks we explicitly want visible vs hidden so we can hide
			// every other dock that's NOT in the preset (Focused, in
			// particular, wants every dock gone except bottomDrawer).
			QSet<QString> referenced_names;

			for (const QJsonValue& v : docks)
			{
				if (!v.isObject()) continue;
				const QJsonObject d = v.toObject();
				const QString name = d.value("name").toString();
				const QString areaStr = d.value("area").toString();
				const bool visible = d.value("visible").toBool(true);
				const int size = d.value("size").toInt(0);
				referenced_names.insert(name);

				QDockWidget* dock = main->findChild<QDockWidget*>(name);
				if (dock == nullptr)
				{
					// Not yet registered — InspectorDock from 999.44 may not be
					// present in a partial v1.7 RC build. Skip with a debug
					// log; don't fail the whole apply().
					BALL::Log.info() << "WorkspaceManager: dock '" << name.toStdString()
					                 << "' not present — skipping" << std::endl;
					continue;
				}

				const Qt::DockWidgetArea area = parseDockArea(areaStr);
				if (area != Qt::NoDockWidgetArea)
				{
					main->addDockWidget(area, dock);
				}
				dock->setVisible(visible);

				if (size > 0)
				{
					placed_docks.append(dock);
					placed_sizes.append(size);
				}
				placed_any = true;
			}

			// Hide every dock not mentioned in the preset descriptor (Focused
			// preset, for instance, only lists bottomDrawer — every other
			// dock should disappear).
			const QList<QDockWidget*> all = main->findChildren<QDockWidget*>();
			for (QDockWidget* dock : all)
			{
				if (!referenced_names.contains(dock->objectName()))
				{
					dock->setVisible(false);
				}
			}

			// Apply size hints. Group by orientation: horizontal sizes for
			// Left/Right docks, vertical for Top/Bottom. resizeDocks() takes
			// one orientation at a time.
			if (!placed_docks.isEmpty())
			{
				QList<QDockWidget*> h_docks;
				QList<int> h_sizes;
				QList<QDockWidget*> v_docks;
				QList<int> v_sizes;
				for (int i = 0; i < placed_docks.size(); ++i)
				{
					const Qt::DockWidgetArea area = main->dockWidgetArea(placed_docks[i]);
					if (area == Qt::LeftDockWidgetArea || area == Qt::RightDockWidgetArea)
					{
						h_docks.append(placed_docks[i]);
						h_sizes.append(placed_sizes[i]);
					}
					else
					{
						v_docks.append(placed_docks[i]);
						v_sizes.append(placed_sizes[i]);
					}
				}
				if (!h_docks.isEmpty()) main->resizeDocks(h_docks, h_sizes, Qt::Horizontal);
				if (!v_docks.isEmpty()) main->resizeDocks(v_docks, v_sizes, Qt::Vertical);
			}

			// Toolbar visibility — walk every QToolBar child.
			const QList<QToolBar*> toolbars = main->findChildren<QToolBar*>();
			for (QToolBar* tb : toolbars)
			{
				tb->setVisible(toolbarsVisible);
			}

			if (placed_any || preset == Focused)
			{
				// Focused legitimately places zero docks visible besides
				// bottomDrawer — still a successful apply.
				current_preset_ = preset;
				Q_EMIT presetChanged(preset);
				return true;
			}
			return false;
		}

		namespace
		{
			// Qt INI key under [Workspace] for the user-defined payload of a
			// given name. Avoids accidental collisions with reserved keys
			// ("currentPreset", "firstRunPromptSeen") by prefixing.
			QString userDefinedKey(const QString& name)
			{
				return QString("Workspace/userDefined__") + name;
			}

			QSettings* settingsForApp()
			{
				// QSettings with explicit IniFormat in the platform-correct
				// AppConfigLocation. Post-migration this is the same file
				// ConfigMigration::run produces. Pre-migration the legacy
				// flat ~/.BALLView is still read by the existing BALL
				// INIFile path; the [Workspace] section is new in 999.45
				// so we own its location entirely.
				const QString dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
				QDir().mkpath(dir);
				const QString path = dir + "/BALLView.ini";
				return new QSettings(path, QSettings::IniFormat);
			}
		}

		bool WorkspaceManager::save(QMainWindow* main, const QString& name)
		{
			if (main == nullptr || name.isEmpty()) return false;
			const QByteArray state = main->saveState();
			QSettings* s = settingsForApp();
			s->setValue(userDefinedKey(name), state.toBase64());
			s->sync();
			delete s;
			return true;
		}

		bool WorkspaceManager::load(QMainWindow* main, const QString& name)
		{
			if (main == nullptr || name.isEmpty()) return false;
			QSettings* s = settingsForApp();
			const QVariant v = s->value(userDefinedKey(name));
			delete s;
			if (!v.isValid()) return false;
			const QByteArray state = QByteArray::fromBase64(v.toByteArray());
			if (state.isEmpty()) return false;
			if (!main->restoreState(state)) return false;
			current_preset_ = UserDefined;
			Q_EMIT presetChanged(UserDefined);
			return true;
		}

		QStringList WorkspaceManager::userPresets() const
		{
			QSettings* s = settingsForApp();
			s->beginGroup("Workspace");
			const QStringList keys = s->allKeys();
			s->endGroup();
			delete s;
			QStringList out;
			const QString prefix = "userDefined__";
			for (const QString& k : keys)
			{
				if (k.startsWith(prefix))
				{
					out.append(k.mid(prefix.size()));
				}
			}
			return out;
		}

		// =======================================================================
		// WorkspaceStatusLabel
		// =======================================================================

		WorkspaceStatusLabel::WorkspaceStatusLabel(QMainWindow* mainWindow)
			: QObject(mainWindow),
			  main_window_(mainWindow),
			  label_(new QLabel(mainWindow)),
			  event_filter_installed_(false)
		{
			label_->setObjectName("workspaceStatusLabel");
			label_->setCursor(Qt::PointingHandCursor);
			// Initial text reflects current preset.
			onPresetChanged(WorkspaceManager::instance().currentPreset());

			connect(&WorkspaceManager::instance(),
			        &WorkspaceManager::presetChanged,
			        this,
			        &WorkspaceStatusLabel::onPresetChanged);

			label_->installEventFilter(this);
			event_filter_installed_ = true;
		}

		WorkspaceStatusLabel::~WorkspaceStatusLabel()
		{
			if (event_filter_installed_ && label_ != nullptr)
			{
				label_->removeEventFilter(this);
			}
		}

		bool WorkspaceStatusLabel::eventFilter(QObject* watched, QEvent* event)
		{
			if (watched == label_ && event->type() == QEvent::MouseButtonPress)
			{
				QMouseEvent* me = static_cast<QMouseEvent*>(event);
				if (me->button() == Qt::LeftButton)
				{
					onLabelClicked();
					return true;
				}
			}
			return QObject::eventFilter(watched, event);
		}

		void WorkspaceStatusLabel::onPresetChanged(WorkspaceManager::Preset preset)
		{
			label_->setText(WorkspaceManager::instance().presetDisplayName(preset));
		}

		void WorkspaceStatusLabel::onLabelClicked()
		{
			if (main_window_ == nullptr) return;
			QMenu menu(label_);
			WorkspaceManager& wm = WorkspaceManager::instance();

			auto addPresetAction = [&](WorkspaceManager::Preset p) {
				QAction* a = menu.addAction(wm.presetDisplayName(p));
				QObject::connect(a, &QAction::triggered, [this, p]() {
					WorkspaceManager::instance().apply(p, main_window_);
				});
			};
			addPresetAction(WorkspaceManager::Default);
			addPresetAction(WorkspaceManager::Classic);
			addPresetAction(WorkspaceManager::Focused);

			menu.addSeparator();

			const QStringList userNames = wm.userPresets();
			for (const QString& name : userNames)
			{
				QAction* a = menu.addAction(name);
				QObject::connect(a, &QAction::triggered, [this, name]() {
					WorkspaceManager::instance().load(main_window_, name);
				});
			}
			if (!userNames.isEmpty())
			{
				menu.addSeparator();
			}

			QAction* saveAction = menu.addAction(tr("Save current as…"));
			QObject::connect(saveAction, &QAction::triggered, [this]() {
				bool ok = false;
				const QString name = QInputDialog::getText(
				    main_window_,
				    tr("Save Workspace"),
				    tr("Preset name:"),
				    QLineEdit::Normal,
				    QString(),
				    &ok);
				if (ok && !name.isEmpty())
				{
					WorkspaceManager::instance().save(main_window_, name);
				}
			});

			menu.exec(QCursor::pos());
		}

	} // namespace VIEW
} // namespace BALL
