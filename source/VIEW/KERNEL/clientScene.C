// $Id: clientScene.C,v 1.3 1999/12/19 17:15:40 oliver Exp $

#include <BALL/VIEW/KERNEL/clientScene.h>


namespace BALL
{

	namespace VIEW
	{


		ClientScene::ClientScene()
			:
			host_(0),
			port_(0),
			scenehandle_(0),
			pm_()
		{
		}

		ClientScene::ClientScene(char *host, int port, SceneHandle scenehandle)
			:
			host_(host),
			port_(port),
			scenehandle_(scenehandle),
			pm_()
		{
		}

		ClientScene::ClientScene
			(const ClientScene&  clientScene , bool /* deep */)
				:
			host_(clientScene.host_),
			port_(clientScene.port_),
			scenehandle_(clientScene.scenehandle_),
			pm_()
		{
		}

		ClientScene::~ClientScene()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI<ClientScene>::getName() << endl;
			#endif 

			destroy();
		}

		void ClientScene::clear()
		{
			/*
			host_ = 0;
			port_ = 0;
			scenehandle_ = 0;
			*/
		}

		void ClientScene::destroy()
		{
			/*
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__DESTROY_SCENE << " ";
			iostream_socket << scenehandle_ << endl;

			iostream_socket->close();

			clear();
			*/
		}

		void ClientScene::show()
		{
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__SHOW_SCENE << " ";
			iostream_socket << scenehandle_ << endl;

			iostream_socket->close();
    }

		void ClientScene::hide()
		{
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__HIDE_SCENE << " ";
			iostream_socket << scenehandle_ << endl;
		
			iostream_socket->close();
    }

		void ClientScene::resize(int width, int height)
		{
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__RESIZE_SCENE << " ";
			iostream_socket << scenehandle_ << " ";
			iostream_socket << width << " ";
			iostream_socket << height << endl;
		
			iostream_socket->close();
    }

	  void ClientScene::insert(Composite &composite)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}	
				
			if (!RTTI<PersistentObject *>::isKindOf(&composite))
			{
				throw NoPersistentObject(__FILE__, __LINE__);
			}

			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__SEND_OBJECT << " ";
			iostream_socket << scenehandle_ << " ";
			iostream_socket << (unsigned long)(&composite) << endl;

			pm_.setOstream(iostream_socket);
			composite >> pm_;

			iostream_socket->close();
    }

		bool ClientScene::update(bool rebuild_displaylists)
		{
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__UPDATE_SCENE << " ";
			iostream_socket << scenehandle_ << " ";
			iostream_socket << rebuild_displaylists << endl;

			bool finished = false;

			iostream_socket >> finished;

			iostream_socket->close();

			return finished;
    }

		void ClientScene::setCreatorValue(int address, int value)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__SET_CREATOR_VALUE << " ";
			iostream_socket << address << " ";
			iostream_socket << value << endl;
		
			iostream_socket->close();
		}

		int ClientScene::getCreatorValue(int address)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__GET_CREATOR_VALUE << " ";
			iostream_socket << address << endl;

			int value = -1;

			iostream_socket >> value;

			iostream_socket->close();

			return value;
		}

		bool ClientScene::hasCreatorValue(int address, int value)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__HAS_CREATOR_VALUE << " ";
			iostream_socket << address << " ";
			iostream_socket << value << endl;

			bool has_value = false;

			iostream_socket >> has_value;

			iostream_socket->close();

			return has_value;
		}

		void ClientScene::setProcessorValue(int address, int value)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__SET_PROCESSOR_VALUE << " ";
			iostream_socket << address << " ";
			iostream_socket << value << endl;
		
			iostream_socket->close();
		}

		int ClientScene::getProcessorValue(int address)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__GET_PROCESSOR_VALUE << " ";
			iostream_socket << address << endl;

			int value = -1;

			iostream_socket >> value;

			iostream_socket->close();

			return value;
		}

		bool ClientScene::hasProcessorValue(int address, int value)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << (int)COMMAND__HAS_PROCESSOR_VALUE << " ";
			iostream_socket << address << " ";
			iostream_socket << value << endl;

			bool has_value = false;

			iostream_socket >> has_value;

			iostream_socket->close();

			return has_value;
		}

		bool ClientScene::isValid() const
		{
			return (bool)(host_ != 0
										&& port_ != 0
										&& scenehandle_ != 0);
		}

		void ClientScene::dump
			(ostream& s, unsigned long depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ClientScene::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void ClientScene::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/clientScene.iC>
#		endif 
		
	} // namespace VIEW

} // namespace BALL
