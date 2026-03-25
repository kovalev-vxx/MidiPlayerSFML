#include <iostream>
#include "App/App.hpp"


int main(int argc, char const** argv)
{
    App app = App(argc, argv);
    app.config();
    app.start();
}
