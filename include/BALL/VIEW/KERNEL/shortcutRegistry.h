// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_SHORTCUTREGISTRY_H
#define BALL_VIEW_KERNEL_SHORTCUTREGISTRY_H

#ifndef BALL_CONCEPT_EMBEDDABLE_H
# include <BALL/CONCEPT/embeddable.h>
#endif

#include <map>

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY_H
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include <QtCore/QObject>

class QAction;
class QKeySequence;

namespace BALL
{
	namespace VIEW
	{
		/** ShortcutRegistry.
		 *  It takes care of handling all shortcuts.
		 *  It is responsible for registering, changing single shortcuts
		 *  or loading entire shortcut-configuration files.
		*/
		class BALL_VIEW_EXPORT ShortcutRegistry
			: public QObject,
			  public Embeddable,
			  public PreferencesEntry::ExtendedPreferencesObject
		{
			Q_OBJECT

			public:
				BALL_EMBEDDABLE(ShortcutRegistry, Embeddable)

				///
				ShortcutRegistry();

				/** Destructor
				*/
				virtual ~ShortcutRegistry();

				///
				void registerShortcut(String description, QAction* shortcut);

				///
				void clear();

				/// Clear all key sequences for all registered QActions.
				void clearKeySequences();

				/// Read the shortcuts from a data file
				bool readShortcutsFromFile(const String& filename);

				/// Write the shortcuts to a data file.
				bool writeShortcutsToFile(const String& filename);

				/// 
				bool changeShortcut(QAction* shortcut, const String& new_sequence);
				bool changeShortcut(int index, const String& new_sequence);

				size_t size();

				bool hasDescription(const String& description);
				bool hasKey(const QString& key_seq);
				bool hasKey(const QKeySequence& key_seq);
				bool hasKey(const String& key_seq);

				std::pair<String, QAction*> operator[](Index i);
				
				// needed for storing this classes' preferences
				virtual bool getValue(String&) const;
				virtual bool setValue(const String&);

			signals:
				void shortcutChanged();

			protected:
				static const char* BETWEEN_SC_SEPERATOR;
				static const char* IN_SC_SEPERATOR;

				std::pair<String, QAction*> getEntry_(Index pos);

				std::map<String, QAction*> shortcuts_;
				std::map<String, String> unknown_shortcuts_;
				HashSet<String> shortcut_keys_;
		};

	}// namespace VIEW
}// namespace BALL

#endif // BALL_VIEW_KERNEL_SHORTCUTREGISTRY_H

