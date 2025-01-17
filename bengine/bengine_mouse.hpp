#ifndef BENGINE_MOUSE_hpp
#define BENGINE_MOUSE_hpp

#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

#include "bengine_helpers.hpp"
#include "bengine_render_window.hpp"

namespace bengine {
    // \brief A virtual class that contains the basic aspects of a mouse's state that can be built off of for more specific uses
    class base_mouse_state {
        public:
            // \brief Names of the supported mouse buttons
            enum class button_names : const unsigned char {
                MOUSE1 = 1,                   // Left mouse button
                MOUSE2 = 4,                   // Right mouse button
                MOUSE3 = 2,                   // Middle mouse button (scroll wheel)
                MOUSE4 = 8,                   // Mouse button usually used to 'go back' in a web page
                MOUSE5 = 16,                  // Mouse button usually used to 'go forwards' in a web page
                LEFT_MOUSE_BUTTON = 1,        // Left mouse button
                RIGHT_MOUSE_BUTTON = 4,       // Right mouse button
                MIDDLE_MOUSE_BUTTON = 2,      // Middle mouse button (scroll wheel)
                BACKWARD_MOUSE_BUTTON = 8,    // Mouse button usually used to 'go back' in a web page
                FORWARD_MOUSE_BUTTON = 16     // Mouse button usually used to 'go forwards' in a web page
            };

        protected:
            // \brief x-position of the mouse
            int x_pos;
            // \brief y-position of the mouse
            int y_pos;
            // \brief Whether the mouse has moved or not
            bool motion = false;
            // \brief A bitmask representing which of the 5 mouse buttons are pressed/released
            Uint8 buttons = 0;

        public:
            // \brief bengine::base_mouse_state constructor
            base_mouse_state() {}
            // \brief bengine::base_mouse_state deconstructor
            ~base_mouse_state() {}

            /** Update the mouse's position/motion; virtual function
             * \param event An SDL_Event used to update the mouse's position
             */
            virtual void update_motion(const SDL_Event &event) = 0;

            /** Press a button (update bitmask) using an SDL_Event
             * \param event An SDL_Event used to determine the button to press
             */
            void press_button(const SDL_Event &event) {
                this->buttons |= (Uint8)std::pow<Uint8, Uint8>(2, event.button.button - 1);
            }
            /** Press several buttons (update bitmask) using OR'd together elements from bengine::base_mouse_state::button_names
             * \param buttons A bitmask produced from bengine::base_mouse_state::button_names elements OR'd together
             */
            void press_buttons(const button_names &buttons) {
                this->buttons |= static_cast<unsigned char>(buttons);
            }
            /** Release a button (update bitmask) using an SDL_Event
             * \param event An SDL_Event used to determine the button to release
             */
            void release_button(const SDL_Event &event) {
                this->buttons &= ((Uint8)std::pow<Uint8, Uint8>(2, event.button.button - 1) ^ 255);
            }
            /** Release several buttons (update bitmask) using OR'd together elements from bengine::base_mouse_state::button_names
             * \param buttons A bitmask produced from bengine::base_mouse_state::button_names elements OR'd together
             */
            void release_buttons(const button_names &buttons) {
                this->buttons &= (static_cast<unsigned char>(buttons) ^ 255);
            }
            /** Do a general update based on SDL_Events
             * \param event The SDL_Event used to update the mouse's state
             */
            void update_general(const SDL_Event &event) {
                switch (event.type) {
                    case SDL_MOUSEMOTION:
                        this->update_motion(event);
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        this->press_button(event);
                        break;
                    case SDL_MOUSEBUTTONUP:
                        this->release_button(event);
                        break;
                }
            }

            /** Get the x-position of the mouse
             * \returns The x-position of the mouse
             */
            int get_x_pos() const {
                return this->x_pos;
            }
            /** Get the y-position of the mouse
             * \returns The y-position of the mouse
             */
            int get_y_pos() const {
                return this->y_pos;
            }
            /** Determine whether the mouse has moved or not
             * \returns Whether the mouse has moved or not
             */
            bool has_moved() const {
                return this->motion;
            }
            // \brief Set the mouse as no longer moving
            void stop_motion() {
                this->motion = false;
            }
            /** Determine if a set of buttons are pressed or not
             * \param buttons The set of buttons to check for (OR'd together from bengine::base_mouse_state::button_names)
             */
            bool pressed(const button_names &buttons) const {
                return (this->buttons & static_cast<unsigned char>(buttons)) == static_cast<unsigned char>(buttons);
            }

