#include "../Include/SoundSystem.hpp"

void SoundSystem::LoadSound(Sound::SoundType type, const std::string& path)
{
  Sound sound;
  sound.type = type;
  if (!sound.buff.loadFromFile(path)) {
    throw std::runtime_error("Erreur de chargement de " + path);
  }
  sound.sound.setBuffer(sound.buff);
  sounds[type].sound.setVolume(100.0f);
  sounds[type] = sound;

}

void SoundSystem::PlaySound(Sound::SoundType type)
{
  if (sounds.find(type)!= sounds.end())
  {
    sounds[type].sound.play();
  } else {
    std::cerr << "le son n'est pas chargé" << std::endl;
  }
}

void SoundSystem::StopSound(Sound::SoundType type)
{
    if (sounds.find(type)!= sounds.end()) {
        sounds[type].sound.stop();
    } else {
        std::cerr << "Erreur : le son n'est pas chargé pour le type " << static_cast<int>(type) << std::endl;
    }
}