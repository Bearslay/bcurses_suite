#include "bengine_curses.hpp"

class test : public bengine::curses_loop {
    private:
        bengine::curses_window window;

        unsigned short repeats1 = 0, repeats2 = 0;
    
        void handle_event() {
            switch (this->input_character) {
                case 'q':
                    this->loop_running = false;
                    break;
                // case 'a':
                //     this->window.write_character(repeats1, 0, L'a', bengine::curses_window::make_write_args(bengine::curses_window::write_arg_options::COLOR | bengine::curses_window::write_arg_options::ATTRIBUTES, {bengine::curses_window::preset_colors::RED, bengine::curses_window::cell_attributes::BOLD}));
                //     this->window.write_character(0, 1, this->window.check_cell_attribute_states(0, 0, bengine::curses_window::cell_attributes::BOLD | bengine::curses_window::cell_attributes::ITALIC) ? L'1' : L'0');
                //     repeats1++;
                //     this->visuals_changed = true;
                //     break;
                // case 'b':
                //     this->window.write_string(this->window.write_string(4, 2 + repeats2, L"hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello", bengine::curses_window::make_write_args(bengine::curses_window::write_arg_options::COLOR | bengine::curses_window::write_arg_options::WRAPPING_WIDTH | bengine::curses_window::write_arg_options::WRAPPING_MODE, {bengine::curses_window::preset_colors::MAROON, 45, bengine::curses_window::wrapping_modes::INDENTED})), L":)");
                //     repeats2++;
                //     this->visuals_changed = true;
                //     break;
                // case 'l':
                //     this->window.draw_horizontal_line(10, 10, 10, bengine::curses_window::LIGHT_SQUARE | bengine::curses_window::NO_DASH);
                //     this->window.draw_horizontal_line(10, 9, 5, bengine::curses_window::DOUBLED_BOTH | bengine::curses_window::DOUBLE_DASH);
                //     this->visuals_changed = true;
                //     break;
            }
        }
        void compute() {}
        void render() {
            this->window.apply_to_screen();
        }

    public:
        test() {
            // this->window.write_character(19, 1, L'┈');
            this->window.write_character(20, 2, L'┆');
            this->window.write_character(25, 0, L'├');
            this->window.write_character(27, 2, L'╣');
            this->window.write_character(29, 0, L'╽');
            this->window.draw_horizontal_line(20, 1, 10, bengine::curses_window::DOUBLED_BOTH | bengine::curses_window::TRIPLE_DASH, 5, {}, true);
            this->window.draw_vertical_line(20, 5, 15, bengine::curses_window::HEAVY_VERTICAL | bengine::curses_window::TRIPLE_DASH, 5);

            this->window.draw_horizontal_line(30, 10, 10, bengine::curses_window::LIGHT_SQUARE);
            this->window.draw_horizontal_line(30, 19, 10, bengine::curses_window::LIGHT_SQUARE);
            this->window.draw_vertical_line(30, 10, 10, bengine::curses_window::LIGHT_SQUARE);
            this->window.draw_vertical_line(39, 10, 10, bengine::curses_window::LIGHT_SQUARE);
            this->window.draw_vertical_line(50, 25, 1, bengine::curses_window::HEAVY_BOTH | bengine::curses_window::TRIPLE_DASH);
            this->window.draw_horizontal_line(50, 24, 1, bengine::curses_window::LIGHT_ROUNDED | bengine::curses_window::QUADRUPLE_DASH);
            this->window.draw_horizontal_line(39, 15, 20, bengine::curses_window::DOUBLED_HORIZONTAL);

            this->window.draw_vertical_line(this->window.draw_horizontal_line(this->window.draw_vertical_line(this->window.draw_horizontal_line(100, 50, 20, bengine::curses_window::HEAVY_HORIZONTAL), 10, bengine::curses_window::HEAVY_HORIZONTAL), -20, bengine::curses_window::HEAVY_HORIZONTAL), -10, bengine::curses_window::HEAVY_HORIZONTAL);
            this->visuals_changed = true;
        }
        ~test() {}
};

int main(/*int argc, char* argv[]*/) {
    test t;
    t.run();
    return 0;
}