            void print_button_state() const {
                std::cout << this->pressed(button_names::MOUSE1) << this->pressed(button_names::MOUSE2) << this->pressed(button_names::MOUSE3) << this->pressed(button_names::MOUSE4) << this->pressed(button_names::MOUSE5) << "\n";
            }
    };

    // \brief A mouse state that holds 2D data (most generic/common mouse state implementation)
    class generic_mouse_state : public bengine::base_mouse_state {
        public:
            // \brief bengine::generic_mouse_state constructor
            generic_mouse_state() {}
            // \brief bengine::generic_mouse_state deconstructor
            ~generic_mouse_state() {}

            /** Update the mouse's position/motion, giving it a 2-D position within a window
             * \param event An SDL_Event used to update the mouse's position
             */
            void update_motion(const SDL_Event &event) override {
                this->motion = true;
                this->x_pos = event.motion.x;
                this->y_pos = event.motion.y;
            }
    };

    // \brief A mouse state that holds 3D data (applicable to implementations requiring a camera in a 3D world)
    class relative_mouse_state : public bengine::base_mouse_state {
        private:
            double sensitivity = 10;

            bool released = true;
            bool captured = false;

        public:
            relative_mouse_state() {}
            ~relative_mouse_state() {}

            void update_motion(const SDL_Event &event) override {
                this->motion = true;
                this->x_pos = event.motion.xrel;
                this->y_pos = event.motion.yrel;

                if (this->released) {
                    this->released = false;
                    this->x_pos = this->y_pos = 0;
                }
            }

            bool is_captured() const {
                return this->captured;
            }
            bool capture() {
                const bool output = this->captured;
                if (SDL_SetRelativeMouseMode(SDL_TRUE) != 0) {
                    std::cout << "Could not enter relative mouse mode\nERROR [" << SDL_GetTicks() << "]: " << SDL_GetError() << "\n";
                }
                this->x_pos = this->y_pos = 0;
                this->captured = true;
                return output;
            }
            bool release() {
                const bool output = this->captured;
                if (SDL_SetRelativeMouseMode(SDL_FALSE) != 0) {
                    std::cout << "Could not exit relative mouse mode\nERROR [" << SDL_GetTicks() << "]: " << SDL_GetError() << "\n";
                }
                this->x_pos = this->y_pos = 0;
                this->captured = false;
                this->released = true;
                return output;
            }
            bool release(bengine::render_window &win) {
                const bool output = this->captured;
                if (SDL_SetRelativeMouseMode(SDL_FALSE) != 0) {
                    std::cout << "Could not exit relative mouse mode\nERROR [" << SDL_GetTicks() << "]: " << SDL_GetError() << "\n";
                }
                win.center_mouse();
                this->x_pos = this->y_pos = 0;
                this->captured = false;
                this->released = true;
                return output;
            }

            double get_sensitivity() const {
                return this->sensitivity;
            }
            void set_sensitivity(const double &sensitivity) {
                this->sensitivity = sensitivity;
            }
    };

    // \brief A rectangle on a 2D screen that serves as a "button"
    class click_rectangle {
        protected:
            // \brief x-position of the top-left corner of the rectangle
            int x_pos = 0;
            // \brief y-position of the top-left corner of the rectangle
            int y_pos = 0;
            // \brief width of the rectangle
            int width = 0;
            // \brief height of the rectangle
            int height = 0;

        public:
            /** bengine::click_rectangle constructor
             * \param x_pos x-position of the top-left corner of the rectangle
             * \param y_pos y-position of the top-left corner of the rectangle
             * \param width width of the rectangle
             * \param height height of the rectangle
             */
            click_rectangle(const int x_pos = 0, const int y_pos = 0, const int width = 0, const int height = 0) {
                this->set_rectangle(x_pos, y_pos, width, height);
            }
            // \brief bengine::click_rectangle deconstructor
            ~click_rectangle() {}

            bengine::click_rectangle& operator=(const bengine::click_rectangle &rhs) {
                this->x_pos = rhs.get_x_pos();
                this->y_pos = rhs.get_y_pos();
                this->width = rhs.get_width();
                this->height = rhs.get_height();
                return *this;
            }

