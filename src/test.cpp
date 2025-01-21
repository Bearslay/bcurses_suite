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
                    this->window.write_character(repeats, 0, L'a');
                    repeats++;
                    this->visuals_changed = true;
                    break;
            }
        }
        void compute() {}
        void render() {
            this->window.present_window();
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
