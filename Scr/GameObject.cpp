#include "GameObject.h"
#include "TextureManager.h"


GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren, int x , int y)
{
	renderer = ren;
	objTexture = TextureManager::LoadTexture(texturesheet, renderer);
	xpos = x;
	ypos = y;
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	xpos++;
	ypos++;

	srcRect.h = 32;
	srcRect.w = 32;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.h * 4;
	destRect.h = srcRect.w * 4;
}

void GameObject::Renderer()
{
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}
