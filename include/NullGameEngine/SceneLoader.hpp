#include <filesystem>

#include <NullGameEngine.hpp>
#include <RenderLayer.hpp>

namespace null {

    class SceneLoader {
    private:
        SceneLoader() = default;

        static std::shared_ptr<Scene> getDemoScene();

        static std::shared_ptr<Scene> getMenuScene();

        static std::shared_ptr<Scene> getPlayScene();

        static std::shared_ptr<Scene> getCreateRoomScene();

        static std::shared_ptr<Scene> getJoinRoomScene();

        static std::shared_ptr<Scene> getGameScene();

        static std::shared_ptr<Scene> getNetworkDemoClientScene();

        static std::shared_ptr<Scene> getNetworkDemoServerScene();
        static std::shared_ptr<Scene> getGameServerScene();
        static std::shared_ptr<Scene> getRoomCreateConnectScene();

    public:
        enum HostType {
            Server, Client
        };
        static void loadSceneFromFile(const std::filesystem::path& path);

        static void changeScene(const std::filesystem::path& path, std::shared_ptr<void> newContext = nullptr);

        /**
         * Describes data that was provided between changing scenes
         */
        static const void* getContext();

    private:
        static std::shared_ptr<Scene> mainGame(HostType ht);
    private:
        static std::shared_ptr<void> context;
    };

    class UnknownSceneException : public std::exception {
    };

}
