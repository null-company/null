#include "Utility.hpp"


namespace null {
    const std::unordered_map<serial::Script::ScriptInstanceCase, std::function<std::unique_ptr<Script>(google::protobuf::Message*)>>
        Utility::scriptSerializationMap = {
                {serial::Script::ScriptInstanceCase::kPlayerAnimation, PlayerAnimation::deserialize},
                {serial::Script::ScriptInstanceCase::kExampleCameraScript, ExampleCameraScript::deserialize},
                {serial::Script::ScriptInstanceCase::kExampleClockedScript, ExampleClockedScript::deserialize},
                {serial::Script::ScriptInstanceCase::kReloadSceneScript, ReloadSceneScript::deserialize},
                {serial::Script::ScriptInstanceCase::kCameraScript, CameraScript::deserialize},
                {serial::Script::ScriptInstanceCase::kButtonScript, ButtonScript::deserialize},
                {serial::Script::ScriptInstanceCase::kClockedScript, ClockedScript::deserialize},
                {serial::Script::ScriptInstanceCase::kExampleScript, ExampleScript::deserialize},
                {serial::Script::ScriptInstanceCase::kStraightWeaponScript, StraightWeaponScript::deserialize},
                {serial::Script::ScriptInstanceCase::kWeaponScript, WeaponScript::deserialize},
                {serial::Script::ScriptInstanceCase::kBulletScript, BulletScript::deserialize},
                {serial::Script::ScriptInstanceCase::kNetworkPlayerScript, NetworkPlayerScript::deserialize},
                {serial::Script::ScriptInstanceCase::kCursorAnimation, CursorAnimation::deserialize},
        };
}