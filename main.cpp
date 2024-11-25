#include "GameScene.hpp"

int main()
{
    Logger::showConsole(true);
    Renderer::setBackgroundColor(Color::LightBlue);
    Renderer::showFps(true);
    Renderer::showBodyShapes();
    if (Game::init("Warcraft-SovietUnion"))//500-800
    {     
        MoveWindow(Window::getHWnd(), 900, 100, 500 ,800, false);
        auto gs = gcnew GamePlay;
        SceneManager::enter(gs);
        Game::start();
    }
    Game::destroy();
    return 0;
}