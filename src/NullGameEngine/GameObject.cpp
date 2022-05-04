#include <memory>

#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include <box2d/b2_body.h>

#include <GameObject.hpp>
#include <Script.hpp>
#include <Scene.hpp>

#include <Utility.hpp>

namespace null {

    constexpr static int meterToPixel = 100;
    constexpr static float pixelToMeter = 1.0f / static_cast<float>(meterToPixel);
    constexpr static double pi = 3.14159265358979323846;

    GameObject::GameObject(uint64_t guid) : visible(false), guid(guid) {}

    GameObject::GameObject() : visible(false), guid(Utility::generateGuid()) {}

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

    sf::Sprite& GameObject::getSprite() {
        return sprite;
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

    std::weak_ptr<GameObject> GameObject::getChild(int index) {
        return children[index];
    }

    // todo concern pointer leakage
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

    GameObject::GameObject(uint64_t guid, std::set<std::string> tags) : GameObject(guid) {
        this->tags = std::move(tags);
    }

    GameObject::GameObject(std::set<std::string> tags) : GameObject(Utility::generateGuid()) {
        this->tags = std::move(tags);
    }

    std::weak_ptr<GameObject> GameObject::getParent() const {
        return parent;
    }

    uint64_t GameObject::getGuid() {
        return guid;
    }

}

