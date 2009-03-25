// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/VIEW/KERNEL/common.h>

namespace BALL
{
	namespace VIEW
	{
		ShortcutRegistry::ShortcutRegistry()
			: Embeddable(),
			  shortcuts_()
		{
			registerThis();
		}

		ShortcutRegistry::~ShortcutRegistry()
		{
			unregisterThis();
		}
		
		void ShortcutRegistry::registerShortcut(String description, QAction* shortcut)
		{
			// eat white spaces and tabs in the description
			while (description.substitute(" ", "")  != String::EndPos) {};
			while (description.substitute("\t", "") != String::EndPos) {};
			while (description.substitute("\r", "") != String::EndPos) {};
			while (description.substitute("\n", "") != String::EndPos) {};

			if (!shortcuts_.has(description))
				shortcuts_.insert(description, shortcut);
			else
				Log.warn() << "Double shortcut entry " << description << std::endl;
		}

		void ShortcutRegistry::clear()
		{
			shortcuts_.clear();
		}

		void ShortcutRegistry::clearKeySequences()
		{
			StringHashMap<QAction*>::Iterator it;
			for (it=shortcuts_.begin(); it!=shortcuts_.end(); ++it)
			{
				it->second->setShortcut(QKeySequence());
			}
		}

		bool ShortcutRegistry::readShortcutsFromFile(const String& filename)
		{
			try 
			{
				LineBasedFile infile(filename, std::ios::in);
				infile.enableTrimWhitespaces(true);

				while (infile.readLine())
				{
					std::vector<String> fields;
					infile.getLine().split(fields);

					if (fields.size() != 2)
						continue; // no shortcut given

					if (!shortcuts_.has(fields[0]))
					{
						Log.warn() << "ShortcutRegistry: no action associated with description " << fields[0] << std::endl;
					}
					else
					{
						QAction* action = shortcuts_[fields[0]];
						if (!action->parent())
							Log.error() << "ShortcutRegistry: action associated with description " << fields[0] << " has no parent! Ignoring!" << std::endl;
						else
							action->setShortcut(QKeySequence(action->parent()->tr(fields[1].c_str(), fields[0].c_str())));
					}
				}

				infile.close();
			} 
			catch (...) // todo: explain what went wrong
			{
				return false;
			}

			return true;
		}

		bool ShortcutRegistry::writeShortcutsToFile(const String& filename)
		{
			try 
			{
				File outfile(filename, std::ios::out);

				StringHashMap<QAction*>::ConstIterator it;
				for (it=shortcuts_.begin(); it!=shortcuts_.end(); ++it)
				{
					outfile << it->first << "\t" << ascii(it->second->shortcut().toString()) << std::endl;
				}

				outfile.close();
			} 
			catch (...) // todo: explain what went wrong
			{
				return false;
			}

			return true;
		}
	} // namespace VIEW
} // namespace BALL
