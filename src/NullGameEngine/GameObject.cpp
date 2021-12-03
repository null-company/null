#include <memory>

#include <box2d/box2d.h>
#include <box2d/b2_math.h>

#include <GameObject.hpp>
#include <Script.hpp>
#include <ResourceManager.hpp>
#include <google/protobuf/message.h>
#include <Scene.hpp>

namespace null {

    constexpr static int meterToPixel = 100;
    constexpr static float pixelToMeter = 1.0f / static_cast<float>(meterToPixel);
    constexpr static double pi = 3.14159265358979323846;
    static uint32_t global_id = 0;

    GameObject::GameObject() : visible(false), id(global_id++) {};

    GameObject::~GameObject() {
        if (scene.lock()) {
            if (rigidBody) {
                rigidBody->GetWorld()->DestroyBody(rigidBody);
            }
        }
    }

    std::weak_ptr<Scene> GameObject::getScene() {
        return scene;
    }

    sf::Sprite &GameObject::getSprite() {
        return sprite;
    }

    b2Body *GameObject::getRigidBody() {
        return rigidBody;
    }

    template<typename T>
    static const b2Vec2 pixelToMetersVector(const sf::Vector2<T> &pixelVec) {
        const b2Vec2 metersVec(pixelVec.x * pixelToMeter, pixelVec.y * pixelToMeter);
        return metersVec;
    }

    template<typename T>
    static const sf::Vector2<T> meterToPixelVector(const b2Vec2 &meterVec) {
        T x = static_cast<T>(meterVec.x * meterToPixel);
        T y = static_cast<T>(meterVec.y * meterToPixel);

        return sf::Vector2<T>(x, y);
    }

    static const sf::Vector2u getSpriteSize(sf::Sprite &sprite) {
        const sf::Vector2u spriteSize(
                sprite.getTextureRect().getSize().x * sprite.getScale().x,
                sprite.getTextureRect().getSize().y * sprite.getScale().y);
        return spriteSize;
    }

    void GameObject::setRigidBodyDefPositionBySprite(b2BodyDef &bodyDef) {
        auto spritePosMeters = pixelToMetersVector(sprite.getPosition());
        bodyDef.position.Set(spritePosMeters.x, spritePosMeters.y);
    }

    void GameObject::setShapeAsBoxBySprite(b2PolygonShape &shape) {
        auto spriteSizeMeters = pixelToMetersVector(getSpriteSize(sprite));
        b2Vec2 boxCenter(spriteSizeMeters.x / 2, spriteSizeMeters.y / 2);
        shape.SetAsBox(spriteSizeMeters.x / 2, spriteSizeMeters.y / 2, boxCenter, 0.0f);
    }

    void GameObject::assertSpriteHasSize() {
        if (sprite.getTextureRect().height == 0 || sprite.getTextureRect().width == 0) {
            throw std::invalid_argument("No sprite attached, can't guess rigidbody size");
        }
    }

    void GameObject::makeStatic(b2World &box2dWorld) {
        assertSpriteHasSize();
        detachFromPhysicsWorld();

        b2BodyDef bodyDef;
        setRigidBodyDefPositionBySprite(bodyDef);
        b2Body *rigidBody = box2dWorld.CreateBody(&bodyDef);

        b2PolygonShape shape;
        setShapeAsBoxBySprite(shape);
        rigidBody->CreateFixture(&shape, 0.0f);

        this->rigidBody = rigidBody;
    }

    void GameObject::makeDynamic(b2World &box2dWorld) {
        assertSpriteHasSize();
        detachFromPhysicsWorld();

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        setRigidBodyDefPositionBySprite(bodyDef);

        b2Body *rigidBody = box2dWorld.CreateBody(&bodyDef);

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
        for (const auto &child_ref: children) {
            result.push_back(std::weak_ptr<GameObject>(child_ref));
        }
        return result;
    }

