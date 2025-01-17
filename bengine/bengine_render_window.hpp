#ifndef BENGINE_RENDER_WINDOW_hpp
#define BENGINE_RENDER_WINDOW_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "bengine_helpers.hpp"
#include "bengine_texture.hpp"

namespace bengine {
    // \brief A wrapper class that combines SDL_Window and SDL_Renderer while also replacing many other SDL2 functions
    class render_window {
        public:
            // \brief List of 16 basic colors
            const static SDL_Color preset_colors[16];
            
            // \brief A preset color that is used in place of custom colors for rendering
            enum class preset_color : unsigned char {
                BLACK,         // preset_color representing black (0, 0, 0)
                WHITE,         // preset_color representing white (255, 255, 255)
                LIGHT_GRAY,    // preset_color representing light gray (170, 170, 170)
                DARK_GRAY,     // preset_color representing dark gray (85, 85, 85)
                BROWN,         // preset_color representing brown (117, 60, 19)
                RED,           // preset_color representing red (255, 0, 0)
                MAROON,        // preset_color representing maroon (115, 0, 0)
                ORANGE,        // preset_color representing orange (255, 115, 0)
                YELLOW,        // preset_color representing yellow (255, 255, 0)
                LIME,          // preset_color representing lime (0, 255, 0)
                GREEN,         // preset_color representing green (0, 115, 0)
                CYAN,          // preset_color representing cyan (0, 255, 255)
                TEAL,          // preset_color representing teal (0, 115, 115)
                BLUE,          // preset_color representing blue (0, 0, 255)
                MAGENTA,       // preset_color representing magenta (255, 0, 255)
                PURPLE         // preset_color representing purple (115, 0, 115)
            };

            /** Get an SDL_Color from one of the bengine::render_window color presets
             * \param color A bengine::render_window::preset_color indicating the desired color preset
             * \returns An SDL_Color that matches the preset provided
             */
            static SDL_Color get_color_from_preset(const bengine::render_window::preset_color &color) {
                return bengine::render_window::preset_colors[static_cast<unsigned char>(color)];
            }

            // \brief A thickness mode used when drawing (hollow) shapes that have a thickness other than 1
            enum class thickness_mode : unsigned char {
                INNER,    // thickness_mode instructing the renderer to add the line thickness to the inside of the shape
                OUTER,    // thickness_mode instructing the renderer to add the line thickness to the outside of the shape
                MIDDLE    // thickness_mode instructing the renderer to add half of the line thickness to the inside of the shape and half to the outside of the shape
            };

            enum class fullscreen_mode : unsigned char {
                FULL,      // fullscreen_mode that indicates an application that is truly fullscreened
                DESKTOP    // fullscreen_mode that indicates a window that takes up the entire screen, not a true fullscreen application
            };

        private:
            // \brief The SDL_Window that the whole class is based around
            SDL_Window *window = NULL;
            // \brief The SDL_Renderer that the whole class is based around
            SDL_Renderer *renderer = NULL;

            // \brief The width of the window (px)
            int width;
            // \brief The height of the window (px)
            int height;
            // \brief Half of the width of the window (px) (useful for referencing the center of the window)
            int width_2;
            // \brief Half of the height of the window (px) (useful for referencing the center of the window)
            int height_2;

            // \brief Whether the window is fullscreen or not
            bool is_fullscreen = false;

            // \brief Whether to lock the aspect ratio of the window for resizing (WIP)
            bool has_locked_ratio = true;
            // \brief Smallest possible width for the window (px)
            int ratio_lock_width;
            // \brief Smallest possible height for the window (px)
            int ratio_lock_height;

            // \brief Whether to stretch graphics to fill the entire window whenever the baseWidth and baseHeight of the window do not match the current width and height of the window
            bool stretch_graphics = true;
            // \brief The base width of the window that will be used to determine the amount of horizontal stretching that will happen
            int base_width;
            // \brief The base height of the window that will be used to determine the amount of vertical stretching that will happen
            int base_height;
            // \brief The factor used to stretch an x-input when bengine::render_window::stretch_graphics is true
            double x_stretch_factor;
            // \brief The factor used to stretch a y-input when bengine::render_window::stretch_graphics is true
            double y_stretch_factor;

            // \brief The SDL_Texture that is used whenever the window's dummy texture is initialized and drawn to
            SDL_Texture *dummy_texture = NULL;
            // \brief The SDL_PixelFormat that the window's dummy texture will use
            SDL_PixelFormat dummy_pixel_format;
            // \brief Whether the renderer is targeting the window (false) or the dummy texture
            bool render_target = false;

            /** Pretty much does the same thing as SDL_SetRenderDrawColor, but will also print an error if something goes wrong
             * \param color The SDL_Color to change the renderer's color to
             * \returns 0 on success or a negative error code on failure
             */
            int change_draw_color(const SDL_Color &color) {
                const int output = SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
                if (output != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to change its renderer's drawing color [bengine::render_window::change_draw_color]";
                    this->print_error();
                }
                return output;
            }
            // \brief Print the output of SDL_GetError with a timestamp and some extra formatting
            void print_error() const {
                std::cout << "\nERROR [" << SDL_GetTicks() << "]: " << SDL_GetError() << "\n";
            }
 
            /** Scale an x-value (that is assumed to be) relative to the window's base width to one relative to the window's current width
             * \param x An x-value relative to the window's base width (px)
             * \returns The inputted x-value scaled to the window's current width (px)
             */
            int stretch_x(const int &x) const {
                return x * this->x_stretch_factor;
            }
            /** Scale a y-value (that is assumed to be) relative to the window's base height to one relative to the window's current height
             * \param y A y-value relative to the window's base height (px)
             * \returns The inputted y-value scaled to the window's current height (px)
             */
            int stretch_y(const int &y) const {
                return y * this->y_stretch_factor;
            }

