#include "GameObject.h"
#include "Transform.h"
#include "TextureRenderer.h"

that::GameObject::~GameObject() = default;

void that::GameObject::Init()
{
	m_pTransform = AddComponent<Transform>();
}

void that::GameObject::Update()
{
	// Update every component
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->Update();
	}
}

void that::GameObject::LateUpdate()
{
	// LateUpdate every component
	for (const auto& pComponent : m_pComponents)
	{
		pComponent->LateUpdate();
	}
}

void that::GameObject::UpdateCleanup()
{
	// Remove components from their containers if they are marked as dead
	m_pComponents.erase(std::remove_if(begin(m_pComponents), end(m_pComponents), [](const auto& pComponent)
		{
			return pComponent->IsMarkedAsDead();
		}), end(m_pComponents));

	// Remove the render component if its marked as dead
	if (m_pRenderComponent && m_pRenderComponent->IsMarkedAsDead()) m_pRenderComponent = nullptr;
}

void that::GameObject::Render() const 
{
	if(m_pRenderComponent) m_pRenderComponent->Render();
}

void that::GameObject::SetParent(std::shared_ptr<GameObject> pParent)
{
	// Cache the previous parent
	std::shared_ptr<GameObject> pOldParent{};
	if (!m_pParent.expired()) pOldParent = m_pParent.lock();

	// If this GO has a parent
	if (pOldParent)
	{
		// If the newly assigned parent is the same as the current parent, do nothing
		if (pOldParent == pParent) return;

		// Remove itself from the children list of the previous parent
		for (int i{ static_cast<int>(pOldParent->m_pChildren.size() - 1) }; i >= 0; --i)
		{
			const auto pChild{ pOldParent->m_pChildren[i].lock() };

			if (pChild.get() == this)
			{
				pOldParent->m_pChildren[i] = pOldParent->m_pChildren[pOldParent->m_pChildren.size() - 1];
				pOldParent->m_pChildren.pop_back();
				break;
			}
		}
	}
	else
	{
		// If there is no previous parent, and the new parent is also null, do nothing
		if (!pParent) return;
	}

	// Set the parent of this GO
	m_pParent = pParent;

	// Add itself to the children list of the new parent
	if(pParent) pParent->m_pChildren.push_back(weak_from_this());

	// Get the transform
	auto pTransform{ GetTransform() };
	if (!pTransform) return;

	// If a new parent is assigned
	if (pParent)
	{
		// Set the local position to the position relative to the new parent
		auto pParentTransform{ pParent->GetTransform() };
		if (pParentTransform) pTransform->SetLocalPosition(pTransform->GetWorldPosition() - pParentTransform->GetWorldPosition());
	}
	else // The parent got removed
	{
		// Set the local position to the world position
		pTransform->SetLocalPosition(pTransform->GetWorldPosition());
	}
}

std::shared_ptr<that::GameObject> that::GameObject::GetParent() const
{
	if (m_pParent.expired()) return nullptr;

	return m_pParent.lock();
}

std::shared_ptr<that::GameObject> that::GameObject::GetChild(int index) const
{
	// TODO: Log a warning that the user tries to get a child out of bounds
	if (index >= static_cast<int>(m_pChildren.size())) return nullptr;

	const auto pChild{ m_pChildren[index] };

	if (pChild.expired()) return nullptr;

	return pChild.lock();
}

void that::GameObject::Destroy()
{
	m_IsMarkedDead = true;

	// Destroy all children
	for (const auto& child : m_pChildren)
	{
		if (child.expired()) continue;

		child.lock()->Destroy();
	}
}

bool that::GameObject::Destroy(Component* pComponent)
{
	// If the owner of this component is not this GO, do nothing
	if (pComponent->GetOwner().get() != this) return false;
	// TODO: log warning that the user is trying to remove a component that is not on this GO

	// Mark the component as dead
	pComponent->Destroy();

	return true;
}
