#include <base.hpp>

#include "testjson.hpp"
#include "testwindow.hpp"


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
        Mat4x4f a, b;
        a * b;
        a *= b;
        Vec3f z;
        cout << a;
        // JSON testing
        testJSON(resHandler);
        cout << std::flush;
        launch();
    }

    void TestApp::loopEvent()
    {
        // Window testing
        testWindow(resHandler);
    }
}


int main()
{
    new Base::TestApp();
    return 0;
}