#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Engine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "FPSCounter.h"

void load()
{
	// Create a new scene
	auto& scene = that::SceneManager::GetInstance().CreateScene("Demo");

	// Load fonts and textures used in the scene
	auto pFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	auto pFPSFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18) };
	auto pBackgroundTexture{ that::ResourceManager::GetInstance().LoadTexture("background.tga") };
	auto pLogoTexture{ that::ResourceManager::GetInstance().LoadTexture("logo.tga") };

	// Create a GO that renders the background texture
	auto pBackgroundGO{ scene.CreateGameObject() };
	auto pBackgroundTransform{ pBackgroundGO->AddComponent<that::Transform>() };
	pBackgroundGO->AddComponent<that::TextureRenderer>()->SetTexture(pBackgroundTexture);

	// Create a GO that renders the logo texture
	auto pLogoGO{ scene.CreateGameObject() };
	pLogoGO->AddComponent<that::Transform>()->SetPosition(216, 180);
	pLogoGO->AddComponent<that::TextureRenderer>()->SetTexture(pLogoTexture);

	// Create a GO that renders a text on the screen
	auto pHeaderGO{ scene.CreateGameObject() };
	pHeaderGO->AddComponent<that::Transform>()->SetPosition(80, 20);
	pHeaderGO->AddComponent<that::TextureRenderer>();
	auto pHeaderText{ pHeaderGO->AddComponent<that::TextComponent>() };
	pHeaderText->SetFont(pFont);
	pHeaderText->SetText("Programming 4 Assignment");

	// Create a GO that renders the FPS on the screen
	auto go{ scene.CreateGameObject() };
	go->AddComponent<that::Transform>()->SetPosition(0.0f, 0.0f);
	go->AddComponent<that::TextureRenderer>();
	go->AddComponent<that::FPSCounter>();
	auto pText{ go->AddComponent<that::TextComponent>() };
	pText->SetFont(pFPSFont);
	pText->SetColor(255, 255, 0);
}

int main(int, char*[]) {
	that::Engine engine("../Data/");
	engine.Run(load);
    return 0;
}