// $Id: client.C,v 1.5 2000/01/13 22:32:43 oliver Exp $

#include <BALL/VIEW/KERNEL/client.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{


		Client::Client()
			:	host_(),
				port_(VIEW_DEFAULT_PORT)
		{
		}

		Client::Client
			(const Client&  /* client */, bool /* deep */)
			:	host_(),
				port_(VIEW_DEFAULT_PORT)
		{
		}

		Client::Client(const String& host, int port)
		{
			connect(host, port);
		}

		Client::~Client()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Client>() << endl;
			#endif 

			destroy();
		}

		void Client::clear()
		{
		}

		void Client::destroy()
		{
			clear();
		}

		void Client::connect(const String& host, int port)
		{
			host_ = host;
			port_ = port;
		}

		ClientScene Client::getScene()
		{
			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_.c_str(), port_);

			iostream_socket << (int)COMMAND__OPEN_SCENE << endl;

			SceneHandle scene_handle;
			iostream_socket >> scene_handle;

			iostream_socket->close();

			ClientScene scene(host_, port_, scene_handle);

			return scene;
		}

		bool Client::isValid() const
		{
			return (bool)(host_ != "" && port_ != 0);
		}

		void Client::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Client::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Client::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/client.iC>
#		endif 
		
	} // namespace VIEW

} // namespace BALL
