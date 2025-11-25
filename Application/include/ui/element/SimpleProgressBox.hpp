#ifndef SIMPLEPROGRESSBOX_HPP
#define SIMPLEPROGRESSBOX_HPP

#include "Aether/Aether.hpp"

namespace CustomScreen {
    class SimpleProgressBox : public Aether::Screen {
        private:
            // Background rectangle
            Aether::Rectangle * rect;

            // Message
            //Aether::TextBlock * heading;

            // Progress bar
            Aether::RoundProgressBar * bar;

            // Progress bar value
            //Aether::Text * value;

        public:
            // Constructs a new (blank) SimpleProgressBox
            SimpleProgressBox();

            //void setHeading(const std::string &);
            void setValue(const double);

            void setBackgroundColour(const Aether::Colour);
            void setBarBackgroundColour(const Aether::Colour);
            void setBarForegroundColour(const Aether::Colour);
            void setTextColour(const Aether::Colour);

            // Close method - hides the progress box
            void close();
    };
};

#endif