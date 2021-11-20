#include <memory>

#include <box2d/box2d.h>
#include <box2d/b2_math.h>

#include <GameObject.hpp>
#include <Script.hpp>

namespace null {
    GameObject::GameObject():
    isVisible(false) {
        this->children = std::vector<std::shared_ptr<GameObject>>();
        this->tags = std::set<std::string>();
        this->sprite = sf::Sprite();
    }

    GameObject::~GameObject() {
        if (rigidBody) {
            rigidBody->GetWorld()->DestroyBody(rigidBody);
        }
    }

    sf::Sprite& GameObject::getSprite() {
        return this->sprite;
    }

    b2Body* GameObject::getRigidBody() {
        return rigidBody;
    }

    constexpr static int meterToPixel = 100;
    constexpr static float pixelToMeter = 1.0f / static_cast<float>(meterToPixel);

    static const b2Vec2 pixelToMetersVector(const sf::Vector2f& pixelVec) {
        const b2Vec2 metersVec(pixelVec.x * pixelToMeter, pixelVec.y * pixelToMeter);
        return metersVec;
    }

    template<typename T>
    static const sf::Vector2<T> meterToPixelVector(const b2Vec2& meterVec) {
        T x = static_cast<T>(meterVec.x * meterToPixel);
        T y = static_cast<T>(meterVec.y * meterToPixel);

        return sf::Vector2<T>(x, y);
    }

    static const sf::Vector2f getSpriteSize(sf::Sprite& sprite) {
        const sf::Vector2f spriteSize(
                sprite.getTexture()->getSize().x * sprite.getScale().x,
                sprite.getTexture()->getSize().y * sprite.getScale().y);
        return spriteSize;
    }

    void GameObject::setRigidBodyDefPositionBySprite(b2BodyDef& bodyDef) {
        auto spritePosMeters = pixelToMetersVector(sprite.getPosition());
        bodyDef.position.Set(spritePosMeters.x, spritePosMeters.y);
    }

    void GameObject::setShapeAsBoxBySprite(b2PolygonShape& shape) {
        auto spriteSizeMeters = pixelToMetersVector(getSpriteSize(sprite));
        shape.SetAsBox(spriteSizeMeters.x / 2, spriteSizeMeters.y / 2);
    }

    void GameObject::assertTextureIsAttached() {
        if (sprite.getTexture() == nullptr) {
            throw std::invalid_argument("No sprite attached, can't guess rigidbody size");
        }
    }

    void GameObject::makeStatic(b2World& box2dWorld) {
        assertTextureIsAttached();
        detachFromPhysicsWorld();

        b2BodyDef bodyDef;
        setRigidBodyDefPositionBySprite(bodyDef);
        b2Body* rigidBody = box2dWorld.CreateBody(&bodyDef);

        b2PolygonShape shape;
        setShapeAsBoxBySprite(shape);
        rigidBody->CreateFixture(&shape, 0.0f);

        this->rigidBody = rigidBody;
    }

    void GameObject::makeDynamic(b2World& box2dWorld) {
        assertTextureIsAttached();
        detachFromPhysicsWorld();

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        setRigidBodyDefPositionBySprite(bodyDef);

        b2Body* rigidBody = box2dWorld.CreateBody(&bodyDef);

        b2PolygonShape shape;
        setShapeAsBoxBySprite(shape);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.5f;

        rigidBody->CreateFixture(&fixtureDef);
        this->rigidBody = rigidBody;
    }

    void GameObject::detachFromPhysicsWorld() {
        if (rigidBody) {
            rigidBody->GetWorld()->DestroyBody(rigidBody);
            rigidBody = nullptr;
        }
    }

    bool GameObject::getIsVisible() {
        return this->isVisible;
    }

    void GameObject::setIsVisible(bool isVisible){
        this->isVisible = isVisible;
    }

    std::vector<std::weak_ptr<GameObject>> GameObject::getChildren() {
        auto result = std::vector<std::weak_ptr<GameObject>>();
        for (const auto& child_ref : this->children) {
            result.push_back(std::weak_ptr<GameObject>(child_ref));
        }
        return result;
    }

    std::weak_ptr<GameObject> GameObject::getChild(int index) {
        return std::weak_ptr<GameObject>(this->children[index]);
    }

    void GameObject::addChild(const std::shared_ptr<GameObject> &child) {
        children.push_back(child);
    }

    // todo concern pointer leakage
    std::vector<std::unique_ptr<Script>> &GameObject::getScripts() {
        return scripts;
    }

    void GameObject::addScript(std::unique_ptr<Script> script) {
        scripts.push_back(std::move(script));
    }

    void GameObject::addTag(const std::string &str) {
        tags.insert(str);
    }

    bool GameObject::removeTag(const std::string &str) {
        auto result = !tags.find(str)->empty();
        tags.erase(str);
        return result;
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

    void GameObject::setPosition(sf::Vector2f &pos) {
        sprite.setPosition(pos);
        if (rigidBody) {
            b2Vec2 newPosition = pixelToMetersVector(pos);
            rigidBody->SetTransform(newPosition, rigidBody->GetAngle());
        }
    }

    void GameObject::start() {
        for (auto &script : scripts) {
            script->start();
        }
    }

    void GameObject::update() {

        // box2d is expected to have done something,
        // so we have to adjust the sprite
        if (rigidBody) {
            sf::Vector2f newPosition =
                meterToPixelVector<float>(rigidBody->GetPosition());
            sprite.setPosition(newPosition);
        }

        for (auto &script : scripts) {
            script->update();
        }
    }

}
