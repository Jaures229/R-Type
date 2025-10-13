/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** AnimationSystem
*/

#include "../Include/AnimationSystem.hpp"

// update the animation of all entity
// void AnimationSystem::update(Mediator &medi, float deltaTime)
// {
//     for (auto const &entity : Entities) {
//         auto &sprite = medi.GetComponent<Sprite>(entity);
//         auto &animations = medi.GetComponent<AnimationComponent>(entity);

//         if (!animations.active) continue;

//         // Mettre à jour le temps écoulé
//         animations.elapsedTime += deltaTime;

//         // Récupérer l'animation actuelle
//         auto &currentAnimation = animations.animations[animations.currentAnimation];

//         // Vérifiez si nous devons passer au cadre suivant
//         if (animations.elapsedTime >= currentAnimation.frameDuration) {
//             // Réinitialiser le temps écoulé pour ce cadre
//             animations.elapsedTime -= currentAnimation.frameDuration;

//             // Passer au cadre suivant
//             animations.currentFrame++;

//             // Si on dépasse le nombre de cadres
//             if (animations.currentFrame >= currentAnimation.frameCount) {
//                 if (currentAnimation.looping) {
//                     animations.currentFrame = 0; // Retour au premier cadre
//                 } else {
//                     animations.active = false; // Désactiver l'animation
//                     continue;
//                 }
//             }

//             // Calculer le rectangle du cadre actuel
//             int left = currentAnimation.startFrame.left + 
//                        (animations.currentFrame * (currentAnimation.frameSize.x + currentAnimation.frameOffset.x));
//             int top = currentAnimation.startFrame.top + 
//                       (animations.currentFrame * currentAnimation.frameOffset.y);

//             // Définir le rectangle de texture pour le sprite
//             sprite.sprite.setTextureRect(sf::IntRect(left, top, 
//                                                      currentAnimation.frameSize.x, 
//                                                      currentAnimation.frameSize.y));
//         }
//     }
// }


// set the animation of an entity
void AnimationSystem::setAnimation(Mediator &medi, const std::string& name, Entity entity)
{
    auto &sprite = medi.GetComponent<Sprite>(entity);
    auto &animations = medi.GetComponent<AnimationComponent>(entity);

    if (animations.animations.find(name) != animations.animations.end()) {
        sf::IntRect frame = animations.animations[name].startFrame;
        sprite.sprite.setTextureRect(frame);
        animations.currentAnimation = name;
        animations.currentFrame = 0;
        animations.elapsedTime = 0.0f;
        animations.active = true;
    }
}



void AnimationSystem::update(Mediator &medi, float deltaTime)
{
    for (auto const &entity : Entities) {
        auto &sprite = medi.GetComponent<Sprite>(entity);
        auto &animation = medi.GetComponent<AnimationComponent>(entity);

        if (!animation.active)
            continue;

        // Met à jour le temps écoulé
        animation.elapsedTime += deltaTime;

        // Si assez de temps s'est écoulé pour changer de frame
        if (animation.elapsedTime >= animation.animations[animation.currentAnimation].frameDuration) {
            animation.elapsedTime = 0.0f; // Réinitialiser le temps écoulé

            // Avancer d'une frame
            auto &currentAnim = animation.animations[animation.currentAnimation];
            animation.currentFrame++;

            // Déplace le rectangle de texture à la frame suivante
            currentAnim.startFrame.left += currentAnim.frameSize.x;

            // Si on dépasse la largeur totale de la feuille, revenir à la première colonne
            if (currentAnim.startFrame.left >= currentAnim.frameSize.x * currentAnim.frameCount) {
                currentAnim.startFrame.left = 0; // Revenir à la première colonne
                animation.currentFrame = 0;     // Réinitialiser l'index de frame
            }

            // Mettre à jour la texture rect pour afficher la nouvelle frame
            sprite.sprite.setTextureRect(currentAnim.startFrame);
        }
    }
}
