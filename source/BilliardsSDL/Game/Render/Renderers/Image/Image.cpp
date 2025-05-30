#include "Image.h"


Image::Image(GameObject* gameObject, const Vector2<float>& size)
	: Renderer(gameObject, size), 
	m_sourceRect()
{
}

Image::~Image()
{
}

void Image::Init(const GameRenderManager& renderManager, const ImageResourceData& resourceData)
{
	m_sourceRect = resourceData.p_sourceRect;
	InitTexture(renderManager.LoadImageTexture(resourceData));
}

SDL_Rect* Image::GetSourceRect()
{
	return &m_sourceRect;
}
