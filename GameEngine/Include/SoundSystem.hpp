#ifndef SOUNDSYS
  #define SOUNDSYS
  #include "Entity.hpp"
  #include "Systeme.hpp"
  #include "Mediator.hpp"

  class SoundSystem {
    public:
      void LoadSound(Sound::SoundType type, const std::string& path);
      void PlaySound(Sound::SoundType type);
      void StopSound(Sound::SoundType type);

      std::unordered_map<Sound::SoundType, Sound> sounds;
    private:
  };

#endif
