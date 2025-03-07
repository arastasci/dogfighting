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
	virtual void Test() override
	{
		/*auto p = at::Input::GetMousePos();
		AT_INFO("Mouse pos: {0}, {1}",p.first, p.second);*/

		if (Input::GetMouseButtonPress(AT_MOUSE_BUTTON_1))
		{
			AT_INFO("GELMEZSEN GELME");
		}
	}
};


int  main()
{ 
	at::Logger::Init();
	//AT_INFO("Well, we lent some code from {0}...", "Cherno");
	Sandbox* a = new Sandbox();
	a->Init();

	a->Run();
	delete a;
}