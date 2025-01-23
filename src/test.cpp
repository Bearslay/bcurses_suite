#include "bengine_curses.hpp"

class test : public bengine::curses_loop {
    private:
        bengine::curses_window window;

        unsigned short repeats = 0;
    
        void handle_event() {
            switch (this->input_character) {
                case 'q':
                    this->loop_running = false;
                    break;
                case 'a':
                    this->window.write_character(repeats, 0, L'a', bengine::curses_window::preset_colors::RED, bengine::curses_window::cell_attributes::BOLD);
                    this->window.write_character(0, 1, this->window.check_cell_attribute_states(0, 0, bengine::curses_window::cell_attributes::BOLD | bengine::curses_window::cell_attributes::ITALIC) ? L'1' : L'0');
                    repeats++;
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
