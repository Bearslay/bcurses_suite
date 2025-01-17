#ifndef BENGINE_CURSES_WINDOW_hpp
#define BENGINE_CURSES_WINDOW_hpp

#include <ncurses.h>
#include <locale.h>
#include <vector>
#include <unordered_map>
#include <string>

#include "bengine_helpers.hpp"

namespace bengine {
    class curses_window {
        private:
            static const std::unordered_map<unsigned char, wchar_t> box_drawing_key;
            static const std::unordered_map<char, std::vector<std::wstring>> matrix_text_key;

            static wchar_t default_cell_character;
            static unsigned char default_cell_color_pair;
            static unsigned short default_cell_attributes;
            static unsigned short default_box_drawing_style;

        public:
            // \brief A number representing one of the first 16 color pairs initialized upon startup (names assume that nothing was changed)
            enum class preset_colors : unsigned char {
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

            // \brief Different primary and dash styles to be used when utilizing the box drawing functions of a bengine::curses_window; note that doubled can't cleanly merge with heavy and that doubled also can't be dashed
            enum class box_drawing_styles : unsigned short {
                LIGHT_SQUARE = 1,           // box_drawing_style representing light lines in both the horizontal and vertical directions where corner characters are square
                LIGHT_ROUNDED = 2,          // box_drawing_style representing light lines in both the horizontal and vertical directions where corner characters are rounded
                HEAVY_BOTH = 4,             // box_drawing_style representing heavy lines in both the horizontal and vertical directions
                HEAVY_HORIZONTAL = 8,       // box_drawing_style representing heavy lines only in the horizontal direction with light lines in the vertical direction
                HEAVY_VERTICAL = 16,        // box_drawing_style representing heavy lines only in the vertical direction with light lines in the horizontal direction
                DOUBLED_BOTH = 32,          // box_drawing_style representing doubled lines in both the horizontal and vertical directions
                DOUBLED_HORIZONTAL = 64,    // box_drawing_style representing doubled lines only in the horizontal direction with light lines in the vertical direction
                DOUBLED_VERTICAL = 128,     // box_drawing_style representing doubled lines only in the vertical direction with light lines in the horizontal direction
                NO_DASH = 256,              // box_drawing_dash_style representing a line with no gaps
                DOUBLE_DASH = 512,          // box_drawing_dash_style representing a line with 1 gap
                TRIPLE_DASH = 1024,         // box_drawing_dash_style representing a line with 2 gaps
                QUADRUPLE_DASH = 2048,      // box_drawing_dash_style representing a line with 3 gaps
                SINGLE_DASH_1 = 4096,       // box_drawing_dash_style representing the first variation of a line with 1 large gap using half of the character
                SINGLE_DASH_2 = 8192        // box_drawing_dash_style representing the second variation of a line with 1 large gap using half of the character
            };

            // \brief Different attributes of a cell supported by ncurses or bengine
            enum class cell_attributes : unsigned short {
                BOLD = 1,                      // cell_attribute representing whether the cell is bolded or not
                ITALIC = 2,                    // cell_attribute representing whether the cell is italicized or not
                UNDERLINED = 4,                // cell_attribute representing whether the cell is underlined or not
                REVERSED_COLOR = 8,            // cell_attribute representing whether the cell's foreground color and background color are reversed or not (good for highlighting in many cases)
                BLINKING = 16,                 // cell_attribute representing whether the cell blinks or not
                DIM = 32,                      // cell_attribute representing whether the cell is dim or not
                INVISIBLE = 64,                // cell_attribute representing whether the cell is invisible or not
                STANDOUT = 128,                // cell_attribute representing whether the cell "stands out" or not
                PROTECTED = 256,               // cell_attribute representing whether the cell is "protected" or not
                ALTERNATE_CHARACTER = 512,     // cell_attribute representing whether the cell uses the alternate ncurses character set or not
                BOX_DRAWING_MERGABLE = 1024    // cell_attribute representing whether the cell (if a box drawing character) "merges" with other box drawing characters rather than be overridden & ignored
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

        private:
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

                this->toggle_attributes(this->grid.at(y).at(x).attributes, true);
                attroff(COLOR_PAIR(this->grid.at(y).at(x).color_pair));
            }

