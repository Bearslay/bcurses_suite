#ifndef BENGINE_CURSES_WINDOW_hpp
#define BENGINE_CURSES_WINDOW_hpp

#include <ncurses.h>
#include <locale.h>

#include "bengine_helpers.hpp"

namespace bengine {
    class curses_window {
        private:
            static unsigned char default_cell_color_pair;
            static unsigned short default_cell_attributes;
            static unsigned short default_wrapping_width;
            static unsigned char default_wrapping_mode;

            static wchar_t default_cell_character;
            static unsigned short default_box_drawing_settings;
            static const std::wstring box_drawing_key;
            static const std::wstring box_drawing_key_alt;

            static const std::vector<std::vector<std::wstring>> matrix_text_key;

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

            // \brief Different settings related to box drawing, like the primary or dash styles to be used, whether to trim the ends of a line, or to merge with other lines (these settings don't perfectly mesh with each other and some settings don't work properly for certain styles/scenarios, but will serve well in most cases)
            enum box_drawing_settings : unsigned short {
                LIGHT_SQUARE = 0,           // light lines in both the horizontal and vertical directions where corner characters are square (set as zero since its the default value; if bits 0-6 == 0 then light-square is assumed)
                LIGHT_ROUNDED = 1,          // light lines in both the horizontal and vertical directions where corner characters are rounded
                HEAVY_BOTH = 2,             // heavy lines in both the horizontal and vertical directions
                HEAVY_HORIZONTAL = 4,       // heavy lines only in the horizontal direction with light lines in the vertical direction
                HEAVY_VERTICAL = 8,         // heavy lines only in the vertical direction with light lines in the horizontal direction
                DOUBLED_BOTH = 16,          // doubled lines in both the horizontal and vertical directions
                DOUBLED_HORIZONTAL = 32,    // doubled lines only in the horizontal direction with light lines in the vertical direction
                DOUBLED_VERTICAL = 64,      // doubled lines only in the vertical direction with light lines in the horizontal direction
                NO_DASH = 0,                // each line drawing character has no gaps (set as zero since its the default value; if bits 7-12 == 0 then no dash is assumed)
                DOUBLE_DASH = 128,          // each line drawing character has 1 gap
                TRIPLE_DASH = 256,          // each line drawing character has 2 gaps
                QUADRUPLE_DASH = 512,       // each line drawing character has 3 gaps
                SINGLE_DASH_1 = 1024,       // each line drawing character has 1 gap that spans half of the character (first variation; gap is either on right or bottom half of character)
                SINGLE_DASH_2 = 2048,       // each line drawing character has 1 gap that spans half of the character (second variation; gap is either on left or top half of character)
                TRIM_ENDS = 4096,           // whether to place a full line or not at the start/end of a line (mainly nice for making corners/tees when 2 lines intersect)
                SKIP_LINE_MERGING = 8192    // whether to skip merging with other lines when intersecting with existing lines or not
            };

            enum wrapping_modes : unsigned char {
                NONE = 0,              // do not wrap at all
                BASIC = 1,             // wrap to the left edge of a window (where x = 0); wrap without trying to keep words together
                FANCY = 2,             // wrap to the left edge of a window (where x = 0); wrap while trying to keep words together
                BASIC_INDENTED = 4,    // wrap to the x-position that the string/int/whatever originated at; wrap without trying to keep words together
                FANCY_INDENTED = 8,    // wrap to the x-position that the string/int/whatever originated at; wrap while trying to keep words together
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
                    mvaddch(this->y_pos + y, this->x_pos + x, '%');
                } else {
                    mvaddwstr(this->y_pos + y, this->x_pos + x, std::wstring(1, this->grid.at(y).at(x).character).c_str());
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

            bool check_coordinate_bounds(const int &x, const int &y) const {
                return x >= 0 && x < this->get_width() && y >= 0 && y < this->get_height();
            }
            bool check_coordinate_bounds(const std::pair<int, int> &pos) const {
                return this->check_coordinate_bounds(pos.first, pos.second);
            }

        public:
            // default, take up available terminal
            curses_window() {}
            // position is totally custom
            curses_window(const int &x_pos, const int &y_pos, const unsigned short &width, const unsigned short &height) {
                this->x_pos = x_pos;
                this->y_pos = y_pos;
                this->grid = std::vector<std::vector<bengine::curses_window::cell>>(height == 0 ? 1 : height, std::vector<bengine::curses_window::cell>(width == 0 ? 1 : width));
                this->width_2 = this->get_width() / 2;
                this->height_2 = this->get_height() / 2;
            }
            // position is centered relative to the terminal
            curses_window(const unsigned short &width, const unsigned short &height) {
                this->x_pos = COLS / 2 - width / 2;
                this->y_pos = LINES / 2 - height / 2;
                this->grid = std::vector<std::vector<bengine::curses_window::cell>>(height == 0 ? 1 : height, std::vector<bengine::curses_window::cell>(width == 0 ? 1 : width));
                this->width_2 = this->get_width() / 2;
                this->height_2 = this->get_height() / 2;
            }
            // position is centered relative to another window
            curses_window(const bengine::curses_window &window, const unsigned short &width, const unsigned short &height) {
                this->x_pos = window.get_x_pos() + window.get_width_2() - width / 2;
                this->y_pos = window.get_y_pos() + window.get_height_2() - height / 2;
                this->grid = std::vector<std::vector<bengine::curses_window::cell>>(height == 0 ? 1 : height, std::vector<bengine::curses_window::cell>(width == 0 ? 1 : width));
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
                return this->grid.front().size();
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
                    for (unsigned short row = this->get_height() - processed_height + 1; row > 1; row--) {
                        this->grid.pop_back();
                    }
                    return;
                }
                for (unsigned short row = processed_height - this->get_height() + 1; row > 1; row--) {
                    this->grid.emplace_back(std::vector<bengine::curses_window::cell>(this->get_width()));
                }
                this->height_2 = this->get_height() / 2;
            }

