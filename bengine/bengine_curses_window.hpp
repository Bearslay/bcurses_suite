#ifndef BENGINE_CURSES_WINDOW_hpp
#define BENGINE_CURSES_WINDOW_hpp

#include <ncurses.h>
#include <locale.h>
#include <unordered_map>

#include "bengine_helpers.hpp"

namespace bengine {
    class curses_window {
        private:
            static const std::vector<unsigned char> box_drawing_key;
            static const std::wstring box_drawing_chars;
            static const std::vector<std::wstring> matrix_text_key;

            static unsigned char default_cell_color_pair;
            static unsigned short default_cell_attributes;
            static unsigned short default_wrapping_width;
            static unsigned char default_wrapping_mode;

            static wchar_t default_cell_character;
            static unsigned short default_box_drawing_style;

        public:
            // \brief A number representing one of the first 16 color pairs initialized upon startup (names assume that nothing was changed)
            enum preset_colors : unsigned char {
                BLACK = 0,         // black (0, 0, 0)
                WHITE = 1,         // white (255, 255, 255)
                LIGHT_GRAY = 2,    // light gray (170, 170, 170)
                DARK_GRAY = 3,     // dark gray (85, 85, 85)
                BROWN = 4,         // brown (117, 60, 19)
                RED = 5,           // red (255, 0, 0)
                MAROON = 6,        // maroon (115, 0, 0)
                ORANGE = 7,        // orange (255, 115, 0)
                YELLOW = 8,        // yellow (255, 255, 0)
                LIME = 9,          // lime (0, 255, 0)
                GREEN = 10,        // green (0, 115, 0)
                CYAN = 11,         // cyan (0, 255, 255)
                TEAL = 12,         // teal (0, 115, 115)
                BLUE = 13,         // blue (0, 0, 255)
                MAGENTA = 14,      // magenta (255, 0, 255)
                PURPLE = 15        // purple (115, 0, 115)
            };

            // \brief Different primary and dash styles to be used when utilizing the box drawing functions of a bengine::curses_window; note that doubled can't cleanly merge with heavy and that doubled also can't be dashed
            enum box_drawing_styles : unsigned short {
                LIGHT_SQUARE = 1,           // light lines in both the horizontal and vertical directions where corner characters are square
                LIGHT_ROUNDED = 2,          // light lines in both the horizontal and vertical directions where corner characters are rounded
                HEAVY_BOTH = 4,             // heavy lines in both the horizontal and vertical directions
                HEAVY_HORIZONTAL = 8,       // heavy lines only in the horizontal direction with light lines in the vertical direction
                HEAVY_VERTICAL = 16,        // heavy lines only in the vertical direction with light lines in the horizontal direction
                DOUBLED_BOTH = 32,          // doubled lines in both the horizontal and vertical directions
                DOUBLED_HORIZONTAL = 64,    // doubled lines only in the horizontal direction with light lines in the vertical direction
                DOUBLED_VERTICAL = 128,     // doubled lines only in the vertical direction with light lines in the horizontal direction
                NO_DASH = 256,              // a line with no gaps
                DOUBLE_DASH = 512,          // a line with 1 gap
                TRIPLE_DASH = 1024,         // a line with 2 gaps
                QUADRUPLE_DASH = 2048,      // a line with 3 gaps
                SINGLE_DASH_1 = 4096,       // the first variation of a line with 1 large gap using half of the character
                SINGLE_DASH_2 = 8192        // the second variation of a line with 1 large gap using half of the character
            };

            enum wrapping_modes : unsigned char {
                NONE = 0,       // do not wrap at all
                BASIC = 1,      // wrap to the left edge of a window (where x = 0)
                INDENTED = 2    // wrap to the x-position that the string/int/whatever originated at
            };

            // \brief Different attributes of a cell supported by ncurses or bengine
            enum cell_attributes : unsigned short {
                BOLD = 1,                      // whether the cell is bolded or not
                ITALIC = 2,                    // whether the cell is italicized or not
                UNDERLINED = 4,                // whether the cell is underlined or not
                REVERSED_COLOR = 8,            // whether the cell's foreground color and background color are reversed or not (good for highlighting in many cases)
                BLINKING = 16,                 // whether the cell blinks or not
                DIM = 32,                      // whether the cell is dim or not
                INVISIBLE = 64,                // whether the cell is invisible or not
                STANDOUT = 128,                // whether the cell "stands out" or not
                PROTECTED = 256,               // whether the cell is "protected" or not
                ALTERNATE_CHARACTER = 512,     // whether the cell uses the alternate ncurses character set or not
                BOX_DRAWING_MERGABLE = 1024    // whether the cell (if a box drawing character) "merges" with other box drawing characters rather than be overridden & ignored
            };

