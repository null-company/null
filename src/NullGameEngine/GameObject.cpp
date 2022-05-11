#include <memory>

#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <iostream>
#include <GameObject.hpp>
#include <Script.hpp>
#include <Scene.hpp>

namespace null {

    constexpr static int meterToPixel = 100;
    constexpr static float pixelToMeter = 1.0f / static_cast<float>(meterToPixel);
    constexpr static double pi = 3.14159265358979323846;

    GameObject::GameObject() : visible(false) {};

    GameObject::~GameObject() {
        if (scene.lock()) {
            if (rigidBody) {
                rigidBody->GetWorld()->DestroyBody(rigidBody);
            }
        }
    }

    std::weak_ptr<GameObject> GameObject::addChild(std::shared_ptr<GameObject>&& child) {
        child->scene = scene;
        auto parentwptr = weak_from_this();
        child->parent = parentwptr;
        children.push_back(child);

        return child;
    }

    std::weak_ptr<Scene> GameObject::getScene() {
        return scene;
    }

    Scene& GameObject::getSceneForce() {
        return *scene.lock();
    }

    sf::Sprite& GameObject::getSprite() {
        return sprite;
    }


    std::shared_ptr<GameObject> GameObject::findFirstChildrenByTag(const std::string& tag) {
        auto resIt = std::find_if(children.begin(), children.end(),
                                  [&tag](std::shared_ptr<GameObject>& gameObject) {
                                      return gameObject->hasTag(tag);
                                  });
        if (resIt == children.end()) {
            return nullptr;
        }
        return *resIt;
    }

    b2Body* GameObject::getRigidBody() {
        return rigidBody;
    }

    template<typename T>
    static const b2Vec2 pixelToMetersVector(const sf::Vector2<T>& pixelVec) {
        const b2Vec2 metersVec(pixelVec.x * pixelToMeter, pixelVec.y * pixelToMeter);
        return metersVec;
    }

    template<typename T>
    static const sf::Vector2<T> meterToPixelVector(const b2Vec2& meterVec) {
        T x = static_cast<T>(meterVec.x * meterToPixel);
        T y = static_cast<T>(meterVec.y * meterToPixel);

        return sf::Vector2<T>(x, y);
    }

    static const sf::Vector2u getSpriteSize(sf::Sprite& sprite) {
        const sf::Vector2u spriteSize(
                sprite.getTextureRect().getSize().x * sprite.getScale().x,
                sprite.getTextureRect().getSize().y * sprite.getScale().y);
        return spriteSize;
    }

    void GameObject::setRigidBodyDefPositionBySprite(b2BodyDef& bodyDef) {
        auto spritePosMeters = pixelToMetersVector(sprite.getPosition());
        bodyDef.position.Set(spritePosMeters.x, spritePosMeters.y);
    }

    void GameObject::setShapeAsBoxBySprite(b2PolygonShape& shape) {
        auto spriteSizeMeters = pixelToMetersVector(getSpriteSize(sprite));
        b2Vec2 boxCenter(spriteSizeMeters.x / 2, spriteSizeMeters.y / 2);
        shape.SetAsBox(spriteSizeMeters.x / 2, spriteSizeMeters.y / 2, boxCenter, 0.0f);
    }

    void GameObject::assertSpriteHasSize() {
        if (sprite.getTextureRect().height == 0 || sprite.getTextureRect().width == 0) {
            throw std::invalid_argument("No sprite attached, can't guess rigidbody size");
        }
    }

    void GameObject::makeStatic(b2World& box2dWorld) {
        assertSpriteHasSize();
        detachFromPhysicsWorld();

        b2BodyDef bodyDef;
        b2BodyUserData userData;
        userData.pointer = reinterpret_cast<uintptr_t>(this);
        bodyDef.userData = userData;
        setRigidBodyDefPositionBySprite(bodyDef);
        b2Body* rigidBody = box2dWorld.CreateBody(&bodyDef);

        b2PolygonShape shape;
        setShapeAsBoxBySprite(shape);
        rigidBody->CreateFixture(&shape, 0.0f);

        this->rigidBody = rigidBody;
    }

    void GameObject::makeDynamic() {
        makeDynamic(getScene().lock()->getBox2dWorld());
    }

    void GameObject::makeDynamic(b2World& box2dWorld) {
        assertSpriteHasSize();
        detachFromPhysicsWorld();

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        b2BodyUserData userData;
        userData.pointer = reinterpret_cast<uintptr_t>(this);
        bodyDef.userData = userData;
        setRigidBodyDefPositionBySprite(bodyDef);

        b2Body* rigidBody = box2dWorld.CreateBody(&bodyDef);

        b2PolygonShape shape;
        setShapeAsBoxBySprite(shape);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        rigidBody->CreateFixture(&fixtureDef);
        this->rigidBody = rigidBody;
    }