        public:
            /** bengine::render_window constructor
             * \param title The title for the window
             * \param width The width for the window (px)
             * \param height The height for the window (px)
             * \param flags SDL_WINDOW flags that modify how the window will behave as a Uint32 mask
             */
            render_window(const char* title = "window", const int &width = 1920, const int &height = 1080, const Uint32 &flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE) {
                if ((this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags)) == NULL) {
                    std::cout << "Window \"" << title << "\" failed to initialize [bengine::render_window::render_window]";
                    this->print_error();
                }
                if ((this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
                    std::cout << "Window \"" << title << "\" failed to initialize its renderer [bengine::render_window::render_window]";
                    this->print_error();
                }

                this->width = width;
                this->height = height;
                this->syncronize_dimensions();

                this->base_width = this->width;
                this->base_height = this->height;
                this->x_stretch_factor = this->width / this->base_width;
                this->y_stretch_factor = this->height / this->base_height;

                const int gcd = bengine::math_helper::greatest_common_divisor<int>(this->width, this->height);
                this->ratio_lock_width = this->width / gcd;
                this->ratio_lock_height = this->height / gcd;

                this->generate_dummy_pixel_format();
            }
            // \brief bengine::render_window deconstructor
            ~render_window() {
                SDL_DestroyRenderer(this->renderer);
                SDL_DestroyWindow(this->window);
                this->renderer = nullptr;
                this->window = nullptr;
            }

            /** Get the refresh rate of the monitor that the window was created on
             *\returns The refresh rate of the monitor that the window was created on
             */
            int get_refresh_rate() const {
                SDL_DisplayMode mode;
                const int display_index = SDL_GetWindowDisplayIndex(this->window);
                if (display_index < 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to fetch its display index [bengine::render_window::get_refresh_rate]";
                    this->print_error();
                }
                if (SDL_GetDisplayMode(display_index, 0, &mode) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to fetch its display mode information [bengine::render_window::get_refresh_rate]";
                    this->print_error();
                }
                return mode.refresh_rate;
            }
            /** Get the SDL_WINDOW flags currently associated with the window
             * \returns The SDL_WINDOW flags currently associated with the window as a Uint32 mask
             */
            Uint32 get_window_flags() const {
                return SDL_GetWindowFlags(this->window);
            }
            /** Set the SDL_WINDOW flags that the window should have; conflicting flags are dealt with here
             * \param flags A Uint32 mask that represents the flags the window should have
             */
            void set_window_flags(const Uint32 &flags) {
                if ((flags & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN && (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != SDL_WINDOW_FULLSCREEN_DESKTOP) {
                    bengine::render_window::make_fullscreen(bengine::render_window::fullscreen_mode::FULL);
                } else if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) == SDL_WINDOW_FULLSCREEN_DESKTOP && (flags & SDL_WINDOW_FULLSCREEN) != SDL_WINDOW_FULLSCREEN) {
                    bengine::render_window::make_fullscreen(bengine::render_window::fullscreen_mode::DESKTOP);
                } else if ((flags & SDL_WINDOW_FULLSCREEN) != SDL_WINDOW_FULLSCREEN && (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != SDL_WINDOW_FULLSCREEN_DESKTOP) {
                    bengine::render_window::make_windowed();
                }
                if ((flags & SDL_WINDOW_SHOWN) == SDL_WINDOW_SHOWN && (flags & SDL_WINDOW_HIDDEN) != SDL_WINDOW_HIDDEN) {
                    bengine::render_window::show();
                } else if ((flags & SDL_WINDOW_HIDDEN) == SDL_WINDOW_HIDDEN && (flags & SDL_WINDOW_SHOWN) != SDL_WINDOW_SHOWN) {
                    bengine::render_window::hide();
                }
                if ((flags & SDL_WINDOW_BORDERLESS) == SDL_WINDOW_BORDERLESS) {
                    bengine::render_window::remove_borders();
                } else {
                    bengine::render_window::add_borders();
                }
                if ((flags & SDL_WINDOW_RESIZABLE) == SDL_WINDOW_RESIZABLE) {
                    bengine::render_window::unlock_dimensions();
                } else {
                    bengine::render_window::lock_dimensions();
                }
                if ((flags & SDL_WINDOW_MINIMIZED) == SDL_WINDOW_MINIMIZED && (flags & SDL_WINDOW_MAXIMIZED) != SDL_WINDOW_MAXIMIZED) {
                    bengine::render_window::minimize();
                } else if ((flags & SDL_WINDOW_MAXIMIZED) == SDL_WINDOW_MAXIMIZED && (flags & SDL_WINDOW_MINIMIZED) != SDL_WINDOW_MINIMIZED) {
                    bengine::render_window::maximize();
                }
                if ((flags & SDL_WINDOW_MOUSE_GRABBED) == SDL_WINDOW_MOUSE_GRABBED) {
                    bengine::render_window::grab_mouse();
                } else {
                    bengine::render_window::release_mouse();
                }
                if ((flags & SDL_WINDOW_ALWAYS_ON_TOP) == SDL_WINDOW_ALWAYS_ON_TOP) {
                    bengine::render_window::set_order_to_topmost();
                } else {
                    bengine::render_window::set_order_to_any();
                }
            }
            // \brief Lock the window's current size (removes the SDL_WINDOW_RESIZABLE flag from the window)
            void lock_dimensions() {
                SDL_SetWindowResizable(this->window, SDL_FALSE);
            }
            // \brief Unlock the window's current size (adds the SDL_WINDOW_RESIZABLE flag to the window)
            void unlock_dimensions() {
                SDL_SetWindowResizable(this->window, SDL_TRUE);
            }
            // \brief Show the window (adds the SDL_WINDOW_SHOWN flag to the window)
            void show() {
                SDL_ShowWindow(this->window);
            }
            // \brief Hide the window (adds the SDL_WINDOW_HIDDEN flag to the window)
            void hide() {
                SDL_HideWindow(this->window);
            }
            // \brief Remove borders from the window (removes the SDL_WINDOW_BORDERLESS flag from the window)
            void remove_borders() {
                SDL_SetWindowBordered(this->window, SDL_FALSE);
            }
            // \brief Add borders to the window (adds the SDL_WINDOW_BORDERLESS flag to the window)
            void add_borders() {
                SDL_SetWindowBordered(this->window, SDL_TRUE);
            }
            // \brief Minimize the window to an iconic representation (adds the SDL_WINDOW_MINIMIZED flag to the window)
            void minimize() {
                SDL_MinimizeWindow(this->window);
            }
            // \brief Maximize the window to as large of a size as possible (adds the SDL_WINDOW_MAXIMIZED flag to the window)
            void maximize() {
                SDL_MaximizeWindow(this->window);
            }
            // \brief Confine the mouse to the window (adds the SDL_WINDOW_MOUSE_GRABBED flag to the window)
            void grab_mouse() {
                SDL_SetWindowGrab(this->window, SDL_TRUE);
            }
            // \brief Release the mouse from the confines of the window (removes the SDL_WINDOW_MOUSE_GRABBED flag from the window)
            void release_mouse() {
                SDL_SetWindowGrab(this->window, SDL_FALSE);
            }
            // \brief Set this window to always be visible/on top (adds the SDL_WINDOW_ALWAYS_ON_TOP flag to the window)
            void set_order_to_topmost() {
                SDL_SetWindowAlwaysOnTop(this->window, SDL_TRUE);
            }
            // \brief Allow this window to be ordered in any configuration (removes the SDL_WINDOW_ALWAYS_ON_TOP flag from the window)
            void set_order_to_any() {
                SDL_SetWindowAlwaysOnTop(this->window, SDL_FALSE);
            }
            /** Make the window fullscreen in one of two ways
             * \param fullscreen_mode Whether to make the window fullscreen while taking advantage of fullscreen-exclusive benefits (true) or not
             */
            void make_fullscreen(const bengine::render_window::fullscreen_mode &fullscreen_mode = bengine::render_window::fullscreen_mode::FULL) {
                switch (fullscreen_mode) {
                    default:
                    case bengine::render_window::fullscreen_mode::FULL:
                        SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN);
                        break;
                    case bengine::render_window::fullscreen_mode::DESKTOP:
                        SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        break;
                }
                this->is_fullscreen = true;
                this->syncronize_dimensions();
            }
            // \brief Make the window windowed
            void make_windowed() {
                SDL_SetWindowFullscreen(this->window, SDL_FALSE);
                this->is_fullscreen = false;
                this->syncronize_dimensions();
            }
            /** Toggle whether the window should be fullscreened or not
             * \param fullscreen_mode Whether to make the window fullscreen while taking advantage of fullscreen-exclusive benefits (true) or not
             */
            void toggle_fullscreen(const bengine::render_window::fullscreen_mode &fullscreen_mode = bengine::render_window::fullscreen_mode::FULL) {
                if (this->is_fullscreen) {
                    bengine::render_window::make_windowed();
                    return;
                }
                bengine::render_window::make_fullscreen(fullscreen_mode);
            }

