#include "Window.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "Graphics.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Clock.h"
#include "Texture.h"
#include "Sampler.h"
#include "Primitive.h"
#include "LightManager.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Blender.h"

#include "RenderCamera.h"
#include "Player.h"
//#include "Debug.h"
#include <fstream>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     CmdLine,
	int       ShowCmdNum
)
{ 
	cl::Window::initWindowClass();

	cl::Window wnd(200, 200, 1000, 600, WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU, SW_SHOW); //sizeof(wnd) 48b
	//cl::Window wnd(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), WS_POPUP, SW_SHOWMAXIMIZED);
	wnd.setTitle("Myencraft");
	wnd.gfx().setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	int seed = 1337;
	{
		std::ifstream seedFile("seed.txt");
		if (seedFile)
		{
			seedFile >> seed;
			seedFile.close();
		}
	}

	cl::Blender blender(wnd.gfx());
	blender.bind(wnd.gfx());

	cl::Image tilemap(L"tiles.png");
	cl::Texture tilemapTexture(wnd.gfx(), tilemap, 0u);
	tilemapTexture.bind(wnd.gfx());
	cl::Sampler sampler(wnd.gfx());
	sampler.bind(wnd.gfx());
	cl::VertexShader terrainVS(wnd.gfx(), L"TerrainVS.cso");
	cl::VertexShader plantsVS(wnd.gfx(), L"PlantsVS.cso");
	cl::VertexShader waterVS(wnd.gfx(), L"WaterVS.cso");
	cl::PixelShader pixelShader(wnd.gfx(), L"PhongPS.cso");
	pixelShader.bind(wnd.gfx());

	mc::Player player({0.0f, 120.0f, 0.0f});
	mc::RenderCamera camera(wnd.gfx(), wnd.getSize(), &terrainVS, &plantsVS, &waterVS, &player.getPosition());

	mc::World world(wnd.gfx(), seed, player.getPosition());
	cl::Vector2i centerWnd = wnd.getSize() / 2u;
	cl::Mouse::setMousePos(centerWnd, wnd);

	cl::LightManager lm(wnd.gfx());
	lm.setDirectionalLight(cl::normalize<float>({ -0.5f, -1.0f, 0.8f }), { 1.0f, 1.0f, 1.0f });
	lm.updateDirectionalLightCBuffer(wnd.gfx());

	Sleep(1000);

	cl::Clock timer;
	cl::Clock clock;

	while (wnd.isOpen())
	{
		cl::Event event;
		while (wnd.pollEvent(&event))
		{
			if (event.type == cl::EventType::Closed) { wnd.close(); }
		}

		cl::Vector2f delta = cl::Mouse::getMousePos(wnd) - centerWnd;
		cl::Mouse::setMousePos(centerWnd, wnd);
		player.handleInput(wnd.gfx(), delta, world);
		player.update(world, clock.restart());

		camera.rot = player.getRotation();
		camera.update(wnd.gfx());

		cl::TimeCBuf buff = { timer.getElapsedTime() };
		GET_TIME_CBUFF(wnd.gfx()).update(wnd.gfx(), buff);
		GET_TIME_CBUFF(wnd.gfx()).bind(wnd.gfx());

		wnd.gfx().clear(0.0f, 0.6f, 1.0f);

		camera.draw(wnd.gfx(), world);

		wnd.gfx().swapBuffer();
	}

	cl::Window::deinitWindowClass();

	return 0;
}