            void toggle_attributes(const unsigned char &attributes, const bool &attribute_state) const {
                if (bengine::bitwise_manipulator::get_bit_state(attributes, static_cast<unsigned char>(bengine::curses_window::cell_attributes::BOLD))) {
                    if (attribute_state) {
                        attron(A_BOLD);
                    } else {
                        attroff(A_BOLD);
                    }
                }
                if (bengine::bitwise_manipulator::get_bit_state(attributes, static_cast<unsigned char>(bengine::curses_window::cell_attributes::ITALIC))) {
                    if (attribute_state) {
                        attron(A_ITALIC);
                    } else {
                        attroff(A_ITALIC);
                    }
                }
                if (bengine::bitwise_manipulator::get_bit_state(attributes, static_cast<unsigned char>(bengine::curses_window::cell_attributes::UNDERLINED))) {
                    if (attribute_state) {
                        attron(A_UNDERLINE);
                    } else {
                        attroff(A_UNDERLINE);
                    }
                }
                if (bengine::bitwise_manipulator::get_bit_state(attributes, static_cast<unsigned char>(bengine::curses_window::cell_attributes::REVERSED_COLOR))) {
                    if (attribute_state) {
                        attron(A_REVERSE);
                    } else {
                        attroff(A_REVERSE);
                    }
                }
                if (bengine::bitwise_manipulator::get_bit_state(attributes, static_cast<unsigned char>(bengine::curses_window::cell_attributes::BLINKING))) {
                    if (attribute_state) {
                        attron(A_BLINK);
                    } else {
                        attroff(A_BLINK);
                    }
                }
                if (bengine::bitwise_manipulator::get_bit_state(attributes, static_cast<unsigned char>(bengine::curses_window::cell_attributes::DIM))) {
                    if (attribute_state) {
                        attron(A_DIM);
                    } else {
                        attroff(A_DIM);
                    }
                }
                if (bengine::bitwise_manipulator::get_bit_state(attributes, static_cast<unsigned char>(bengine::curses_window::cell_attributes::INVISIBLE))) {
                    if (attribute_state) {
                        attron(A_INVIS);
                    } else {
                        attroff(A_INVIS);
                    }
                }
                if (bengine::bitwise_manipulator::get_bit_state(attributes, static_cast<unsigned char>(bengine::curses_window::cell_attributes::STANDOUT))) {
                    if (attribute_state) {
                        attron(A_STANDOUT);
                    } else {
                        attroff(A_STANDOUT);
                    }
                }
                if (bengine::bitwise_manipulator::get_bit_state(attributes, static_cast<unsigned char>(bengine::curses_window::cell_attributes::PROTECTED))) {
                    if (attribute_state) {
                        attron(A_PROTECT);
                    } else {
                        attroff(A_PROTECT);
                    }
                }
                if (bengine::bitwise_manipulator::get_bit_state(attributes, static_cast<unsigned char>(bengine::curses_window::cell_attributes::ALTERNATE_CHARACTER))) {
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
            unsigned char get_cell_color_pair(const unsigned short &x, const unsigned short &y) const {
                return this->check_coordinate_bounds(x, y) ? this->grid.at(y).at(x).color_pair : bengine::curses_window::default_cell_color_pair;
            }
            unsigned short get_cell_attributes(const unsigned short &x, const unsigned short &y) const {
                return this->check_coordinate_bounds(x, y) ? this->grid.at(y).at(x).attributes : bengine::curses_window::default_cell_attributes;
            }

            // see if a cell has the desired attributes active or not
            bool check_cell_attribute_states(const unsigned short &x, const unsigned short &y, const bengine::curses_window::cell_attributes &attributes) const {
                return this->check_coordinate_bounds(x, y) && bengine::bitwise_manipulator::check_for_activated_bits(this->grid.at(y).at(x).attributes, static_cast<unsigned short>(attributes));
            }

            void present_window() const {
                if (this->get_left_x() >= COLS || this->get_right_x() < 0 || this->get_top_y() < 0 || this->get_bottom_y() >= LINES) {
                    return;
                }

                for (int row = this->y_pos; row < this->y_pos + this->get_height(); row++) {
                    for (int col = this->x_pos; col < this->x_pos + this->get_width(); col++) {
                        if (col < 0) {
                            continue;
                        }
                        if (col >= LINES) {
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
                refresh();
            }
            void clear_window() const {
                if (this->get_left_x() >= COLS || this->get_right_x() < 0 || this->get_top_y() < 0 || this->get_bottom_y() >= LINES) {
                    return;
                }

                for (int row = this->y_pos; row < this->y_pos + this->get_height(); row++) {
                    for (int col = this->x_pos; col < this->x_pos + this->get_width(); col++) {
                        if (col < 0) {
                            continue;
                        }
                        if (col >= LINES) {
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
                refresh();
            }

            // write character to window and return the position of where the cursor would be after writing the character (x + 1 unless wrapping)
            unsigned short write_character(const unsigned short &x, const unsigned short &y, const wchar_t &character, const unsigned char &color = bengine::curses_window::default_cell_color_pair, const unsigned short &attributes = bengine::curses_window::default_cell_attributes) {
                if (!this->check_coordinate_bounds(x, y)) {
                    return x;
                }
                this->grid.at(y).at(x).character = character;
                this->grid.at(y).at(x).color_pair = color;
                this->grid.at(y).at(x).attributes = attributes;
                return x == this->get_width() - 1 ? 0 : x + 1;
            }

            void reset_all_cells() {
                const unsigned short width = this->grid.at(0).size(), height = this->grid.size();
                this->grid = std::vector<std::vector<bengine::curses_window::cell>>(height, std::vector<bengine::curses_window::cell>(width));
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
    };
    wchar_t bengine::curses_window::default_cell_character = L' ';
    unsigned char bengine::curses_window::default_cell_color_pair = 1;
    unsigned short bengine::curses_window::default_cell_attributes = 1024;
    unsigned short bengine::curses_window::default_box_drawing_style = static_cast<unsigned short>(bengine::curses_window::box_drawing_styles::LIGHT_SQUARE) | static_cast<unsigned short>(bengine::curses_window::box_drawing_styles::NO_DASH);
    const std::unordered_map<unsigned char, wchar_t> bengine::curses_window::box_drawing_key = {
        {5, L'┌'}, {6, L'┎'}, {7, L'╓'}, {9, L'┍'},
        {10, L'┏'}, {13, L'╒'}, {15, L'╔'}, {17, L'┐'}, {18, L'┒'}, {19, L'╖'},
        {20, L'─'}, {21, L'┬'}, {22, L'┰'}, {23, L'╥'}, {24, L'╼'}, {25, L'┮'}, {26, L'┲'},
        {33, L'┑'}, {34, L'┓'}, {36, L'╾'}, {37, L'┭'}, {38, L'┱'},
        {40, L'━'}, {41, L'┯'}, {42, L'┳'}, {49, L'╕'},
        {51, L'╗'},
        {60, L'═'}, {61, L'╤'}, {63, L'╦'}, {65, L'│'}, {66, L'╽'}, {68, L'└'}, {69, L'├'},
        {70, L'┟'}, {72, L'┕'}, {73, L'┝'}, {74, L'┢'}, {76, L'╘'}, {77, L'╞'},
        {80, L'┘'}, {81, L'┤'}, {82, L'┧'}, {84, L'┴'}, {85, L'┼'}, {86, L'╁'}, {88, L'┶'}, {89, L'┾'},
        {90, L'╆'}, {96, L'┙'}, {97, L'┥'}, {98, L'┪'},
        {100, L'┵'}, {101, L'┽'}, {102, L'╅'}, {104, L'┷'}, {105, L'┿'}, {106, L'╈'},
        {112, L'╛'}, {113, L'╡'},
        {124, L'╧'}, {125, L'╪'}, {129, L'╿'},
        {130, L'┃'}, {132, L'┖'}, {133, L'┞'}, {134, L'┠'}, {136, L'┗'}, {137, L'┡'}, {138, L'┣'},
        {144, L'┚'}, {145, L'┦'},  {146, L'┨'}, {148, L'┸'}, {149, L'╀'},
        {150, L'╂'}, {152, L'┺'}, {153, L'╄'}, {154, L'╊'},
        {160, L'┛'}, {161, L'┩'}, {162, L'┫'}, {164, L'┹'}, {165, L'╃'}, {166, L'╉'}, {168, L'┻'}, {169, L'╇'},
        {170, L'╋'},
        {195, L'║'}, {196, L'╙'}, {199, L'╟'}, {204, L'╚'}, {207, L'╠'}, {208, L'╜'}, {211, L'╢'}, {212, L'╨'}, {215, L'╫'}, {240, L'╝'}, {243, L'╣'}, {252, L'╩'}, {255, L'╬'}
    };
    const std::unordered_map<char, std::vector<std::wstring>> bengine::curses_window::matrix_text_key = {
        {' ', {L"         ", L"                "}},
        {'!', {L" █  █  ▄ ", L" ▐▌  ▐▌  ▝▘  ▐▌ "}},
        {'"', {L"▗ ▖▝ ▘   ", L"  ▖   ▘         "}},
        {'#', {L"▟▄▙▐ ▌▜▀▛", L" ▌▐ ▀▛▜▀▄▙▟▄ ▌▐ "}},
        {'$', {L"▗▙▖▚▙▖▗▙▞", L"  ▖ ▞▀▛▘▝▀▛▚▝▀▛▘"}},
        {'%', {L"█ ▞ ▞ ▞ █", L"▞▚ ▞▚▞▞  ▞▞▚▞ ▚▞"}},
        {'&', {L"▞▚ ▞▌▖▚▞▖", L"▗▀▖ ▝▄▘ ▞▝▖▐▚▄▞▚"}},
        {'\'', {L" ▗▖  ▘   ", L" ▖▖  ▘▘         "}},
        {'(', {L" ▞▘▐   ▚▖", L" ▗▀  ▌   ▌   ▝▄ "}},
        {')', {L"▝▚   ▌▗▞ ", L" ▀▖   ▐   ▐  ▄▘ "}},
        {'*', {L"▝▄▘▗▀▖   ", L" ▚▙▘ ▘▘▘        "}},
        {'+', {L"   ▝▀▘▝▀▘", L"  ▖ ▗▄▙▖  ▌     "}},
        {',', {L"       ▜ ", L"             ▝▌ "}},
        {'-', {L"   ▗▄▖   ", L"    ▗▄▄▖        "}},
        {'.', {L"       ▄ ", L"             ▐▌ "}},
        {'/', {L"  ▞ ▞ ▞  ", L"▞▀▀▚  ▗▞ ▐▌  ▗▖ "}},
        {'0', {L"▞▀▙▌▞▐▜▄▞", L"▞▀▀▙▌ ▞▐▌▞ ▐▜▄▄▞"}},
        {'1', {L" ▟  ▐  ▟▖", L" ▞▌   ▌   ▌  ▄▙▖"}},
        {'2', {L"▞▀▚ ▗▞▟▙▄", L"▞▀▀▚   ▞ ▄▀ ▟▄▄▄"}},
        {'3', {L"▞▀▚ ▀▚▚▄▞", L"▞▀▀▚ ▄▄▞   ▐▚▄▄▞"}},
        {'4', {L"▌ ▌▝▀▛  ▌", L"▌  ▌▙▄▄▙   ▌   ▌"}},
        {'5', {L"▛▀▀▀▀▚▚▄▞", L"▛▀▀▀▚▄▄▖   ▐▚▄▄▞"}},
        {'6', {L"▞▀▀▛▀▚▚▄▞", L"▞▀▀▀▙▄▄▖▌  ▐▚▄▄▞"}},
        {'7', {L"▀▀▜ ▗▘ ▌ ", L"▀▀▀▜ ▄▄▙  ▌  ▐  "}},
        {'8', {L"▞▀▚▞▀▚▚▄▞", L"▞▀▀▚▚▄▄▞▌  ▐▚▄▄▞"}},
        {'9', {L"▞▀▚▚▄▟▗▄▟", L"▞▀▀▚▚▄▄▟   ▐▗▄▄▟"}},
        {':', {L"    ▀  ▄ ", L"     ▗▖  ▝▘  ▐▌ "}},
        {';', {L"    ▀  ▜ ", L"     ▗▖  ▝▘  ▝▌ "}},
        {'<', {L" ▗▖▐▌  ▝▘", L"  ▄▖▗▀  ▝▄    ▀▘"}},
        {'=', {L"   ▄█▄ ▀ ", L"    ▗▄▄▖▗▄▄▖    "}},
        {'>', {L"▗▖  ▐▌▝▘ ", L"▗▄    ▀▖  ▄▘▝▀  "}},
        {'?', {L"▞▀▚ ▄▘ ▄ ", L"   ▞  ▞  ▞  ▞   "}},
        {'@', {L"▞▀▚▌█▟▚▄▄", L"▞▀▀▚▌▞▚▐▌▚▟▟▚▄▄▄"}},
        {'A', {L"▞▀▚▙▄▟▌ ▐", L"▞▀▀▚▌  ▐▛▀▀▜▌  ▐"}},
        {'B', {L"▛▀▚▛▀▚▙▄▞", L"▛▀▀▚▙▄▄▞▌  ▐▙▄▄▞"}},
        {'C', {L"▞▀▚▌  ▚▄▞", L"▞▀▀▚▌   ▌   ▚▄▄▞"}},
        {'D', {L"▛▀▚▌ ▐▙▄▞", L"▛▀▀▚▌  ▐▌  ▐▙▄▄▞"}},
        {'E', {L"▛▀▀▛▀▀▙▄▄", L"▛▀▀▀▙▄▄▄▌   ▙▄▄▄"}},
        {'F', {L"▛▀▀▛▀▀▌  ", L"▛▀▀▀▙▄▄▄▌   ▌   "}},
        {'G', {L"▞▀▚▌ ▄▚▄▟", L"▞▀▀▚▌   ▌ ▀▜▚▄▄▜"}},
        {'H', {L"▌ ▐▛▀▜▌ ▐", L"▌  ▐▙▄▄▟▌  ▐▌  ▐"}},
        {'I', {L"▀▜▀ ▐ ▄▟▄", L"▀▀▛▀  ▌   ▌ ▄▄▙▄"}},
        {'J', {L"▀▜▀ ▐ ▚▟ ", L"▀▀▛▀  ▌   ▌ ▚▄▘ "}},
        {'K', {L"▌ ▞▛▀▖▌ ▐", L"▌  ▐▙▄▞▘▌ ▝▚▌  ▐"}},
        {'L', {L"▌  ▌  ▙▄▄", L"▌   ▌   ▌   ▙▄▄▄"}},
        {'M', {L"▙ ▟▌▀▐▌ ▐", L"▙  ▟▌▚▞▐▌  ▐▌  ▐"}},
        {'N', {L"▙ ▐▌▚▐▌ ▜", L"▙  ▐▌▚ ▐▌ ▚▐▌  ▜"}},
        {'O', {L"▞▀▚▌ ▐▚▄▞", L"▞▀▀▚▌  ▐▌  ▐▚▄▄▞"}},
        {'P', {L"▛▀▚▙▄▞▌  ", L"▛▀▀▚▙▄▄▞▌   ▌   "}},
        {'Q', {L"▞▀▚▌▗▐▚▄▚", L"▞▀▀▚▌  ▐▌ ▚▐▚▄▄▚"}},
        {'R', {L"▛▀▚▙▄▞▌ ▐", L"▛▀▀▚▙▄▄▞▌  ▚▌  ▐"}},
        {'S', {L"▞▀▘▝▀▚▚▄▞", L"▞▀▀▚▚▄    ▀▚▚▄▄▞"}},
        {'T', {L"▀▜▀ ▐  ▐ ", L"▀▀▛▀  ▌   ▌   ▌ "}},
        {'U', {L"▌ ▐▌ ▐▚▄▞", L"▌  ▐▌  ▐▌  ▐▚▄▄▞"}},
        {'V', {L"▌ ▐▚ ▞▝▄▘", L"▌  ▐▌  ▐▚  ▞ ▚▞ "}},
        {'W', {L"▌ ▐▌▄▐▛ ▜", L"▌  ▐▌  ▐▌▞▚▐▛  ▜"}},
        {'X', {L"▚ ▞ █ ▞ ▚", L"▚  ▞ ▚▞  ▞▚ ▞  ▚"}},
        {'Y', {L"▌ ▐▝▄▘ █ ", L"▌  ▐▝▖▗▘ ▝▌   ▌ "}},
        {'Z', {L"▀▀▜▗▞▘▙▄▄", L"▀▀▀▜  ▄▘▗▀  ▙▄▄▄"}},
        {'[', {L"▐▀▘▐  ▐▄▖", L" ▛▀  ▌   ▌   ▙▄ "}},
        {'\\', {L" █  █  █ ", L"▚    ▚    ▚    ▚"}},
        {']', {L"▝▀▌  ▌▗▄▌", L" ▀▜   ▐   ▐  ▄▟ "}},
        {'^', {L" ▄ ▝ ▘   ", L" ▗▖  ▘▝         "}},
        {'_', {L"      ▄▄▄", L"            ▄▄▄▄"}},
        {'`', {L"▗   ▘    ", L" ▗    ▘         "}},
        {'a', {L"   ▞▀▟▚▄▜", L"▗▄▄▖▗▄▄▐▌  █▚▄▄▜"}},
        {'b', {L"▌  ▙▀▚▛▄▞", L"▌   ▌▄▄▖█  ▐▛▄▄▞"}},
        {'c', {L"   ▞▀▀▚▄▄", L"    ▗▄▄▄▌   ▚▄▄▄"}},
        {'d', {L"  ▐▞▀▟▚▄▜", L"   ▐▗▄▄▐▌  █▚▄▄▜"}},
        {'e', {L"   ▟█▙▚▄▄", L"    ▗▄▄▖▙▄▄▟▚▄▄▄"}},
        {'f', {L" ▞▖▗▙▖ ▌ ", L"  ▞▖ ▄▙▖  ▌   ▚ "}},
        {'g', {L"▞▀▟▚▄▜▗▄▞", L"▗▄▄▗▌  █▚▄▄▜▗▄▄▞"}},
        {'h', {L"▌  ▙▀▚▌ ▐", L"▌   ▌▄▄▖█  ▐▌  ▐"}},
        {'i', {L" ▘  ▌  ▚ ", L"  ▖   ▖   ▌   ▚ "}},
        {'j', {L" ▝  ▐ ▝▞ ", L"  ▖   ▖   ▌  ▚▘ "}},
        {'k', {L"▌  ▙▄▘▌ ▌", L"▌   ▌  ▗▙▄▄▘▌  ▚"}},
        {'l', {L" ▌  ▌  ▚ ", L"  ▌   ▌   ▌   ▚ "}},
        {'m', {L"   ▛▞▖▌▌▌", L"    ▖▄▗▖▛ ▌▐▌ ▌▐"}},
        {'n', {L"   ▛▀▚▌ ▐", L"    ▖▄▄▖▛  ▐▌  ▐"}},
        {'o', {L"   ▞▀▚▚▄▞", L"    ▗▄▄▖▌  ▐▚▄▄▞"}},
        {'p', {L"▞▀▚▙▄▞▌  ", L"▖▄▄▖█  ▐▛▄▄▞▌   "}},
        {'q', {L"▞▀▚▚▄▟  ▐", L"▗▄▄▗▌  █▚▄▄▜   ▐"}},
        {'r', {L"   ▙▀▚▌  ", L"    ▖▄▄▖▛  ▝▌   "}},
        {'s', {L"▗▄▖▚▄▖▗▄▞", L"    ▗▄▄▖▚▄▄▖▗▄▄▞"}},
        {'t', {L" ▌ ▀▛▀ ▚ ", L"  ▌  ▄▙▖  ▌   ▚ "}},
        {'u', {L"   ▌ ▐▚▄▟", L"    ▖  ▗▌  ▐▝▄▄▜"}},
        {'v', {L"   ▌ ▐▝▄▘", L"    ▖  ▗▚  ▞ ▚▞ "}},
        {'w', {L"   ▐▐▐▝▞▟", L"    ▖▗ ▗▌▐ ▐▚▞▄▜"}},
        {'x', {L"   ▝▄▘▗▀▖", L"    ▗  ▖ ▚▞ ▗▘▝▖"}},
        {'y', {L"▌ ▐▚▄▟▗▄▞", L"▖  ▗▌  ▐▝▄▄▌▗▄▄▘"}},
        {'z', {L"▄▄▄▗▄▞▙▄▄", L"    ▄▄▄▄ ▄▄▘▟▄▄▄"}},
        {'{', {L" ▛▘█   ▙▖", L" ▛▀ ▗▘  ▝▖   ▙▄ "}},
        {'|', {L"▚   ▚   ▚", L" ▐▌  ▐▌  ▐▌  ▐▌ "}},
        {'}', {L"▝▜   █▗▟ ", L" ▀▜   ▝▖  ▗▘ ▄▟ "}},
        {'~', {L"▗▖▗▘▝▘   ", L" ▄ ▖▝ ▀         "}}
    };
}

#endif // BENGINE_CURSES_WINDOW_hpp