            /** Get the x-position of the top-left corner of the rectangle
             * \returns The x-position of the top-left corner of the rectangle
             */
            int get_x_pos() const {
                return this->x_pos;
            }
            /** Get the y-position of the top-left corner of the rectangle
             * \returns The y-position of the top-left corner of the rectangle
             */
            int get_y_pos() const {
                return this->y_pos;
            }
            /** Get the width of the rectangle
             * \returns The width of the rectangle
             */
            int get_width() const {
                return this->width;
            }
            /** Get the height of the rectangle
             * \returns The height of the rectangle
             */
            int get_height() const {
                return this->height;
            }
            /** Set the x-position of the top-left corner of the rectangle while maintaining the rectangle's width
             * \param x_pos The new x-position of the top-left corner of the rectangle
             */
            void set_x_pos(const int &x_pos) {
                this->x_pos = x_pos;
            }
            /** Set the y-position of the top-left corner of the rectangle while maintaining the rectangle's height
             * \param y_pos The new y-position of the top-left corner of the rectangle
             */
            void set_y_pos(const int &y_pos) {
                this->y_pos = y_pos;
            }
            /** Set the width of the rectangle (top-left corner remains the same, bottom-right corner moves)
             * \param width The new width of the rectangle
             */
            void set_width(const int &width) {
                this->width = width;
            }
            /** Set the height of the rectangle (top-left corner remains the same, bottom-right corner moves)
             * \param height The new height of the rectangle
             */
            void set_height(const int &height) {
                this->height = height;
            }
            /** Set the position of the top-left corner of the rectangle as well as its width and height
             * \param x_pos The new x-position of the top-left corner of the rectangle
             * \param y_pos The new y-position of the top-left corner of the rectangle
             * \param width The new width of the rectangle
             * \param height The new height of the rectangle
             */
            void set_rectangle(const int x_pos, const int y_pos, const int &width, const int &height) {
                this->x_pos = x_pos;
                this->y_pos = y_pos;
                this->width = width;
                this->height = height;
            }

            /** Get the x-position of the top-left corner of the rectangle
             * \returns The x-position of the top-left corner of the rectangle
             */
            int get_x1() const {
                return this->x_pos;
            }
            /** Get the y-position of the top-left corner of the rectangle
             * \returns The y-position of the top-left corner of the rectangle
             */
            int get_y1() const {
                return this->y_pos;
            }
            /** Get the x-position of the bottom-right corner of the rectangle
             * \returns The x-position of the bottom-right corner of the rectangle
             */
            int get_x2() const {
                return this->x_pos + this->width;
            }
            /** Get the y-position of the bottom-right corner of the rectangle
             * \returns The y-position of the bottom-right corner of the rectangle
             */
            int get_y2() const {
                return this->y_pos + this->height;
            }
            /** Set the x-position of the top-left corner of the rectangle
             * \param x_pos The new x-position of the top-left corner of the rectangle
             */
            void set_x1(const int &x_pos) {
                this->x_pos = x_pos;
            }
            /** Set the y-position of the top-left corner of the rectangle
             * \param y_pos The new y-position of the top-left corner of the rectangle
             */
            void set_y1(const int &y_pos) {
                this->y_pos = y_pos;
            }
            /** Set the x-position of the bottom-right corner of the rectangle
             * 
             * If the inputted x-position is less than the rectangle's x1, then x1 becomes x2 and the inputted x-position becomes x1; width is adjusted accordingly
             * 
             * \param x_pos The new x-position of the bottom-right corner of the rectangle
             */
            void set_x2(const int &x_pos) {
                if (x_pos <= this->x_pos) {
                    this->width = this->x_pos - x_pos;
                    this->x_pos = x_pos;
                    return;
                }
                this->width = x_pos - this->x_pos;
            }
            /** Set the y-position of the bottom-right corner of the rectangle
             * 
             * If the inputted y-position is less than the rectangle's y1, then y1 becomes y2 and the inputted y-position becomes y1; height is adjusted accordingly
             * 
             * \param y_pos The new y-position of the bottom-right corner of the rectangle
             */
            void set_y2(const int &y_pos) {
                if (y_pos <= this->y_pos) {
                    this->height = this->y_pos - y_pos;
                    this->y_pos = y_pos;
                    return;
                }
                this->height = y_pos - this->y_pos;
            }
            /** Change the position of all four corners of the rectangle
             * \param x1 The new x-position of the top-left corner of the rectangle
             * \param y1 The new y-position of the top-left corner of the rectangle
             * \param x2 The new x-position of the bottom-right corner of the rectangle
             * \param x2 The new y-position of the bottom-right corner of the rectangle
             */
            void set_corners(const int x1, const int y1, const int x2, const int y2) {
                if (x1 <= x2) {
                    this->x_pos = x1;
                    this->width = x2 - x1;
                } else {
                    this->x_pos = x2;
                    this->width = x1 - x2;
                }
                if (y1 <= y2) {
                    this->y_pos = y1;
                    this->height = y2 - y1;
                } else {
                    this->y_pos = y2;
                    this->height = y1 - y2;
                }
            }