            // \brief A single cell within the grid that makes up a window containing its character, color, and other attributes
            struct cell {
                // \brief Character contained in the cell
                wchar_t character = bengine::curses_window::default_cell_character;
                // \brief Color pair used for the cell
                unsigned char color_pair = bengine::curses_window::default_cell_color_pair;
                // \brief Attributes of a cell, stored as several bitwise booleans
                unsigned short attributes = bengine::curses_window::default_cell_attributes;
            };

            enum write_arg_options : unsigned char {
                COLOR = 1,
                ATTRIBUTES = 2,
                WRAPPING_WIDTH = 4,
                WRAPPING_MODE = 8
            };

            struct write_args {
                unsigned char color_pair = bengine::curses_window::default_cell_color_pair;
                unsigned short attributes = bengine::curses_window::default_cell_attributes;
                unsigned short wrapping_width = bengine::curses_window::default_wrapping_width;
                unsigned char wrapping_mode = bengine::curses_window::default_wrapping_mode;
            };

            static bengine::curses_window::write_args make_write_args(const unsigned char &write_args, const std::vector<unsigned short> &args) {
                if (args.size() < 1) {
                    return bengine::curses_window::default_write_args;
                }
                bengine::curses_window::write_args output = bengine::curses_window::default_write_args;

                unsigned char index = 0;
                if (bengine::bitwise_manipulator::check_for_activated_bits(write_args, static_cast<unsigned char>(bengine::curses_window::write_arg_options::COLOR))) {
                    output.color_pair = static_cast<unsigned char>(args.at(index++));
                    if (index >= args.size()) {
                        return output;
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(write_args, static_cast<unsigned char>(bengine::curses_window::write_arg_options::ATTRIBUTES))) {
                    output.attributes = args.at(index++);
                    if (index >= args.size()) {
                        return output;
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(write_args, static_cast<unsigned char>(bengine::curses_window::write_arg_options::WRAPPING_WIDTH))) {
                    output.wrapping_width = args.at(index++);
                    if (index >= args.size()) {
                        return output;
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(write_args, static_cast<unsigned char>(bengine::curses_window::write_arg_options::WRAPPING_MODE))) {
                    output.wrapping_mode = static_cast<unsigned char>(args.at(index));
                }
                return output;
            }

        private:
            static bengine::curses_window::write_args default_write_args;

            // \brief x-position (col) of the top-left corner of the window
            int x_pos = 0;
            // \brief y-position (row) of the top-left corner of the window
            int y_pos = 0;

            unsigned short width_2 = COLS / 2;
            unsigned short height_2 = LINES / 2;

            std::vector<std::vector<bengine::curses_window::cell>> grid = std::vector<std::vector<bengine::curses_window::cell>>(LINES, std::vector<bengine::curses_window::cell>(COLS));

            void apply_cell_to_screen(const unsigned short &x, const unsigned short &y) const {
                attron(COLOR_PAIR(this->grid.at(y).at(x).color_pair));
                this->toggle_attributes(this->grid.at(y).at(x).attributes, true);

                if (this->grid.at(y).at(x).character == L'%') {
                    mvaddch(y, x, '%');
                } else {
                    mvaddwstr(y, x, std::wstring(1, this->grid.at(y).at(x).character).c_str());
                }

                this->toggle_attributes(this->grid.at(y).at(x).attributes, false);
                attroff(COLOR_PAIR(this->grid.at(y).at(x).color_pair));
            }

            void toggle_attributes(const unsigned short &attributes, const bool &attribute_state) const {
                if (bengine::bitwise_manipulator::check_for_activated_bits(attributes, static_cast<unsigned short>(bengine::curses_window::cell_attributes::BOLD))) {
                    if (attribute_state) {
                        attron(A_BOLD);
                    } else {
                        attroff(A_BOLD);
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(attributes, static_cast<unsigned short>(bengine::curses_window::cell_attributes::ITALIC))) {
                    if (attribute_state) {
                        attron(A_ITALIC);
                    } else {
                        attroff(A_ITALIC);
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(attributes, static_cast<unsigned short>(bengine::curses_window::cell_attributes::UNDERLINED))) {
                    if (attribute_state) {
                        attron(A_UNDERLINE);
                    } else {
                        attroff(A_UNDERLINE);
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(attributes, static_cast<unsigned short>(bengine::curses_window::cell_attributes::REVERSED_COLOR))) {
                    if (attribute_state) {
                        attron(A_REVERSE);
                    } else {
                        attroff(A_REVERSE);
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(attributes, static_cast<unsigned short>(bengine::curses_window::cell_attributes::BLINKING))) {
                    if (attribute_state) {
                        attron(A_BLINK);
                    } else {
                        attroff(A_BLINK);
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(attributes, static_cast<unsigned short>(bengine::curses_window::cell_attributes::DIM))) {
                    if (attribute_state) {
                        attron(A_DIM);
                    } else {
                        attroff(A_DIM);
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(attributes, static_cast<unsigned short>(bengine::curses_window::cell_attributes::INVISIBLE))) {
                    if (attribute_state) {
                        attron(A_INVIS);
                    } else {
                        attroff(A_INVIS);
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(attributes, static_cast<unsigned short>(bengine::curses_window::cell_attributes::STANDOUT))) {
                    if (attribute_state) {
                        attron(A_STANDOUT);
                    } else {
                        attroff(A_STANDOUT);
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(attributes, static_cast<unsigned short>(bengine::curses_window::cell_attributes::PROTECTED))) {
                    if (attribute_state) {
                        attron(A_PROTECT);
                    } else {
                        attroff(A_PROTECT);
                    }
                }
                if (bengine::bitwise_manipulator::check_for_activated_bits(attributes, static_cast<unsigned short>(bengine::curses_window::cell_attributes::ALTERNATE_CHARACTER))) {
                    if (attribute_state) {
                        attron(A_ALTCHARSET);
                    } else {
                        attroff(A_ALTCHARSET);
                    }
                }
            }

            bool check_coordinate_bounds(const unsigned short &x, const unsigned short &y) const {
                return x < this->get_width() && y < this->get_height();
            }
            bool check_coordinate_bounds(const std::pair<unsigned short, unsigned short> &pos) const {
                return this->check_coordinate_bounds(pos.first, pos.second);
            }

        public:
            // default, take up available terminal
            curses_window() {}
            // position is totally custom
            curses_window(const int &x_pos, const int &y_pos, const unsigned short &width, const unsigned short &height) {
                this->x_pos = x_pos;
                this->y_pos = y_pos;
                this->grid = std::vector<std::vector<bengine::curses_window::cell>>(height > 0 ? height : 1, std::vector<bengine::curses_window::cell>(width > 0 ? width : 1));
                this->width_2 = this->get_width() / 2;
                this->height_2 = this->get_height() / 2;
            }
            // position is centered relative to the terminal
            curses_window(const unsigned short &width, const unsigned short &height) {
                this->x_pos = COLS / 2 - width / 2;
                this->y_pos = LINES / 2 - height / 2;
                this->grid = std::vector<std::vector<bengine::curses_window::cell>>(height > 0 ? height : 1, std::vector<bengine::curses_window::cell>(width > 0 ? width : 1));
                this->width_2 = this->get_width() / 2;
                this->height_2 = this->get_height() / 2;
            }
            // position is centered relative to another window
            curses_window(const bengine::curses_window &window, const unsigned short &width, const unsigned short &height) {
                this->x_pos = window.get_x_pos() + window.get_width() / 2 - width / 2;
                this->y_pos = window.get_y_pos() + window.get_height() / 2 - height / 2;
                this->grid = std::vector<std::vector<bengine::curses_window::cell>>(height > 0 ? height : 1, std::vector<bengine::curses_window::cell>(width > 0 ? width : 1));
                this->width_2 = this->get_width() / 2;
                this->height_2 = this->get_height() / 2;
            }
            ~curses_window() {}

            int get_x_pos() const {
                return this->x_pos;
            }
            int get_y_pos() const {
                return this->y_pos;
            }
            void set_x_pos(const int &x_pos) {
                this->x_pos = x_pos;
            }
            void set_y_pos(const int &y_pos) {
                this->y_pos = y_pos;
            }

            int get_left_x() const {
                return this->x_pos;
            }
            int get_right_x() const {
                return this->x_pos + this->get_width() - 1;
            }
            int get_top_y() const {
                return this->y_pos;
            }
            int get_bottom_y() const {
                return this->y_pos + this->get_height() - 1;
            }

            unsigned short get_width() const {
                return this->grid.at(0).size();
            }
            unsigned short get_height() const {
                return this->grid.size();
            }
            unsigned short get_width_2() const {
                return this->width_2;
            }
            unsigned short get_height_2() const {
                return this->height_2;
            }
            void set_width(const unsigned short &width) {
                const unsigned short processed_width = width == 0 ? 1 : width;
                if (processed_width < this->get_width()) {
                    for (unsigned short row = 0; row < this->get_height(); row++) {
                        for (unsigned short col = this->get_width() - processed_width + 1; col > 1; col--) {
                            this->grid.at(row).pop_back();
                        }
                    }
                }
                for (unsigned short row = 0; row < this->get_height(); row++) {
                    for (unsigned short col = this->get_width(); col < processed_width; col++) {
                        this->grid.at(row).emplace_back();
                    }
                }
                this->width_2 = this->get_width() / 2;
            }
            void set_height(const unsigned short &height) {
                const unsigned short processed_height = height == 0 ? 1 : height;
                if (processed_height < this->get_height()) {
                    for (int row = this->get_height() - processed_height + 1; row > 1; row--) {
                        this->grid.pop_back();
                    }
                    return;
                }
                for (int row = processed_height - this->get_height() + 1; row > 1; row--) {
                    this->grid.emplace_back(std::vector<bengine::curses_window::cell>(this->get_width()));
                }
                this->height_2 = this->get_height() / 2;
            }

            // get desired cell's character, if cell is out of bounds then return a space (cell default)
            wchar_t get_cell_character(const unsigned short &x, const unsigned short &y) const {
                return this->check_coordinate_bounds(x, y) ? this->grid.at(y).at(x).character : bengine::curses_window::default_cell_character;
            }
            // get desired cell's color pair, if the cell is out of bounds then return 1 (cell default)
            unsigned char get_cell_color(const unsigned short &x, const unsigned short &y) const {
                return this->check_coordinate_bounds(x, y) ? this->grid.at(y).at(x).color_pair : bengine::curses_window::default_cell_color_pair;
            }
            unsigned short get_cell_attributes(const unsigned short &x, const unsigned short &y) const {
                return this->check_coordinate_bounds(x, y) ? this->grid.at(y).at(x).attributes : bengine::curses_window::default_cell_attributes;
            }
            bengine::curses_window::cell get_cell(const unsigned short &x, const unsigned short &y) const {
                return this->check_coordinate_bounds(x, y) ? this->grid.at(y).at(x) : bengine::curses_window::cell();
            }

            // see if a cell has the desired attributes active or not
            bool check_cell_attribute_states(const unsigned short &x, const unsigned short &y, const unsigned short &attributes) const {
                return this->check_coordinate_bounds(x, y) && bengine::bitwise_manipulator::check_for_activated_bits(this->grid.at(y).at(x).attributes, static_cast<unsigned short>(attributes));
            }

            void apply_to_screen() const {
                if (this->get_left_x() >= COLS || this->get_right_x() < 0 || this->get_bottom_y() < 0 || this->get_top_y() >= LINES) {
                    return;
                }

                for (int row = this->y_pos; row < this->y_pos + this->get_height(); row++) {
                    for (int col = this->x_pos; col < this->x_pos + this->get_width(); col++) {
                        if (col < 0) {
                            continue;
                        }
                        if (col >= COLS) {
                            break;
                        }
                        this->apply_cell_to_screen(col, row);
                    }
                    if (row < 0) {
                        continue;
                    }
                    if (row >= LINES) {
                        return;
                    }
                }
            }
            void apply_region_to_screen(const unsigned short &x, const unsigned short &y, const unsigned short &width, const unsigned short &height) const {
                if (this->get_left_x() + x >= COLS || this->get_left_x() + x + width < 0 || this->get_top_y() + y + height < 0 || this->get_top_y() + y >= LINES) {
                    return;
                }

                for (int row = this->y_pos + y; row < this->y_pos + y + height; row++) {
                    for (int col = this->x_pos + x; col < this->x_pos + x + width; col++) {
                        if (col < 0) {
                            continue;
                        }
                        if (col >= COLS || col >= this->get_width()) {
                            break;
                        }
                        this->apply_cell_to_screen(col, row);
                    }
                    if (row < 0) {
                        continue;
                    }
                    if (row >= LINES || row >= this->get_height()) {
                        return;
                    }
                }
            }

            void clear_from_screen() const {
                if (this->get_left_x() >= COLS || this->get_right_x() < 0 || this->get_bottom_y() < 0 || this->get_top_y() >= LINES) {
                    return;
                }

                for (int row = this->y_pos; row < this->y_pos + this->get_height(); row++) {
                    for (int col = this->x_pos; col < this->x_pos + this->get_width(); col++) {
                        if (col < 0) {
                            continue;
                        }
                        if (col >= COLS) {
                            break;
                        }
                        mvaddch(row, col, ' ');
                    }
                    if (row < 0) {
                        continue;
                    }
                    if (row >= LINES) {
                        return;
                    }
                }
            }
            void clear_region_from_screen(const unsigned short &x, const unsigned short &y, const unsigned short &width, const unsigned short &height) const {
                if (this->get_left_x() + x >= COLS || this->get_left_x() + x + width < 0 || this->get_top_y() + y + height < 0 || this->get_top_y() + y >= LINES) {
                    return;
                }

                for (int row = this->y_pos + y; row < this->y_pos + y + height; row++) {
                    for (int col = this->x_pos + x; col < this->x_pos + x + width; col++) {
                        if (col < 0) {
                            continue;
                        }
                        if (col >= COLS || col >= this->get_width()) {
                            break;
                        }
                        mvaddch(row, col, ' ');
                    }
                    if (row < 0) {
                        continue;
                    }
                    if (row >= LINES || row >= this->get_height()) {
                        return;
                    }
                }
            }

            // write character to window and return the position of where the cursor would be after writing the character (x + 1 unless wrapping)
            std::pair<unsigned short, unsigned short> write_character(const unsigned short &x, const unsigned short &y, const wchar_t &character, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                if (!this->check_coordinate_bounds(x, y)) {
                    return std::make_pair(x, y);
                }

                this->grid.at(y).at(x).character = character;
                this->grid.at(y).at(x).color_pair = args.color_pair;
                this->grid.at(y).at(x).attributes = args.attributes;

                if (x >= this->get_width() - 1) {
                    switch (args.wrapping_mode) {
                        case bengine::curses_window::wrapping_modes::NONE:
                            return std::make_pair(x + 1, y);
                        default:
                        case bengine::curses_window::wrapping_modes::BASIC:
                            return std::make_pair(0, y + 1);
                        case bengine::curses_window::wrapping_modes::INDENTED:
                            return std::make_pair(x, y);
                    }
                }
                return std::make_pair(x + 1, y);
            }
            std::pair<unsigned short, unsigned short> write_character(const std::pair<unsigned short, unsigned short> &pos, const wchar_t &character, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                return this->write_character(pos.first, pos.second, character, args);
            }

            std::pair<unsigned short, unsigned short> write_string(const unsigned short &x, const unsigned short &y, const std::wstring &string, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                if (!this->check_coordinate_bounds(x, y)) {
                    return std::make_pair(x, y);
                }

                std::pair<unsigned short, unsigned short> pos = std::make_pair(x, y);
                unsigned short line_count = 1;    // keeps track of the amount of characters written for each line, if the wrapping width is reached before the edge of the window then the function should newline anyways
                for (std::size_t i = 0; i < string.length(); i++) {
                    this->grid.at(pos.second).at(pos.first).character = string.at(i);
                    this->grid.at(pos.second).at(pos.first).color_pair = args.color_pair;
                    this->grid.at(pos.second).at(pos.first).attributes = args.attributes;

                    if (pos.first >= this->get_width() - 1 || (args.wrapping_width > 0 && line_count >= args.wrapping_width)) {
                        switch (args.wrapping_mode) {
                            case bengine::curses_window::wrapping_modes::NONE:
                                return std::make_pair(pos.first + 1, pos.second);
                            default:
                            case bengine::curses_window::wrapping_modes::BASIC:
                                pos.first = 0;
                                break;
                            case bengine::curses_window::wrapping_modes::INDENTED:
                                pos.first = x;
                                break;
                        }
                        pos.second++;
                        if (pos.second >= this->get_height() - 1) {
                            break;
                        }
                        line_count = 1;
                    } else {
                        line_count++;
                        pos.first++;
                    }
                }

                return pos;
            }
            std::pair<unsigned short, unsigned short> write_string(const std::pair<unsigned short, unsigned short> &pos, const std::wstring &string, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                return this->write_string(pos.first, pos.second, string, args);
            }

            template <class type> std::pair<unsigned short, unsigned short> write_number(const unsigned short &x, const unsigned short &y, const type &number, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                return this->write_string(x, y, bengine::string_helper::to_string(number), args);
            }
            template <class type> std::pair<unsigned short, unsigned short> write_number(const std::pair<unsigned short, unsigned short> &pos, const type &number, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                return this->write_string(pos.first, pos.second, bengine::string_helper::to_string(number), args);
            }

            void reset_all_cells() {
                this->grid = std::vector<std::vector<bengine::curses_window::cell>>(this->grid.size(), std::vector<bengine::curses_window::cell>(this->grid.at(0).size()));
            }
            void reset_cells(const unsigned short &x, const unsigned short &y, const unsigned short &width, const unsigned short &height) {
                for (unsigned short row = y; row < height; row++) {
                    for (unsigned short col = x; col < width; col++) {
                        if (col >= this->get_width()) {
                            break;
                        }
                        this->grid[row][col] = bengine::curses_window::cell();
                    }
                    if (row >= this->get_height()) {
                        return;
                    }
                }
            }

            void draw_line(const unsigned short &x, const unsigned short &y, const int &length, const bool &horiztonal = true, const unsigned short &style) {
                // if starting coordinate is out of bounds and length is greater than 0 then the line will never enter the window
                if (!this->check_coordinate_bounds(x, y) && length >= 0) {
                    return;
                }


            }
    };
    unsigned char bengine::curses_window::default_cell_color_pair = bengine::curses_window::preset_colors::WHITE;
    unsigned short bengine::curses_window::default_cell_attributes = bengine::curses_window::cell_attributes::BOX_DRAWING_MERGABLE;
    unsigned short bengine::curses_window::default_wrapping_width = 0;
    unsigned char bengine::curses_window::default_wrapping_mode = bengine::curses_window::wrapping_modes::BASIC;
    bengine::curses_window::write_args bengine::curses_window::default_write_args = {bengine::curses_window::default_cell_color_pair, bengine::curses_window::default_cell_attributes, bengine::curses_window::default_wrapping_width, bengine::curses_window::default_wrapping_mode};

    wchar_t bengine::curses_window::default_cell_character = L' ';
    unsigned short bengine::curses_window::default_box_drawing_style = bengine::curses_window::box_drawing_styles::LIGHT_SQUARE | bengine::curses_window::box_drawing_styles::NO_DASH;

    const std::vector<unsigned char> bengine::curses_window::box_drawing_key = {5, 6, 7, 9, 10, 13, 15, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 33, 34, 36, 37, 38, 40, 41, 42, 49, 51, 60, 61, 63, 65, 66, 68, 69, 70, 72, 73, 74, 76, 77, 80, 81, 82, 84, 85, 86, 88, 89, 90, 96, 97, 98, 100, 101, 102, 104, 105, 106, 112, 113, 124, 125, 129, 130, 132, 133, 134, 136, 137, 138, 144, 145, 146, 148, 149, 150, 152, 153, 154, 160, 161, 162, 164, 165, 166, 168, 169, 170, 195, 196, 199, 204, 207, 208, 211, 212, 215, 240, 243, 252, 255};
    const std::wstring bengine::curses_window::box_drawing_chars = L"┌┎╓┍┏╒╔┐┒╖─┬┰╥╼┮┲┑┓╾┭┱━┯┳╕╗═╤╦│╽└├┟┕┝┢╘╞┘┤┧┴┼╁┶┾╆┙┥┪┵┽╅┷┿╈╛╡╧╪╿┃┖┞┠┗┡┣┚┦┨┸╀╂┺╄╊┛┩┫┹╃╉┻╇╋║╙╟╚╠╜╢╨╫╝╣╩╬";
    const std::vector<std::wstring> bengine::curses_window::matrix_text_key = {
        {L"         ", L"                "},
        {L" █  █  ▄ ", L" ▐▌  ▐▌  ▝▘  ▐▌ "},
        {L"▗ ▖▝ ▘   ", L"  ▖   ▘         "},
        {L"▟▄▙▐ ▌▜▀▛", L" ▌▐ ▀▛▜▀▄▙▟▄ ▌▐ "},
        {L"▗▙▖▚▙▖▗▙▞", L"  ▖ ▞▀▛▘▝▀▛▚▝▀▛▘"},
        {L"█ ▞ ▞ ▞ █", L"▞▚ ▞▚▞▞  ▞▞▚▞ ▚▞"},
        {L"▞▚ ▞▌▖▚▞▖", L"▗▀▖ ▝▄▘ ▞▝▖▐▚▄▞▚"},
        {L" ▗▖  ▘   ", L" ▖▖  ▘▘         "},
        {L" ▞▘▐   ▚▖", L" ▗▀  ▌   ▌   ▝▄ "},
        {L"▝▚   ▌▗▞ ", L" ▀▖   ▐   ▐  ▄▘ "},
        {L"▝▄▘▗▀▖   ", L" ▚▙▘ ▘▘▘        "},
        {L"   ▝▀▘▝▀▘", L"  ▖ ▗▄▙▖  ▌     "},
        {L"       ▜ ", L"             ▝▌ "},
        {L"   ▗▄▖   ", L"    ▗▄▄▖        "},
        {L"       ▄ ", L"             ▐▌ "},
        {L"  ▞ ▞ ▞  ", L"▞▀▀▚  ▗▞ ▐▌  ▗▖ "},
        {L"▞▀▙▌▞▐▜▄▞", L"▞▀▀▙▌ ▞▐▌▞ ▐▜▄▄▞"},
        {L" ▟  ▐  ▟▖", L" ▞▌   ▌   ▌  ▄▙▖"},
        {L"▞▀▚ ▗▞▟▙▄", L"▞▀▀▚   ▞ ▄▀ ▟▄▄▄"},
        {L"▞▀▚ ▀▚▚▄▞", L"▞▀▀▚ ▄▄▞   ▐▚▄▄▞"},
        {L"▌ ▌▝▀▛  ▌", L"▌  ▌▙▄▄▙   ▌   ▌"},
        {L"▛▀▀▀▀▚▚▄▞", L"▛▀▀▀▚▄▄▖   ▐▚▄▄▞"},
        {L"▞▀▀▛▀▚▚▄▞", L"▞▀▀▀▙▄▄▖▌  ▐▚▄▄▞"},
        {L"▀▀▜ ▗▘ ▌ ", L"▀▀▀▜ ▄▄▙  ▌  ▐  "},
        {L"▞▀▚▞▀▚▚▄▞", L"▞▀▀▚▚▄▄▞▌  ▐▚▄▄▞"},
        {L"▞▀▚▚▄▟▗▄▟", L"▞▀▀▚▚▄▄▟   ▐▗▄▄▟"},
        {L"    ▀  ▄ ", L"     ▗▖  ▝▘  ▐▌ "},
        {L"    ▀  ▜ ", L"     ▗▖  ▝▘  ▝▌ "},
        {L" ▗▖▐▌  ▝▘", L"  ▄▖▗▀  ▝▄    ▀▘"},
        {L"   ▄█▄ ▀ ", L"    ▗▄▄▖▗▄▄▖    "},
        {L"▗▖  ▐▌▝▘ ", L"▗▄    ▀▖  ▄▘▝▀  "},
        {L"▞▀▚ ▄▘ ▄ ", L"   ▞  ▞  ▞  ▞   "},
        {L"▞▀▚▌█▟▚▄▄", L"▞▀▀▚▌▞▚▐▌▚▟▟▚▄▄▄"},
        {L"▞▀▚▙▄▟▌ ▐", L"▞▀▀▚▌  ▐▛▀▀▜▌  ▐"},
        {L"▛▀▚▛▀▚▙▄▞", L"▛▀▀▚▙▄▄▞▌  ▐▙▄▄▞"},
        {L"▞▀▚▌  ▚▄▞", L"▞▀▀▚▌   ▌   ▚▄▄▞"},
        {L"▛▀▚▌ ▐▙▄▞", L"▛▀▀▚▌  ▐▌  ▐▙▄▄▞"},
        {L"▛▀▀▛▀▀▙▄▄", L"▛▀▀▀▙▄▄▄▌   ▙▄▄▄"},
        {L"▛▀▀▛▀▀▌  ", L"▛▀▀▀▙▄▄▄▌   ▌   "},
        {L"▞▀▚▌ ▄▚▄▟", L"▞▀▀▚▌   ▌ ▀▜▚▄▄▜"},
        {L"▌ ▐▛▀▜▌ ▐", L"▌  ▐▙▄▄▟▌  ▐▌  ▐"},
        {L"▀▜▀ ▐ ▄▟▄", L"▀▀▛▀  ▌   ▌ ▄▄▙▄"},
        {L"▀▜▀ ▐ ▚▟ ", L"▀▀▛▀  ▌   ▌ ▚▄▘ "},
        {L"▌ ▞▛▀▖▌ ▐", L"▌  ▐▙▄▞▘▌ ▝▚▌  ▐"},
        {L"▌  ▌  ▙▄▄", L"▌   ▌   ▌   ▙▄▄▄"},
        {L"▙ ▟▌▀▐▌ ▐", L"▙  ▟▌▚▞▐▌  ▐▌  ▐"},
        {L"▙ ▐▌▚▐▌ ▜", L"▙  ▐▌▚ ▐▌ ▚▐▌  ▜"},
        {L"▞▀▚▌ ▐▚▄▞", L"▞▀▀▚▌  ▐▌  ▐▚▄▄▞"},
        {L"▛▀▚▙▄▞▌  ", L"▛▀▀▚▙▄▄▞▌   ▌   "},
        {L"▞▀▚▌▗▐▚▄▚", L"▞▀▀▚▌  ▐▌ ▚▐▚▄▄▚"},
        {L"▛▀▚▙▄▞▌ ▐", L"▛▀▀▚▙▄▄▞▌  ▚▌  ▐"},
        {L"▞▀▘▝▀▚▚▄▞", L"▞▀▀▚▚▄    ▀▚▚▄▄▞"},
        {L"▀▜▀ ▐  ▐ ", L"▀▀▛▀  ▌   ▌   ▌ "},
        {L"▌ ▐▌ ▐▚▄▞", L"▌  ▐▌  ▐▌  ▐▚▄▄▞"},
        {L"▌ ▐▚ ▞▝▄▘", L"▌  ▐▌  ▐▚  ▞ ▚▞ "},
        {L"▌ ▐▌▄▐▛ ▜", L"▌  ▐▌  ▐▌▞▚▐▛  ▜"},
        {L"▚ ▞ █ ▞ ▚", L"▚  ▞ ▚▞  ▞▚ ▞  ▚"},
        {L"▌ ▐▝▄▘ █ ", L"▌  ▐▝▖▗▘ ▝▌   ▌ "},
        {L"▀▀▜▗▞▘▙▄▄", L"▀▀▀▜  ▄▘▗▀  ▙▄▄▄"},
        {L"▐▀▘▐  ▐▄▖", L" ▛▀  ▌   ▌   ▙▄ "},
        {L" █  █  █ ", L"▚    ▚    ▚    ▚"},
        {L"▝▀▌  ▌▗▄▌", L" ▀▜   ▐   ▐  ▄▟ "},
        {L" ▄ ▝ ▘   ", L" ▗▖  ▘▝         "},
        {L"      ▄▄▄", L"            ▄▄▄▄"},
        {L"▗   ▘    ", L" ▗    ▘         "},
        {L"   ▞▀▟▚▄▜", L"▗▄▄▖▗▄▄▐▌  █▚▄▄▜"},
        {L"▌  ▙▀▚▛▄▞", L"▌   ▌▄▄▖█  ▐▛▄▄▞"},
        {L"   ▞▀▀▚▄▄", L"    ▗▄▄▄▌   ▚▄▄▄"},
        {L"  ▐▞▀▟▚▄▜", L"   ▐▗▄▄▐▌  █▚▄▄▜"},
        {L"   ▟█▙▚▄▄", L"    ▗▄▄▖▙▄▄▟▚▄▄▄"},
        {L" ▞▖▗▙▖ ▌ ", L"  ▞▖ ▄▙▖  ▌   ▚ "},
        {L"▞▀▟▚▄▜▗▄▞", L"▗▄▄▗▌  █▚▄▄▜▗▄▄▞"},
        {L"▌  ▙▀▚▌ ▐", L"▌   ▌▄▄▖█  ▐▌  ▐"},
        {L" ▘  ▌  ▚ ", L"  ▖   ▖   ▌   ▚ "},
        {L" ▝  ▐ ▝▞ ", L"  ▖   ▖   ▌  ▚▘ "},
        {L"▌  ▙▄▘▌ ▌", L"▌   ▌  ▗▙▄▄▘▌  ▚"},
        {L" ▌  ▌  ▚ ", L"  ▌   ▌   ▌   ▚ "},
        {L"   ▛▞▖▌▌▌", L"    ▖▄▗▖▛ ▌▐▌ ▌▐"},
        {L"   ▛▀▚▌ ▐", L"    ▖▄▄▖▛  ▐▌  ▐"},
        {L"   ▞▀▚▚▄▞", L"    ▗▄▄▖▌  ▐▚▄▄▞"},
        {L"▞▀▚▙▄▞▌  ", L"▖▄▄▖█  ▐▛▄▄▞▌   "},
        {L"▞▀▚▚▄▟  ▐", L"▗▄▄▗▌  █▚▄▄▜   ▐"},
        {L"   ▙▀▚▌  ", L"    ▖▄▄▖▛  ▝▌   "},
        {L"▗▄▖▚▄▖▗▄▞", L"    ▗▄▄▖▚▄▄▖▗▄▄▞"},
        {L" ▌ ▀▛▀ ▚ ", L"  ▌  ▄▙▖  ▌   ▚ "},
        {L"   ▌ ▐▚▄▟", L"    ▖  ▗▌  ▐▝▄▄▜"},
        {L"   ▌ ▐▝▄▘", L"    ▖  ▗▚  ▞ ▚▞ "},
        {L"   ▐▐▐▝▞▟", L"    ▖▗ ▗▌▐ ▐▚▞▄▜"},
        {L"   ▝▄▘▗▀▖", L"    ▗  ▖ ▚▞ ▗▘▝▖"},
        {L"▌ ▐▚▄▟▗▄▞", L"▖  ▗▌  ▐▝▄▄▌▗▄▄▘"},
        {L"▄▄▄▗▄▞▙▄▄", L"    ▄▄▄▄ ▄▄▘▟▄▄▄"},
        {L" ▛▘█   ▙▖", L" ▛▀ ▗▘  ▝▖   ▙▄ "},
        {L"▚   ▚   ▚", L" ▐▌  ▐▌  ▐▌  ▐▌ "},
        {L"▝▜   █▗▟ ", L" ▀▜   ▝▖  ▗▘ ▄▟ "},
        {L"▗▖▗▘▝▘   ", L" ▄ ▖▝ ▀         "}
    };
}

#endif // BENGINE_CURSES_WINDOW_hpp
