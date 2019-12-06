#include "screenmanager.hpp"

ScreenManager * ScreenManager::instance = nullptr;

ScreenManager::ScreenManager() {
    this->changed = false;
    this->looping = true;
    this->screen_ptr = nullptr;
    this->selection = nullptr;
    this->selection_active = false;
    this->touch_active = true;
}

ScreenManager * ScreenManager::getInstance() {
    // Create instance if it doesn't exist
    if (!ScreenManager::instance) {
        ScreenManager::instance = new ScreenManager();
    }
    return ScreenManager::instance;
}

void ScreenManager::setScreen(UI::Screen * s) {
    if (this->screen_ptr != nullptr) {
        delete this->screen_ptr;
    }
    this->screen_ptr = s;
    this->changed = true;
}

void ScreenManager::pushScreen() {
    this->stack.push(this->screen_ptr);
    this->screen_ptr = nullptr;
}

void ScreenManager::popScreen() {
    if (this->screen_ptr != nullptr) {
        delete this->screen_ptr;
        this->screen_ptr = nullptr;
    }
    if (this->stack.size() > 0) {
        this->screen_ptr = this->stack.top();
        this->stack.pop();
    }
}

void ScreenManager::createSelection(std::string t, std::vector<std::string> v) {
    if (this->selection != nullptr) {
        delete this->selection;
    }
    this->selection = new UI::Selection(&this->touch_active, t, v);
    this->selection_active = true;
}

int ScreenManager::getSelectionValue() {
    int val = -2;
    if (this->selection_active && this->selection->getChosen() != -1) {
        val = this->selection->getChosen();
        delete this->selection;
        this->selection = nullptr;
        this->selection_active = false;
    }

    return val;
}

void ScreenManager::event() {
    if (this->selection_active) {
        this->selection->event();
        return;
    }

    if (this->screen_ptr != nullptr && !this->changed) {
        this->screen_ptr->event();
    }
}

void ScreenManager::update(uint32_t dt) {
    if (this->selection_active) {
        this->selection->update(dt);
        if (this->selection->closed()) {
            delete this->selection;
            this->selection = nullptr;
            this->selection_active = false;
        }
    }

    if (this->screen_ptr != nullptr && !this->changed) {
        this->screen_ptr->update(dt);
    }
}

void ScreenManager::draw() {
    if (this->screen_ptr != nullptr && !this->changed) {
        this->screen_ptr->draw();
    }

    if (this->selection_active) {
        SDLHelper::setColour(SDL_Color{0, 0, 0, 150});
        SDLHelper::drawRect(0, 0, WIDTH, HEIGHT-this->selection->getY());
        this->selection->draw();
    }
    this->changed = false;
}

bool ScreenManager::loop() {
    return this->looping;
}

void ScreenManager::stopLoop() {
    this->looping = false;
}

void ScreenManager::free() {
    do {
        this->popScreen();
    } while (this->screen_ptr != nullptr);

    // Delete selection
    if (this->selection != nullptr) {
        delete this->selection;
    }
}