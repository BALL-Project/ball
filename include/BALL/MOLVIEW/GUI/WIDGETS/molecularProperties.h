// $Id: molecularProperties.h,v 1.1 2000/09/23 15:38:17 hekl Exp $

#ifndef BALL_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H
#define BALL_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <iostream>

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
#	include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#	include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_LOGVIEW_H
#	include <BALL/VIEW/KERNEL/logView.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
#	include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H
#	include <BALL/MOLVIEW/FUNCTOR/molecularInformation.h>
#endif


using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


namespace BALL
{

	namespace MOLVIEW
	{

		/**
		 */
		class MolecularProperties
			: public ConnectionObject
		{
		public:
			
			/**	@name	Enums
			 */
			//@{
			//@}
			
			/**	@name	Type Definitions
			 */
			
			/**	@name	Constructors and Destructors
			 */
			//@{
			
			MolecularProperties();
			
			virtual ~MolecularProperties();
			//@}
			
			/**	@name Exceptions
			 */
			//@{
			class FragmentDBMissing
				: public Exception::GeneralException
			{
			  public:
				  FragmentDBMissing(const char* file, int line, const string& data);
			};
			//@}
			
			/**	@name	Accessors
			 */
			//@{
			void registerFragmentDB(const FragmentDB &fragment_db);
			
			void onNotify(Message *message);
			//@}
			
			
		private:
			
			FragmentDB* fragment_db_;  			
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/WIDGETS/molecularProperties.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIE_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H_