            /** Check to see if the mouse is within the rectangle
             * \param mstate The mouse's state
             * \returns Whether the mouse is within the rectangle or not
             */
            bool check_position(const bengine::generic_mouse_state &mstate) const {
                if (this->get_width() == 0 || this->get_height() == 0) {
                    return false;
                }
                return mstate.get_x_pos() >= this->x_pos && mstate.get_y_pos() >= this->y_pos && mstate.get_x_pos() <= this->x_pos + this->width && mstate.get_y_pos() <= this->y_pos + height;
            }
            /** Check to see if the mouse is within the rectangle and clicked the correct button
             * \param mstate The mouse's state
             * \param buttons Which buttons to check for (OR'd together from bengine::base_mouse_state::button_names)
             * \returns Whether the mouse is both within the rectangle and clicked the right button or not
             */
            bool check_button(const bengine::generic_mouse_state &mstate, const bengine::generic_mouse_state::button_names &buttons) const {
                return mstate.pressed(buttons) ? this->check_position(mstate) : false;
            }
    };

    // \brief A circle on a 2D screen that serves as a "button"
    class click_circle {
        protected:
            // \brief x-position of the center of the circle
            int x_pos = 0;
            // \brief y-position of the center of the circle
            int y_pos = 0;
            // \brief Radius of the circle
            int radius = 0;

        public:
            /** bengine::click_circle constructor
             * \param x_pos x-position of the center of the circle
             * \param y_pos y-position of the center of the circle
             * \param radius Radius of the circle
             */
            click_circle(const int &x_pos = 0, const int &y_pos = 0, const int &radius = 0) : x_pos(x_pos), y_pos(y_pos), radius(radius) {}
            // \brief bengine::click_circle deconstructor
            ~click_circle() {}

            /** Get the x-position of the center of the circle
             * \returns The x-position of the center of the circle
             */
            int get_x_pos() const {
                return this->x_pos;
            }
            /** Get the y-position of the center of the circle
             * \returns The y-position of the center of the circle
             */
            int get_y_pos() const {
                return this->y_pos;
            }
            /** Get the radius of the circle
             * \returns The radius of the circle
             */
            int get_radius() const {
                return this->radius;
            }
            /** Set the x-position of the center of the circle
             * \param x_pos The new x-position of the center of the circle
             * \returns The old x-position of the center of the circle
             */
            void set_x_pos(const int &x_pos) {
                this->x_pos = x_pos;
            }
            /** Set the y-position of the center of the circle
             * \param y_pos The new y-position of the center of the circle
             * \returns The old y-position of the center of the circle
             */
            void set_y_pos(const int &y_pos) {
                this->y_pos = y_pos;
            }
            /** Set the radius of the circle
             * \param radius The new radius of the circle
             * \returns The old radius of the circle
             */
            void set_radius(const int &radius) {
                this->radius = radius;
            }

            /** Check to see if the mouse is within the circle
             * \param mstate The mouse's state
             * \returns Whether the mouse is within the circle or not
             */
            bool check_position(const bengine::generic_mouse_state &mstate) const {
                if (this->radius == 0) {
                    // A radius of zero will still let you click on the center of the circle
                    return mstate.get_x_pos() == this->x_pos && mstate.get_y_pos() == this->y_pos;
                }
                // First a square is checked to save on some math in the most common cases, then the circle is checked if the mouse is within the matching square
                if (mstate.get_x_pos() < this->x_pos - radius || mstate.get_x_pos() > this->x_pos + radius || mstate.get_y_pos() < this->y_pos - radius || mstate.get_y_pos() > this->y_pos + radius) {
                    return false;
                }
                const double root = std::round(std::sqrt(this->radius * this->radius - (mstate.get_x_pos() - this->x_pos) * (mstate.get_x_pos() - this->x_pos)));
                return mstate.get_y_pos() < this->y_pos + root && mstate.get_y_pos() > this->y_pos - root;
            }
            /** Check to see if the mouse is within the circle and clicked the correct button
             * \param mstate The mouse's state
             * \param buttons Which buttons to check for (OR'd together from bengine::base_mouse_state::button_names)
             * \returns Whether the mouse is both within the circle and clicked the right button or not
             */
            bool check_buttons(const bengine::generic_mouse_state &mstate, const bengine::generic_mouse_state::button_names &buttons) const {
                return mstate.pressed(buttons) ? this->check_position(mstate) : false;
            }
    };

