// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularMessage.h,v 1.14 2003/03/26 13:56:55 anhi Exp $

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
#define BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
#	include <BALL/VIEW/KERNEL/message.h>
#endif

using BALL::VIEW::CompositeMessage;
using BALL::VIEW::RemovedCompositeMessage;
using BALL::VIEW::ChangedCompositeMessage;
using BALL::VIEW::SelectionMessage;

namespace BALL
{
	namespace MOLVIEW
	{
		/** NewMolecularMessage class.
				The class NewMolecularMessage is responsible for making known a new molecular
				structure in the  \link ConnectionObject ConnectionObject \endlink  tree. This message will be converted
				from the message  \link NewCompositeMessage NewCompositeMessage \endlink  if the  \link Composite Composite \endlink  object contains
				a molecular structure. The conversion will be done by the widget class
				 \link MolecularProperties MolecularProperties \endlink .
				There are widgets that will be react only to the class newMolecularMessage but
				not to the message class  \link NewCompositeMessage NewCompositeMessage \endlink  such as  \link DisplayProperties DisplayProperties \endlink .
				Because the RTTI mechanism of BALL is used to identify the different messages
				this class is not derived from  \link NewCompositeMessage NewCompositeMessage \endlink . Otherwise every method
				accepting  \link NewCompositeMessage NewCompositeMessage \endlink  object would also accept 
				 \link NewMolecularMessage NewMolecularMessage \endlink  objects and that is not allowed.  \par
				
    		\ingroup  MolviewKernel
		*/
		class NewMolecularMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new newMolecularMessage.
					@return      NewMolecularMessage new constructed newMolecularMessage
					@see         CompositeMessage
			*/
			NewMolecularMessage()
				throw();

			/** Copy constructor.
					Construct new newMolecularMessage by copying the newMolecularMessage {\em message}.
					The state of {\em *this} newMolecularMessage is set to the set of {\em message} 
					newMolecularMessage.
					@param       message the newMolecularMessage to be copied
					@return      NewMolecularMessage new constructed newMolecularMessage copied from {\em message}
					@see         CompositeMessage
			*/
			NewMolecularMessage(const CompositeMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} newMolecularMessage.
			*/
			virtual ~NewMolecularMessage()
				throw();
			//@}
		
			private:
		};


		/** RemovedMolecularMessage class.
				The class RemovedCompositeMessage is the message class that is responsible for
				marking a  \link Composite Composite \endlink  object as removed inside the  \link ConnectionObject ConnectionObject \endlink  tree.
				It will be sent by existing  \link ConnectionObject ConnectionObject \endlink  objects that remove (or would like to
				remove)  \link Composites Composites \endlink  objects. One can use the  \link RTTI RTTI \endlink  mechanism to identify
				such messages in the {\em onNotify} method of user constructed  \link ConnectionObject ConnectionObject \endlink 
				objects.
				This class is only provided for convinience. So in MOLVIEW applications one can
				use this class in the same way as the class  \link RemovedCompositeMessage RemovedCompositeMessage \endlink .  \par
				
		*/
		class RemovedMolecularMessage: public RemovedCompositeMessage
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new removedMolecularMessage.
					@return      RemovedMolecularMessage new constructed removedMolecularMessage
					@see         RemovedCompositeMessage
			*/
			RemovedMolecularMessage()
				throw();

			/** Copy constructor.
					Construct new removedMolecularMessage by copying the removedMolecularMessage {\em message}.
					The state of {\em *this} removedMolecularMessage is set to the set of {\em message} 
					removedMolecularMessage.
					@param       message the removedMolecularMessage to be copied
					@return      RemovedMolecularMessage new constructed removedMolecularMessage copied from {\em message}
					@see         CompositeMessage
					@see         RemovedCompositeMessage
			*/
			RemovedMolecularMessage(const CompositeMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} removedMolecularMessage.
			*/
			virtual ~RemovedMolecularMessage()
				throw();
			//@}
		
			private:
		};



		/** ChangedMolecularMessage class.
				The class ChangedCompositeMessage is the message class that is responsible for
				marking a  \link Composite Composite \endlink  object as changed inside the  \link ConnectionObject ConnectionObject \endlink  tree.
				It will be sent by existing  \link ConnectionObject ConnectionObject \endlink  objects that change (or would like to
				change)  \link Composites Composites \endlink  objects. One can use the  \link RTTI RTTI \endlink  mechanism to identify
				such messages in the {\em onNotify} method of user constructed  \link ConnectionObject ConnectionObject \endlink 
				objects.
				This class is only provided for convinience. So in MOLVIEW applications one can
				use this class in the same way as the class  \link ChangedCompositeMessage ChangedCompositeMessage \endlink .  \par
				
		*/
		class ChangedMolecularMessage: public ChangedCompositeMessage
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new changedMolecularMessage.
					@return      ChangedMolecularMessage new constructed changedMolecularMessage
					@see         ChangedCompositeMessage
			*/
			ChangedMolecularMessage()
				throw();

			/** Copy constructor.
					Construct new changedMolecularMessage by copying the changedMolecularMessage {\em message}.
					The state of {\em *this} changedMolecularMessage is set to the set of {\em message} 
					changedMolecularMessage.
					@param       message the changedMolecularMessage to be copied
					@return      ChangedMolecularMessage new constructed changedMolecularMessage copied from {\em message}
					@see         CompositeMessage
					@see         ChangedCompositeMessage
			*/
			ChangedMolecularMessage(const CompositeMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} changedMolecularMessage.
			*/
			virtual ~ChangedMolecularMessage()
				throw();
			//@}
		
			private:
		};


		/** MolecularSelectionMessage class.
				The class MolecularSelectionMessage is the message class that is a container for
				 \link Composite Composite \endlink  objects that are somehow selected.
				It will be sent by existing  \link ConnectionObject ConnectionObject \endlink  objects that collect
				 \link Composites Composites \endlink  objects for a certain purpose.
				One can use the  \link RTTI RTTI \endlink  mechanism to identify
				such messages in the {\em onNotify} method of user constructed  \link ConnectionObject ConnectionObject \endlink 
				objects.
				This class is only provided for convinience. So in MOLVIEW applications one can
				use this class in the same way as the class  \link SelectionMessage SelectionMessage \endlink .  \par
				
		*/
		class MolecularSelectionMessage: public SelectionMessage
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new molecularSelectionMessage.
					@return      MolecularSelectionMessage new constructed molecularSelectionMessage
					@see         SelectionMessage
			*/
			MolecularSelectionMessage()
				throw();

			/** Copy constructor.
					Construct new molecularSelectionMessage by copying the molecularSelectionMessage
					{\em message}.
					The state of {\em *this} molecularSelectionMessage is set to the set of 
					{\em message} molecularSelectionMessage.
					@param       message the molecularSelectionMessage to be copied
					@return      MolecularSelectionMessage new constructed molecularSelectionMessage copied from {\em message}
					@see         SelectionMessage
			*/
			MolecularSelectionMessage(const SelectionMessage& message)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} molecularSelectionMessage.
			*/
			virtual ~MolecularSelectionMessage()
				throw();
			//@}
		
			private:
		};

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