    void GameObject::detachFromPhysicsWorld() {
        if (rigidBody) {
            rigidBody->GetWorld()->DestroyBody(rigidBody);
            rigidBody = nullptr;
        }
    }

    std::vector<std::weak_ptr<GameObject>> GameObject::getChildren() {
        auto result = std::vector<std::weak_ptr<GameObject>>();
        for (const auto& childRef: children) {
            result.push_back(childRef);
        }
        return result;
    }

    void GameObject::deleteChild(GameObject* childToDelete) {
        auto it = std::remove_if(children.begin(), children.end(), [childToDelete](auto child) {
            return &*child == childToDelete;
        });

        children.erase(it, children.end());
    }

    void GameObject::deleteMe() {
        parent.lock()->deleteChild(this);
    }

    std::weak_ptr<GameObject> GameObject::getChild(int index) {
        return children[index];
    }

    // todo concern pointer leakage // todo todo todo todo todo я люблю когда волосатые мужики обмазываются маслом
    std::vector<std::unique_ptr<Script>>& GameObject::getScripts() {
        return scripts;
    }

    void GameObject::addScript(std::unique_ptr<Script> script) {
        scripts.push_back(std::move(script));
    }


    void GameObject::addTag(const std::string& str) {
        tags.insert(str);
    }

    bool GameObject::removeTag(const std::string& str) {
        auto result = !tags.find(str)->empty();
        tags.erase(str);
        return result;
    }

    bool GameObject::hasTag(const std::string& tag) {
        return tags.count(tag);
    }

    const sf::Transform& GameObject::getTransform() {
        return sprite.getTransform();
    }

    const sf::Vector2f& GameObject::getPosition() {
        return sprite.getPosition();
    }

    void GameObject::setPosition(float x, float y) {
        sprite.setPosition(x, y);
        if (rigidBody) {
            b2Vec2 newPosition = pixelToMetersVector(sf::Vector2f(x, y));
            rigidBody->SetTransform(newPosition, rigidBody->GetAngle());
        }
    }

    GameObject* GameObject::getCollied() {
        auto body = getRigidBody();
        auto contactList = rigidBody->GetContactList();
        for (b2ContactEdge* edge = contactList; edge; edge = edge->next) {
            auto go1 = reinterpret_cast<GameObject*>(edge->contact->GetFixtureA()->GetBody()->GetUserData().pointer);
            auto go2 = reinterpret_cast<GameObject*>(edge->contact->GetFixtureB()->GetBody()->GetUserData().pointer);
            if (!edge->contact->IsTouching()) {
                return nullptr;
            }
            if (go1 == this) {
                std::cout << "go2 returned" << std::endl;
                return go2;
            } else {
                std::cout << "go1 returned" << std::endl;
                return go1;
            }
        }
        return nullptr;

    }

    void GameObject::setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos);
        if (rigidBody) {
            b2Vec2 newPosition = pixelToMetersVector(pos);
            rigidBody->SetTransform(newPosition, rigidBody->GetAngle());
        }
    }


    void GameObject::start() {
        gameObjectStatus = GameObjectStatus::RUNNING;
        for (auto& script: scripts) {
            script->start();
        }
    }

    void GameObject::update() {

        // box2d is expected to have done something,
        // so we have to adjust the sprite
        if (gameObjectStatus == GameObjectStatus::NONE) {
            start();
        }
        if (rigidBody) {
            sf::Vector2f newPosition =
                    meterToPixelVector<float>(rigidBody->GetPosition());
            sprite.setPosition(newPosition);
            sprite.setRotation(rigidBody->GetAngle() * (180.0 / pi));
        }

        for (auto& script: scripts) {
            script->update();
        }
    }

    GameObject::GameObject(std::set<std::string> tags) : GameObject() {
        this->tags = std::move(tags);
    }

    std::weak_ptr<GameObject> GameObject::getParent() const {
        return parent;
    }

    const std::string& GameObject::getName() const {
        return name;
    }

    void GameObject::setName(const std::string& name) {
        GameObject::name = name;
    }

    void GameObject::setCollisionCategories(uint16_t categoryBits) {
        b2Filter b2Filter = this->getRigidBody()->GetFixtureList()->GetFilterData();
        b2Filter.categoryBits = categoryBits;
        this->getRigidBody()->GetFixtureList()->SetFilterData(b2Filter);
    }

    void GameObject::setCollisionMasks(uint16_t maskBits) {
        b2Filter b2Filter = this->getRigidBody()->GetFixtureList()->GetFilterData();
        b2Filter.maskBits = maskBits;
        this->getRigidBody()->GetFixtureList()->SetFilterData(b2Filter);
    }

    void GameObject::makeStatic() {
        makeStatic(getScene().lock()->getBox2dWorld());
    }

}

