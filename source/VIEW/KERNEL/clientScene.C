// $Id: clientScene.C,v 1.1 1999/08/26 08:02:46 oliver Exp $

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

			iostream_socket << COMMAND__DESTROY_SCENE << " ";
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

			iostream_socket << COMMAND__SHOW_SCENE << " ";
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

			iostream_socket << COMMAND__HIDE_SCENE << " ";
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

			iostream_socket << COMMAND__RESIZE_SCENE << " ";
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

			iostream_socket << COMMAND__SEND_OBJECT << endl;
			iostream_socket << scenehandle_ << endl;
			iostream_socket << (unsigned long)(&composite) << endl;

			pm_.setOstream(iostream_socket);
			composite >> pm_;

			iostream_socket->close();
    }

		void ClientScene::update()
		{
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClientScene(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << COMMAND__UPDATE_SCENE << " ";
			iostream_socket << scenehandle_ << endl;
		
			iostream_socket->close();
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