            // get desired cell's character, if cell is out of bounds then return `bengine::curses_window::default_cell_character`
            wchar_t get_cell_character(const int &x, const int &y) const {
                return this->check_coordinate_bounds(x, y) ? this->grid.at(y).at(x).character : bengine::curses_window::default_cell_character;
            }
            // get desired cell's color pair, if the cell is out of bounds then return 1 `bengine::curses_window::default_cell_color_pair`
            unsigned char get_cell_color(const int &x, const int &y) const {
                return this->check_coordinate_bounds(x, y) ? this->grid.at(y).at(x).color_pair : bengine::curses_window::default_cell_color_pair;
            }
            unsigned short get_cell_attributes(const int &x, const int &y) const {
                return this->check_coordinate_bounds(x, y) ? this->grid.at(y).at(x).attributes : bengine::curses_window::default_cell_attributes;
            }
            bengine::curses_window::cell get_cell(const int &x, const int &y) const {
                return this->check_coordinate_bounds(x, y) ? this->grid.at(y).at(x) : bengine::curses_window::cell();
            }

            // see if a cell has the desired attributes active or not
            bool check_cell_attribute_states(const int &x, const int &y, const unsigned short &attributes) const {
                return this->check_coordinate_bounds(x, y) && bengine::bitwise_manipulator::check_for_activated_bits<unsigned short>(this->grid.at(y).at(x).attributes, attributes);
            }

            void apply_to_screen() const {
                if (this->get_left_x() >= COLS || this->get_right_x() < 0 || this->get_bottom_y() < 0 || this->get_top_y() >= LINES) {
                    return;
                }
                const unsigned short x_f = this->x_pos + this->get_width() > COLS ? COLS - this->x_pos : this->get_width();
                const unsigned short y_f = this->y_pos + this->get_height() > LINES ? LINES - this->y_pos : this->get_height();
                for (unsigned short row = this->y_pos < 0 ? -this->y_pos : 0; row < y_f; row++) {
                    for (unsigned short col = this->x_pos < 0 ? -this->x_pos : 0; col < x_f; col++) {
                        this->apply_cell_to_screen(col, row);
                    }
                }
            }
            void apply_region_to_screen(int x, int y, int width, int height) const {
                if (this->get_left_x() >= COLS || this->get_left_x() < 0 || this->get_top_y() < 0 || this->get_top_y() >= LINES) {
                    return;
                }
                
                // make width and height positive and adjust x and y to represent the top-left corner
                if (width < 0) {
                    width = -width;
                    x -= width - 1;
                }
                if (height < 0) {
                    height = -height;
                    y -= height - 1;
                }

                // see if region is within window at all
                if (x + width < 0 || y + height < 0 || x >= this->get_width() || y >= this->get_height()) {
                    return;
                }
                // trim width and height as well as adjust x and y so that everything is contained within window
                if (x < 0) {
                    width += x;
                    x = 0;
                }
                if (x + width - 1 >= this->get_width()) {
                    width = this->get_width() - x;
                }
                if (y < 0) {
                    height += y;
                    y = 0;
                }
                if (y + height - 1 >= this->get_height()) {
                    height = this->get_height() - y;
                }

                const unsigned short x_f = this->x_pos + x + width > COLS ? COLS - this->x_pos - x : x + width;
                const unsigned short y_f = this->y_pos + y + height > LINES ? LINES - this->y_pos - y : y + height;
                for (unsigned short row = this->y_pos + y < 0 ? -this->y_pos : y; row < y_f; row++) {
                    for (unsigned short col = this->x_pos + x < 0 ? -this->x_pos : x; col < x_f; col++) {
                        this->apply_cell_to_screen(col, row);
                    }
                }
            }

