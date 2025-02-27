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
	Sandbox* game = new Sandbox();
	game->Run();
	delete game;
}