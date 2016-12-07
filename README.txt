GameSample reprsents the realiztion of Component Entity System pattern.
Creation of entites and component are managed by gEntityManager. CreateEntity() method creates and returns the Entity, that is just the numeric identifier. 
Template method AddComponentsToEntity(...) creates the component, that was passed as the typename for specified entity.

Component is stored in ComponentPool. Each Component has own memory pool. ComponentPool provides a resizable, semi-continues and cahche-firendly memory pool for creating, storing and destroying components.
ComponentPool doesn't free memory after deleting the component, instead it reuses that memory for new components. Each ComponentPool knows what Entity is owner of the component.

Systems are managed by SystemsManager. It provides functionality for registering and updating the systems. Systems uses EntityManger for processing the components. 
