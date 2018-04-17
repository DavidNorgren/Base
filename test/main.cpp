#include "base.hpp"

class App : public Base::AppHandler
{
public:
    App();
    void loopEvent();
    void mouseEvent();
    void keyEvent();
    void resizeEvent();
};

/*int main() 
{
    new App();
    return 0;
}*/