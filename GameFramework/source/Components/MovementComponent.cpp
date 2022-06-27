#include "..\Components\MovementComponent.h"
#include <iostream>

MovementComponent::MovementComponent() : Component ("Movement Component")
{
	speed = 1000;
	currentJumpForce = 0;
}

MovementComponent::~MovementComponent()
{
	delete inputController;
	delete transform;
}

sf::Vector2f MovementComponent::GetVelocity() 
{
	direction = inputController->Get_Axis_Vector();
	sf::Vector2f velocity = { direction.x * speed, direction.y * speed };
	return velocity;
}

sf::Vector2f MovementComponent::GetMotionVector() 
{
	velocity = GetVelocity();
	const float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

	if (length == 0.0f) return { 0,0 };

	return { velocity.x / length, velocity.y / length };
}

void MovementComponent::On_Update(const float delta_time)
{
	if (!IsMotionValid()) return;
	auto nextPos = sf::Vector2f{ 0,0 };

	if ((GetMotionVector().x == -1 && isJumping == false))
	{

		if (orientation != -1 || (orientation == -1 && wasJumping==true))
		{
			wasJumping = false;
			orientation = -1;
			rederer->SetTexturePath("source/resources/BackIdle.png", true, true);
			transform->GetTransform()->setTextureRect(sf::IntRect(0, 0, 300, 300));
		}

	}
	else if(GetMotionVector().x == 1 && isJumping == false)
	{
		if (orientation != 1 || (orientation == 1 && wasJumping == true))
		{
			wasJumping = false;
			orientation = 1;
			rederer->SetTexturePath("source/resources/idle.png", true, true);
			transform->GetTransform()->setTextureRect(sf::IntRect(0, 0, 300, 300));
		}
	}


	if (isJumping == true)
	{
		if (GetMotionVector().x == -1 && isJumping == true)
		{
				orientation = -1;
				rederer->SetTexturePath("source/resources/backJump.png", true, true);
				transform->GetTransform()->setTextureRect(sf::IntRect(0, 0, 300, 300));
			
		}
		else if (GetMotionVector().x == 1)
		{	
				orientation = 1;
				rederer->SetTexturePath("source/resources/Jump.png", true, true);
				transform->GetTransform()->setTextureRect(sf::IntRect(0, 0, 300, 300));
		}
	}


	nextPos = transform->GetTransform()->getPosition() + sf::Vector2f(GetMotionVector().x * 600 * delta_time, 
											 GetMotionVector().y * speed * delta_time - currentJumpForce * delta_time);
	//printf("new pos: %f, %f\n", nextPos.x, nextPos.y);

	//Gravity like negative Yaxis acceleration implementation
	nextPos.y += 14,6 * delta_time * delta_time;
	transform->SetPosition(nextPos.x, nextPos.y);



	//Decreasing jump force
	currentJumpForce -= 14, 6 * delta_time * delta_time;
	if(currentJumpForce<=0.0f)
	{
		currentJumpForce = 0.0f;
	}


	if (collider->collisionDirection->y == 1 && isJumping == true)
	{
		isJumping = false;
		wasJumping = true;
	}
}

void MovementComponent::Jump()
{
	if (collider == nullptr) { return; }
	//std::cout << "COLL DIR IN MOV:" << collider->collisionDirection.x << "     " << collider->collisionDirection.y << std::endl;
		if ( collider->collisionDirection->y == 1 && isJumping == false)
		{
			//printf("								Jumping\n\n\n");
			currentJumpForce = maxJumpForce;
			isJumping = true;
			wasJumping = false;
			collider->collisionDirection->y = 0;
		}
}
