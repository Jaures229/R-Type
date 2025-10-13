#ifndef SYSTEM_MANAGER
  #define SYSTEM_MANAGER
  #include "Systeme.hpp"

  class SystemManager {
    public:
      template <typename T>
      std::shared_ptr<T> RegisterSystem() {
        const char * typeName = typeid(T).name();
        assert(Systems.find(typeName) == Systems.end() && "System already exist.");
        auto system = std::make_shared<T>();
        Systems.insert({typeName, system});
        return system;
      };
      template <typename T>
      void SetSignature(Signature signature) {
        const char * typeName = typeid(T).name();
        assert(Systems.find(typeName) != Systems.end() && "System does't exist.");
        Signatures.insert({typeName, signature});
      };
      void EntityDeleted(Entity entity);
      void EntitySignatureChanged(Entity entity, Signature entitySignature);

    private:
      std::unordered_map<const char*, Signature>Signatures{};
      std::unordered_map<const char*, std::shared_ptr<System>>Systems;
  };
#endif