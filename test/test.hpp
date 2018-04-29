#pragma once

#include "base.hpp"
#include "common.hpp"


namespace Base
{
    class TestApp : public AppHandler
    {
      public:
        TestApp()
        {
            mainWindow->setTitle("Base");
            mainWindow->setTargetFramerate(60);
            mainWindow->setBackgroundColor(Color(0.05f, 0.05f, 0.1f));

            //testJSON();
            testSceneInit();

            cout << std::flush;
            launch();
        }
    
      private:
        void loopEvent() override
        {
            testSceneRender();
            testDraw();
        }

        void mouseEvent() override
        {
            testSceneInput();
        }
        
        void keyEvent() override
        {
            testSceneInput();
        }

        // testScene.cpp
        void testSceneInit();
        void testSceneInput();
        void testSceneRender();
        float camAngleXZ,     camAngleY,     camZoom;
        float camGoalAngleXZ, camGoalAngleY, camGoalZoom;
        bool camMove;

        // testjson.cpp
        void testJSON();

        // testdraw.cpp
        void testDraw();
    };
}