#include "bengine_curses.hpp"

class test : public bengine::curses_loop {
    private:
        bengine::curses_window window;
    
        void handle_event() {
            switch (this->input_character) {
                case 'q':
                    this->loop_running = false;
                    break;
            }
        }
        void compute() {
            this->visuals_changed = true;
        }
        void render() {
            mvprintw(0, 0, "%u", this->get_ticks());
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
