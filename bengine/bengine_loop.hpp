#ifndef BENGINE_LOOP_hpp
#define BENGINE_LOOP_hpp

#include "bengine_render_window.hpp"

namespace bengine {
    // \brief A virtual class used to contain the basic looping mechanism required to seperate rendering/computing while maintaining consistent computational behavior
    class loop {
        protected:
            // \brief How long the loop has been active (seconds)
            long double time = 0.0;
            // \brief How long each computation frame should take (in seconds)
            double delta_time = 0.01;

            // \brief Whether the loop is running or not
            bool loop_running = true;
            // \brief Whether the renderer needs to update the visuals or not (saves on performance when nothing visual is happening)
            bool visuals_changed = true;

            // \brief The window that is interacted with and displays everything
            bengine::render_window window = bengine::render_window("window", 1280, 720, SDL_WINDOW_SHOWN);
            // \brief The SDL_Event structure used to process events
            SDL_Event event;
            // \brief The state of the keyboard; good for instantaneous feedback on which keys are pressed and which aren't
            const Uint8 *keystate = SDL_GetKeyboardState(NULL);

            // \brief A virtual function that will be called whenever there is an event that needs to be addressed
            virtual void handle_event() = 0;
            // \brief A virtual function that will be called each computation frame to handle any non-rendering-related tasks
            virtual void compute() = 0;
            // \brief A virtual function that will be called each rendering frame to handle all of the rendering-related tasks
            virtual void render() = 0;

        public:
            /** bengine::loop constructor; mainly creates the window that will be used
             * \param title The title of the window being created
             * \param width The width of the window being created
             * \param height The height of the window being created
             * \param flags SDL2 flags to create the window with
             * \param image_init_flags SDL2 image flags to initialize SDL_image with (-1 to not initialize)
             * \param use_TTF Whether to initialize SDL_ttf or not
             */
            loop(const char* title = "window", const Uint16 &width = 1920, const Uint16 &height = 1080, const Uint32 &flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, const int &image_init_flags = IMG_INIT_PNG, const bool &use_TTF = true) {
                if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                    std::cout << "Error initializing SDL2\nERROR: " << SDL_GetError() << "\n";
                }
                if (image_init_flags != -1 && IMG_Init(image_init_flags) != image_init_flags) {
                    std::cout << "Error initializing SDL2_image\nERROR: " << IMG_GetError() << "\n";
                }
                if (use_TTF && TTF_Init() == -1) {
                    std::cout << "Error initializing SDL2_ttf\nERROR: " << TTF_GetError() << "\n";
                }

                this->window.set_title(title);
                this->window.set_width(width);
                this->window.set_height(height);
                this->window.set_window_flags(flags);
                this->window.set_base_width(width);
                this->window.set_base_height(height);

                SDL_StopTextInput();
            }
            // \brief bengine::loop deconstructor; pretty much just handles some SDL cleanup
            ~loop() {
                TTF_Quit();
                IMG_Quit();
                SDL_Quit();
            }

            /** The main function that handles the looping behavior and virtual function calling
             * \returns 0 (anything additional hasn't been added yet)
             */
            int run() {
                Uint32 start_ticks = 0;
                Uint32 frame_ticks = 0;
                long double current_time = SDL_GetTicks() * 0.01;
                long double new_time = 0.0;
                double frame_time = 0.0;
                double accumulator = 0.0;

                while (this->loop_running) {
                    start_ticks = SDL_GetTicks();
                    new_time = SDL_GetTicks() * 0.01;
                    frame_time = new_time - current_time;
                    current_time = new_time;
                    accumulator += frame_time;

                    while (accumulator >= this->delta_time) {
                        while (SDL_PollEvent(&this->event)) {
                            switch (this->event.type) {
                                case SDL_QUIT:
                                    this->loop_running = false;
                                    break;
                                case SDL_WINDOWEVENT:
                                    this->window.handle_event(this->event.window);
                                    this->visuals_changed = true;
                                    break;
                            }
                            this->handle_event();
                        }
                        this->compute();
                        this->time += this->delta_time;
                        accumulator -= this->delta_time;
                    }

                    if (this->visuals_changed) {
                        this->visuals_changed = false;
                        this->window.clear_renderer();
                        this->render();
                        this->window.present_renderer();
                    }

                    if ((frame_ticks = SDL_GetTicks() - start_ticks) < (Uint32)(1000 / this->window.get_refresh_rate())) {
                        SDL_Delay(1000 / this->window.get_refresh_rate() - frame_ticks);
                    }
                }
                return 0;
            }
    };
}

#endif // BENGINE_LOOP_hpp
