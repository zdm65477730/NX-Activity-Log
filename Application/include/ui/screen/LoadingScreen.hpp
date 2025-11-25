#ifndef LOADINGSCREEN_HPP
#define LOADINGSCREEN_HPP

#include "Aether/Aether.hpp"
#include "ui/element/SimpleProgressBox.hpp"

namespace Main {
    class Application;
};

namespace Screen {
    class LoadingScreen : public Aether::Screen {
        private:
            Main::Application * app_;
            Aether::Rectangle * bg_;
            Aether::Text * title_;
            CustomScreen::SimpleProgressBox * progressBox_;

        public:
            LoadingScreen(Main::Application *);
            ~LoadingScreen();
            void setProgress(float);
    };
};

#endif