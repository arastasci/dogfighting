 #include <at.h>

class Sandbox : public at::Application
{
public:
	Sandbox()
	{

	}
	
	~Sandbox()
	{

	}
};


int  main()
{ 
	at::Logger::Init();
	AT_INFO("Well, we lent some code from {0}...", "Cherno");
	Sandbox* game = new Sandbox();
	game->Run();
	delete game;
}