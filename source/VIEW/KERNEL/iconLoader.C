// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/KERNEL/iconLoader.h>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtGui/QIcon>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/DATATYPE/string.h>

namespace BALL
{
	namespace VIEW
	{

		QMutex IconLoader::mutex_;
		boost::shared_ptr<IconLoader> IconLoader::loader_;

		IconLoader::IconLoader()
			: invalid_(new QIcon())
		{
			Path p;
			String found = p.find("graphics/icons");
			if (found != "")
				icon_dirs_.append(found.c_str());

			setup_();
		}

		IconLoader::IconLoader(const QStringList& icon_dirs)
			: invalid_(new QIcon()), icon_dirs_(icon_dirs)
		{
			setup_();
		}

		IconLoader::IconLoader(const std::list<String>& icon_dirs)
			: invalid_(new QIcon())
		{
			for(std::list<String>::const_iterator it = icon_dirs.begin(); it != icon_dirs.end(); ++it) 
			{
				icon_dirs_.append(it->c_str());
			}

			setup_();
		}

		IconLoader::~IconLoader()
		{
			for(HashMap<String, QIcon*>::iterator it = icon_map_.begin(); it != icon_map_.end(); ++it) 
			{
				delete it->second;
			}

			delete invalid_;
		}

		void IconLoader::setup_()
		{
			sizes_.push_back(16);
			sizes_.push_back(22);
			sizes_.push_back(32);
			sizes_.push_back(48);
			sizes_.push_back(64);
			sizes_.push_back(128);
		}

		void IconLoader::appendIconPath(const String& path)
		{
			icon_dirs_.append(path.c_str());
		}

		const QIcon& IconLoader::getIcon(const String& name)
		{
			//See if we have already loaded this icon
			hash_map_lock_.lockForRead();
			HashMap<String, QIcon*>::iterator res = icon_map_.find(name);
			hash_map_lock_.unlock();

			if (res != icon_map_.end()) 
			{
				return *res->second;
			}

			hash_map_lock_.lockForWrite();
			//We cannot be sure that another thread did not yet load the icon.
			//This must be rechecked or memory leaks will occur
			res = icon_map_.find(name);
			if (res != icon_map_.end()) 
			{
				hash_map_lock_.unlock();
				return *res->second;
			}
			QIcon* tmp = loadIcon_(name);
			hash_map_lock_.unlock();

			if (!tmp) 
			{
				Log.error() << "IconLoader: Could not find the icon " << name << std::endl;
				return *invalid_;
			}

			return *tmp;
		}

		IconLoader& IconLoader::instance()
		{
			if (!loader_)
			{
				mutex_.lock();

				// Double check that the instance has not been created while we were aquiring the lock
				// Yes, it has to look like this ;-)
				if (!loader_)
				{
					loader_ = boost::shared_ptr<IconLoader>(new IconLoader());
				}

				mutex_.unlock();
			}

			return *loader_;
		}

		QIcon* IconLoader::loadIcon_(const String& name)
		{
#ifdef BALL_UI_V2
			// === Phase 999.41 — BALLView Refresh: SVG-preferred wedge =========
			// When BALL_UI_V2 is ON, first try a compiled-in SVG from the
			// theme.qrc resource bundle under the `:/icons/<name>.svg` prefix
			// (see source/VIEW/KERNEL/theme/theme.qrc for the BALLView-key
			// aliases that map e.g. "actions/quicksave" → Lucide's save.svg).
			//
			// Per maintainer-Q3 (single neutral theme) we ship the SVG as
			// authored — the Lucide stroke uses currentColor which Qt resolves
			// against the hosting widget's QSS text color at paint time. No
			// custom QIconEngine, no per-DPR cache: Qt 6's built-in
			// QSvgIconEngine handles HiDPI rendering correctly already.
			//
			// If the resource is absent we fall through to the legacy
			// PNG-from-disk walk below, so this branch is purely additive —
			// every icon key that doesn't have an SVG keeps loading its PNG.
			//
			// TODO(v1.8): once maintainer-Q3 is revisited and we need per-
			// mode tinting (--ink-soft / --ink / --accent / --ink-muted),
			// replace this branch with the Handover Phase 2 ThemedIconEngine
			// — see Handover doc § 2.2 for the design.
			{
				const QString svg_path =
					QStringLiteral(":/icons/") + QString(name.c_str()) + ".svg";
				if (QFile::exists(svg_path))
				{
					QIcon* result = new QIcon(svg_path);
					if (!result->isNull())
					{
						icon_map_[name] = result;
						return result;
					}
					delete result;
				}
			}
			// === end Phase 999.41 wedge =======================================
#endif

			const QString filename = QString(name.c_str()) + ".png";

			for (QStringList::iterator it = icon_dirs_.begin(); it != icon_dirs_.end(); ++it)
			{
				QDir base_dir(*it);

				if (!base_dir.exists()) 
				{
					Log.error() << "Could not locate the icon directory: " << it->toStdString() << std::endl;
					return 0;
				}

				QString category_name;
				QIcon* result = 0;

				for(std::list<int>::iterator sit = sizes_.begin(); sit != sizes_.end(); ++sit) 
				{
					QDir size_dir = *it + FileSystem::PATH_SEPARATOR + QString::number(*sit) + "x" + QString::number(*sit);

					if (!size_dir.exists())
				 	{
						continue;
					}

					if (size_dir.exists(filename)) 
					{
						if (!result) 
						{
							result = new QIcon();
						}
						result->addFile(size_dir.path() + FileSystem::PATH_SEPARATOR + filename);
					}
				}

				if (result) 
				{
					icon_map_[name] = result;
					return result;
				}
			}

			return 0;
		}

	}
}