            void clear_from_screen() const {
                if (this->get_left_x() >= COLS || this->get_right_x() < 0 || this->get_bottom_y() < 0 || this->get_top_y() >= LINES) {
                    return;
                }
                const unsigned short x_f = this->x_pos + this->get_width() > COLS ? COLS - this->x_pos : this->get_width();
                const unsigned short y_f = this->y_pos + this->get_height() > LINES ? LINES - this->y_pos : this->get_height();
                for (unsigned short y = this->y_pos < 0 ? -this->y_pos : 0; y < y_f; y++) {
                    for (unsigned short x = this->x_pos < 0 ? -this->x_pos : 0; x < x_f; x++) {
                        mvaddch(y, x, ' ');
                    }
                }
            }
            void clear_region_from_screen(int x, int y, int width, int height) const {
                if (this->get_left_x() >= COLS || this->get_left_x() < 0 || this->get_top_y() < 0 || this->get_top_y() >= LINES) {
                    return;
                }

                // make width and height positive and adjust x and y to represent the top-left corner
                if (width < 0) {
                    width = -width;
                    x -= width - 1;
                }
                if (height < 0) {
                    height = -height;
                    y -= height - 1;
                }

                // see if region is within window at all
                if (x + width < 0 || y + height < 0 || x >= this->get_width() || y >= this->get_height()) {
                    return;
                }
                // trim width and height as well as adjust x and y so that everything is contained within window
                if (x < 0) {
                    width += x;
                    x = 0;
                }
                if (x + width - 1 >= this->get_width()) {
                    width = this->get_width() - x;
                }
                if (y < 0) {
                    height += y;
                    y = 0;
                }
                if (y + height - 1 >= this->get_height()) {
                    height = this->get_height() - y;
                }

                const unsigned short x_f = this->x_pos + x + width > COLS ? COLS - this->x_pos - x : x + width;
                const unsigned short y_f = this->y_pos + y + height > LINES ? LINES - this->y_pos - y : y + height;
                for (unsigned short row = this->y_pos + y < 0 ? -this->y_pos : y; row < y_f; row++) {
                    for (unsigned short col = this->x_pos + x < 0 ? -this->x_pos : x; col < x_f; col++) {
                        mvaddch(row, col, ' ');
                    }
                }
            }

            // write character to window and return the position of where the cursor would be after writing the character (x + 1 unless wrapping)
            std::pair<int, int> write_character(const int &x, const int &y, const wchar_t &character, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                if (!this->check_coordinate_bounds(x, y)) {
                    return {x, y};
                }

                this->grid.at(y).at(x).character = character;
                this->grid.at(y).at(x).color_pair = args.color_pair;
                this->grid.at(y).at(x).attributes = args.attributes;

                if (x >= this->get_width() - 1) {
                    switch (args.wrapping_mode) {
                        case bengine::curses_window::wrapping_modes::NONE:
                            return {x + 1, y};
                        default:
                        case bengine::curses_window::wrapping_modes::BASIC:
                        case bengine::curses_window::wrapping_modes::FANCY:
                            return {0, y + 1};
                        case bengine::curses_window::wrapping_modes::BASIC_INDENTED:
                        case bengine::curses_window::wrapping_modes::FANCY_INDENTED:
                            return {x, y + 1};
                    }
                }
                return {x + 1, y};
            }
            std::pair<int, int> write_character(const std::pair<int, int> &pos, const wchar_t &character, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                return this->write_character(pos.first, pos.second, character, args);
            }

