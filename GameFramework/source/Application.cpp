#include "Application.h"
#include "Components/Component.h"
#include "Classes/IDrawable.h"
#include "Components/RectTransform.h"
#include "Classes/Sprite.h"
#include "Classes/Character.h"
#include "Classes/Agent.h"
#include "Components/Collider.h"
#include "Components/MusicComponent.h"
#include "Classes/MusicObject.h"


#include "iostream"
#include "SFML/Graphics.hpp"



Application::Application()
{
	_Window = nullptr;
	fpsLimitEnabled = false;
	maxFPS = 0;
	backgroundcolor = sf::Color::Black;
}

Application::~Application()
{
	delete this;
}



/// <summary>
/// Creates application window
/// </summary>
/// <param name="windowWidth">the window width</param>
/// <param name="windowHeight">the window height</param>
/// <param name="windowTitle">window main title</param>
void Application::CreateWindow(float windowWidth, float windowHeight, const char* windowTitle)
{
	_Window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), windowTitle);
	if (!_Window)
	{
		std::cerr << "Application window initialization failed\n";
		return;
	}
}
/// <summary>
/// Return TRUE if application's window is still open.
/// </summary>
/// <returns> TRUE or FALSE</returns>
bool Application::isRunning() const
{
	return _Window->isOpen();
}


/// <summary>
/// Set background coloro using sf::Color class.
/// </summary>
/// <returns></returns>
void Application::SetBackgroundColor(sf::Color color)
{
	backgroundcolor = color;
}
/// <summary>
/// Set background coloro using 4 float value for color component: red, green, blue, alpha.
/// </summary>
/// <returns></returns>
void Application::SetBackgroundColor(float red, float green, float blue, float alpha)
{
	backgroundcolor = sf::Color(red, green, blue, alpha);
}

////
////	GameLoop functions
////
void Application::UpdateGameTime()
{
	currentTime = tm.getCurrentTime();
	elapsedTime = TimeManager::calculateElapsedTime(currentTime.asSeconds(), lastTime.asSeconds());
	lag += elapsedTime;
	lastTime = currentTime;
}

/// <summary>
/// Calculate the total frames in one seconds by elapsed time
/// </summary>
/// <returns>total frames</returns>
unsigned Application::GetFrameRate()const { return 1 / elapsedTime; }

void Application::ProcessWindowEvents()
{
	sf::Event evt{};
	while (_Window->pollEvent(evt))
	{
		if (evt.type == sf::Event::EventType::Closed)
		{
			_Window->close();
		}
	}
}

void Application::FixedUpdate()
{
	for (auto item : allEntities)
	{
		if (item->isActive && item->Is_Tick_Enabled()) item->On_Fixed_Update(elapsedTime);
	}
}

void Application::Update()
{
	for (GameObject* item : allEntities)
	{
		if (item->isActive && item->Is_Tick_Enabled()) item->On_Update(elapsedTime);
	}
}


void Application::Draw()
{
	_Window->clear(backgroundcolor);

	for (auto item : allEntities)
	{
		const auto renders = item->Get_Components<RectTranform>();

		for (auto rend : renders)
		{
			_Window->draw(*rend->GetTransform());
		}
	}

	_Window->display();
}

void Application::Initialize()
{
	auto background = new Sprite();
	background->renderer->SetTexturePath("source/resources/roma.jpg", true, true);
	background->rectTransform->SetScale(1920, 1080);
	auto soundTrack = new MusicComponent("source/resources/music1.wav", true, 100, true);
	background->Add_Component(soundTrack);
	allEntities.push_back(background);

	auto wall = new Sprite();
	wall->renderer->SetTexturePath("source/resources/duce.jpg", true, true);
	auto collider = new Collider(wall->rectTransform, 1.0f);
	wall->Add_Component(collider);
	wall->rectTransform->SetScale(1, 1);
	wall->rectTransform->SetPosition(700, 700);
	wall->rectTransform->GetTransform()->setSize(sf::Vector2f{185,300});
	wall->rectTransform->GetTransform()->setOrigin(sf::Vector2f{ 185/2, 300/2 });
	allEntities.push_back(wall);

	auto dux = new Character();
	auto colliderDux = new Collider(dux->rectTransform, 1.0f);
	dux->Add_Component(colliderDux);
	dux->renderer->SetTexturePath("source/resources/duce.jpg", true, true);
	dux->rectTransform->SetScale(1, 1);
	dux->rectTransform->GetTransform()->setSize(sf::Vector2f{ 185,300 });
	dux->rectTransform->GetTransform()->setOrigin(sf::Vector2f{ 185 / 2, 300 / 2 });
	dux->movementComponent->speed = 200;
	allEntities.push_back(dux);

	auto runningDux = new Agent(this);
	runningDux->renderer->SetTexturePath("source/resources/hitler.jpg", true, true);
	runningDux->rectTransform->SetScale(256, 256);
	runningDux->rectTransform->SetPosition(500, 500);
	allEntities.push_back(runningDux);
}

void Application::Run()
{
	Initialize();
	PlayMusicsInScene();
	lastTime = tm.getCurrentTime();
	while (_Window->isOpen())
	{
		UpdateGameTime();
		std::cout << "FPS: " << GetFrameRate() << std::endl;

		ProcessWindowEvents();
		if (fixedUpdateEnabled)
		{
			while (lag < msForFixedUpdate)
			{
				FixedUpdate();
				lag -= msForFixedUpdate;
			}
		}
		Update();
		CheckCollision();
		Draw();
		if (fpsLimitEnabled)
		{
			sf::sleep(sf::seconds((1.0f / maxFPS)));
		}
	}
}

void Application::CheckCollision() {

	std::vector<Collider*> colliders;

	for (auto item : allEntities)
	{
		if(item->Get_Component<Collider>()!=nullptr)
		{
			colliders.push_back(dynamic_cast<Collider*>(item->Get_Component<Collider>()));
		}
	}
	for (auto coll1 : colliders)
	{
		for (auto coll2 : colliders)
		{
			if (coll1 != coll2) {
				coll1->CheckCollision(coll2, coll2->push);
			}

		}
	}
}

void Application::PlayMusicsInScene()
{
	std::vector<MusicComponent*> musics;

	for (auto item : allEntities)
	{
		if (item->Get_Component<MusicComponent>() != nullptr)
		{
			musics.push_back(dynamic_cast<MusicComponent*>(item->Get_Component<MusicComponent>()));
		}
	}
	for (auto item : musics)
	{
		if (item->playOnStart == true)
		{
			item->Play();
		}
	}

	for (auto item : allEntities)
	{
		const auto val = dynamic_cast<MusicObject*>(item);
		if (!val) continue;
		else if (val->playOnStart == true)
		{
			val->Play();
		}
	}
}




