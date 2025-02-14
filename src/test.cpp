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
                case 'a':
                    this->window.write_character(repeats1, 0, L'a', bengine::curses_window::make_write_args(bengine::curses_window::write_arg_options::COLOR | bengine::curses_window::write_arg_options::ATTRIBUTES, {bengine::curses_window::preset_colors::RED, bengine::curses_window::cell_attributes::BOLD}));
                    this->window.write_character(0, 1, this->window.check_cell_attribute_states(0, 0, bengine::curses_window::cell_attributes::BOLD | bengine::curses_window::cell_attributes::ITALIC) ? L'1' : L'0');
                    repeats1++;
                    this->visuals_changed = true;
                    break;
                case 'b':
                    this->window.write_string(this->window.write_string(4, 2 + repeats2, L"hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello", bengine::curses_window::make_write_args(bengine::curses_window::write_arg_options::COLOR | bengine::curses_window::write_arg_options::WRAPPING_WIDTH | bengine::curses_window::write_arg_options::WRAPPING_MODE, {bengine::curses_window::preset_colors::MAROON, 45, bengine::curses_window::wrapping_modes::INDENTED})), L":)");
                    repeats2++;
                    this->visuals_changed = true;
                    break;
            }
        }
        void compute() {}
        void render() {
            this->window.apply_to_screen();
        }

    public:
        test() {}
        ~test() {}
};

int main(int argc, char* argv[]) {
    test t;
    t.run();
    return 0;
}