            // TODO: Fix it so that negative x and y dont fuck shit up
            std::pair<int, int> write_string(int x, int y, const std::wstring &string, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                if (!this->check_coordinate_bounds(x, y)) {
                    return {x, y};
                }

                unsigned short line_count = 1;    // keeps track of the amount of characters written for each line, if the wrapping width is reached before the edge of the window then the function should newline anyways
                for (std::size_t i = 0; i < string.length(); i++) {
                    this->grid.at(y).at(x).character = string.at(i);
                    this->grid.at(y).at(x).color_pair = args.color_pair;
                    this->grid.at(y).at(x).attributes = args.attributes;

                    if (x >= this->get_width() - 1 || (args.wrapping_width > 0 && line_count >= args.wrapping_width)) {
                        switch (args.wrapping_mode) {
                            case bengine::curses_window::wrapping_modes::NONE:
                                return {x + 1, y};
                            default:
                            case bengine::curses_window::wrapping_modes::BASIC:
                                x = 0;
                                break;
                            case bengine::curses_window::wrapping_modes::FANCY:
                                break;
                            case bengine::curses_window::wrapping_modes::BASIC_INDENTED:
                                x = x;
                                break;
                            case bengine::curses_window::wrapping_modes::FANCY_INDENTED:
                                break;
                        }
                        y++;
                        if (y >= this->get_height() - 1) {
                            break;
                        }
                        line_count = 1;
                    } else {
                        line_count++;
                        x++;
                    }
                }

                return {x, y};
            }
            std::pair<int, int> write_string(const std::pair<int, int> &pos, const std::wstring &string, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                return this->write_string(pos.first, pos.second, string, args);
            }

            template <class type> std::pair<int, int> write_number(const int &x, const int &y, const type &number, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                return this->write_string(x, y, bengine::string_helper::to_string(number), args);
            }
            template <class type> std::pair<int, int> write_number(const std::pair<int, int> &pos, const type &number, const bengine::curses_window::write_args &args = bengine::curses_window::default_write_args) {
                return this->write_string(pos.first, pos.second, bengine::string_helper::to_string(number), args);
            }

            void reset_all_cells() {
                this->grid = std::vector<std::vector<bengine::curses_window::cell>>(this->grid.size(), std::vector<bengine::curses_window::cell>(this->grid.front().size()));
            }
            void reset_cells(int x, int y, int width, int height) {
                // make width and height positive and adjust x and y to represent the top-left corner
                if (width < 0) {
                    width = -width;
                    x -= width - 1;
                }
                if (height < 0) {
                    height = -height;
                    y -= height - 1;
                }

                // see if region is within window at all
                if (x + width < 0 || y + height < 0 || x >= this->get_width() || y >= this->get_height()) {
                    return;
                }
                // trim width and height as well as adjust x and y so that everything is contained within window
                if (x < 0) {
                    width += x;
                    x = 0;
                }
                if (x + width - 1 >= this->get_width()) {
                    width = this->get_width() - x;
                }
                if (y < 0) {
                    height += y;
                    y = 0;
                }
                if (y + height - 1 >= this->get_height()) {
                    height = this->get_height() - y;
                }

                while (y < height) {
                    while (x < width) {
                        this->grid.at(y++).at(x++) = bengine::curses_window::cell();
                    }
                }
            }

        private:
            /** Take a box drawing style and convert it into an integer
             * \param settings Line drawing settings
             * \param horizontal Whether the line to draw will be horizontal or not
             * \returns A 1, 2, or 3 (light, heavy, doubled)
             */
            static unsigned char extract_main_style(const unsigned short &settings, const bool &horizontal) {
                switch (bengine::bitwise_manipulator::get_subvalue(settings, 0, 7)) {
                    default:
                    case 0:
                    case 1:
                        return 1;
                    case 2:
                        return 2;
                    case 4:
                        return horizontal ? 2 : 1;
                    case 8:
                        return horizontal ? 1 : 2;
                    case 16:
                        return 3;
                    case 32:
                        return horizontal ? 3 : 1;
                    case 64:
                        return horizontal ? 1 : 3;
                }
            }
            /** Take a box drawing dash style and convert it into an integer
             * \param settings Line drawing settings
             * \param horizontal Whether the line to draw will be horiztonal or not
             * \returns A number 0-5 representing the dashing style
             */
            static unsigned char extract_dash_style(const unsigned short &settings) {
                switch (bengine::bitwise_manipulator::get_subvalue(settings, 7, 5)) {
                    default:
                    case 0:
                        return 0;
                    case 1:
                        return 1;
                    case 2:
                        return 2;
                    case 4:
                        return 3;
                    case 8:
                        return 4;
                    case 16:
                        return 5;
                }
            }

