#ifndef ALLACTIVITY_HPP
#define ALLACTIVITY_HPP

#include <future>
#include "Aether/Aether.hpp"
#include "ui/element/SortedList.hpp"
#include "Types.hpp"

// Forward declare classes which are only used as pointers
namespace Main {
    class Application;
};
namespace NX {
    struct RecentPlayStatistics;
    struct PlayStatistics;
};

namespace Screen {
    // AllActivity is the screen which shows all of a user's played titles
    class AllActivity : public Aether::Screen {
        private:
            // Pointer to application class
            Main::Application * app;

            // Elements
            Aether::Text * heading;
            Aether::Text * hours;
            Aether::Image * image;
            CustomElm::SortedList * list;
            Aether::Menu * menu;
            Aether::PopupList * sortOverlay;
            Aether::Element * updateElm;

            std::future<void> updateThread;

            // Setups the sort overlay with entries
            void setupOverlay();

            // Updates the activity list based on current data
            void updateActivity();

        public:
            // Constructor requires pointer to application class
            AllActivity(Main::Application *);

            // Creates "static" elements (which don't change between loads)
            // onLoad() is called after constructor
            void onLoad();

            // Destroys elements created in onLoad() and createListElements()
            void onUnload();

            // Destructor
            ~AllActivity();
    };
};

#endif