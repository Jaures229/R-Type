#ifndef COMPONENT_MANAGER
    #define COMPONENT_MANAGER
    #include "Component.hpp"

class ComponentManager {
    private:
        std::unordered_map<std::string, Components> ComponentType{};
        std::unordered_map<std::string, std::shared_ptr<IComponentArray>> ComponentArrays{};
        Components nextComponent{};

        public:
        template <typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray() {
            std::string typeName = std::type_index(typeid(T)).name();
            assert(ComponentType.find(typeName) != ComponentType.end() && "Type doesn't exist.");
            return std::static_pointer_cast<ComponentArray<T>>(ComponentArrays[typeName]);
        };

        template <typename T>
        void registerComponent() {
            std::string typeName = std::type_index(typeid(T)).name();
            assert(ComponentType.find(typeName) == ComponentType.end() && "Component already exists.");
            ComponentType.insert({typeName, nextComponent});
            ComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
            ++nextComponent;
        };

        template <typename T>
        Components GetComponentType() {
            std::string typeName = std::type_index(typeid(T)).name();
            assert(ComponentType.find(typeName) != ComponentType.end() && "Component doesn't exist.");
            return ComponentType[typeName];
        };

        template <typename T>
        void AddComponent(Entity entity, T component) {
            GetComponentArray<T>()->PutData(entity, component);
        };

        template <typename T>
        void DeleteComponent(Entity entity) {
            GetComponentArray<T>()->deleteData(entity);
        };

        template <typename T>
        T& GetComponent(Entity entity) {
            return GetComponentArray<T>()->GetData(entity);
        };

        template <typename T>
        bool HasComponent (Entity entity) {
            return GetComponentArray<T>()->HasData(entity);
        }

        void EntityDeleted(Entity entity);
    };

#endif