#ifndef BENGINE_CURSES_LOOP_hpp
#define BENGINE_CURSES_LOOP_hpp

#include <chrono>

#include "bengine_curses_window.hpp"

namespace bengine {
    class curses_loop {
        protected:
            std::chrono::system_clock::time_point epoch;

            // get the duration of how long the loop has been running in milliseconds
            unsigned long long get_ticks() const {
                return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->epoch).count();
            }

            // \brief How long the loop has been active (in seconds)
            long double time = 0.0;

            // \brief How long each computation frame should take (in seconds)
            double delta_time = 0.01;

            // \brief A replacement for a monitor's refresh rate, lower values decrease performance impacts but also decrease program responsiveness; overall timing should be constant though
            unsigned short refresh_rate = 5;

            // \brief Whether the loop is running or not
            bool loop_running = true;
            // \brief Whether the to update the visuals or not (saves on performance when nothing visual is happening)
            bool visuals_changed = true;

            int input_character = ERR;

            virtual void handle_event() = 0;
            virtual void compute() = 0;
            virtual void render() = 0;

        public:
            curses_loop() {}
            ~curses_loop() {}

            int run() {
                this->epoch = std::chrono::high_resolution_clock::now();

                unsigned long long start_ticks = 0;
                unsigned long long frame_ticks = 0;
                long double current_time = this->get_ticks() * 0.01;
                long double new_time = 0.0;
                double frame_time = 0.0;
                double accumulator = 0.0;

                setlocale(LC_ALL, "");
                initscr();
                noecho();
                curs_set(0);
                nodelay(stdscr, true);

                while (this->loop_running) {
                    start_ticks = this->get_ticks();
                    new_time = this->get_ticks() * 0.01;
                    frame_time = new_time - current_time;
                    current_time = new_time;
                    accumulator += frame_time;

                    while (accumulator >= this->delta_time) {
                        if ((this->input_character = getch()) != ERR) {
                            this->handle_event();
                        }
                        this->compute();
                        this->time += this->delta_time;
                        accumulator -= this->delta_time;
                    }

                    if (this->visuals_changed) {
                        this->visuals_changed = false;
                        this->render();
                    }

                    if ((frame_ticks = this->get_ticks() - start_ticks) < (unsigned long long)(1000 / this->refresh_rate)) {
                        napms(1000 / this->refresh_rate - frame_ticks);
                    }
                }

                endwin();
                return 0;
            }
    };
}

#endif // BENGINE_CURSES_LOOP_hpp
