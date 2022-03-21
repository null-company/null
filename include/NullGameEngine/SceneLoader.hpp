#include <filesystem>

#include <NullGameEngine.hpp>
#include <RenderLayer.hpp>

namespace null {

    class SceneLoader {
        private:
            SceneLoader();
            static std::shared_ptr<Scene> getDemoScene();
            static std::shared_ptr<Scene> getMenuScene();
            static std::shared_ptr<Scene> getPlayScene();
            static std::shared_ptr<Scene> getCreateRoomScene();
            static std::shared_ptr<Scene> getJoinRoomScene();
            static std::shared_ptr<Scene> getGameScene();

        public:
            static void loadSceneFromFile(std::filesystem::path);
            static void changeScene(std::filesystem::path);

    };

}

