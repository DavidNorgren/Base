#pragma once

#include "base.hpp"


namespace Base
{
    class TestApp : public AppHandler
    {
      public:
        TestApp()
        {
            mainWindow->setTitle("BaseTest");
            mainWindow->setTargetFramerate(60);

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
        float camAngleXZ[2],     camAngleY[2],     camZoom[2];
        float camGoalAngleXZ[2], camGoalAngleY[2], camGoalZoom[2];
        int camMove;
        Camera cameras[2];
        Surface surfaces[2];

        // testjson.cpp
        void testJSON();

        // testdraw.cpp
        void testDraw();
    };
}