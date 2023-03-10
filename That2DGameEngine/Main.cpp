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
#include "RotatorComponent.h"

void load()
{
	// Create a new scene
	auto& scene = that::SceneManager::GetInstance().CreateScene("Demo");

	// Load fonts and textures used in the scene
	const auto pFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
	const auto pFPSFont{ that::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18) };
	const auto pBackgroundTexture{ that::ResourceManager::GetInstance().LoadTexture("background.tga") };
	const auto pLogoTexture{ that::ResourceManager::GetInstance().LoadTexture("logo.tga") };

	// Create a GO that renders the background texture
	const auto pBackgroundGO{ scene.CreateGameObject() };
	pBackgroundGO->AddComponent<that::TextureRenderer>()->SetTexture(pBackgroundTexture);

	// Create a GO that renders the logo texture
	const auto pLogoGO{ scene.CreateGameObject() };
	pLogoGO->GetComponent<that::Transform>()->SetWorldPosition(216, 180);
	pLogoGO->AddComponent<that::TextureRenderer>()->SetTexture(pLogoTexture);

	// Create a GO that renders a text on the screen
	const auto pHeaderGO{ scene.CreateGameObject() };
	pHeaderGO->GetComponent<that::Transform>()->SetWorldPosition(80, 20);
	pHeaderGO->AddComponent<that::TextureRenderer>();
	const auto pHeaderText{ pHeaderGO->AddComponent<that::TextComponent>() };
	pHeaderText->SetFont(pFont);
	pHeaderText->SetText("Programming 4 Assignment");

	// Create a GO that renders the FPS on the screen
	const auto pFpsText{ scene.CreateGameObject() };
	pFpsText->GetComponent<that::Transform>()->SetWorldPosition(0.0f, 0.0f);
	pFpsText->AddComponent<that::TextureRenderer>();
	pFpsText->AddComponent<that::FPSCounter>();
	const auto pText{ pFpsText->AddComponent<that::TextComponent>() };
	pText->SetFont(pFPSFont);
	pText->SetColor(255, 255, 0);

	// Create a empty gameobject as pivot for the rotating character
	const auto pPivot{ scene.CreateGameObject() };
	pPivot->GetComponent<that::Transform>()->SetWorldPosition(300, 300);

	// Create a GO that renders the main character and rotates around the empty pivot object
	const auto pCharacter{ pPivot->CreateGameObject() };
	pCharacter->AddComponent<that::RotatorComponent>();
	pCharacter->AddComponent<that::TextureRenderer>()->SetTexture(that::ResourceManager::GetInstance().LoadTexture("MainCharacter.png"));

	// Create a GO that renders the enemy and rotates around the character at double speed
	const auto pEnemy{ pCharacter->CreateGameObject() };
	pEnemy->AddComponent<that::RotatorComponent>()->SetRotateSpeedDegrees(360.0f);
	pEnemy->AddComponent<that::TextureRenderer>()->SetTexture(that::ResourceManager::GetInstance().LoadTexture("Enemy.png"));

	pCharacter->Destroy();
}

int main(int, char*[]) {
	that::Engine engine("../Data/");
	engine.Run(load);
    return 0;
}