            /** Get the width of the window (px)
             * \returns The width of the window (px)
             */
            int get_width() const {
                return this->width;
            }
            /** Set the width of the window (px)
             * \param width The new width of the window (px)
             */
            void set_width(const int &width) {
                SDL_SetWindowSize(this->window, width, this->height);
                this->syncronize_dimensions();
            }

            /** Get the height of the window (px)
             * \returns The height of the window (px)
             */
            int get_height() const {
                return this->height;
            }
            /** Set the height of the window (px)
             * \param height The new height of the window (px)
             */
            void set_height(const int &height) {
                SDL_SetWindowSize(this->window, this->width, height);
                this->syncronize_dimensions();                
            }

            /** Get half of the width of the window (px)
             * \returns Half of the width of the window (px)
             */
            int get_width_2() const {
                return this->width_2;
            }
            /** Get half of the height of the window (px)
             * \returns Half of the height of the window (px)
             */
            int get_height_2() const {
                return this->height_2;
            }

            /** Get the base width of the window (px) that will be used to determine the amount of horizontal stretching that will happen
             * \returns The base width of the window (px) that will be used to determine the amount of horizontal stretching that will happen
             */
            int get_base_width() const {
                return this->base_width;
            }
            /** Set the base width of the window (px) that will be used to determine the amount of horizontal stretching that will happen
             * \param width The new base width of the window (px) that will be used to determine the amount of horizontal stretching that will happen
             */
            void set_base_width(const int &width) {
                this->base_width = width;
            }

            /** Get the base height of the window (px) that will be used to determine the amount of vertical stretching that will happen
             * \returns The base height of the window (px) that will be used to determine the amount of vertical stretching that will happen
             */
            int get_base_height() const {
                return this->base_height;
            }
            /** Set the base height of the window (px) that will be used to determine the amount of vertical stretching that will happen
             * \param height The new base height of the window (px) that will be used to determine the amount of vertical stretching that will happen
             */
            void set_base_height(const int &height) {
                this->base_height = height;
            }

            /** Get whether the window will stretch graphics based off of a base width/height or not
             * \returns Whether the window will stretch graphics based off of a base width/height or not
             */
            bool is_stretching_graphics() const {
                return this->stretch_graphics;
            }
            // \brief Make the window start stretching graphics based off of a base width/height
            void start_graphical_stretching() {
                this->stretch_graphics = true;
            }
            // \brief Make the window stop stretching graphics based off of a base width/height
            void halt_graphical_stretching() {
                this->stretch_graphics = false;
            }
            // \brief Toggle whether the window will stretch graphics based off of a base width/height or not
            void toggle_graphical_stretching() {
                this->stretch_graphics = !this->stretch_graphics;
            }

