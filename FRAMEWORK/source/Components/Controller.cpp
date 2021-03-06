#include "..\Components\Component.h"
#include "..\Components\Controller.h"
#include "Components/MovementComponent.h"

//Update shaped around a 2D Platform like character
void Controller::On_Update(const float delta_time)
{
	Component::On_Update(delta_time);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		movementComponent->Jump();
	}

	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		//forwardMovement = true;
		//YAxis = -1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		//forwardMovement = true;
		//YAxis = -1;
	}*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		forwardMovement = true;
		YAxis = 1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		forwardMovement = true;
		YAxis = 1;
	}
	else
	{
		forwardMovement = false;
		YAxis = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		rightwardMovement = true;
		XAxis = 1;
	}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			rightwardMovement = true;
			XAxis = 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			rightwardMovement = true;
			XAxis = -1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			rightwardMovement = true;
			XAxis = -1;
		}
	else
	{
		forwardMovement = false;
		XAxis = 0;
	}
	//printf("axis: %f, %f \n", XAxis, YAxis);
}

void Controller::On_Fixed_Update(const float delta_time)
{
	Component::On_Fixed_Update(delta_time);

}

Controller::Controller() : Component("Controller"){}

Controller::Controller(MovementComponent* movementComponent ) : Component ("Controller")
{
	this->movementComponent = movementComponent;
}

Controller::~Controller() = default;

