#include "Entity.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Math.h"
#include "Game.h"

Entity::Entity(Vector2f p_pos, SDL_Texture* p_tex)
	:pos(p_pos), tex(p_tex)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 32;
	currentFrame.h = 32;
	SDL_QueryTexture(p_tex, NULL, NULL, &currentFrame.w, &currentFrame.h);
}

SDL_Texture* Entity::getTex()
{
	return tex;
}

void Entity::Update()
{
	currentFrame.x++;
	currentFrame.y++;

	srcRect.h = 32;
	srcRect.w = 32;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = currentFrame.x;
	destRect.y = currentFrame.y;
	destRect.w = srcRect.h * 4;
	destRect.h = srcRect.w * 4;
}


void Entity::setPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

void Entity::setAngle(float p_angle)
{
	angle = p_angle;
}

void Entity::setScale(float w, float h)
{
	scale.x = w;
	scale.y = h;
}

SDL_Rect Entity::getCurrentFrame()
{
	return currentFrame;
}
void Entity::Renderer()
{
	SDL_RenderCopy(Game::renderer, tex, &srcRect, &destRect);
}