            /** Get the window's title as a C-style string
             * \returns The window's title as a C-style string
             */
            const char* get_title() const {
                return SDL_GetWindowTitle(this->window);
            }
            /** Set the window's title as a C-style string
             * \param title The window's new title as a C-style string
             */
            void set_title(const char* title) {
                SDL_SetWindowTitle(this->window, title);
            }

            /** Clear the renderer
             * \param color The color to make the newly blank screen as an SDL_Color
             */
            void clear_renderer(const SDL_Color &color = bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::BLACK)) {
                this->change_draw_color(color);
                if (SDL_RenderClear(this->renderer) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to clear renderer [bengine::render_window::clear_renderer]";
                    this->print_error();
                }
            }
            // \brief Present the renderer's buffer to the window to see
            void present_renderer() {
                SDL_RenderPresent(this->renderer);
            }

            // \brief Syncronize the class's dimensional members with the SDL_Window to clear any potential discrepancies
            void syncronize_dimensions() {
                int width, height;
                SDL_GetWindowSize(this->window, &width, &height);

                this->width = width;
                this->height = height;
                this->width_2 = this->width / 2;
                this->height_2 = this->height / 2;
            }
            /** Handles the general behavior that windows should have when certain events trigger (for now, just window resizing)
             * \param event The SDL_WindowEvent to handle
             */
            void handle_event(const SDL_WindowEvent &event) {
                switch (event.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        // if (this->has_locked_ratio) {
                        //     int width, height;
                        //     SDL_GetWindowSize(this->window, &width, &height);
                            
                        //     if (std::abs(this->width - width) / this->ratio_lock_width > std::abs(this->height - height) / this->ratio_lock_height) {
                        //         SDL_SetWindowSize(this->window, (int)(height * (double)(this->ratio_lock_width / this->ratio_lock_height)), height);
                        //     } else {
                        //         SDL_SetWindowSize(this->window, width, (int)(width * (double)(this->ratio_lock_height / this->ratio_lock_width)));
                        //     }
                        // }
                        this->syncronize_dimensions();
                        break;
                }
            }
            // \brief Center the mouse within the window
            void center_mouse() {
                SDL_WarpMouseInWindow(this->window, this->width_2, this->height_2);
            }
            /** Set the current mouse's position to wherever specified and relative to the window
             * \param x The x-position for the mouse's new position relative to the window (px)
             * \param y The y-position for the mouse's new position relative to the window (px)
             */
            void position_mouse(const int &x, const int &y) {
                SDL_WarpMouseInWindow(this->window, x, y);
            }

