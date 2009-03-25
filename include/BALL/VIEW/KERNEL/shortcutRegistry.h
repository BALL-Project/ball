// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_SHORTCUTREGISTRY_H
#define BALL_VIEW_KERNEL_SHORTCUTREGISTRY_H

#ifndef BALL_CONCEPT_EMBEDDABLE_H
#	include <BALL/CONCEPT/embeddable.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
# include <BALL/DATATYPE/stringHashMap.h>
#endif

#include <QtGui/QAction>

namespace BALL
{
	namespace VIEW
	{	

		/** ShortcutRegistry 
		*/
		class BALL_VIEW_EXPORT ShortcutRegistry
			: public Embeddable
		{
			public:
				BALL_EMBEDDABLE(ShortcutRegistry, Embeddable)
				BALL_CREATE(ShortcutRegistry)
				
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

			protected:
				StringHashMap<QAction*> shortcuts_;
		};
		
	}// namespace VIEW
}// namespace BALL



#endif // BALL_VIEW_KERNEL_SHORTCUTREGISTRY_H