            /** Get the main box drawing style from a character in a specific direction
             * \param character The character to examine
             * \param direction The direction to examine relative to the character (0 = up, 1 = left, 2 = right, 3 = down)
             * \returns An integer 0-3 matching the given character's style (0 for no style or non-box drawing character, 1 for light, 2 for heavy, 3 for doubled)
             */
            static unsigned char extract_style_from_character(const wchar_t &character, const unsigned char &direction) {
                std::size_t index = bengine::curses_window::box_drawing_key.find(character);
                if (index == std::string::npos) {
                    switch (character) {
                        case L'─':
                        case L'╌':
                        case L'┄':
                        case L'┈':
                            index = 19;
                            break;
                        case L'━':
                        case L'╍':
                        case L'┅':
                        case L'┉':
                            index = 39;
                            break;
                        case L'│':
                        case L'╎':
                        case L'┆':
                        case L'┊':
                            index = 64;
                            break;
                        case L'┃':
                        case L'╏':
                        case L'┇':
                        case L'┋':
                            index = 64;
                            break;
                        case L'╭':
                            index = 4;
                            break;
                        case L'╮':
                            index = 16;
                            break;
                        case L'╰':
                            index = 67;
                            break;
                        case L'╯':
                            index = 79;
                            break;
                        default:
                            return 0;
                    }
                }
                return bengine::bitwise_manipulator::get_subvalue(index + 1, 6 - direction * 2, 2);
            }
            /** Find a box drawing character that would merge properly with surrounding neighbors
             * \param neighbors The values of the neighbors (light, heavy, doubled, none) to base the output character on
             * \param main_style The main style that the returned character should be (needed if the line is doubled so that an alternative key can be used to make better looking lines)
             * \param dash_style The dash style that the returned character should be (needed for any non-doubled straight lines)
             * \param use_hard_corners Whether to use hard corners or not (should only be true when the main style is LIGHT_ROUNDED)
             * \returns A box drawing character that merges properly with surrounding neighbors (a space if no appropriate character is found)
             */
            static wchar_t find_character_with_style_values(const unsigned char &neighbors, const unsigned char &main_style, const unsigned char &dash_style, const bool &use_hard_corners) {
                switch (neighbors) {
                    case 0:
                        return L' ';
                    case 20:
                        switch (dash_style) {
                            default:
                            case 0:
                                return L'─';
                            case 1:
                                return L'╌';
                            case 2:
                                return L'┄';
                            case 3:
                                return L'┈';
                            case 4:
                                return L'╴';
                            case 5:
                                return L'╶';
                        }
                    case 40:
                        switch (dash_style) {
                            default:
                            case 0:
                                return L'━';
                            case 1:
                                return L'╍';
                            case 2:
                                return L'┅';
                            case 3:
                                return L'┉';
                            case 4:
                                return L'╸';
                            case 5:
                                return L'╺';
                        }
                    case 65:
                        switch (dash_style) {
                            default:
                            case 0:
                                return L'│';
                            case 1:
                                return L'╎';
                            case 2:
                                return L'┆';
                            case 3:
                                return L'┊';
                            case 4:
                                return L'╵';
                            case 5:
                                return L'╷';
                        }
                    case 130:
                        switch (dash_style) {
                            default:
                            case 0:
                                return L'┃';
                            case 1:
                                return L'╏';
                            case 2:
                                return L'┇';
                            case 3:
                                return L'┋';
                            case 4:
                                return L'╹';
                            case 5:
                                return L'╻';
                        }
                    case 5:
                        if (use_hard_corners) {
                            return L'╭';
                        }
                        return L'┌';
                    case 17:
                        if (use_hard_corners) {
                            return L'╮';
                        }
                        return L'┐';
                    case 68:
                        if (use_hard_corners) {
                            return L'╰';
                        }
                        return L'└';
                    case 80:
                        if (use_hard_corners) {
                            return L'╯';
                        }
                        return L'┘';
                    default:
                        return main_style == 3 ? bengine::curses_window::box_drawing_key_alt.at(neighbors - 1) : bengine::curses_window::box_drawing_key.at(neighbors - 1);
                }
                return L' ';
            }
            /** Find a box drawing character that would merge properly with surrounding neighbors
             * \param up The style of the upper neighbor (light, heavy, doubled)
             * \param left The style of the left neighbor (light, heavy, doubled)
             * \param right The style of the right neighbor (light, heavy, doubled)
             * \param down The style of the down neighbor (light, heavy, doubled)
             * \param main_style The main style that the returned character should be (needed if the line is doubled so that an alternative key can be used to make better looking lines)
             * \param dash_style The dash style that the returned character should be (needed for any non-doubled straight lines)
             * \param use_hard_corners Whether to use hard corners or not (should only be true when the main style is LIGHT_ROUNDED)
             * \returns A box drawing character that merges properly with surrounding neighbors (a space if no appropriate character is found)
             */
            static wchar_t find_character_with_style_values(const unsigned char &up, const unsigned char &left, const unsigned char &right, const unsigned char &down, const unsigned char &main_style, const unsigned char &dash_style, const bool &use_hard_corners) {
                return bengine::curses_window::find_character_with_style_values((up << 6) + (static_cast<unsigned char>(left << 6) >> 2) + (static_cast<unsigned char>(right << 6) >> 4) + (static_cast<unsigned char>(down << 6) >> 6), main_style, dash_style, use_hard_corners);
            }

