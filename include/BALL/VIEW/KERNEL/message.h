// $Id: message.h,v 1.1 2000/03/21 16:23:27 hekl Exp $

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
#define BALL_VIEW_KERNEL_MESSAGE_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_AUTODELETABLE_H
#	include <BALL/CONCEPT/autoDeletable.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif


namespace BALL
{

	namespace VIEW
	{
		// class forward
		class ConnectionObject;

		/**	General Message Class
				
		*/
		class Message
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			Message();

			Message(const Message& message, bool deep = true);

			virtual ~Message();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			void setSender(const void* object);

			const void* getSender() const;

			void setDeletable(bool flag = true);

		  bool isDeletable() const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:

			void *connection_object_;

			bool deletable_;
		};

		
		/**	Composite Message Class		
		*/
		class CompositeMessage: public Message
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			CompositeMessage();

			CompositeMessage(const CompositeMessage& message, bool deep = true);

			virtual ~CompositeMessage();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			void setComposite(const Composite& composite);

			void setComposite(const Composite* composite);

			Composite* getComposite() const;

			void setCompositeName(string name);

			string getCompositeName() const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:

			Composite *composite_;

			string composite_name_;
		};

		/**	New Composite Message Class		
		*/
		class NewCompositeMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			NewCompositeMessage();

			NewCompositeMessage(const CompositeMessage& message, bool deep = true);

			virtual ~NewCompositeMessage();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:
		};


		/**	Removed Composite Message Class		
		*/
		class RemovedCompositeMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			RemovedCompositeMessage();

			RemovedCompositeMessage(const CompositeMessage& message, bool deep = true);

			virtual ~RemovedCompositeMessage();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:
		};


		/**	Changed Composite Message Class		
		*/
		class ChangedCompositeMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			ChangedCompositeMessage();

			ChangedCompositeMessage(const CompositeMessage& message, bool deep = true);

			virtual ~ChangedCompositeMessage();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:
		};


		/**	Display Changed Message Class		
		*/
		class DisplayMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			DisplayMessage();

			DisplayMessage(const DisplayMessage& message, bool deep = true);

			virtual ~DisplayMessage();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:
		};


		/**	Scene Message Class		
		*/
		class SceneMessage: public Message
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			SceneMessage();

			SceneMessage(const SceneMessage& message, bool deep = true);

			virtual ~SceneMessage();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			void updateOnly(bool value = true);

			bool isUpdateOnly();

			void setCameraLookAt(Vector3 vector3);

			Vector3& getCameraLookAt();

			const Vector3& getCameraLookAt() const;

			void setCameraViewPoint(Vector3 vector3);

			Vector3& getCameraViewPoint();

			const Vector3& getCameraViewPoint() const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:

			bool update_only_;

			Vector3 camera_look_at_;
			Vector3 camera_view_point_;
		};

		/**	Window Message Class		
		*/
		class WindowMessage: public Message
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			WindowMessage();

			WindowMessage(const WindowMessage& message, bool deep = true);

			virtual ~WindowMessage();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			void setStatusBar(const string &message);
			
			string getStatusBar() const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:

			string status_bar_;
		};
		
		/**	Selection Message Class		
		*/
		class SelectionMessage: public Message
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			SelectionMessage();

			SelectionMessage(const SelectionMessage& message, bool deep = true);

			virtual ~SelectionMessage();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			void setSelection(const List<Composite*> &selection);

			void setSelection(const List<Composite*> *selection);

			const List<Composite*>* getSelection() const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:

			List<Composite*>* selection_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/message.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_MESSAGE_H
