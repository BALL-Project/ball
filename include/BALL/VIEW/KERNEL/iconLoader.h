#ifndef BALL_VIEW_KERNEL_ICONLOADER_H
#define BALL_VIEW_KERNEL_ICONLOADER_H

#include <list>
#include <boost/shared_ptr.hpp>
#include <BALL/DATATYPE/hashMap.h>

#include <QtCore/QMutex>
#include <QtCore/QReadWriteLock>
#include <QtCore/QStringList>

class QIcon;

namespace BALL
{
	class String;

	namespace VIEW
	{
		// Phase 999.42: ThemedIconEngine LANDED — see
		// `BALL/VIEW/KERNEL/theme/themedIconEngine.h` for the per-DPR
		// state-tinted SVG renderer (Normal/Active/Selected/Disabled tints
		// per QIcon::Mode). Public entry point is
		// `BALL::VIEW::Icons::get(name)` in
		// `BALL/VIEW/KERNEL/theme/iconRegistry.h`. Under BALL_UI_V2 it
		// routes to the engine; under !BALL_UI_V2 it delegates here.
		//
		// NEW CALL SITES MUST use Icons::get() — IconLoader is preserved
		// as the BALL_UI_V2=OFF fallback path and for plugin ABI compat
		// for one release. Slated for removal in v2.0 (see BALL_DEPRECATED
		// annotation below).
		/**
		 * This class is an icon loader for the VIEW classes.
		 * It handles icon themes as defined in the freedesktop.org standard.
		 * It automatically detects all present resolutions of an icon and loads them.
		 * Furthermore it features an icon cache in order to minimize disk access. There
		 * are essentially two ways to use this class: Use it in a singleton wise fashion or
		 * create an specialized instance.
		 */
		class BALL_VIEW_EXPORT IconLoader
		{
			public:
				/**
				 * Creates an IconLoader instance using BALL_DATA_PATH/graphics/icons as
				 * icon path.
				 */
				IconLoader();

				/**
				 * Creates an IconLoader using the directories in icon_dirs as icon paths.
				 *
				 * @param icon_dirs A list of paths to icon directories.
				 */
				explicit IconLoader(const QStringList& icon_dirs);

				/**
				 * Creates an IconLoader using the directories in icon_dirs as icon paths.
				 *
				 * @param icon_dirs A list of paths to icon directories.
				 */
				explicit IconLoader(const std::list<String>& icon_dirs);

				/**
				 * Destructor. It deletes all cached icons and thus invalidates all external
				 * pointers to these icons.
				 */
				~IconLoader();

				/**
				 * Return the global instance of the icon loader.
				 */
				static IconLoader& instance();

				/**
				 * Add another icon directory. The directory must be organised
				 * in a freedesktop.org compliant fashion. Essentially this means its layout
				 * has to look like this:
				 * 	path/nxn/context/icon.png
				 *
				 * @param path The path to an icon directory.
				 */
				void appendIconPath(const String& path);

				/**
				 * Retrieve the icon identified by name from the directory.
				 *
				 * @param The icons name. A usual example looks like: "actions/document-save"
				 *
				 * @deprecated Phase 999.42: use BALL::VIEW::Icons::get(name)
				 *             (see BALL/VIEW/KERNEL/theme/iconRegistry.h).
				 *             Slated for removal in v2.0.
				 */
				[[deprecated("Phase 999.42 — use BALL::VIEW::Icons::get from theme/iconRegistry.h")]]
				const QIcon& getIcon(const String& name);

			private:
				//Threads and singleton
				static QMutex mutex_;
				static boost::shared_ptr<IconLoader> loader_;
				QReadWriteLock hash_map_lock_;

				//Private members
				const QIcon* const invalid_;
				std::list<int> sizes_;
				QStringList icon_dirs_;
				HashMap<String, QIcon*> icon_map_;

				//Private methods
				void setup_();
				QIcon* loadIcon_(const String& name);
		};
	}
}

#endif //BALL_VIEW_KERNEL_ICONLOADER_H

