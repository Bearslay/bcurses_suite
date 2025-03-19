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
            static bool default_line_merging_preference;

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

        private:
            /** Take a box drawing style and convert it into an integer
             * \param style Line drawing style
             * \param horizontal Whether the line to draw will be horizontal or not
             * \returns A 1, 2, or 3 (light, heavy, doubled)
             */
            static unsigned char extract_main_style(const unsigned short &style, const bool &horizontal) {
                switch (bengine::bitwise_manipulator::get_subvalue(style, 0, 8)) {
                    default:
                    case bengine::curses_window::box_drawing_styles::LIGHT_SQUARE:
                    case bengine::curses_window::box_drawing_styles::LIGHT_ROUNDED:
                        return 1;
                    case bengine::curses_window::box_drawing_styles::HEAVY_BOTH:
                        return 2;
                    case bengine::curses_window::box_drawing_styles::HEAVY_HORIZONTAL:
                        return horizontal ? 2 : 1;
                    case bengine::curses_window::box_drawing_styles::HEAVY_VERTICAL:
                        return horizontal ? 1 : 2;
                    case bengine::curses_window::box_drawing_styles::DOUBLED_BOTH:
                        return 3;
                    case bengine::curses_window::box_drawing_styles::DOUBLED_HORIZONTAL:
                        return horizontal ? 3 : 1;
                    case bengine::curses_window::box_drawing_styles::DOUBLED_VERTICAL:
                        return horizontal ? 1 : 3;
                }
            }
            /** Take a box drawing dash style and convert it into an integer
             * \param style Line drawing dash
             * \param horizontal Whether the line to draw will be horiztonal or not
             * \returns A number 0-5 representing the dashing style
             */
            static unsigned char extract_dash_style(const unsigned short &style/*, const bool &horizontal*/) {
                // const unsigned char main_style = bengine::bitwise_manipulator::get_subvalue(style, 0, 8);
                // // following styles and directions will never have dashes
                // if (main_style == bengine::curses_window::box_drawing_styles::DOUBLED_BOTH) {
                //     return 0;
                // } else if (horizontal && main_style == bengine::curses_window::box_drawing_styles::HEAVY_VERTICAL || main_style == bengine::curses_window::box_drawing_styles::DOUBLED_VERTICAL) {
                //     return 0;
                // } else if (main_style == bengine::curses_window::box_drawing_styles::HEAVY_HORIZONTAL || main_style == bengine::curses_window::box_drawing_styles::DOUBLED_HORIZONTAL) {
                //     return 0;
                // }

                switch (bengine::bitwise_manipulator::get_subvalue(style, 8, 6) - 256) {
                    default:
                    case bengine::curses_window::box_drawing_styles::NO_DASH:
                        return 0;
                    case bengine::curses_window::box_drawing_styles::DOUBLE_DASH:
                        return 1;
                    case bengine::curses_window::box_drawing_styles::TRIPLE_DASH:
                        return 2;
                    case bengine::curses_window::box_drawing_styles::QUADRUPLE_DASH:
                        return 3;
                    case bengine::curses_window::box_drawing_styles::SINGLE_DASH_1:
                        return 4;
                    case bengine::curses_window::box_drawing_styles::SINGLE_DASH_2:
                        return 5;
                }
            }

            /** Get the main box drawing style from a character in a specific direction
             * \param character The character to examine line
             * \param direction The direction to analyze relative to the character (0 = up, 1 = left, 2 = right, 3 = down)
             * \returns An integer 0-3 matching the style (0 for no style, 1 for light, 2 for heavy, 3 for doubled)
             */
            static unsigned char extract_style_from_character(const wchar_t &character, const unsigned char &direction) {
                for (std::size_t i = 0; i < bengine::curses_window::box_drawing_chars.size(); i++) {
                    if (character == bengine::curses_window::box_drawing_chars.at(i)) {
                        bengine::bitwise_manipulator::get_subvalue(bengine::curses_window::box_drawing_key.at(i), 6 - direction * 2, 2);
                    }
                }
                return 0;
            }
            /** Find a box drawing character that would merge properly with surrounding neighbors
             * \param neighbors The values of the neighbors (light, heavy, doubled, none) to base the output character on
             * \param style The style that the returned character should be (needed for a few special character cases)
             * \returns A box drawing character that merges properly with surrounding neighbors (a space if no appropriate character is found)
             */
            static wchar_t find_character_with_style_values(const unsigned char &neighbors, const unsigned short &style) {
                switch (neighbors) {
                    // Light and heavy box drawing pieces must be processed seperately because they might be dashed (while having the same style signatures)
                    case 20:
                        return bengine::curses_window::box_drawing_chars.at(93 + bengine::curses_window::extract_dash_style(style));
                    case 40:
                        return bengine::curses_window::box_drawing_chars.at(99 + bengine::curses_window::extract_dash_style(style));
                    case 65:
                        return bengine::curses_window::box_drawing_chars.at(105 + bengine::curses_window::extract_dash_style(style));
                    case 130:
                        return bengine::curses_window::box_drawing_chars.at(111 + bengine::curses_window::extract_dash_style(style));
                    // Light corners pieces must be processed seperated because they might be hard or rounded (while having the same style signatures)
                    case 5:
                        return bengine::curses_window::box_drawing_chars.at(117 + (bengine::bitwise_manipulator::get_subvalue(style, 0, 8) == bengine::curses_window::box_drawing_styles::LIGHT_ROUNDED));
                    case 17:
                        return bengine::curses_window::box_drawing_chars.at(119 + (bengine::bitwise_manipulator::get_subvalue(style, 0, 8) == bengine::curses_window::box_drawing_styles::LIGHT_ROUNDED));
                    case 68:
                        return bengine::curses_window::box_drawing_chars.at(121 + (bengine::bitwise_manipulator::get_subvalue(style, 0, 8) == bengine::curses_window::box_drawing_styles::LIGHT_ROUNDED));
                    case 80:
                        return bengine::curses_window::box_drawing_chars.at(123 + (bengine::bitwise_manipulator::get_subvalue(style, 0, 8) == bengine::curses_window::box_drawing_styles::LIGHT_ROUNDED));
                    default:
                        for (std::size_t i = 0; i < bengine::curses_window::box_drawing_key.size(); i++) {
                            if (neighbors == bengine::curses_window::box_drawing_key.at(i)) {
                                return bengine::curses_window::box_drawing_chars.at(i);
                            }
                        }
                }
                return L' ';
            }
            /** Find a box drawing character that would merge properly with surrounding neighbors
             * \param up The style of the upper neighbor (light, heavy, doubled)
             * \param left The style of the left neighbor (light, heavy, doubled)
             * \param right The style of the right neighbor (light, heavy, doubled)
             * \param down The style of the down neighbor (light, heavy, doubled)
             * \param style The style that the returned character should be (needed for a few special character cases)
             * \returns A box drawing character that merges properly with surrounding neighbors (a space if no appropriate character is found)
             */
            static wchar_t find_character_with_style_values(const unsigned char &up, const unsigned char &left, const unsigned char &right, const unsigned char &down, const unsigned short &style) {
                return bengine::curses_window::find_character_with_style_values((up << 6) + (static_cast<unsigned char>(left << 6) >> 2) + (static_cast<unsigned char>(right << 6) >> 4) + (static_cast<unsigned char>(down << 6) >> 6), style);
            }

        public:
            void draw_horizontal_line(unsigned short x, const unsigned short &y, int length, const unsigned short &style, const unsigned char &color = bengine::curses_window::default_cell_color_pair, const unsigned short &attributes = bengine::curses_window::default_cell_attributes, const bool &merge_with_other_lines = bengine::curses_window::default_line_merging_preference) {
                // set x, y, and length to values that ensures that length is a positive number
                if (length < 0) {
                    length = -length;
                    x -= length;
                }

                // no line to draw if starting x or y is beyond the window's bounds
                if (!check_coordinate_bounds(x, y)) {
                    return;
                }

                // trim the line if it extends beyond the window's bounds
                if (x + length >= this->get_width()) {
                    length = this->get_width() - x;
                }

                const unsigned char main_style = bengine::curses_window::extract_main_style(style, true);
                const unsigned char dash_style = bengine::curses_window::extract_dash_style(style/*, horiztonal*/);

                // If no merging is to happen, then drawing the line becomes very simple
                if (!merge_with_other_lines) {
                    const wchar_t line_char = bengine::curses_window::find_character_with_style_values(0, main_style, main_style, 0, style);
                    for (unsigned short i = 0; i < length; i++) {
                        this->write_character(x + i, y, line_char, {color, attributes, 0, 0});
                    }
                    return;
                }

                for (unsigned short i = 0; i < length; i++) {
                    unsigned char neighbors = (bengine::curses_window::extract_style_from_character(this->get_cell_character(x, y - 1), 3) << 6) + bengine::curses_window::extract_style_from_character(this->get_cell_character(x, y + 1), 0);

                    // if not at the start of the line, then the left neighbor shall always be the desired style
                    // if at the start of the line and the vertical neighbors + the left neighbor are all non-existent, then also fabricate left neighbor
                    if (i > 0 || (bengine::curses_window::extract_style_from_character(this->get_cell_character(x - 1, y), 2) == 0 && neighbors == 0)) {
                        neighbors = bengine::bitwise_manipulator::set_subvalue(neighbors, main_style, 4, 2);
                    }
                    // if at the start of the line and either vertical neighbors or the left neighbor exists, then the left neighbor should match whatever was found (even if zero)
                    else {
                        neighbors = bengine::bitwise_manipulator::set_subvalue(neighbors, bengine::curses_window::extract_style_from_character(this->get_cell_character(x - 1, y), 2), 4, 2);
                    }

                    // if not at the end of the line, then the right neighbor shall always be the desired style
                    // if at the end of the line and the vertical neighbors + the right neighbors are all non-existent, then also fabricate the right neighbor
                    if (i < length - 1 || (bengine::curses_window::extract_style_from_character(this->get_cell_character(x + 1, y), 1) == 0 && bengine::bitwise_manipulator::get_subvalue(neighbors, 6, 2) == 0 && bengine::bitwise_manipulator::get_subvalue(neighbors, 0, 2) == 0)) {
                        neighbors = bengine::bitwise_manipulator::set_subvalue(neighbors, main_style, 2, 2);
                    }
                    // if at the end of the line and either vertical neighbors or the right neighbor exists, then the right neighbor should match whatever was found (even if zero)
                    else {
                        neighbors = bengine::bitwise_manipulator::set_subvalue(neighbors, bengine::curses_window::extract_style_from_character(this->get_cell_character(x + 1, y), 1), 2, 2);
                    }

                    wchar_t piece = bengine::curses_window::find_character_with_style_values(neighbors, style);

                    // the character set that unicode provides doesn't account for every possible intersection of light, heavy, or doubled lines
                    // the main issue stems from a lack of characters relating to the doubled set, where it can only merge with the light char set (and itself) and only if the other line doesn't end at the intersection
                    // to account for the missing characters, heavy neighbors are converted into light ones if the line is doubled or doubled neighbors are converted into heavy ones in any other case
                    // while the results aren't perfect, these neighbor conversions keep the most recent line intact while looking just about right; this section of the algorithm should be relatively rare anyways
                    if (piece == L' ') {
                        // doubled line: convert any existent side neighbors to light
                        if (bengine::bitwise_manipulator::get_subvalue(style, 0, 8) >= 32 && bengine::bitwise_manipulator::get_subvalue(style, 0, 8) <= 128) {
                            
                        } else {

                        }
                    }
                }
            }
            void draw_vertical_line(const unsigned short &x, unsigned short y, int length, const unsigned short &style, const unsigned char &color = bengine::curses_window::default_cell_color_pair, const unsigned short &attributes = bengine::curses_window::default_cell_attributes, const bool &merge_with_other_lines = bengine::curses_window::default_line_merging_preference) {
                // see bengine::curses_window::draw_horizontal_line() for comments relating to the rationale behind this function's code
                if (length < 0) {
                    length = -length;
                    y -= length;
                }

                if (!check_coordinate_bounds(x, y)) {
                    return;
                }

                if (y + length >= this->get_height()) {
                    length = this->get_height() - y;
                }

                const unsigned char main_style = bengine::curses_window::extract_main_style(style, false);
                const unsigned char dash_style = bengine::curses_window::extract_dash_style(style/*, horiztonal*/);

                if (!merge_with_other_lines) {
                    const wchar_t line_char = bengine::curses_window::find_character_with_style_values(0, main_style, main_style, 0, style);
                    for (unsigned short i = 0; i < length; i++) {
                        this->write_character(x, y + i, line_char, {color, attributes, 0, 0});
                    }
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

    bool bengine::curses_window::default_line_merging_preference = true;
    // const std::vector<unsigned char> bengine::curses_window::box_drawing_key = {5, 6, 7, 9, 10, 13, 15, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 33, 34, 36, 37, 38, 40, 41, 42, 49, 51, 60, 61, 63, 65, 66, 68, 69, 70, 72, 73, 74, 76, 77, 80, 81, 82, 84, 85, 86, 88, 89, 90, 96, 97, 98, 100, 101, 102, 104, 105, 106, 112, 113, 124, 125, 129, 130, 132, 133, 134, 136, 137, 138, 144, 145, 146, 148, 149, 150, 152, 153, 154, 160, 161, 162, 164, 165, 166, 168, 169, 170, 195, 196, 199, 204, 207, 208, 211, 212, 215, 240, 243, 252, 255, 5, 17, 68, 80, 20, 20, 20, 20, 20, 40, 40, 40, 40, 40, 65, 65, 65, 65, 65, 130, 130, 130, 130, 130}; // [00000000][__,__,__,__]: up left right down
    const std::vector<unsigned char> bengine::curses_window::box_drawing_key = {60, 195, 21, 69, 81, 84, 85, 10, 34, 136, 160, 42, 138, 162, 168, 170, 15, 51, 204, 240, 63, 207, 243, 252, 255, 9, 33, 72, 96, 41, 73, 97, 104, 105, 6, 18, 132, 144, 22, 134, 146, 148, 150, 13, 49, 76, 112, 61, 77, 113, 124, 125, 7, 19, 196, 208, 23, 199, 211, 212, 215, 24, 25, 26, 36, 37, 38, 66, 70, 74, 82, 86, 88, 89, 90, 98, 100, 101, 102, 106, 129, 133, 137, 145, 149, 152, 153, 154, 161, 164, 165, 166, 169, 20, 20, 20, 20, 20, 20, 40, 40, 40, 40, 40, 40, 65, 65, 65, 65, 65, 65, 130, 130, 130, 130, 130, 130, 5, 17, 68, 80, 5, 17, 68, 80};
    // const std::wstring bengine::curses_window::box_drawing_chars = L"┌┎╓┍┏╒╔┐┒╖─┬┰╥╼┮┲┑┓╾┭┱━┯┳╕╗═╤╦│╽└├┟┕┝┢╘╞┘┤┧┴┼╁┶┾╆┙┥┪┵┽╅┷┿╈╛╡╧╪╿┃┖┞┠┗┡┣┚┦┨┸╀╂┺╄╊┛┩┫┹╃╉┻╇╋║╙╟╚╠╜╢╨╫╝╣╩╬╭╮╰╯╌┄┈╴╶╍┅┉╸╺╎┆┊╵╷╏┇┋╹╻"; // indexes for special case characters: light rounded: 100, light horizontal: 104, heavy horizontal: 109, light vertical: 114, heavy vertical: 119
    const std::wstring bengine::curses_window::box_drawing_chars = L"═║┬├┤┴┼┏┓┗┛┳┣┫┻╋╔╗╚╝╦╠╣╩╬┍┑┕┙┯┝┥┷┿┎┒┖┚┰┠┨┸╂╒╕╘╛╤╞╡╧╪╓╖╙╜╥╟╢╨╫╼┮┲╾┭┱╽┟┢┧╁┶┾╆┪┵┽╅╈╿┞┡┦╀┺╄╊┩┹╃╉╇─╌┄┈╴╶━╍┅┉╸╺│╎┆┊╵╷┃╏┇┋╹╻┌╭┐╮└╰┘╯";
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
