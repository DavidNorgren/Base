#include <base.hpp>

#include "common.hpp"
#include "testjson.hpp"
#include "testrender.hpp"
#include "testdraw.hpp"



namespace Base
{
    class TestApp : public AppHandler
    {
      public:
        TestApp();
    
      private:
        void loopEvent() override;
    };
    
    TestApp::TestApp() : AppHandler::AppHandler()
    {
        mainWindow->setTitle("Base");
        mainWindow->setTargetFramerate(60);
        mainWindow->setBackgroundColor(Colors::LIGHT_GRAY);
        //testJSON(this);

        testRenderInit(this);

        cout << std::flush;
        launch();
    }

    void TestApp::loopEvent()
    {
        resHandler->checkReload();
        testRender(this);
        testDraw(this);
    }
}


int main()
{
    new Base::TestApp();
    return 0;
}