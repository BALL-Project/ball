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
			  shortcuts_(),
				shortcut_keys_()
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

			String key_seq = ascii(shortcut->shortcut().toString());

			if ( !(hasDescription(description) || shortcut_keys_.has(key_seq)))
			{
				shortcuts_[description] = shortcut;
				if (key_seq != "")
				{
					shortcut_keys_.insert(key_seq);
				}

				emit shortcutChanged();
			}
			else
				Log.warn() << "Double shortcut entry " << description << std::endl;
		}

		void ShortcutRegistry::clear()
		{
			shortcuts_.clear();
			shortcut_keys_.clear();

			emit shortcutChanged();
		}

		void ShortcutRegistry::clearKeySequences()
		{
			std::map<String, QAction*>::iterator it;
			for (it = shortcuts_.begin(); it != shortcuts_.end(); ++it)
			{
				it->second->setShortcut(QKeySequence());
			}
			shortcut_keys_.clear();

			emit shortcutChanged();
		}

		bool ShortcutRegistry::readShortcutsFromFile(const String& filename)
		{
			LineBasedFile infile;
			try
			{
				infile.open(filename, std::ios::in);
			}
			catch(Exception::FileNotFound&)
			{
				Log.error() << "ShortcutRegistry: Could not open file " << filename << "." << std::endl;
				return false;
			}
			infile.enableTrimWhitespaces(true);

			while (infile.readLine())
			{
				std::vector<String> fields;
				infile.getLine().split(fields);

				if (fields.size() != 2)
					continue; // no shortcut given

				if (!hasDescription(fields[0]))
				{
					Log.warn() << "ShortcutRegistry: no action associated with description " << fields[0] << std::endl;
				}
				else // we have this action
				{
					QAction* action = shortcuts_[fields[0]];
					if (!action->parent())
					{
						Log.error() << "ShortcutRegistry: action associated with description " << fields[0] << " has no parent! Ignoring!" << std::endl;
					}
					else
					{
						if (!shortcut_keys_.has(fields[1]))
						{
							action->setShortcut(QKeySequence(action->parent()->tr(fields[1].c_str(), fields[0].c_str())));
							shortcut_keys_.insert(fields[1]);
						}
						else
						{
							Log.warn() << "ShortcutRegistry: duplicate shortcut " << fields[1] << ". Ignoring!" << std::endl;
						}
					}
				}
			}

			infile.close();

			emit shortcutChanged();

			return true;
		}

		bool ShortcutRegistry::writeShortcutsToFile(const String& filename)
		{
			File outfile;

			try
			{
				outfile.open(filename, std::ios::out);
			}
			catch (Exception::FileNotFound& e) // todo: explain what went wrong
			{
				Log.error() << "ShortcutRegistry: " << e.what() << std::endl;
				return false;
			}

			std::map<String,QAction*>::const_iterator it;
			for (it=shortcuts_.begin(); it!=shortcuts_.end(); ++it)
			{
				outfile << it->first << "\t" << ascii(it->second->shortcut().toString()) << std::endl;
			}

			outfile.close();

			return true;
		}

		size_t ShortcutRegistry::size()
		{
			return shortcuts_.size();
		}

		bool ShortcutRegistry::hasDescription(const String& description)
		{
			return (shortcuts_.find(description) != shortcuts_.end());
		}

		bool ShortcutRegistry::hasKey(const QString& key_seq)
		{
			return shortcut_keys_.has(ascii(key_seq));
		}
	
		bool ShortcutRegistry::hasKey(const  QKeySequence& key_seq)
		{
			return hasKey(key_seq.toString());
		}

		bool ShortcutRegistry::hasKey(const String& key_seq)
		{
			return shortcut_keys_.has(key_seq);
		}

		std::pair<String, QAction*> ShortcutRegistry::operator[](Index i)
		{
			return getEntry_(i);
		}

		std::pair<String, QAction*> ShortcutRegistry::getEntry_(Index pos)
		{
			if ((size_t)pos >= shortcuts_.size()) {
				throw Exception::OutOfRange(__FILE__, __LINE__);
			}

			std::map<String, QAction*>::iterator it = shortcuts_.begin();
			for(int i = 0; i < pos; ++i, ++it) ;

			return *it;
		}
	} // namespace VIEW
} // namespace BALL
