#ifndef BENGINE_CURSES_LOOP_hpp
#define BENGINE_CURSES_LOOP_hpp

#include <chrono>
#include <cmath>

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

            unsigned short terminal_cols = COLS;
            unsigned short terminal_rows = LINES;

            unsigned short minimum_cols = 250;
            unsigned short minimum_rows = 20;

            virtual void handle_event() = 0;
            virtual void compute() = 0;
            virtual void render() = 0;

        private:
            // whether the tests required for ncurses to run as intented are passed or not
            bool can_support_colors = true;

        public:
            curses_loop() {
                setlocale(LC_ALL, "");
                initscr();
                noecho();
                curs_set(0);
                nodelay(stdscr, true);

                if (!can_change_color() || !has_colors()) {
                    this->can_support_colors = false;
                    return;
                }

                start_color();

                init_color( 0,   0,   0,   0);    // black
                init_color( 1, 999, 999, 999);    // white
                init_color( 2, 666, 666, 666);    // light gray
                init_color( 3, 333, 333, 333);    // dark gray
                init_color( 4, 459, 235,  74);    // brown
                init_color( 5, 999,   0,   0);    // red
                init_color( 6, 451,   0,   0);    // maroon
                init_color( 7, 999, 451,   0);    // orange
                init_color( 8, 999, 999,   0);    // yellow
                init_color( 9,   0, 999,   0);    // lime
                init_color(10,   0, 451,   0);    // green
                init_color(11,   0, 999, 999);    // cyan
                init_color(12,   0, 451, 451);    // teal
                init_color(13,   0,   0, 999);    // blue
                init_color(14, 999,   0, 999);    // magenta
                init_color(15, 451,   0, 451);    // purple

                for (unsigned char pair = 1; pair < 16; pair++) {    // pair initiation starts at 1 bc ncurses doesn't let you change color pair 0
                    init_pair(pair, pair, 0);    // make 15 color pairs with each non-black color as foreground and black as background
                }
            }
            ~curses_loop() {
                endwin();
            }

            int run() {
                // If the terminal emulator that the user
                if (!can_support_colors) {
                    nodelay(stdscr, false);
                    mvprintw(LINES / 2 - 1, COLS / 2 - 36, "[ERROR] Your terminal emulator doesn't support full color capabilites.");
                    mvprintw(LINES / 2, COLS / 2 - 36, "Please try running this program on a different emulator with color support.");
                    mvprintw(LINES / 2 + 1, COLS / 2 - 36, "Press any key to continue...");
                    getch();
                    clear();
                    return -1;
                }

                this->epoch = std::chrono::high_resolution_clock::now();

                unsigned long long start_ticks = 0;
                unsigned long long frame_ticks = 0;
                long double current_time = this->get_ticks() * 0.01;
                long double new_time = 0.0;
                double frame_time = 0.0;
                double accumulator = 0.0;

                while (this->loop_running) {
                    getmaxyx(stdscr, this->terminal_rows, this->terminal_cols);

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

                    if (this->terminal_cols < this->minimum_cols || this->terminal_rows < this->minimum_rows) {
                        short r1, g1, b1, r2, g2, b2, r3, g3, b3, r4, g4, b4;
                        color_content( 0, &r1, &g1, &b1);
                        color_content( 1, &r2, &g2, &b2);
                        color_content( 5, &r3, &g3, &b3);
                        color_content(10, &r4, &g4, &b4);
                        init_color( 0,   0,   0,   0);
                        init_color( 1, 999, 999, 999);
                        init_color( 5, 999,   0,   0);
                        init_color(10,   0, 999,   0);

                        short fg1, bg1, fg2, bg2, fg3, bg3;
                        pair_content( 1, &fg1, &bg1);
                        pair_content( 5, &fg2, &bg2);
                        pair_content(10, &fg3, &bg3);
                        init_pair( 1,  1, 0);
                        init_pair( 5,  5, 0);
                        init_pair(10, 10, 0);

                        while (this->terminal_cols < this->minimum_cols || this->terminal_rows < this->minimum_rows) {
                            attron(COLOR_PAIR(1));

                            clear();
                            mvprintw(this->terminal_rows / 2 - 2, this->terminal_cols / 2 - 12, "Terminal Size Too Small");
                            mvprintw(this->terminal_rows / 2 - 1, this->terminal_cols / 2 - 9 - static_cast<unsigned char>(std::log10(this->terminal_cols)) - static_cast<unsigned char>(std::log10(this->terminal_rows)), "Width = %d Height = %d", this->terminal_cols, this->terminal_rows);
                            mvprintw(this->terminal_rows / 2 + 1, this->terminal_cols / 2 - 13, "Needed For Current Config:");
                            mvprintw(this->terminal_rows / 2 + 2, this->terminal_cols / 2 - 9 - static_cast<unsigned char>(std::log10(this->minimum_cols)) - static_cast<unsigned char>(std::log10(this->minimum_rows)), "Width = %d Height = %d", this->minimum_cols, this->minimum_rows);

                            attron(COLOR_PAIR(this->terminal_cols < this->minimum_cols ? 5 : 10));
                            mvprintw(this->terminal_rows / 2 - 1, this->terminal_cols / 2 - 2 - static_cast<unsigned char>(std::log10(this->terminal_cols)), "%d", this->terminal_cols);

                            attron(COLOR_PAIR(this->terminal_rows < this->minimum_rows ? 5 : 10));
                            mvprintw(this->terminal_rows / 2 - 1, this->terminal_cols / 2 + 10 - static_cast<unsigned char>(std::log10(this->terminal_rows)), "%d", this->terminal_rows);

                            refresh();

                            attroff(COLOR_PAIR(this->terminal_rows < this->minimum_rows ? 5 : 10));
                            getmaxyx(stdscr, this->terminal_rows, this->terminal_cols);
                        }
                        clear();
                        this->visuals_changed = true;

                        init_color( 0, r1, g1, b1);
                        init_color( 1, r2, g2, b2);
                        init_color( 5, r3, g3, b3);
                        init_color(10, r4, g4, b4);
                        init_pair( 1, fg1, bg1);
                        init_pair( 5, fg2, bg2);
                        init_pair(10, fg3, bg3);
                    }
                }
                return 0;
            }
    };
}

#endif // BENGINE_CURSES_LOOP_hpp
