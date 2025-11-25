#include "ui/screen/LoadingScreen.hpp"
#include "Application.hpp"
#include "utils/Lang.hpp"
#include "utils/Utils.hpp"

namespace Screen {
    LoadingScreen::LoadingScreen(Main::Application *app) : app_(app) {
        // Create "static" elements
        this->bg_ = new Aether::Rectangle(0, 0, 1280, 720);
        this->bg_->setColour(this->app_->theme()->bg());
        this->addElement(this->bg_);

        // Create title text and center it manually
        std::pair<int, int> title = Aether::Text::getDimensions("common.loading.title"_lang, 24);
        this->title_ = new Aether::Text(640 - title.first/2, 250, "common.loading.title"_lang, 24);
        this->title_->setColour(app_->theme()->text());
        this->addElement(this->title_);

        this->progressBox_ = new CustomScreen::SimpleProgressBox();
        this->progressBox_->setBackgroundColour(app_->theme()->altBG());
        this->progressBox_->setBarBackgroundColour(app_->theme()->mutedLine());
        this->progressBox_->setBarForegroundColour(app_->theme()->accent());
        this->progressBox_->setTextColour(app_->theme()->text());
        //this->progressBox_->setHeading("common.loading.init"_lang);
        this->progressBox_->setValue(0.0);
        this->addElement(this->progressBox_);
    }

    LoadingScreen::~LoadingScreen() {}

    void LoadingScreen::setProgress(float progress) {
        if (progress < 0) {
            //this->progressBox_->setHeading("common.loading.failed"_lang);
            this->progressBox_->setValue(0.0);
            return;
        }

        // progress should be 0.0f ~ 1.0f, and progressBox vaule should be percentage range (0.0f - 100.0f)
        this->progressBox_->setValue(progress * 100.0);

        if (progress >= 1.0f) {
            this->progressBox_->setValue(100.0);
            this->progressBox_->close();
        }
        /*
        if (progress < 0.05f) ;//this->progressBox_->setHeading("common.loading.init"_lang);
        else if (progress < 0.35f) ;//this->progressBox_->setHeading("common.loading.readPDM"_lang);
        else if (progress < 0.5f) ;//this->progressBox_->setHeading("common.loading.readImport"_lang);
        else if (progress < 0.75f) ;//this->progressBox_->setHeading("common.loading.mergeData"_lang);
        else if (progress < 1.0f) ;//this->progressBox_->setHeading("common.loading.finish"_lang);
        else {
            //this->progressBox_->setHeading("common.loading.done"_lang);
            this->progressBox_->setValue(100.0);
            this->progressBox_->close();
        }*/
    }
}