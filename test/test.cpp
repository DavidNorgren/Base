#include <base.hpp>

#include "common.hpp"
#include "testjson.hpp"
#include "testrender.hpp"
#include "testdraw.hpp"


extern char resData[] asm("_binary_res_zip_start");
extern char resSize[] asm("_binary_res_zip_size");


namespace Base
{
    class TestApp : public AppHandler
    {
      public:
        TestApp();
    
      private:
        void loopEvent() override;
        void mouseEvent() override {};
        void keyEvent() override {};
        void resizeEvent() override {};
    };
    
    TestApp::TestApp() : AppHandler::AppHandler(resData, (uint)resSize)
    {
        mainWindow->backgroundColor = Colors::LIGHT_GRAY;
        //testJSON(this);

        cout << std::flush;
        testRenderInit(this);
        launch();
    }

    void TestApp::loopEvent()
    {
        testRender(this);
        //testDraw(this);
    }
}


int main()
{
    new Base::TestApp();
    return 0;
}