GameSample reprsents the realiztion of Component Entity System pattern.
Creation of entites and component are managed by gEntityManager. CreateEntity() method creates and returns the Entity, that is just the numeric identifier. 
Template method AddComponentsToEntity(...) creates the component, that was passed as the typename for specified entity. Component are stored in pools 