            /** Draw a singular pixel
             * \param x x-position of the pixel to change relative to the window
             * \param y y-position of the pixel to change relative to the window
             * \param color The color to change the pixel to as an SDL_Color
             */
            void draw_pixel(const int &x, const int &y, const SDL_Color &color = bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::WHITE)) {
                this->change_draw_color(color);

                if (this->stretch_graphics) {
                    if (SDL_RenderDrawPoint(this->renderer, this->stretch_x(x), this->stretch_y(y)) != 0) {
                        std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to draw a pixel [bengine::render_window::draw_pixel]";
                        this->print_error();
                    }
                    return;
                }
                if (SDL_RenderDrawPoint(this->renderer, x, y) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to draw a pixel [bengine::render_window::draw_pixel]";
                    this->print_error();
                }
            }
            /** Draw a line; includes minor optimizations for totally horizontal/vertical lines and single points
             * \param x1 x-position of the starting point relative to the window (px)
             * \param y1 y-position of the starting point relative to the window (px)
             * \param x2 x-position of the ending point relative to the window (px)
             * \param y2 y-position of the ending point relative to the window (px)
             * \param color The color to draw the line with as an SDL_Color
             */
            void draw_line(const int &x1, const int &y1, const int &x2, const int &y2, const SDL_Color &color = bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::WHITE)) {
                if (x1 == x2 && y1 == y2) {
                    this->draw_pixel(x1, y1, color);
                    return;
                }

                if (this->stretch_graphics) {
                    if (x1 == x2) {
                        this->draw_rectangle(this->stretch_x(x1), this->stretch_y(y1), 1, this->stretch_y(y2) - this->stretch_y(y1), color);
                        return;
                    }
                    if (y1 == y2) {
                        this->draw_rectangle(this->stretch_x(x1), this->stretch_y(y1), this->stretch_x(x2) - this->stretch_x(x1), 1, color);
                        return;
                    }

                    this->change_draw_color(color);
                    if (SDL_RenderDrawLine(this->renderer, this->stretch_x(x1), this->stretch_y(y1), this->stretch_x(x2), this->stretch_y(y2)) != 0) {
                        std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to draw a line [bengine::render_window::draw_line]";
                        this->print_error();
                    }
                    return;
                }
                if (x1 == x2) {
                    this->draw_rectangle(x1, y1, 1, y2 - y1, color);
                    return;
                }
                if (y1 == y2) {
                    this->draw_rectangle(x1, y1, x2 - x1, 1, color);
                    return;
                }

                this->change_draw_color(color);
                if (SDL_RenderDrawLine(this->renderer, x1, y1, x2, y2) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to draw a line [bengine::render_window::draw_line]";
                    this->print_error();
                }
            }

            /** Draw a rectangle (not filled, will only draw the perimeter)
             * \param x x-position of the top-left corner relative to the window (px) (assuming positive width and height)
             * \param y y-position of the top-left corner relative to the window (px) (assuming positive width and height)
             * \param w Width of the rectangle (px) (can be negative, thereby making the "x" parameter reference the right side of the rectangle)
             * \param h Height of the rectangle (px) (can be negative, thereby making the "y" parameter reference the bottom side of the rectangle)
             * \param color The color to draw the rectangle with as an SDL_Color
             */
            void draw_rectangle(const int &x, const int &y, const int &w, const int &h, const SDL_Color &color = bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::WHITE)) {
                this->change_draw_color(color);
                
                if (this->stretch_graphics) {
                    const SDL_Rect dst = {x, y, w, h};
                    if (SDL_RenderDrawRect(this->renderer, &dst) != 0) {
                        std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to draw a rectangle [bengine::render_window::draw_rectangle]";
                        this->print_error();
                    }
                    return;
                }
                const SDL_Rect dst = {this->stretch_x(x), this->stretch_y(y), this->stretch_x(w), this->stretch_y(h)};
                if (SDL_RenderDrawRect(this->renderer, &dst) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to draw a rectangle [bengine::render_window::draw_rectangle]";
                    this->print_error();
                }
            }
            /** Draw a rectangle with a specified edge thickness (not filled, will only draw the perimeter at the set thickness)
             * \param x x-position of the top-left corner relative to the window (px) (assuming positive width and height)
             * \param y y-position of the top-left corner relative to the window (px) (assuming positive width and height)
             * \param w Width of the rectangle (px) (can be negative, thereby making the "x" parameter reference the right side of the rectangle)
             * \param h Height of the rectangle (px) (can be negative, thereby making the "y" parameter reference the bottom side of the rectangle)
             * \param thickness The thickness of the rectangle's sides (px)
             * \param mode The mode to use when thickening the lines; THICKSHAPE_INNER keeps the same width/height, THICKSHAPE_OUTER adds 2*thickness to the width/height, and THICKSHAPE_MIDDLE adds 1*thickness to the width/height
             * \param color The color to draw the rectangle with as an SDL_Color
             */
            void draw_thick_rectangle(const int &x, const int &y, const int &w, const int &h, const int &thickness, const bengine::render_window::thickness_mode &mode = bengine::render_window::thickness_mode::INNER, const SDL_Color &color = bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::WHITE)) {
                this->change_draw_color(color);

                SDL_Rect rect[4];
                switch (mode) {
                    default:
                    case bengine::render_window::thickness_mode::INNER:
                        rect[0] = {x, y, w, thickness};
                        rect[1] = {x, y + h - thickness, w, thickness};
                        rect[2] = {x, y + thickness, thickness, h - thickness * 2};
                        rect[3] = {x + w - thickness, y + thickness, thickness, h - thickness * 2};
                        break;
                    case bengine::render_window::thickness_mode::OUTER:
                        rect[0] = {x - thickness, y - thickness, w + thickness * 2, thickness};
                        rect[1] = {x - thickness, y + h, w + thickness * 2, thickness};
                        rect[2] = {x - thickness, y, thickness, h};
                        rect[3] = {x + w, y, thickness, h};
                        break;
                    case bengine::render_window::thickness_mode::MIDDLE:
                        rect[0] = {x - thickness / 2, y - thickness / 2, w + thickness, thickness};
                        rect[1] = {x - thickness / 2, y + h - thickness / 2, w + thickness, thickness};
                        rect[2] = {x - thickness / 2, y + thickness / 2, thickness, h - thickness};
                        rect[3] = {x + w - thickness / 2, y + thickness / 2, thickness, h - thickness};
                        break;
                }

                if (this->stretch_graphics) {
                    for (unsigned char i = 0; i < 4; i++) {
                        rect[i].x = this->stretch_x(rect[i].x);
                        rect[i].y = this->stretch_y(rect[i].y);
                        rect[i].w = this->stretch_x(rect[i].w);
                        rect[i].h = this->stretch_y(rect[i].h);
                    }
                }

                if (SDL_RenderFillRect(this->renderer, &rect[0]) != 0 || SDL_RenderFillRect(this->renderer, &rect[1]) != 0 || SDL_RenderFillRect(this->renderer, &rect[2]) != 0 || SDL_RenderFillRect(this->renderer, &rect[3]) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to draw a thick rectangle [bengine::render_window::draw_thick_rectangle]";
                    this->print_error();
                }
            }
            /** Fill a rectangle
             * \param x x-position of the top-left corner relative to the window (px) (assuming positive width and height)
             * \param y y-position of the top-left corner relative to the window (px) (assuming positive width and height)
             * \param w Width of the rectangle (px) (can be negative, thereby making the "x" parameter reference the right side of the rectangle)
             * \param h Height of the rectangle (px) (can be negative, thereby making the "y" parameter reference the bottom side of the rectangle)
             * \param color The color to fill the rectangle with as an SDL_Color
             */
            void fill_rectangle(const int &x, const int &y, const int &w, const int &h, const SDL_Color &color = bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::WHITE)) {
                this->change_draw_color(color);

                if (this->stretch_graphics) {
                    const SDL_Rect dst = {x, y, w, h};
                    if (SDL_RenderFillRect(this->renderer, &dst) != 0) {
                        std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to fill a rectangle [bengine::render_window::fill_rectangle]";
                        this->print_error();
                    }
                    return;
                }
                const SDL_Rect dst = {this->stretch_x(x), this->stretch_y(y), this->stretch_x(w), this->stretch_y(h)};
                if (SDL_RenderFillRect(this->renderer, &dst) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to fill a rectangle [bengine::render_window::fill_rectangle]";
                    this->print_error();
                }
            }
            /** Draw a circle (not filled, will only draw the perimeter at the set thickness; DOES NOT GET STRETCHED)
             * \param x x-position of the center of the circle relative to the window (px)
             * \param y y-position of the center of the circle relative to the window (px)
             * \param r Radius of the circle (px)
             * \param color The color to draw the circle with as an SDL_Color
             */
            void draw_circle(const int &x, const int &y, const int &r, const SDL_Color &color = bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::WHITE)) {
                SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
                const int diameter = r * 2;
                int ox = r - 1;
                int oy = 0;
                int tx = 1;
                int ty = 1;
                int error = tx - diameter;
                while (ox >= oy) {
                    SDL_RenderDrawPoint(this->renderer, x + ox, y - oy);
                    SDL_RenderDrawPoint(this->renderer, x + ox, y + oy);
                    SDL_RenderDrawPoint(this->renderer, x - ox, y - oy);
                    SDL_RenderDrawPoint(this->renderer, x - ox, y + oy);
                    SDL_RenderDrawPoint(this->renderer, x + oy, y - ox);
                    SDL_RenderDrawPoint(this->renderer, x + oy, y + ox);
                    SDL_RenderDrawPoint(this->renderer, x - oy, y - ox);
                    SDL_RenderDrawPoint(this->renderer, x - oy, y + ox);
                    if (error <= 0) {
                        oy++;
                        error += ty;
                        ty += 2;
                    } else if (error > 0) {
                        ox--;
                        tx += 2;
                        error += tx - diameter;
                    }
                }
            }
            /** Fill a circle (DOES NOT GET STRETCHED)
             * \param x x-position of the center of the circle relative to the window (px)
             * \param y y-position of the center of the circle relative to the window (px)
             * \param r Radius of the circle (px)
             * \param color The color to fill the circle with as an SDL_Color
             */
            void fill_circle(const int &x, const int &y, const int &r, const SDL_Color &color = bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::WHITE)) {
                SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
                int ox = 0;
                int oy = r;
                int error = r - 1;
                while (oy >= ox) {
                    SDL_RenderDrawLine(this->renderer, x - oy, y + ox, x + oy, y + ox);
                    SDL_RenderDrawLine(this->renderer, x - ox, y + oy, x + ox, y + oy);
                    SDL_RenderDrawLine(this->renderer, x - ox, y - oy, x + ox, y - oy);
                    SDL_RenderDrawLine(this->renderer, x - oy, y - ox, x + oy, y - ox);
                    if (error >= ox * 2) {
                        error -= ox * 2 + 1;
                        ox++;
                    } else if (error < 2 * (r - oy)) {
                        error += oy * 2 - 1;
                        oy--;
                    } else {
                        error += 2 * (oy - ox - 1);
                        oy--;
                        ox++;
                    }
                }
            }

            /** Load an SDL_Texture using the window's renderer
             * \param filepath The path to the file to load in as an SDL_Texture
             * \returns An SDL_Texture of the image file located at filepath
             */
            SDL_Texture *load_texture(const char* filepath) {
                SDL_Texture *output = NULL;
                if ((output = IMG_LoadTexture(this->renderer, filepath)) == NULL) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to load texture [bengine::render_window::load_texture]";
                    this->print_error();
                }
                return output;
            }

            /** Get the pixelformat that the window's dummy texture uses
             * \returns The pixelformat that the window's dummy texture uses
             */
            SDL_PixelFormat get_dummy_pixel_format() const {
                return this->dummy_pixel_format;
            }
            // \brief Automatically generate the pixelformat that the window's dummy texture uses
            void generate_dummy_pixel_format() {
                SDL_RendererInfo info;
                if (SDL_GetRendererInfo(this->renderer, &info) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to get a valid pixel format [bengine::render_window::generate_dummy_pixel_format]";
                    this->print_error();
                    this->dummy_pixel_format.format = SDL_PIXELFORMAT_UNKNOWN;
                } else {
                    this->dummy_pixel_format.format = info.texture_formats[0];
                }
            }

            /** Initialize the dummy texture for the ability to render to textures rather than just the window
             * \param width The width of the dummy texture (px)
             * \param height The height of the dummy texture (px)
             */
            int initialize_dummy(const int &width, const int &height) {
                if (this->dummy_pixel_format.format == SDL_PIXELFORMAT_UNKNOWN) {
                    this->generate_dummy_pixel_format();
                }
                this->dummy_texture = SDL_CreateTexture(this->renderer, this->dummy_pixel_format.format, SDL_TEXTUREACCESS_TARGET, width, height);
                if (this->dummy_texture == NULL) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to create dummy texture [bengine::render_window::initialize_dummy]";
                    this->print_error();
                    return -1;
                }
                // Re-initializing the dummy texture requires the renderer to be re-targeted
                if (this->render_target) {
                    this->target_renderer_at_dummy();
                } else {
                    this->target_renderer_at_window();
                }
                return 0;
            }
            /** Target the renderer at the dummy texture
             * \returns 0 on success or a negative error code on failure
             */
            int target_renderer_at_dummy() {
                const int output = SDL_SetRenderTarget(this->renderer, this->dummy_texture);
                if (output != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to switch the rendering target to the dummy texture [bengine::render_window::target_renderer_at_dummy]";
                    this->print_error();
                } else {
                    this->render_target = true;
                }
                return output;
            }
            /** Target the renderer at the window
             * \returns 0 on success or a negative error code on failure
             */
            int target_renderer_at_window() {
                const int output = SDL_SetRenderTarget(this->renderer, NULL);
                if (output != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to switch the rendering target to the window [bengine::render_window::target_renderer_at_window]";
                    this->print_error();
                } else {
                    this->render_target = false;
                }
                return output;
            }
            /** Copy the dummy texture onto another texture (has a few ramifications but should be fine overall)
             * \returns An SDL_Texture that reflects the dummy texture
             */
            SDL_Texture* duplicate_dummy() {
                int width, height;
                SDL_BlendMode blendmode;

                SDL_QueryTexture(this->dummy_texture, NULL, NULL, &width, &height);
                SDL_GetTextureBlendMode(this->dummy_texture, &blendmode);

                SDL_Texture* output = SDL_CreateTexture(this->renderer, this->dummy_pixel_format.format, SDL_TEXTUREACCESS_TARGET, width, height);
                SDL_SetTextureBlendMode(output, SDL_BLENDMODE_NONE);
                
                SDL_SetRenderTarget(this->renderer, output);
                this->clear_renderer();
                SDL_RenderCopy(this->renderer, this->dummy_texture, NULL, NULL);
                this->present_renderer();
                SDL_SetTextureBlendMode(output, blendmode);

                if (this->render_target) {
                    this->target_renderer_at_dummy();
                } else {
                    this->target_renderer_at_window();
                }
                return output;
            }

            /** Render an SDL_Texture
             * \param texture The SDL_Texture to render
             * \param src The portion of the SDL_Texture to copy and render (px for all 4 metrics)
             * \param dst The portion of the window/dummy texture to copy to (px for all 4 metrics) (will stretch the texture to fill the given rectangle)
             */
            void render_SDLTexture(SDL_Texture *texture, const SDL_Rect &src, const SDL_Rect &dst) {
                if (this->stretch_graphics) {
                    const SDL_Rect destination = {this->stretch_x(dst.x), this->stretch_y(dst.y), this->stretch_x(dst.w), this->stretch_y(dst.h)};
                    if (SDL_RenderCopy(this->renderer, texture, &src, &destination) != 0) {
                        std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render SDL_Texture [bengine::render_window::render_SDLTexture]";
                        this->print_error();
                    }
                    return;
                }
                if (SDL_RenderCopy(this->renderer, texture, &src, &dst) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render SDL_Texture [bengine::render_window::render_SDLTexture]";
                    this->print_error();
                }
            }
            /** Render an SDL_Texture while also applying rotations/reflections
             * \param texture The SDL_Texture to render
             * \param src The portion of the SDL_Texture to copy and render (px for all 4 metrics) 
             * \param dst The portion of the window/dummy texture to copy to (px for all 4 metrics)  (will stretch the texture to fill the given rectangle)
             * \param angle The angle to rotate the texture (degrees)
             * \param center The point to rotate around (px for both metrics)  relative to the top-left corner of the destination rectangle
             * \param flip How to flip the rectangle (SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL can be OR'd together)
             */
            void render_SDLTexture(SDL_Texture *texture, const SDL_Rect &src, const SDL_Rect &dst, const double &angle, const SDL_Point &center, const SDL_RendererFlip &flip) {
                if (this->stretch_graphics) {
                    const SDL_Rect destination = {this->stretch_x(dst.x), this->stretch_y(dst.y), this->stretch_x(dst.w), this->stretch_y(dst.h)};
                    if (SDL_RenderCopyEx(this->renderer, texture, &src, &destination, -angle, &center, flip) != 0) {
                        std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render SDL_Texture [bengine::render_window::render_SDLTexture]";
                        this->print_error();
                    }
                    return;
                }
                if (SDL_RenderCopyEx(this->renderer, texture, &src, &dst, -angle, &center, flip) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render SDL_Texture [bengine::render_window::render_SDLTexture]";
                    this->print_error();
                }
            }

            /** Render a bengine::basic_texture
             * \param texture The bengine::basic_texture to render
             * \param dst The portion of the window/dummy texture to copy to (px for all 4 metrics)  (will stretch the texture to fill the given rectangle)
             */
            void render_basic_texture(const bengine::basic_texture &texture, const SDL_Rect &dst) {
                const SDL_Rect frame = texture.get_frame();
                if (this->stretch_graphics) {
                    const SDL_Rect destination = {this->stretch_x(dst.x), this->stretch_y(dst.y), this->stretch_x(dst.w), this->stretch_y(dst.h)};
                    if (SDL_RenderCopy(this->renderer, texture.get_texture(), &frame, &destination) != 0) {
                        std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render bengine::basic_texture [bengine::render_window::render_basic_texture]";
                        this->print_error();
                    }
                    return;
                }
                if (SDL_RenderCopy(this->renderer, texture.get_texture(), &frame, &dst) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render bengine::basic_texture [bengine::render_window::render_basic_texture]";
                    this->print_error();
                }
            }
            /** Render a bengine::basic_texture while also applying rotations/reflections
             * \param texture The bengine::basic_texture to render
             * \param dst The portion of the window/dummy texture to copy to (px for all 4 metrics)  (will stretch the texture to fill the given rectangle)
             * \param angle The angle to rotate the texture (degrees)
             * \param center The point to rotate around (px for both metrics)  relative to the top-left corner of the destination rectangle
             * \param flip How to flip the rectangle (SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL can be OR'd together)
             */
            void render_basic_texture(const bengine::basic_texture &texture, const SDL_Rect &dst, const double &angle, const SDL_Point &pivot, const SDL_RendererFlip &flip) {
                const SDL_Rect frame = texture.get_frame();
                if (this->stretch_graphics) {
                    const SDL_Rect destination = {this->stretch_x(dst.x), this->stretch_y(dst.y), this->stretch_x(dst.w), this->stretch_y(dst.h)};
                    if (SDL_RenderCopyEx(this->renderer, texture.get_texture(), &frame, &destination, -angle, &pivot, flip) != 0) {
                        std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render bengine::basic_texture [bengine::render_window::render_basic_texture]";
                        this->print_error();
                    }
                    return;
                }
                if (SDL_RenderCopyEx(this->renderer, texture.get_texture(), &frame, &dst, -angle, &pivot, flip) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render bengine::basic_texture [bengine::render_window::render_basic_texture]";
                    this->print_error();
                }
            }

            /** Render a bengine::modded_texture
             * \param texture The bengine::modded_texture to render
             * \param dst The portion of the window/dummy texture to copy to (px for all 4 metrics) (will stretch the texture to fill the given rectangle)
             */
            void render_modded_texture(const bengine::modded_texture &texture, const SDL_Rect &dst) {
                const SDL_Rect frame = texture.get_frame();
                if (this->stretch_graphics) {
                    const SDL_Rect destination = {this->stretch_x(dst.x), this->stretch_y(dst.y), this->stretch_x(dst.w), this->stretch_y(dst.h)};
                    if (SDL_RenderCopy(this->renderer, texture.get_texture(), &frame, &destination) != 0) {
                        std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render bengine::modded_texture [bengine::render_window::render_modded_texture]";
                        this->print_error();
                    }
                    return;
                }
                if (SDL_RenderCopy(this->renderer, texture.get_texture(), &frame, &dst) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render bengine::modded_texture [bengine::render_window::render_modded_texture]";
                    this->print_error();
                }
            }
            /** Render a bengine::modded_texture while also applying rotations/reflections
             * \param texture The bengine::modded_texture to render
             * \param dst The portion of the window/dummy texture to copy to (px for all 4 metrics) (will stretch the texture to fill the given rectangle)
             * \param angle The angle to rotate the texture (degrees)
             * \param center The point to rotate around (px for both metrics)  relative to the top-left corner of the destination rectangle
             * \param flip How to flip the rectangle (SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL can be OR'd together)
             */
            void render_modded_texture(const bengine::modded_texture &texture, const SDL_Rect &dst, const double &angle, const SDL_Point &pivot, const SDL_RendererFlip &flip) {
                const SDL_Rect frame = texture.get_frame();
                if (this->stretch_graphics) {
                    const SDL_Rect destination = {this->stretch_x(dst.x), this->stretch_y(dst.y), this->stretch_x(dst.w), this->stretch_y(dst.h)};
                    if (SDL_RenderCopyEx(this->renderer, texture.get_texture(), &frame, &destination, -angle, &pivot, flip) != 0) {
                        std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render bengine::modded_texture [bengine::render_window::render_modded_texture]";
                        this->print_error();
                    }
                    return;
                }
                if (SDL_RenderCopyEx(this->renderer, texture.get_texture(), &frame, &dst, -angle, &pivot, flip) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render bengine::modded_texture [bengine::render_window::render_modded_texture]";
                    this->print_error();
                }
            }

            /** Render a bengine::shifting_texture
             * \param texture The bengine::shifting_texture to render
             * \param dst The portion of the window/dummy texture to copy to (px for all 4 metrics) (will stretch the texture to fill the given rectangle)
             */
            void render_shifting_texture(const bengine::shifting_texture &texture, const SDL_Rect &dst) {
                const SDL_Rect frame = texture.get_frame();
                const SDL_Point pivot = texture.get_pivot();
                if (this->stretch_graphics) {
                    const SDL_Rect destination = {this->stretch_x(dst.x), this->stretch_y(dst.y), this->stretch_x(dst.w), this->stretch_y(dst.h)};
                    if (SDL_RenderCopyEx(this->renderer, texture.get_texture(), &frame, &destination, -texture.get_angle(), &pivot, texture.get_flip()) != 0) {
                        std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render bengine::shifting_texture [bengine::render_window::render_shifting_texture]";
                        this->print_error();
                    }
                    return;
                }
                if (SDL_RenderCopyEx(this->renderer, texture.get_texture(), &frame, &dst, -texture.get_angle(), &pivot, texture.get_flip()) != 0) {
                    std::cout << "Window \"" << SDL_GetWindowTitle(this->window) << "\" failed to render bengine::shifting_texture [bengine::render_window::render_shifting_texture]";
                    this->print_error();
                }
            }

            /** Render text using a TTF_Font based off of a point (supports most unicode characters)
             * \param font The TTF_Font to use (represents both the font and size of the font)
             * \param text The text to display (literals are written as u"[text]", std::u16_string is useful too)
             * \param x x-position of the top-left corner of the text (px)
             * \param y y-position of the top-left corner of the text (px)
             * \param wrapWidth The maximum width for the text to render (px) (a width of zero prevents any wrapping)
             * \param color The color to fill the circle with as an SDL_Color
             */
            void render_text(TTF_Font *font, const char16_t *text, const int &x, const int &y, const Uint32 &wrapWidth = 0, const SDL_Color &color = bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::WHITE)) {
                SDL_Surface *surface = TTF_RenderUNICODE_Blended_Wrapped(font, (Uint16*)text, color, wrapWidth);

                const SDL_Rect src = {0, 0, surface->w, surface->h};
                const SDL_Rect dst = {x, y, surface->w, surface->h};
                this->render_SDLTexture(SDL_CreateTextureFromSurface(this->renderer, surface), src, dst);
                
                SDL_FreeSurface(surface);
                surface = nullptr;
            }
            /** Render text using a TTF_Font based off of a point (supports most unicode characters)
             * \param font The TTF_Font to use (represents both the font and size of the font)
             * \param text The text to display (literals are written as u"[text]", std::u16_string is useful too)
             * \param dst The portion of the window/dummy texture to copy to (px for all 4 metrics) (will stretch the text to fill the given rectangle)
             * \param color The color to fill the circle with as an SDL_Color
             */
            void render_text(TTF_Font *font, const char16_t *text, const SDL_Rect &dst, const SDL_Color &color = bengine::render_window::get_color_from_preset(bengine::render_window::preset_color::WHITE)) {
                SDL_Surface *surface = TTF_RenderUNICODE_Blended_Wrapped(font, (Uint16*)text, color, dst.w);
                SDL_Texture *texture = SDL_CreateTextureFromSurface(this->renderer, surface);
                
                const SDL_Rect src = {0, 0, surface->w, surface->h};
                this->render_SDLTexture(texture, src, dst);

                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
                surface = nullptr;
                texture = nullptr;
            }
    };
    const SDL_Color bengine::render_window::preset_colors[16] = {
        {  0,   0,   0, 255},
        {255, 255, 255, 255},
        {170, 170, 170, 255},
        { 85,  85,  85, 255},
        {117,  60,  19, 255},
        {255,   0,   0, 255},
        {115,   0,   0, 255},
        {255, 115,   0, 255},
        {255, 255,   0, 255},
        {  0, 255,   0, 255},
        {  0, 115,   0, 255},
        {  0, 255, 255, 255},
        {  0, 115, 115, 255},
        {  0,   0, 255, 255},
        {255,   0, 255, 255},
        {115,   0, 115, 255}
    };
}

#endif // BENGINE_RENDER_WINDOW_hpp
