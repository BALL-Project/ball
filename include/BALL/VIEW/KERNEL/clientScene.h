// $Id: clientScene.h,v 1.5 2000/01/13 22:23:16 oliver Exp $

#ifndef BALL_VIEW_KERNEL_CLIENTSCENE_H
#define BALL_VIEW_KERNEL_CLIENTSCENE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H       
# include <BALL/CONCEPT/persistentObject.h>
#endif

#ifndef BALL_CONCET_TEXTPERSISTENCEMANAGER_H
# include <BALL/CONCEPT/textPersistenceManager.h>
#endif

#ifndef BALL_SYSTEM_SOCKET_H
#	include <BALL/SYSTEM/socket.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
#	include <BALL/VIEW/COMMON/global.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/**	
		*/
		class ClientScene
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			/** Default constructor.
					Construct a new clientscene.

					@return      {\bf ClientScene} - new constructed clientscene
			*/
			ClientScene();

			ClientScene(const String& host, int port, SceneHandle scenehandle);

			/** Copy constructor.
					Construct new clientscene by copying the clientscene {\em atom}.
					The copy is either deep or shallow (default).

					@return      {\bf ClientScene} - new constructed clientscene
			*/
			ClientScene(const ClientScene& clientScene, bool deep = true);

			virtual ~ClientScene();

			virtual void clear();

			virtual void destroy();
			//@}
		
			/**	@name	Exceptions
			*/
			//@{
			
			class InvalidClientScene
				:	public Exception::GeneralException
			{
				public:

				InvalidClientScene(const char* file, int line);
			};

			class NoPersistentObject
				:	public Exception::GeneralException
			{
				public:

				NoPersistentObject(const char* file, int line);
			};
			//@}

			/**	@name	Accessors
			*/
			//@{

			void show();

			void hide();

			void resize(int width, int height);

			void insert(Composite &composite);

			bool update(bool rebuild_displaylists = false);

			void setCreatorValue(int address, int value);

			int getCreatorValue(int address);

			bool hasCreatorValue(int address, int value);

			void setProcessorValue(int address, int value);

			int getProcessorValue(int address);

			bool hasProcessorValue(int address, int value);

			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;

			//@}
			
			private:

			String	host_;
			int			port_;
			SceneHandle scenehandle_;
			TextPersistenceManager pm_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/clientScene.iC>
#		endif
		
	} // namespace VIEW
	
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CLIENTSCENE_H