    // \brief A matrix on a 2D screen that serves as a grid of "buttons" (as opposed to making many, many bengine::click_rectangles)
    class click_matrix : public bengine::click_rectangle {
        protected:
            // \brief The amount of rows in the matrix
            int rows = 1;
            // \brief The amount of columns in the matrix
            int cols = 1;
            // \brief A width for the matrix that allows for width / cols without a remainder; will be greater than or equal to the value of the regular width of the matrix
            int adjusted_width = 1;
            // \brief A height for the matrix that allows for height / rows without a remainder; will be greater than or equal to the value of the regular height of the matrix
            int adjusted_height = 1;

            // \brief Update the matrix's evenWidth
            void update_adjusted_width() {
                this->adjusted_width = bengine::math_helper::least_common_multiple<int>(this->get_width(), this->cols);
            }
            // \brief Update the matrix's evenHeight
            void update_adjusted_height() {
                this->adjusted_height = bengine::math_helper::least_common_multiple<int>(this->get_height(), this->rows);
            }

        public:
            /** bengine::click_matrix constructor
             * \param x_pos x-position of the top-left corner of the matrix
             * \param y_pos y-position of the top-left corner of the matrix
             * \param width width of the matrix
             * \param height height of the matrix
             * \param rows The amount of rows that the matrix will have
             * \param cols The amount of columns that the matrix will have
             */
            click_matrix(const int x_pos = 0, const int y_pos = 0, const int width = 0, const int height = 0, const int &rows = 1, const int &cols = 1) : bengine::click_rectangle(x_pos, y_pos, width, height) {
                this->set_rows(rows);
                this->set_cols(cols);
                this->update_adjusted_width();
                this->update_adjusted_height();
            }
            // \brief bengine::click_matrix deconstructor
            ~click_matrix() {}

            /** Get the amount of rows in the matrix
             * \returns The amount of rows in the matrix
             */
            int get_rows() const {
                return this->rows;
            }
            /** Get the amount of columns in the matrix
             * \returns The amount of columns in the matrix
             */
            int get_cols() const {
                return this->cols;
            }
            /** Get the amount of rows in the matrix
             * \param rows The new amount of rows for the matrix
             */
            void set_rows(const int &rows) {
                this->rows = rows > this->get_height() ? this->get_height() : rows;
            }
            /** Get the amount of columns in the matrix
             * \param cols The new amount of columns for the matrix
             */
            void set_cols(const int &cols) {
                this->cols = cols > this->get_width() ? this->get_width() : cols;
            }
            /** Get the width for the matrix that allows for width / cols without a remainder; greater than or equal to the value of the regular width of the matrix
             * \returns The even width of the matrix
             */
            int get_adjusted_width() const {
                return this->adjusted_width;
            }
            /** Get the height for the matrix that allows for height / rows without a remainder; greater than or equal to the value of the regular height of the matrix
             * \returns The even height of the matrix
             */
            int get_adjusted_height() const {
                return this->adjusted_height;
            }

            /** Convert a positional integer to one representing its matching row within the matrix
             * \param pos The positional integer to convert into a row index
             * \returns A row index matching the inputted position and this matrix's dimensions
             */
            int pos_to_row(const long long &pos) const {
                return pos / this->cols;
            }
            /** Convert a positional integer to one representing its matching column within the matrix
             * \param pos The positional integer to convert into a column index
             * \returns A column index matching the inputted position and this matrix's dimensions
             */
            int pos_to_col(const long long &pos) const {
                return pos % this->cols;
            }
            /** Convert a positional integer to one representing its matching row within a matrix
             * \param pos The positional integer to convert into a row index
             * \param cols The amount of columns in the matrix
             * \returns A row index matching the inputted position and the matrix's dimensions
             */
            static int pos_to_row(const long long &pos, const int &cols) {
                return pos / cols;
            }
            /** Convert a positional integer to one representing its matching column within a matrix
             * \param pos The positional integer to convert into a column index
             * \param cols The amount of columns in the matrix
             * \returns A column index matching the inputted position and the matrix's dimensions
             */
            static int pos_to_col(const long long &pos, const int &cols) {
                return pos % cols;
            }

