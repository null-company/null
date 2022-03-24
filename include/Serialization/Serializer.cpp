#include <fstream>
#include "Serializer.hpp"

namespace null{
    void Serializer::serializeSceneToFile(null::Scene* scene, const std::string& filename) {
        auto s_scene = new serial::Scene;
        scene->serialize(s_scene);
        auto ofstream = std::ofstream(filename);
        s_scene->SerializeToOstream(&ofstream);
        ofstream.close();
    }

    Scene Serializer::getSceneFromFile(const std::string& filename) {
        auto scene = Scene();
        auto ifstream = std::ifstream(filename);
        auto s_scene = serial::Scene();
        auto ss = std::string();
        s_scene.ParseFromIstream(&ifstream);
        scene.deserialize(&s_scene);
        ifstream.close();
        return scene;
    }

}