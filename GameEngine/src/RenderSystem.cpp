#include "../Include/RenderSystem.hpp"

void RenderSystem::Update() {

}

void RenderSystem::Render(sf::RenderWindow& window, Mediator& mediator) {
  for (auto const& entity : Entities) {
    auto& sprite = mediator.GetComponent<Sprite>(entity);
    auto& position = mediator.GetComponent<Position>(entity);
    sprite.sprite.setPosition(position.x, position.y);
    window.draw(sprite.sprite);
  }
}
