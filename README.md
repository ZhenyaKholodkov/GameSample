# GameSample
GameSample represents the small game engine using the Component Entity System architecture pattern. 
**Current features:**
- Infrastructure for creating and handling the components and systems,
- 2D rendering,
- Texture Managing(only PNG format or now).

**Example of using can be found:**

- source code: src/Game
- .exe  : data/GameSample.exe

## Short Introduction
### Entities and Components

*source code:*
- Entity Manager: src/Engine/entity
- Components: src/Components

### Entity 

An Entity is nothing more then an id that used for access to its components.
GEntityManager generates the Entity by method createEntity().

### Components

Components are passive data structures consisting of the data for one aspect of the entity. A component doesn't contain any logic.
All logic is in the Systems.

***Creating the Components***

All components should be inherited from GComponent<typename Derived>:
```c++
class GLocation: public GComponent<GLocation>
{
public: 
    GLocation(float x, float y) : mX(x), mY(y) {]
private:
    float mX;
    float mY;
};
```
The new Component should be included to GEntityManager.h.
After that GLocation can be added to the entity by calling the method GEntityManager::addComponentToEntity(Entity, Args&& .. args):
```c++
GEntityManager manager;
manager.addComponentToEntity<GLocation>(entity, 100.0f, 100.0f);
```
***Access to Components***
There are few ways to get access to the components:
-  To handle all components of a particular type the methos GEntityManager::each(std::function<void(Entity entity, Components&...)>::type f)
shoud be used. 'f' can  be a function or a lamda.
**Example**
```c++
	mEntityManager->each<GRenderableComponent, GLocationComponent>([this](Entity entity, GRenderableComponent& renderable, GLocationComponent& location)
	{
  //Rendering
  });
```
-  To get acces to a component of a particular entity use the GEneityManager::getComponent<"Component">(Entity entity)

### Systems
The Systems provide a logic to handle the components.

***Creating a System***
A system class should be inherited from GSystem<typename Derived>. Each System must implement the update(int dt) method.
**Example**
```c++
class GScalableSystem : public GSystem<GScalableSystem>
{
public:
	GScalableSystem(std::shared_ptr<GEntityManager> manager);
	~GScalableSystem();

	virtual void update(int dt);
};
```

***Signals and slots***
Signals and slots conception is used for communication between entities. I used the boost realiztion.
Signals and slots are stored in components like any other data. And systems realizate a call logic.
**Example**
```c++

class GMoveableComponent : public GComponent<GMoveableComponent>
{
/*....*/
public: /*signals*/
	boost::signals2::signal<void()>                 signal_Moved;
/*....*/
};

/*System*/
class GMoveableSystem : public GSystem<GMoveableSystem>
{
public:
	GMoveableSystem(std::shared_ptr<GEntityManager> manager);
	~GMoveableSystem();

	virtual void update(int dt)
  {
	    mEntityManager->each<GMoveableComponent, GLocationComponent>([&](Entity entity, GMoveableComponent& moveable, GLocationComponent& location)
	    {
         /*...*/
	     	moveable.signal_Moved();
        /*....*/
	    });
  }
};
```
For more information about boost signals visit [BOOST.Signals](http://www.boost.org/doc/libs/1_64_0/doc/html/signals.html)

### Component Pools
Components are stored in Pools.
- source code: src/Engine/help
GPool provide resizeable, semi-contiguous pool of memory for constructing components in. It provide cach-friendly iteration.
GPool reuse the memory of destroyed components. 
GEntityManager manage the pools of components. The component id is used to get acces to it pool. 
The id is stored in GComponent<C> class.
Also, the GEntityManager stores the masks of components for each enity and indexes of its components in a pool.


### Resources
- source code : src/Engine/resources, src/Engine/graphics

The png format is used for texture. I use the [libpng](http://www.libpng.org/pub/png/libpng.html) for texure loading . 
The information about textures and sprites are stored in .xml config file.
GResManager class parse the config file and creates textureAtlas and sprites objects.
GTextureManager loads a texture after the first using its sprite.

### ThirsParty libraries
[libpng](http://www.boost.org/doc/libs/1_64_0/doc/html/signals.html) - for loading png texture
[boost](http://www.boost.org/) - for using signals and slots
[tinyzml](http://www.grinninglizard.com/tinyxml/) - xml parser
[freetype](https://www.freetype.org/) and [ftgl](http://ftgl.sourceforge.net/docs/html/ftgl-tutorial.html) - for text rendering

### Run the project
To build the project the boost should be installed and evironment variable $(BOOST_LIBS) and $(BOOST_INCLUDE) should point to libs and header files.
[boost installation instruction] (http://www.boost.org/doc/libs/1_64_0/doc/html/bbv2.html#bbv2.installation)