        public:
            /** Draw a horizontal line using Unicode Box Drawing Characters
             * \param x x-position (col) the line starts at
             * \param y y-position (row) the line starts at
             * \param length Length of the line in characters; positive value makes line draw to the right and negative value makes line draw to the left (0 doesn't do anything)
             * \param settings Combination value that encodes the main style of the line (light, heavy, doubled, etc), the dashing style (none, double, triple, etc), whether to use full lines or not for the first/last character of the line, and whether to merge with other lines at all
             * \param color The color pair to use for the line
             * \param attributes Attributes for each cell in the line, though many don't actually affect anything; all cells changed by this function will have the attribute of BOX_DRAWING_MERGABLE added to them
             * \returns A coordinate pair corresponding to the last character drawn in the line
             */
            std::pair<int, int> draw_horizontal_line(int x, const int &y, int length, const unsigned short &settings, const unsigned char &color = bengine::curses_window::default_cell_color_pair, const unsigned short &attributes = bengine::curses_window::default_cell_attributes) {
                if (length == 0) {
                    return {x, y};
                } else if (y < 0 || y >= this->get_height()) {
                    return {x + length + (length < 0 ? 1 : -1), y};
                }

                // position of the last character in the line (doesn't have to be in-bounds)
                int output_x = x + length - 1;
                // convert a line with a negative length into one with a positive length
                if (length < 0) {
                    output_x += 2;
                    length = -length;
                    x -= length - 1;
                }
                // culling for when the line is never within the window's bounds
                // the condition `output_x >= this->get_width()` does not need to be checked
                //   if `length < 0`, then `output_x` can never be less than `x` once processed, so `x >= this->get_width()` kinda checks both values
                if (x + length < 0 || x >= this->get_width()) {
                    return {output_x, y};
                }

                // trim part of line that goes beyond left bound
                if (x < 0) {
                    length += x;
                    x = 0;
                }
                // trim part of line that goes beyond right bound
                if (x + length - 1 >= this->get_width()) {
                    length = this->get_width() - x;
                }

                const unsigned char main_style = bengine::curses_window::extract_main_style(settings, true);
                const unsigned char dash_style = bengine::curses_window::extract_dash_style(settings);
                const bool trim_ends = bengine::bitwise_manipulator::get_bit_state(settings, 12);

                // Skip any line merging, so just make a line of the same character as a line
                if (bengine::bitwise_manipulator::get_bit_state(settings, 13)) {
                    if (trim_ends) {
                        if (length == 1) {
                            return {output_x, y};
                        }
                        this->write_character(x++, y, bengine::curses_window::find_character_with_style_values(0, 0, main_style, 0, main_style, dash_style, false), {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                    }
                    const wchar_t line_char = bengine::curses_window::find_character_with_style_values(0, main_style, main_style, 0, main_style, dash_style, false);
                    for (unsigned short i = trim_ends + trim_ends; i < length; i++) {
                        this->write_character(x++, y, line_char, {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                    }
                    if (trim_ends) {
                        this->write_character(x, y, bengine::curses_window::find_character_with_style_values(0, main_style, 0, 0, main_style, dash_style, false), {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                    }
                    return {output_x, y};
                }

                const bool use_hard_corners = bengine::bitwise_manipulator::get_subvalue(settings, 0, 7) == bengine::curses_window::LIGHT_ROUNDED;
                if (trim_ends) {
                    if (length == 1) {
                        return {output_x, y};
                    }
                    const unsigned char neighbors = (bengine::curses_window::extract_style_from_character(this->get_cell_character(x, y - 1), 3) << 6) + (main_style << 2) + bengine::curses_window::extract_style_from_character(this->get_cell_character(x, y + 1), 0);
                    this->write_character(x++, y, bengine::curses_window::find_character_with_style_values(neighbors, main_style, dash_style, use_hard_corners), {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                }
                for (unsigned short i = trim_ends + trim_ends; i < length; i++) {
                    const unsigned char neighbors = (bengine::curses_window::extract_style_from_character(this->get_cell_character(x, y - 1), 3) << 6) + (main_style << 4) + (main_style << 2) + bengine::curses_window::extract_style_from_character(this->get_cell_character(x, y + 1), 0);
                    this->write_character(x++, y, bengine::curses_window::find_character_with_style_values(neighbors, main_style, dash_style, use_hard_corners), {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                }
                if (trim_ends) {
                    const unsigned char neighbors = (bengine::curses_window::extract_style_from_character(this->get_cell_character(x, y - 1), 3) << 6) + (main_style << 4) + bengine::curses_window::extract_style_from_character(this->get_cell_character(x, y + 1), 0);
                    this->write_character(x, y, bengine::curses_window::find_character_with_style_values(neighbors, main_style, dash_style, use_hard_corners), {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                }
                return {output_x, y};
            }
            std::pair<int, int> draw_horizontal_line(const std::pair<int, int> &pos, int length, const unsigned short &settings, const unsigned char &color = bengine::curses_window::default_cell_color_pair, const unsigned short &attributes = bengine::curses_window::default_cell_attributes) {
                return this->draw_horizontal_line(pos.first, pos.second, length, settings, color, attributes);
            }
            std::pair<int, int> draw_vertical_line(const int &x, int y, int length, const unsigned short &settings, const unsigned char &color = bengine::curses_window::default_cell_color_pair, const unsigned short &attributes = bengine::curses_window::default_cell_attributes) {
                if (length == 0) {
                    return {x, y};
                } else if (x < 0 || x >= this->get_width()) {
                    return {x, y + length + (length < 0 ? 1 : -1)};
                }

                int output_y = y + length - 1;
                if (length < 0) {
                    output_y += 2;
                    length = -length;
                    y -= length - 1;
                }
                if (y + length < 0 || y >= this->get_height()) {
                    return {x, output_y};
                }

                if (y < 0) {
                    length += y;
                    y = 0;
                }
                if (y + length - 1 >= this->get_height()) {
                    length = this->get_height() - y;
                }

                const unsigned char main_style = bengine::curses_window::extract_main_style(settings, false);
                const unsigned char dash_style = bengine::curses_window::extract_dash_style(settings);
                const bool trim_ends = bengine::bitwise_manipulator::get_bit_state(settings, 12);

                if (bengine::bitwise_manipulator::get_bit_state(settings, 13)) {
                    if (trim_ends) {
                        if (length == 1) {
                            return {x, output_y};
                        }
                        this->write_character(x, y++, bengine::curses_window::find_character_with_style_values(0, 0, 0, main_style, main_style, dash_style, false), {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                    }
                    const wchar_t line_char = bengine::curses_window::find_character_with_style_values(main_style, 0, 0, main_style, main_style, dash_style, false);
                    for (unsigned short i = 0; i < length; i++) {
                        this->write_character(x, y++, line_char, {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                    }
                    if (trim_ends) {
                        this->write_character(x, y, bengine::curses_window::find_character_with_style_values(main_style, 0, 0, 0, main_style, dash_style, false), {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                    }
                    return {x, output_y};
                }

                const bool use_hard_corners = bengine::bitwise_manipulator::get_subvalue(settings, 0, 7) == bengine::curses_window::LIGHT_ROUNDED;
                if (trim_ends) {
                    if (length == 1) {
                        return {x, y};
                    }
                    const unsigned char neighbors = (bengine::curses_window::extract_style_from_character(this->get_cell_character(x - 1, y), 2) << 4) + (bengine::curses_window::extract_style_from_character(this->get_cell_character(x + 1, y), 1) << 2) + main_style;
                    this->write_character(x, y++, bengine::curses_window::find_character_with_style_values(neighbors, main_style, dash_style, use_hard_corners), {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                }
                for (unsigned short i = trim_ends + trim_ends; i < length; i++) {
                    const unsigned char neighbors = (main_style << 6) + (bengine::curses_window::extract_style_from_character(this->get_cell_character(x - 1, y), 2) << 4) + (bengine::curses_window::extract_style_from_character(this->get_cell_character(x + 1, y), 1) << 2) + main_style;
                    this->write_character(x, y++, bengine::curses_window::find_character_with_style_values(neighbors, main_style, dash_style, use_hard_corners), {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                }
                if (trim_ends) {
                    const unsigned char neighbors = (main_style << 6) + (bengine::curses_window::extract_style_from_character(this->get_cell_character(x - 1, y), 2) << 4) + (bengine::curses_window::extract_style_from_character(this->get_cell_character(x + 1, y), 1) << 2);
                    this->write_character(x, y, bengine::curses_window::find_character_with_style_values(neighbors, main_style, dash_style, use_hard_corners), {color, static_cast<unsigned short>(attributes | BOX_DRAWING_MERGABLE), 0, 0});
                }
                return {x, output_y};
            }
            std::pair<int, int> draw_vertical_line(const std::pair<int, int> &pos, int length, const unsigned short &settings, const unsigned char &color = bengine::curses_window::default_cell_color_pair, const unsigned short &attributes = bengine::curses_window::default_cell_attributes) {
                return this->draw_vertical_line(pos.first, pos.second, length, settings, color, attributes);
            }
    };
    unsigned char bengine::curses_window::default_cell_color_pair = bengine::curses_window::preset_colors::WHITE;
    unsigned short bengine::curses_window::default_cell_attributes = bengine::curses_window::cell_attributes::BOX_DRAWING_MERGABLE;
    unsigned short bengine::curses_window::default_wrapping_width = 0;
    unsigned char bengine::curses_window::default_wrapping_mode = bengine::curses_window::wrapping_modes::BASIC;
    bengine::curses_window::write_args bengine::curses_window::default_write_args = {bengine::curses_window::default_cell_color_pair, bengine::curses_window::default_cell_attributes, bengine::curses_window::default_wrapping_width, bengine::curses_window::default_wrapping_mode};

    wchar_t bengine::curses_window::default_cell_character = L' ';
    unsigned short bengine::curses_window::default_box_drawing_settings = bengine::curses_window::LIGHT_SQUARE | bengine::curses_window::NO_DASH;
    const std::wstring bengine::curses_window::box_drawing_key = L"╷╻╻╶┌┎╓╺┍┏┏╺╒┏╔╴┐┒╖─┬┰╥╼┮┲┲╼┮┲┲╸┑┓┓╾┭┱┱━┯┳┳━┯┳┳╸╕┓╗╾┱┱┱━┯┳┳═╤┳╦╵│╽╽└├┟┟┕┝┢┢╘╞┢┢┘┤┧┧┴┼╁╁┶┾╆╆┶┾╆╆┙┥┪┪┵┽╅╅┷┿╈╈┷┿╈╈╛╡┪┪┵┽╅╅┷┿╈╈╧╪╈╈╹╿┃┃┖┞┠┠┗┡┣┣┗┡┣┣┚┦┨┨┸╀╂╂┺╄╊╊┺╄╊╊┛┩┫┫┹╃╉╉┻╇╋╋┻╇╋╋┛┩┫┫┹╃╉╉┻╇╋╋┻╇╋╋╹╿┃║╙┞┠╟┗┡┣┣╚┡┣╠╜┦┨╢╨╀╂╫┺╄╊╊┺╄╊╊┛┩┫┫┹╃╉╉┻╇╋╋┻╇╋╋╝┩┫╣┹╃╉╉┻╇╋╋╩╇╋╬";
    const std::wstring bengine::curses_window::box_drawing_key_alt = L"╷╻║╶┌┎╓╺┍┏╔═╒╔╔╴┐┒╖─┬┰╥╼┮┲╦═╤╦╦╸┑┓╗╾┭┱╦━┯┳╦═╤╦╦═╕╗╗═╤╦╦═╤╦╦═╤╦╦╵│╽║└├┟╟┕┝┢╠╘╞╠╠┘┤┧╢┴┼╁╫┶┾╆╬╧╪╬╬┙┥┪╣┵┽╅╬┷┿╈╬╧╪╬╬╛╡╣╣╧╪╬╬╧╪╬╬╧╪╬╬╹╿┃║┖┞┠╟┗┡┣╠╚╠╠╠┚┦┨╢┸╀╂╫┺╄╊╬╩╬╬╬┘┩┫╣┹╃╉╬┻╇╋╬╩╬╬╬╝╣╣╣╩╬╬╬╩╬╬╬╩╬╬╬║║║║╙╟╟╟╚╠╠╠╚╠╠╠╜╢╢╢╨╫╫╫╩╬╬╬╩╬╬╬╝╣╣╣╩╬╬╬╩╬╬╬╩╬╬╬╝╣╣╣╩╬╬╬╩╬╬╬╩╬╬╬";
    
    const std::vector<std::vector<std::wstring>> bengine::curses_window::matrix_text_key = {
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