            /** Set the x-position of the top-left corner of the matrix
             * \param x_pos The new x-position of the top-left corner of the matrix
             */
            void set_x1(const int &x_pos) {
                bengine::click_rectangle::set_x1(x_pos);
                this->update_adjusted_width();
            }
            /** Set the y-position of the top-left corner of the matrix
             * \param y_pos The new y-position of the top-left corner of the matrix
             */
            void set_y1(const int &y_pos) {
                bengine::click_rectangle::set_y1(y_pos);
                this->update_adjusted_height();
            }
            /** Set the x-position of the bottom-right corner of the matrix
             * \param x_pos The new x-position of the bottom-right corner of the matrix
             */
            void set_x2(const int &x_pos) {
                bengine::click_rectangle::set_x2(x_pos);
                this->update_adjusted_width();
            }
            /** Set the y-position of the bottom-right corner of the matrix
             * \param y_pos The new y-position of the bottom-right corner of the matrix
             */
            void set_y2(const int &y_pos) {
                bengine::click_rectangle::set_y2(y_pos);
                this->update_adjusted_height();
            }
            /** Change the position of all four corners of the matrix
             * \param x1 The new x-position of the top-left corner of the matrix
             * \param y1 The new y-position of the top-left corner of the matrix
             * \param x2 The new x-position of the bottom-right corner of the matrix
             * \param x2 The new y-position of the bottom-right corner of the matrix
             */
            void set_corners(const int x1, const int y1, const int x2, const int y2) {
                bengine::click_rectangle::set_corners(x1, y1, x2, y2);
                this->update_adjusted_width();
                this->update_adjusted_height();
            }

            /** Set the width of the matrix (top-left corner remains the same, bottom-right corner moves)
             * \param width The new width of the matrix
             */
            void set_width(const int &width) {
                bengine::click_rectangle::set_width(width);
                this->update_adjusted_width();
            }
            /** Set the height of the matrix (top-left corner remains the same, bottom-right corner moves)
             * \param height The new height of the matrix
             */
            void set_height(const Uint16 &height) {
                bengine::click_rectangle::set_height(height);
                this->update_adjusted_height();
            }
            /** Set the position of the top-left corner of the matrix as well as its width and height
             * \param x_pos The new x-position of the top-left corner of the matrix
             * \param y_pos The new y-position of the top-left corner of the matrix
             * \param width The new width of the matrix
             * \param height The new height of the matrix
             */
            void set_rectangle(const int x_pos, const int y_pos, const int &width, const int &height) {
                bengine::click_rectangle::set_rectangle(x_pos, y_pos, width, height);
                this->update_adjusted_width();
                this->update_adjusted_height();
            }

            /** Find the cell within the matrix that the mouse is located in
             * \param mstate The mouse's state
             * \returns The cell within the matrix that the mouse is located in
             */
            long long check_position(const bengine::generic_mouse_state &mstate) const {
                if (this->get_width() == 0 || this->get_height() == 0) {
                    return __LONG_LONG_MAX__;
                }
                if (mstate.get_x_pos() < this->x_pos || mstate.get_x_pos() > this->x_pos + this->width || mstate.get_y_pos() < this->y_pos || mstate.get_y_pos() > this->y_pos + this->width) {
                    return __LONG_LONG_MAX__;
                }
                const int col = ((mstate.get_x_pos() - this->x_pos) * (double)(adjusted_width / this->get_width())) / (adjusted_width / this->cols);
                const int row = ((mstate.get_y_pos() - this->y_pos) * (double)(adjusted_height / this->get_height())) / (adjusted_height / this->rows);

                return (row >= this->rows ? this->rows - 1 : row) * this->cols + (col >= this->cols ? this->cols - 1 : col);
            }
            /** Find the cell within the matrix that the mouse is located in and confirm that the mouse has clicked the correct button
             * \param mstate The mouse's state
             * \param button Which buttons to check for (OR'd together from bengine::base_mouse_state::button_names)
             * \returns The cell within the matrix that the mouse is located in if the mouse has clicked the correct button; __LONG_LONG_MAX__ as a sentinal value otherwise
             */
            long long check_buttons(const bengine::generic_mouse_state &mstate, const bengine::generic_mouse_state::button_names &button) const {
                return mstate.pressed(button) ? this->check_position(mstate) : __LONG_LONG_MAX__;
            }
    };
}

#endif // BENGINE_MOUSE_hpp
