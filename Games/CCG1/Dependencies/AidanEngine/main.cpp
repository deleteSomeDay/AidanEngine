#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/maths/maths.h"
#include "src/graphics/Window.h"
#include "src\graphics\Shader.h"
#include "src\graphics\BatchRenderer2D.h"
#include "src/graphics/Renderable2D.h"
#include "src\graphics\Static_Sprite.h"
#include "src\graphics\Sprite.h"
#include "src/graphics/layers/TileLayer.h"
#include "src/graphics/layers/Layer.h" 
#include "src/graphics/Label.h"
#include "src/graphics/layers/Group.h"
#include "src/graphics/Texture.h"
#include <aws/core/Aws.h>
#include <aws/gamelift/GameLiftClient.h>
#include <iostream>
#include <thread>
#include <vector>
#include "TCPClient.h"
#include "INIReader.h"
#include "Stdafx.h"
#include <map>

int main(void)
{

	using namespace engine;
	using namespace maths;
	using namespace graphics;

	int x = 1920;
	int y = 1080;

	getDesktopResolution( x, y);

	Window window("New Game", x, y);

	/*
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "WSA broke" << std::endl;
		return -1;
	}
	
	INIReader iniReader("config.ini");
	if (iniReader.ParseError() < 0)
	{
		std::cout << "config.ini not found\n";
	}

	const std::string& dns = iniReader.Get("config", "DNS_ADDR", "127.0.0.1");
	int port = iniReader.GetInteger("config", "PORT", 5999); 
	std::string playerName = iniReader.Get("config", "PLAYER_NAME", "DefaultName");
	std::string playerPass = iniReader.Get("config", "PLAYER_PASSWD", "DefaultPass");
	
	TCPClient client = TCPClient();

	if (client.connect(dns, port) == false)
	{
		std::cout << "Couldnt connect" << std::endl;
	}
	else
	{
		std::cout << "connected" << std::endl;
	}
	*/
	if (glewInit())
	{
		std::cout << "error: GLEWINIT" << std::endl;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Shader* shader = new Shader("shaders/basic.vert", "shaders/basic.frag");
	shader->Bind();
	
	BatchRenderer2D rendererB;

	
	Texture* textures[] =
	{
		new Texture("res/textures/board.png"),
		new Texture("res/textures/Grunt2.0.png"),
		new Texture("res/textures/gold.png")
	};
	
	int texIDs[] =
	{
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	};
	
	mat4 ortho = mat4::orthographic(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);

	shader->Bind();
	shader->SetUniform1iv("textures", 32, texIDs);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int health = 10; 
	int damage = 1;

	int gold1 = 100;
	int gold2 = 100;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	TileLayer layer(shader);
	TileLayer Startlayer(shader);


	Group* group = new Group(mat4::translation(vec3(0, 0, 0)));
	group->add(new Sprite(0, 0, 1920, 1080, textures[0]));
	
	std::vector<Group *> p1Cards;
	std::vector<Group *> p2Cards;

	 
	 p1Cards.push_back(new  Group(mat4::translation(vec3(50, 100, 0))));
	 p1Cards[0]->add(new Sprite(0, 0, 144, 256, textures[1]));
	 p1Cards[0]->add(new Label(std::to_string(health), 10, 10, vec4(1, 1, 1, 1), 0.6f));
	 p1Cards[0]->add(new Label(std::to_string(damage), 114, 10, vec4(1, 1, 1, 0.1), 0.6f));

	Group* groupGUI1 = new Group(mat4::translation(vec3(450, 10, 0)));
	groupGUI1->add(new Sprite(0, 0, 40, 40, textures[2]));
	groupGUI1->add(new Label(std::to_string(gold2), 50, 15, maths::vec4(1.0f, 232.0f / 255.0f, 1.0f, 0.0f), 0.5f));

	Group* groupGUI2 = new Group(mat4::translation(vec3(1100, 1020, 0)));
	groupGUI2->add(new Sprite(0, 0, 40, 40, textures[2]));
	groupGUI2->add(new Label(std::to_string(gold1), 50, 15, maths::vec4(1.0f, 232.0f / 255.0f, 1.0f, 0.0f), 0.5f));

	Group* groupButton = new Group(mat4::translation(vec3(1700, 520, 0)));
	groupButton->add(new Sprite(0.0f, 0.0f, 200.0f, 40.0f, (maths::vec4&)maths::vec4(0,1,1,1)));
	groupButton->add(new Label("End Turn", 70, 10, maths::vec4(0.5f, 0.5f, 1.0f, 1.0f), 0.5f));
	//Adding all the Textures
	
	layer.add(group);
	layer.add(groupGUI2);
	layer.add(groupGUI1);
	layer.add(groupButton);
	layer.add(new Label("Player 1", 1040, 230, maths::vec4(1.0f,0.7f, 1.0f, 1.0f), 0.5f));
	layer.add(new Label("Player 2", 800, 835, maths::vec4(1.0f, 0.0f, 1.0f, 0.2f), 0.5f));
	
	for (std::vector<Group *>::iterator it = p1Cards.begin(); it != p1Cards.end(); ++it) 
	{
		layer.add(*it);
	}
	for (std::vector<Group *>::iterator it = p2Cards.begin(); it != p2Cards.end(); ++it)
	{
		layer.add(*it);
	}

	maths::vec2 mouse(0,0);
	while (!window.closed())
	{
		
		window.Clear();


		if (window.isMouseButtonPressed(0) == true)
		{
			window.getMousePosition(mouse);
			if (mouse.x >= groupButton->getMatPos().x &&  y - mouse.y >= groupButton->getMatPos().y && y - mouse.y <= (groupButton->getMatPos().y + groupButton->getSize().y) && mouse.x <= groupButton->getMatPos().x + groupButton->getSize().x)
			{
				//client.requestEndTurn();
#ifdef __unix__
				usleep(100); 
#else
				Sleep(100);
#endif
			}
			
			if (mouse.x >= p1Cards[0]->getMatPos().x &&  y - mouse.y >= p1Cards[0]->getMatPos().y && y - mouse.y <= p1Cards[0]->getMatPos().y + p1Cards[0]->getSize().y && mouse.x <= p1Cards[0]->getMatPos().x + p1Cards[0]->getSize().x)
			{
				p1Cards[0]->setMatrix(mat4::translation(vec3(mouse.x - 50, y - mouse.y - 50, 0)));
				p1Cards[0]->setSize(maths::vec2(p1Cards[0]->getSize().x * 0.5f, p1Cards[0]->getSize().y * 0.5f));
			}
		}

		layer.render();

		window.update();
		
	}
	
	for (int i = 0; i < 3; i++)
	{
		delete textures[i];
	}
	
	//client.disconnect();
	//WSACleanup();

	return 0;


}