    std::weak_ptr<GameObject> GameObject::getChild(int index) {
        return std::weak_ptr<GameObject>(children[index]);
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

    const sf::Transform &GameObject::getTransform() {
        return sprite.getTransform();
    }

    const sf::Vector2f &GameObject::getPosition() {
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
        for (auto &script: scripts) {
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
            sprite.setRotation(rigidBody->GetAngle() * (180.0 / pi));
        }

        for (auto &script: scripts) {
            script->update();
        }
    }

    serial::GameObject GameObject::prefabSerialize() {
        serial::GameObject serializeGameObject;
        serial::BasicGameObject &basicGameObject = *serializeGameObject.mutable_basic_game_object();
        if (rigidBody->GetMass() > 0) { // This I try to check whether it is dynamic or not
            basicGameObject.set_box2d_type(serial::Box2DType::DYNAMIC);
        } else {
            basicGameObject.set_box2d_type(serial::Box2DType::STATIC);
        }
        basicGameObject.set_render_layer(static_cast<serial::RenderLayer>(renderLayer));
        basicGameObject.set_visible(visible);
        serial::Sprite &sprite1 = *basicGameObject.mutable_sprite();
        sprite1.mutable_scale()->set_scale_x(this->sprite.getScale().x);
        sprite1.mutable_scale()->set_scale_y(this->sprite.getScale().y);
        sprite1.mutable_position()->set_x(this->sprite.getPosition().x);
        sprite1.mutable_position()->set_y(this->sprite.getPosition().y);

        //TODO how it can be done in this way. So we need to store Picture ID near the Texture
        sprite1.set_texture_path("");
        for (auto &childGameObject: children) {
            *basicGameObject.add_children_objects() = childGameObject->prefabSerialize();
        }
        return serializeGameObject;
    }

    //TODO I suppose it should be a little more compact
    std::shared_ptr<GameObject>
    GameObject::prefabDeserialize(const serial::BasicGameObject &serialized) {
        std::shared_ptr<GameObject> gameObject = std::make_unique<GameObject>();
        //Sprite deserialize
        if (serialized.has_sprite()) {
            if (!serialized.sprite().texture_path().empty()) {
                auto texture = ResourceManager::loadTexture(serialized.sprite().texture_path());
                gameObject->getSprite().setTexture(*texture);
            }
            if (serialized.sprite().has_scale()) {
                const serial::Sprite::Scale &scale = serialized.sprite().scale();
                gameObject->getSprite().setScale(scale.scale_x(), scale.scale_y());
            }
            if (serialized.sprite().has_position()) {
                const serial::Position &position = serialized.sprite().position();
                gameObject->getSprite().setPosition(position.x(), position.y());
            }
            gameObject->renderLayer = static_cast<RenderLayer>(serialized.render_layer());
            gameObject->visible = serialized.visible();
        }
        //Deserialize window
        if (gameObject->getScene().lock()) {
            if (serialized.box2d_type() == serial::Box2DType::STATIC) {
                gameObject->makeStatic(gameObject->getScene().lock()->getBox2dWorld());
            } else {
                gameObject->makeDynamic(gameObject->getScene().lock()->getBox2dWorld());
            }
        }
        //Add tags
        for (auto &tag: serialized.tags()) {
            gameObject->addTag(tag);
        }
        //Script deserialize
        for (auto &children_object: serialized.children_scripts()) {
            gameObject->addScript(prefabDeserialize(children_object));
        }

        //Recursive deserialize
        for (auto &children_serialized_object: serialized.children_objects()) {
            auto child = prefabDeserialize(children_serialized_object);
            child->parent = std::weak_ptr<GameObject>(gameObject);
            gameObject->addChild(child);
        }

        return gameObject;
    }

    std::shared_ptr<GameObject>
    GameObject::prefabDeserialize(const serial::GameObject &serialized) {
        switch (serialized.game_object_instance_case()) {
            case serial::GameObject::kBasicGameObject:
                return prefabDeserialize(serialized.basic_game_object());
                break;
            case serial::GameObject::GAME_OBJECT_INSTANCE_NOT_SET:
                break;
        }
    }
}

