#include <memory>

#include <box2d/box2d.h>
#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <GameObject.hpp>
#include <Script.hpp>
#include <Scene.hpp>
#include <ResourceManager.hpp>
#include "Utility.hpp"
#include "plog/Log.h"
#include <Serializer.hpp>

namespace null {

    constexpr static int meterToPixel = 100;
    constexpr static float pixelToMeter = 1.0f / static_cast<float>(meterToPixel);
    constexpr static double pi = 3.14159265358979323846;

    GameObject::GameObject(): Entity() {}

    GameObject::GameObject(uint64_t guid) : GameObject() {
        Entity::guid = guid;
    }

    GameObject::~GameObject() {
        if (scene.lock()) {
            if (rigidBody) {
                rigidBody->GetWorld()->DestroyBody(rigidBody);
            }
        }
    }

    std::weak_ptr<GameObject> GameObject::addChild(std::shared_ptr<GameObject>&& child) {
        child->scene = scene;
        child->parent = weak_from_this();
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
        setRigidBodyDefPositionBySprite(bodyDef);
        bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
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
        bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
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

    void GameObject::destroy() {
        parent.lock()->deleteChild(this);
    }

    std::weak_ptr<GameObject> GameObject::getChild(int index) {
        return children[index];
    }

    // TODO(Artem) concern pointer leakage
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
        auto contactList = rigidBody->GetContactList();
        for (b2ContactEdge* edge = contactList; edge; edge = edge->next) {
            auto go1 = reinterpret_cast<GameObject*>(edge->contact->GetFixtureA()->GetBody()->GetUserData().pointer);
            auto go2 = reinterpret_cast<GameObject*>(edge->contact->GetFixtureB()->GetBody()->GetUserData().pointer);
            if (!edge->contact->IsTouching()) {
                return nullptr;
            }
            if (go1 == this) {
                return go2;
            } else {
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

    GameObject::GameObject(uint64_t guid, std::set<std::string> tags) : GameObject(guid) {
        this->tags = std::move(tags);
    }

    GameObject::GameObject(std::set<std::string> tags) : GameObject(Utility::generateGuid()) {
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

    void GameObject::serialize(google::protobuf::Message& msg) const {
        auto new_msg = serial::GameObject();
        new_msg.set_render_layer(renderLayer);
        new_msg.set_guid(guid);
        new_msg.set_visible(visible);
        // TODO: Serialize whole bodies?
        if (rigidBody) {
            if (rigidBody->GetType() == b2_dynamicBody) {
                new_msg.set_box2d_type(serial::DYNAMIC);
                new_msg.set_fixed_rotation(rigidBody->IsFixedRotation());
            } else {
                new_msg.set_box2d_type(serial::STATIC);
            }

        }

        auto s_sprite = serial::Sprite();
        s_sprite.set_texture_path(ResourceManager::getTexturePath(sprite.getTexture()));

        auto s_pos = serial::Vector2f();
        s_pos.set_x(sprite.getPosition().x);
        s_pos.set_y(sprite.getPosition().y);
        *s_sprite.mutable_position() = s_pos;

        auto s_scale = serial::Sprite_Scale();
        s_scale.set_scale_x(sprite.getScale().x);
        s_scale.set_scale_y(sprite.getScale().y);
        *s_sprite.mutable_scale() = s_scale;

        auto s_tr = serial::IntRect();
        s_tr.set_top(sprite.getTextureRect().top);
        s_tr.set_left(sprite.getTextureRect().left);
        s_tr.set_width(sprite.getTextureRect().width);
        s_tr.set_height(sprite.getTextureRect().height);
        *s_sprite.mutable_texture_rect() = s_tr;

        *new_msg.mutable_sprite() = s_sprite;

        for (auto& t: tags) {
            new_msg.add_tags(t);
        }

        for (auto& script: scripts) {
            auto s_script = serial::Script();
            script->serialize(s_script);
            s_script.set_gameobject_guid(guid);
            s_script.set_guid(script->guid);
            new_msg.mutable_children_scripts()->Add(std::move(s_script));
        }

        for (auto& child: children) {
            auto s_child = serial::GameObject();
            child->serialize(s_child);
            new_msg.mutable_children_objects()->Add(std::move(s_child));
        }
        msg.CopyFrom(new_msg);
    }

    std::shared_ptr<GameObject> GameObject::deserialize(const google::protobuf::Message &msg) {
        auto s_go = (const serial::GameObject&) msg;
        auto p_go = std::make_shared<GameObject>();

        LOGD << "Deserializing GameObject with guid: " << s_go.guid();

        // This has to be done so that this object's scripts can get its ref for construciton
        Serializer::currentDeserializationGameObject = p_go.get();

        p_go->guid = s_go.guid();

        p_go->renderLayer = s_go.render_layer();
        p_go->visible = s_go.visible();
        if (!s_go.sprite().texture_path().empty())
            p_go->sprite.setTexture(*ResourceManager::loadTexture(s_go.sprite().texture_path()));
        auto s_tr = s_go.sprite().texture_rect();
        auto texture_rect = sf::IntRect(s_tr.left(), s_tr.top(), s_tr.width(), s_tr.height());
        p_go->sprite.setTextureRect(texture_rect);
        p_go->sprite.setPosition(s_go.sprite().position().x(), s_go.sprite().position().y());
        p_go->sprite.setScale(s_go.sprite().scale().scale_x(), s_go.sprite().scale().scale_y());

        for (auto& t: s_go.tags()) {
            p_go->addTag(t);
        }

        switch (s_go.box2d_type()) {
            case serial::STATIC:
                p_go->makeStatic(Serializer::currentDeserializationScene->getBox2dWorld());
                break;
            case serial::DYNAMIC:
                p_go->makeDynamic(Serializer::currentDeserializationScene->getBox2dWorld());
                p_go->rigidBody->SetFixedRotation(s_go.fixed_rotation());
                break;
            default:
                break;
        }
        for (auto& s: s_go.children_scripts()) {
            LOGD << "Deserializing script with guid " << s_go.guid() << " (" << s.script_instance_case() << ")";
            auto deserializationFunc = Utility::scriptSerializationMap.at(s.script_instance_case());
            auto actualScript = deserializationFunc(dynamic_cast<const google::protobuf::Message&>(s));
            actualScript->guid = s.guid();
            Serializer::deserializedEntitySet.insert(actualScript.get());
            p_go->addScript(std::move(actualScript));
        }

        for (const auto& c: s_go.children_objects()) {
            p_go->addChild(deserialize(c));
        }

        Serializer::deserializedEntitySet.insert(p_go.get());

        return p_go;
    }

}

