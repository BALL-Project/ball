// $Id: client.C,v 1.1 1999/08/26 08:02:46 oliver Exp $

#include <BALL/VIEW/KERNEL/client.h>


namespace BALL
{

	namespace VIEW
	{


		Client::Client()
			:
			host_(0),
			port_(0)
		{
		}

		Client::Client
			(const Client&  /* client */, bool /* deep */)
			:
			host_(0),
			port_(0)
		{
		}

		Client::~Client()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI<Client>::getName() << endl;
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

		void Client::connect(const char* host, int port)
		{
			host_ = (char *)host;
			port_ = port;

			cout << endl;
			cout << "------------------------------------------------" << endl;
			cout << "Welcome to Client VIEW:" << endl;
			cout << "------------------------------------------------" << endl;
			cout << endl;
		}

		ClientScene Client::getNewScene()
		{
			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_, port_);

			iostream_socket << COMMAND__OPEN_SCENE << endl;

			SceneHandle scene_handle;
			iostream_socket >> scene_handle;

			iostream_socket->close();

			ClientScene scene(host_, port_, scene_handle);

			return scene;
		}

		bool Client::isValid() const
		{
			return (bool)(host_ != 0
										&& port_ != 0);
		}

		void Client::dump
			(ostream& s, unsigned long depth) const
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
