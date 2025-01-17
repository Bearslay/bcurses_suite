#ifndef BENGINE_HELPERS_hpp
#define BENGINE_HELPERS_hpp

#include <iostream>
#include <vector>
#include <optional>
#include <utility>
#include <cmath>
#include <string>
#include <type_traits>

// \brief pi/8 rad or 22.5 deg
#define C_PI_8      0.39269908169872415481
// \brief pi/6 rad or 30 deg
#define C_PI_6      0.52359877559829887308
// \brief pi/4 rad or 45 deg
#define C_PI_4      0.78539816339744830961
// \brief pi/3 rad or 60 deg
#define C_PI_3      1.04719755119659774615
// \brief 3pi/8 rad or 67.5 deg
#define C_3PI_8     1.17809724509617246442
// \brief pi/2 rad or 90 deg
#define C_PI_2      1.57079632679489661923
// \brief 5pi/8 rad or 112.5 deg
#define C_5PI_8     1.96349540849362077404
// \brief 2pi/3 rad or 120 deg
#define C_2PI_3     2.09439510239319549231
// \brief 3pi/4 rad or 135 deg
#define C_3PI_4     2.35619449019234492885
// \brief 5pi/6 rad or 150 deg
#define C_5PI_6     2.61799387799149436539
// \brief 7pi/8 rad or 157.5 deg
#define C_7PI_8     2.74889357189106908365
// \brief pi rad or 180 deg
#define C_PI        3.14159265358979323846
// \brief 9pi/8 rad or 202.5 deg
#define C_9PI_8     3.53429173528851739327
// \brief 7pi/6 rad or 210 deg
#define C_7PI_6     3.66519142918809211154
// \brief 5pi/4 rad or 225 deg
#define C_5PI_4     3.92699081698724154808
// \brief 4pi/3 rad or 240 deg
#define C_4PI_3     4.18879020478639098462
// \brief 11pi/8 rad or 247.5 deg
#define C_11PI_8    4.31968989868596570289
// \brief 3pi/2 rad or 270 deg
#define C_3PI_2     4.71238898038468985769
// \brief 13pi/8 rad or 292.5 deg
#define C_13PI_8    5.10508806208341401250
// \brief 5pi/3 rad or 300 deg
#define C_5PI_3     5.23598775598298873077
// \brief 7pi/4 rad or 315 deg
#define C_7PI_4     5.49778714378213816731
// \brief 11pi/6 rad or 330 deg
#define C_11PI_6    5.75958653158128760385
// \brief 15pi/8 rad or 337.5 deg
#define C_15PI_8    5.89048622548086232212
// \brief 2pi rad or 360 deg
#define C_2PI       6.28318530717958647693

// \brief The value of sqrt(3)/2
#define C_SQRT3_2   0.86602540378443864676
// \brief The value of sqrt(2)/2
#define C_SQRT2_2   0.70710678118654752440

// \brief The value of cos(pi/8) or sqrt(2+sqrt(2))/2
#define C_MULT_1_8  0.92387953251128675613
// \brief The value of cos(pi/6) or sqrt(3)/2
#define C_MULT_1_6  0.86602540378443864676
// \brief The value of cos(pi/4) or sqrt(2)/2
#define C_MULT_1_4  0.70710678118654752440
// \brief The value of cos(pi/3) or 1/2
#define C_MULT_1_3  0.50000000000000000000
// \brief The value of cos(3pi/8) or sqrt(2-sqrt(2))/2
#define C_MULT_3_8  0.38268343236508977173

// \brief The value of sqrt(2)
#define C_SQRT2     1.41421356237309504880
// \brief The value of sqrt(3)
#define C_SQRT3     1.73205080756887729353

// \brief The value of the number just to the left of 0
#define C_0_LEFT   -0.00000000000000000001
// \brief The value of the number just to the right of 0
#define C_0_RIGHT   0.00000000000000000001
// \brief The value of the number just to the left of pi
#define C_PI_LEFT   3.14159265358979323845
// \brief The value of the number just to the right of pi
#define C_PI_RIGHT  3.14159265358979323847
// \brief The value of the number just to the left of 2*pi
#define C_2PI_LEFT  6.28318530717958647692
// \brief The value of the number just to the right of 2*pi
#define C_2PI_RIGHT 6.28318530717958647694

// \brief The conversion factor from degrees to radians (pi/180)
#define U_PI_180   0.01745329251994329577
// \brief The conversion factor from radians to degrees (180/pi)
#define U_180_PI   57.2957795130823208770


namespace bengine {
    // \brief A class to contain dimensional data relating to a grid of cells within a set zone; primarily used to calculate and store the size of said cells
    class padded_grid {
        public:
            enum class alignments : const unsigned char {
                TOP_LEFT = 0,
                TOP_CENTER = 1,
                TOP_RIGHT = 2,
                CENTER_LEFT = 3,
                CENTER_CENTER = 4,
                CENTER_RIGHT = 5,
                BOTTOM_LEFT = 6,
                BOTTOM_CENTER = 7,
                BOTTOM_RIGHT = 8
            };

        private:
            // \brief The left-right and top-bottom alignment of the grid within the given zone; relates the top-left corner of the zone to the top-left corner of the top-left cell
            unsigned char alignment = static_cast<unsigned char>(alignments::CENTER_CENTER);
            // \brief Whether the cells are square or not
            bool square_cells = false;

            // \brief The width of the zone that the grid inhabits
            unsigned int width = 0;
            // \brief The height of the zone that the grid inhabits
            unsigned int height = 0;
            // \brief The amount of columns that the grid has
            unsigned short cols = 0;
            // \brief The amount of rows that the grid has
            unsigned short rows = 0;
            // \brief The horizontal distance between cells
            unsigned short gap_width = 0;
            // \brief The vertical distance between cells
            unsigned short gap_height = 0;

            // \brief The width of the area that the cells actually inhabit
            unsigned int usable_width = 0;
            // \brief The height of the area that the cells actually inhabit
            unsigned int usable_height = 0;
            // \brief The width of each cell
            unsigned int cell_width = 0;
            // \brief The height of each cell
            unsigned int cell_height = 0;
            // \brief The x-offset of the top-left cell's top-left corner from the zone's top-left corner
            unsigned short x_offset = 0;
            // \brief The y-offset of the top-left cell's top-left corner from the zone's top-left corner
            unsigned short y_offset = 0;

            // \brief Update all of the "output" values for the padded grid
            void update() {
                this->usable_width = this->width - this->gap_width * (this->cols - 1);
                this->usable_height = this->height - this->gap_height * (this->rows - 1);
                this->cell_width = this->usable_width / this->cols;
                this->cell_height = this->usable_height / this->rows;

                // When using square cells, both the cell's width and height are set to the smaller dimension so that the cells don't escape the zone
                if (this->square_cells) {
                    if (this->cell_width < this->cell_height) {
                        this->cell_height = this->cell_width;
                    } else {
                        this->cell_width = this->cell_height;
                    }
                }

                if (this->alignment % 3 == 0) {
                    this->x_offset = 0;
                } else if (this->alignment % 3 == 1) {
                    this->x_offset = (this->usable_width - this->cell_width * this->cols) / 2;
                } else {
                    this->x_offset = this->usable_width - this->cell_width * this->cols;
                }
                if (this->alignment <= 2) {
                    this->y_offset = 0;
                } else if (this->alignment <= 5) {
                    this->y_offset = (this->usable_height - this->cell_height * this->rows) / 2;
                } else {
                    this->y_offset = this->usable_width - this->cell_width * this->cols;
                }
            }

        public:
            /** bengine::padded_grid constructor
             * \param width The width of the zone that the grid will inhabit
             * \param height The height of the zone that the grid will inhabit
             * \param cols The amount of columns that the grid will have
             * \param rows The amount of rows that the grid will have
             * \param gap_width The amount of horiztonal space between cells
             * \param gap_height The amount of vertical space between cells
             * \param alignment The left-right and top-bottom alignment of the grid within the given zone; relates the top-left corner of the zone to the top-left corner of the top-left cell
             */
            padded_grid(const unsigned int &width, const unsigned int &height, const unsigned short &cols, const unsigned short &rows, const unsigned short &gap_width, const unsigned short &gap_height, const alignments &alignment = alignments::CENTER_CENTER) : width(width), height(height), cols(cols), rows(rows), gap_width(gap_width), gap_height(gap_height) {
                this->alignment = static_cast<unsigned char>(alignment) > static_cast<unsigned char>(alignments::BOTTOM_RIGHT) ? static_cast<unsigned char>(alignments::CENTER_CENTER) : static_cast<unsigned char>(alignment);
                this->update();
            }
            // \brief bengine::padded_grid deconstructor
            ~padded_grid() {}

            /** Get the alignment of the padded grid
             * \returns The alignment of the grid
             */
            unsigned char get_alignment() const {
                return this->alignment;
            }
            /** Set the alignmnet for the padded grid
             * \param alignment The new alignment for the padded grid
             */
            void set_alignment(const alignments &alignment) {
                this->alignment = static_cast<unsigned char>(alignment) > static_cast<unsigned char>(alignments::BOTTOM_RIGHT) ? static_cast<unsigned char>(alignments::CENTER_CENTER) : static_cast<unsigned char>(alignment);
                this->update();
            }

            /** Check if the grid uses square cells or not
             * \returns Whether the grid uses square cells or not
             */
            bool has_square_cells() const {
                return this->square_cells;
            }
            /** Set whether the grid should use square cells or not
             * \param state Whether the grid should use square cells or not
             */
            void set_cell_squareness(const bool &state = false) {
                this->square_cells = state;
                this->update();
            }
            // \brief Toggle whether the grid uses square cells or not
            void toggle_cell_squareness() {
                this->square_cells = !this->square_cells;
                this->update();
            }

            /** Get the width of the grid's zone
             * \returns The width of the grid's zone
             */
            unsigned int get_width() const {
                return this->width;
            }
            /** Get the height of the grid's zone
             * \returns The height of the grid's zone
             */
            unsigned int get_height() const {
                return this->height;
            }
            /** Get the amount of columns that the grid has
             * \returns The amount of columns that the grid has
             */
            unsigned short get_cols() const {
                return this->cols;
            }
            /** Get the amount of rows that the grid has
             * \returns The amount of rows that the grid has
             */
            unsigned short get_rows() const {
                return this->rows;
            }
            /** Get the horizontal distance between each cell
             * \returns The horizontal distance between each cell
             */
            unsigned short get_gap_width() const {
                return this->gap_width;
            }
            /** Get the vertical distance between each cell
             * \returns The vertical distance between each cell
             */
            unsigned short get_gap_height() const {
                return this->gap_height;
            }

            /** Set the width of the zone that the grid will inhabit
             * \param width The new width of the zone
             */
            void set_width(const unsigned int &width) {
                this->width = width;
                this->update();
            }
            /** Set the height of the zone that the grid will inhabit
             * \param width The new height of the zone
             */
            void set_height(const unsigned int &height) {
                this->height = height;
                this->update();
            }
            /** Set the amount of the columns that the grid will have
             * \param cols The new amount of columns
             */
            void set_cols(const unsigned short &cols) {
                this->cols = cols;
                this->update();
            }
            /** Set the amount of the rows that the grid will have
             * \param cols The new amount of rows
             */
            void set_rows(const unsigned short &rows) {
                this->rows = rows;
                this->update();
            }
            /** Set the horizontal distance between cells
             * \param gap_width The new horizontal distance between cells
             */
            void set_gap_width(const unsigned short &gap_width) {
                this->gap_width = gap_width;
                this->update();
            }
            /** Set the vertical distance between cells
             * \param gap_width The new vertical distance between cells
             */
            void set_gap_height(const unsigned short &gap_height) {
                this->gap_height = gap_height;
                this->update();
            }

            /** Get the width of the area that the cells inhabit
             * \returns The width of the area that the cells inhabit
             */
            unsigned int get_usable_width() const {
                return this->usable_width;
            }
            /** Get the height of the area that the cells inhabit
             * \returns The height of the area that the cells inhabit
             */
            unsigned int get_usable_height() const {
                return this->usable_height;
            }
            /** Get the width of each cell
             * \returns The width of each cell
             */
            unsigned int get_cell_width() const {
                return this->cell_width;
            }
            /** Get the height of each cell
             * \returns The height of each cell
             */
            unsigned int get_cell_height() const {
                return this->cell_height;
            }
            /** Get the horizontal distance between the top-left corner of the zone and the top-left corner of the top-left cell in the grid
             * \returns The horizontal distance between the top-left corner of the zone and the top-left corner of the top-left cell in the grid
             */
            unsigned short get_x_offset() const {
                return this->x_offset;
            }
            /** Get the vertical distance between the top-left corner of the zone and the top-left corner of the top-left cell in the grid
             * \returns The vertical distance between the top-left corner of the zone and the top-left corner of the top-left cell in the grid
             */
            unsigned short get_y_offset() const {
                return this->y_offset;
            }
    };

    // \brief A class containing useful functions designed for 4/8-bit autotiling
    class autotiler {
        private:
            // \brief Key containing the 47 bitmasks relevant to 8-bit autotiling
            const static unsigned char eight_bit_mask_key[47];

            // \brief List of unicode characters used in terminal-based 4-bit autotiling
            const static char* four_bit_unicode_key[32];
            // \brief List of unicode characters used in terminal-based 8-bit autotiling
            const static char* eight_bit_unicode_key[94];

            /** Calculate the 4-bit mask value for a given tile within a grid
             * 
             * Any bounds-checking needs to happen outside of this function
             * 
             * \param grid The grid containing 4-bit mask values
             * \param x The x-position (col) of the tile to update within the grid
             * \param y The y-position (row) of the tile to update within the grid
             * \param use_solid_boundaries Whether to consider the edges of the grid as full or empty tiles
             * \returns The updated value of the indicated tile or -1 if the tile is is already -1
             */
            static char calculate_4_bit_mask(const std::vector<std::vector<char>> &grid, const unsigned long int &x, const unsigned long int &y, const bool &use_solid_boundaries = false) {
                // Check to see if the current tile would even display anything
                if (grid.at(y).at(x) < 0) {
                    return -1;
                }
                return (y > 0 ? (grid.at(y - 1).at(x) >= 0) : use_solid_boundaries) + (x > 0 ? (grid.at(y).at(x - 1) >= 0) : use_solid_boundaries) * 2 + (x < grid.at(0).size() - 1 ? (grid.at(y).at(x + 1) >= 0) : use_solid_boundaries) * 4 + (y < grid.size() - 1 ? (grid.at(y + 1).at(x) >= 0) : use_solid_boundaries) * 8;
            }

            /** Calculate the 8-bit mask value for a given tile within a grid
             * 
             * Any bounds-checking needs to happen outside of this function
             * 
             * \param grid The grid containing 8-bit mask values
             * \param x The x-position (col) of the tile to update within the grid
             * \param y The y-position (row) of the tile to update within the grid
             * \param use_solid_boundaries Whether to consider the edges of the grid as full or empty tiles
             * \returns The updated value of the indicated tile or -1 if the tile is is already -1
             */
            static char calculate_8_bit_mask(const std::vector<std::vector<char>> &grid, const unsigned long int &x, const unsigned long int &y, const bool &use_solid_boundaries = false) {
                // Check to see if the current tile would even display anything
                if (grid.at(y).at(x) < 0) {
                    return -1;
                }

                const bool tl = y > 0 && x > 0 ? (grid.at(y - 1).at(x - 1) >= 0) : use_solid_boundaries;
                const bool t = y > 0 ? (grid.at(y - 1).at(x) >= 0) : use_solid_boundaries;
                const bool tr = y > 0 && x < grid.at(0).size() - 1 ? (grid.at(y - 1).at(x + 1) >= 0) : use_solid_boundaries;
                const bool l = x > 0 ? (grid.at(y).at(x - 1) >= 0) : use_solid_boundaries;
                const bool r = x < grid.at(0).size() - 1 ? (grid.at(y).at(x + 1) >= 0) : use_solid_boundaries;
                const bool bl = y < grid.size() - 1 && x > 0 ? (grid.at(y + 1).at(x - 1) >= 0) : use_solid_boundaries;
                const bool b = y < grid.size() - 1 ? (grid.at(y + 1).at(x) >= 0) : use_solid_boundaries;
                const bool br = y < grid.size() - 1 && x < grid.at(0).size() - 1 ? (grid.at(y + 1).at(x + 1) >= 0) : use_solid_boundaries;

                const unsigned char mask = (tl && t && l) + t * 2 + (tr && t && r) * 4 + l * 8 + r * 16 + (bl && b && l) * 32 + b * 64 + (br && b && r) * 128;
                for (unsigned char i = 0; i < 47; i++) {
                    if (mask == bengine::autotiler::eight_bit_mask_key[i]) {
                        return i;
                    }
                }
                return -1;
            }

        public:
            // \brief bengine::autotiler constructor
            autotiler() {}
            // \brief bengine::autotiler deconstructor
            ~autotiler() {}

            /** Change a tile and update surrounding ones in a 4-bit autotiling grid
             * \param grid Grid of indexing values that dictate the source frame for the texture sheet
             * \param x x-position of the changed tile in the grid
             * \param y y-position of the changed tile in the grid
             * \param state Whether to add or remove a tile in the indicated position
             * \param use_solid_boundaries Whether to consider the edges of the grid as full or empty tiles
             * \returns The value of the updated tile
             */
            static char modify_4_bit_grid(std::vector<std::vector<char>> &grid, const unsigned long int &x, const unsigned long int &y, const bool &state = true, const bool &use_solid_boundaries = false) {
                if (y >= grid.size() || x >= grid.at(0).size()) {
                    return -1;
                }
                grid[y][x] = state - 1;

                for (char i = -1; i <= 1; i++) {
                    for (char j = -1; j <= 1; j++) {
                        // A 4-bit autotiling mask only requires cardinal directions, so the corners are skipped
                        if (std::abs(i) == 1 && std::abs(j) == 1) {
                            continue;
                        }
                        // Check if the current tile is in-bounds
                        if (y + i < 0 || y + i >= grid.size() || x + j < 0 || x + j >= grid.at(0).size()) {
                            continue;
                        }
                        // Update the mask value for the current tile
                        grid[y + i][x + j] = bengine::autotiler::calculate_4_bit_mask(grid, x + j, y + i, use_solid_boundaries);
                    }
                }
                return grid.at(y).at(x);
            }
            /** Change a tile and update surrounding ones in an 8-bit autotiling grid
             * \param grid Grid of indexing values that dictate the source frame for the texture sheet
             * \param x x-position of the changed tile in the grid
             * \param y y-position of the changed tile in the grid
             * \param state Whether to add or remove a tile in the indicated position
             * \param use_solid_boundaries Whether to consider the edges of the grid as full or empty tiles
             * \returns The value of the updated tile
             */
            static char modify_8_bit_grid(std::vector<std::vector<char>> &grid, const unsigned long int &x, const unsigned long int &y, const bool &state = true, const bool &use_solid_boundaries = false) {
                if (y >= grid.size() || x >= grid.at(0).size()) {
                    return -1;
                }
                grid[y][x] = state - 1;

                for (char i = -1; i <= 1; i++) {
                    for (char j = -1; j <= 1; j++) {
                        // Check if the current tile is in-bounds
                        if (y + i < 0 || y + i >= grid.size() || x + j < 0 || x + j >= grid.at(0).size()) {
                            continue;
                        }
                        // Update the mask value for the current tile
                        grid[y + i][x + j] = bengine::autotiler::calculate_8_bit_mask(grid, x + j, y + i, use_solid_boundaries);
                    }
                }
                return grid.at(y).at(x);
            }

            /** Populate a grid of full/empty tiles with appropriate 4-bit mask values
             * \param grid The grid containing full (true) or empty (false) tiles to be populated
             * \param use_solid_boundaries Whether to consider the borders of the grid to have full or empty tiles
             * \returns A grid of the same dimensions as the input grid, but containing 4-bit mask values rather than boolean ones
             */
            static std::vector<std::vector<char>> populate_4_bit_grid(const std::vector<std::vector<bool>> &grid, const bool &use_solid_boundaries = false) {
                std::vector<std::vector<char>> output;
                for (std::size_t i = 0; i < grid.size(); i++) {
                    output.emplace_back();
                    for (std::size_t j = 0; j < grid.at(0).size(); j++) {
                        output[i].emplace_back(grid.at(i).at(j) ? 1 : -1);
                    }
                }
                for (std::size_t i = 0; i < grid.size(); i++) {
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        output[i][j] = bengine::autotiler::calculate_4_bit_mask(output, j, i, false);
                    }
                }
                return output;
            }

            /** Populate a grid of full/empty tiles with appropriate 8-bit mask values
             * \param grid The grid containing full (true) or empty (false) tiles to be populated
             * \param use_solid_boundaries Whether to consider the borders of the grid to have full or empty tiles
             * \returns A grid of the same dimensions as the input grid, but containing 8-bit mask values rather than boolean ones
             */
            static std::vector<std::vector<char>> populate_8_bit_grid(const std::vector<std::vector<bool>> &grid, const bool &use_solid_boundaries = false) {
                std::vector<std::vector<char>> output;
                for (std::size_t i = 0; i < grid.size(); i++) {
                    output.emplace_back();
                    for (std::size_t j = 0; j < grid.at(0).size(); j++) {
                        output[i].emplace_back(grid.at(i).at(j) ? 1 : -1);
                    }
                }
                for (std::size_t i = 0; i < grid.size(); i++) {
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        output[i][j] = bengine::autotiler::calculate_8_bit_mask(output, j, i, false);
                    }
                }
                return output;
            }

            /** Print a grid of 4-bit mask values to iostream using unicode block element characters
             * \param grid The grid of 4-bit mask values to print
             */
            static void print_4_bit_grid(const std::vector<std::vector<char>> &grid) {
                for (std::size_t i = 0; i < grid.size(); i++) {
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        if (grid.at(i).at(j) < 0) {
                            std::cout << "    ";
                            continue;
                        }
                        std::cout << bengine::autotiler::four_bit_unicode_key[(unsigned char)grid.at(i).at(j)];
                    }
                    std::cout << "\n";
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        if (grid.at(i).at(j) < 0) {
                            std::cout << "    ";
                            continue;
                        }
                        std::cout << bengine::autotiler::four_bit_unicode_key[(unsigned char)grid.at(i).at(j) + 16];
                    }
                    std::cout << "\n";
                }
            }

            /** Print a grid of 8-bit mask values to iostream using unicode block element characters
             * \param grid The grid of 8-bit mask values to print
             */
            static void print_8_bit_grid(const std::vector<std::vector<char>> &grid) {
                for (std::size_t i = 0; i < grid.size(); i++) {
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        if (grid.at(i).at(j) < 0) {
                            std::cout << "    ";
                            continue;
                        }
                        std::cout << bengine::autotiler::eight_bit_unicode_key[(unsigned char)grid.at(i).at(j)];
                    }
                    std::cout << "\n";
                    for (std::size_t j = 0; j < grid.at(i).size(); j++) {
                        if (grid.at(i).at(j) < 0) {
                            std::cout << "    ";
                            continue;
                        }
                        std::cout << bengine::autotiler::eight_bit_unicode_key[(unsigned char)grid.at(i).at(j) + 47];
                    }
                    std::cout << "\n";
                }
            }
    };
    // \brief Key containing the 47 bitmasks relevant to 8-bit autotiling
    const unsigned char bengine::autotiler::eight_bit_mask_key[47] = {0, 2, 8, 10, 11, 16, 18, 22, 24, 26, 27, 30, 31, 64, 66, 72, 74, 75, 80, 82, 86, 88, 90, 91, 94, 95, 104, 106, 107, 120, 122, 123, 126, 127, 208, 210, 214, 216, 218, 219, 222, 223, 248, 250, 251, 254, 255};
    // \brief List of unicode characters used in terminal-based 4-bit autotiling
    const char* bengine::autotiler::four_bit_unicode_key[32] = {" ▄▄ ", " ██ ", "▄▄▄ ", "▄██ ", " ▄▄▄", " ██▄", "▄▄▄▄", "▄██▄", " ▄▄ ", " ██ ", "▄▄▄ ", "▄██ ", " ▄▄▄", " ██▄", "▄▄▄▄", "▄██▄", " ▀▀ ", " ▀▀ ", "▀▀▀ ", "▀▀▀ ", " ▀▀▀", " ▀▀▀", "▀▀▀▀", "▀▀▀▀", " ██ ", " ██ ", "▀██ ", "▀██ ", " ██▀", " ██▀", "▀██▀", "▀██▀"};
    // \brief List of unicode characters used in terminal-based 8-bit autotiling
    const char* bengine::autotiler::eight_bit_unicode_key[94] = {" ▄▄ ", " ██ ", "▄▄▄ ", "▄██ ", "███ ", " ▄▄▄", " ██▄", " ███", "▄▄▄▄", "▄██▄", "███▄", "▄███", "████", " ▄▄ ", " ██ ", "▄▄▄ ", "▄██ ", "███ ", " ▄▄▄", " ██▄", " ███", "▄▄▄▄", "▄██▄", "███▄", "▄███", "████", "▄▄▄ ", "▄██ ", "███ ", "▄▄▄▄", "▄██▄", "███▄", "▄███", "████", " ▄▄▄", " ██▄", " ███", "▄▄▄▄", "▄██▄", "███▄", "▄███", "████", "▄▄▄▄", "▄██▄", "███▄", "▄███", "████", " ▀▀ ", " ▀▀ ", "▀▀▀ ", "▀▀▀ ", "▀▀▀ ", " ▀▀▀", " ▀▀▀", " ▀▀▀", "▀▀▀▀", "▀▀▀▀", "▀▀▀▀", "▀▀▀▀", "▀▀▀▀", " ██ ", " ██ ", "▀██ ", "▀██ ", "▀██ ", " ██▀", " ██▀", " ██▀", "▀██▀", "▀██▀", "▀██▀", "▀██▀", "▀██▀", "███ ", "███ ", "███ ", "███▀", "███▀", "███▀", "███▀", "███▀", " ███", " ███", " ███", "▀███", "▀███", "▀███", "▀███", "▀███", "████", "████", "████", "████", "████"};

    // \brief A class containing useful functions that help with using bitwise operators in clever ways, especially in the realm of storing several values within a single integral variable
    class bitwise_manipulator {
        public:
            // \brief bengine::bitwise_manipulator constructor
            bitwise_manipulator() {}
            // \brief bengine::bitwise_manipulator deconstructor
            ~bitwise_manipulator() {}

            /** Convert a range of bits indicated by a starting bit and amount of bits into a value where the bits within the range are set to 1's and all others are 0's
             * \tparam type An integral type
             * \param starting_bit The bit where the range starts (least significant; starts from 0)
             * \param bit_range Amount of bits within the range
             * \returns A value that encodes the desired range of bits as consecutive 1's in binary
             */
            template <class type> static type bit_range_to_value(const unsigned char &starting_bit, const unsigned char &bit_range) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return static_cast<type>(static_cast<type>(std::pow<type, type>(2, bit_range) - 1) << starting_bit);    // Basically just calculate max integer value for a number with bit_range amount of bits and then shift it over
            }

            /** Set the indicated bits of an integral variable to 1 regardless of their original state
             * \tparam type An integral type
             * \param input The starting value
             * \param bits The bits to activate as indicated by a 1, best inputted as 0x101 (5 as represented by a 3-bit integer) or something similar to easily visualize the bits that will be activated
             * \returns The value of the input but with the indicated bits set to 1 (literally input | bits)
             */
            template <class type> static type activate_bits(const type &input, const type &bits) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return input | bits;
            }
            /** Set the indicated bits of an integral variable to 1 regardless of their original state
             * \tparam type An integral type
             * \param input The starting value
             * \param starting_bit The bit where the range of bits to activate starts (least significant; starts from 0)
             * \param bit_range Amount of bits within the range to activate
             * \returns The value of the input but with the indicated bits set to 1
             */
            template <class type> static type activate_bits(const type &input, const unsigned char &starting_bit, const unsigned char &bit_range) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return bengine::bitwise_manipulator::activate_bits<type>(input, bengine::bitwise_manipulator::bit_range_to_value<type>(starting_bit, bit_range));
            }
            /** Set the indicated bit of an integral variable to a 1 regardless of its original state
             * \tparam type An integral type
             * \param input The starting value
             * \param bit The bit to activate where 0 indicates the least significant bit while 7 would be the most significant bit of an 8-bit type (like char); if this value is greater than the amount of bits available nothing will be changed
             * \returns The value of the input but with the indicated bit set to 1; value will match the input if the inputted bit is too large
             */
            template <class type> static type activate_bit(const type &input, const unsigned char &bit) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return bit > sizeof(input) * 8 - 1 ? input : input | static_cast<type>(std::pow<type, type>(2, bit));
            }
            /** Set the indicated bits of an integral variable to 0 regardless of their original state
             * \tparam type An integral type
             * \param input The starting value
             * \param bits The bits to deactivate as indicated by a 1, best inputted as 0x101 (5 as represented by a 3-bit integer) or something similar to easily visualize the bits that will be deactivated
             * \returns The value of the input but with the indicated bits set to 0 (essentially input & (bits ^ type_max_value) )
             */
            template <class type> static type deactivate_bits(const type &input, const type &bits) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return input & (bits ^ static_cast<type>(std::pow<type, type>(2, sizeof(input) * 8) - 1));    // sizeof(input) * 8 yields amount of bits in input data type, which is then used to find max unsigned value of input data type with 2^bits - 1
            }
            /** Set the indicated bits of an integral variable to 0 regardless of their original state
             * \tparam type An integral type
             * \param input The starting value
             * \param starting_bit The bit where the range of bits to deactivate starts (least significant; starts from 0)
             * \param bit_range Amount of bits within the range to deactivate
             * \returns The value of the input but with the indicated bits set to 0
             */
            template <class type> static type deactivate_bits(const type &input, const unsigned char &starting_bit, const unsigned char &bit_range) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return bengine::bitwise_manipulator::deactivate_bits<type>(input, bengine::bitwise_manipulator::bit_range_to_value<type>(starting_bit, bit_range));
            }
            /** Set the indicated bit of an integral variable to a 0 regardless of its original state
             * \tparam type An integral type
             * \param input The starting value
             * \param bit The bit to activate where 0 indicates the least significant bit while 7 would be the most significant bit of an 8-bit type (like char); if this value is greater than the amount of bits available nothing will be changed
             * \returns The value of the input but with the indicated bit set to 0
             */
            template <class type> static type deactivate_bit(const type &input, const unsigned char &bit) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return bengine::bitwise_manipulator::deactivate_bits<type>(input, bengine::bitwise_manipulator::activate_bit<type>(0, bit));
            }

            /** Check whether a specified set of bits are true or not
             * \tparam type An integral type
             * \param source Set of bits to check the state of
             * \param bits The bits to check
             */
            template <class type> static bool check_for_activated_bits(const type &source, const type &bits) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                return (source & bits) == bits;
            }
            /** Get whether a particular bit of a source variable is set to a 0 or 1
             * \tparam type An integral type
             * \param source The source value to analyze
             * \param bit The bit to read the value of
             * \returns Whether the designated bit is a 0 (false) or 1 (true); returns false if the inputted bit is too large for the given data type
             */
            template <class type> static bool get_bit_state(const type &source, const unsigned char &bit) {
                static_assert(std::is_integral<type>::value, "Template type \"type\" must be an integral type (int, long, unsigned char, etc)");
                const unsigned char max_bitshift = sizeof(source) * 8 - 1;
                return bit > max_bitshift ? false : static_cast<type>(source << (max_bitshift - bit)) >> max_bitshift;
            }

            /** Extract a set of bits held within a larger set as a "subvalue" of the full value
             * 
             * As an example: A subvalue of the 8-bit integer 0x00010011 (19) could be described by a start of 3 and a range of 5 to yield the 5-bit integer 0x00010 (2)
             * 
             * \tparam value_type An integral type
             * \tparam subvalue_type An integral type for the subvalue (typically smaller than value_type size-wise)
             * \param value The larger value to extract a subvalue from
             * \param subvalue_start The bit where the subvalue starts (least significant; starts from 0)
             * \param subvalue_range Amount of bits that the subvalue has
             * \returns A subvalue of value as represented by a set of bits from value
             */
            template <class value_type, class subvalue_type = value_type> static subvalue_type get_subvalue(const value_type &value, const unsigned char &subvalue_start, const unsigned char &subvalue_range) {
                static_assert(std::is_integral<value_type>::value, "Template type \"value_type\" must be an integral type (int, long, unsigned char, etc)");
                static_assert(std::is_integral<subvalue_type>::value, "Template type \"subvalue_type\" must be an integral type (int, long, unsigned char, etc)");
                const unsigned char max_bitshift = sizeof(value) * 8 - 1 - subvalue_range;
                return static_cast<subvalue_type>(static_cast<value_type>(value << (max_bitshift - subvalue_start)) >> max_bitshift);
            }

            /** Set a set of bits held within a larger set as a "subvalue" of the full value to a new "subvalue"
             * 
             * As an example: Setting the subvalue described by a start of 0, a range of 4, and the value of 0x1001 (9) of the 8-bit integer 0x0110011 (51) changes said integer to 0x0111001 (57) 
             * 
             * \tparam value_type An integral type
             * \tparam subvalue_type An integral type for the subvalue (typically smaller than value_type size-wise)
             * \param value The larger value containing a subvalue to be changed
             * \param subvalue The new subvalue for the main value to have imprinted on it
             * \param subvalue_start The bit where the subvalue starts (least significant; starts from 0)
             * \param subvalue_range Amount of bits that the subvalue has
             * \returns The inputted value but with the subvalue imprinted to it at the designated bitwise location
             */
            template <class value_type, class subvalue_type = value_type> static value_type set_subvalue(const value_type &value, const subvalue_type &subvalue, const unsigned char &subvalue_start, const unsigned char &subvalue_range) {
                static_assert(std::is_integral<value_type>::value, "Template type \"value_type\" must be an integral type (int, long, unsigned char, etc)");
                static_assert(std::is_integral<subvalue_type>::value, "Template type \"subvalue_type\" must be an integral type (int, long, unsigned char, etc)");
                return bengine::bitwise_manipulator::activate_bits<value_type>(bengine::bitwise_manipulator::deactivate_bits<value_type>(value, subvalue_start, subvalue_range), static_cast<value_type>(subvalue << subvalue_start));
            }
    };

    // \brief A class containing useful functions that make converting from arithmetic data types to strings with various formatting additions easy as well as converting strings to numbers
    class string_helper {
        public:
            // \brief bengine::string_helper constructor
            string_helper() {}
            // \brief bengine::string_helper deconstructor
            ~string_helper() {}

            /** Convert an arithmetic data type to an std::string; trailing zeros and decimals are omitted from floating-point conversions
             * \tparam type An arithmetic data type to convert from
             * \param input The arithmetic value to convert into a string
             * \returns An std::string where each character represents a part of the input; trailing zeros and decimals are omitted from floating-point conversions
             */
            template <class type> static std::string to_string(const type &input) {
                static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");

                // std::to_string() is acceptable for anything other than floating-point types
                if (std::is_integral<type>::value) {
                    return std::to_string(input);
                }

                std::string output = std::to_string(input);
                // Remove the last character if it is either a zero or a decimal
                while (output.back() == '0') {
                    output.pop_back();
                    // Break seperately at the decimal so that zeros in positive places aren't removed
                    if (output.back() == '.') {
                        output.pop_back();
                        break;
                    }
                }
                return output;
            }

            /** Convert an arithmetic data type to an std::string while having the ability to specify sign; trailing zeros and decimals are omitted from floating-point conversions
             * \tparam type An arithmetic data type to convert from
             * \param input The arithmetic value to convert into a string
             * \param always_include_sign Whether to include a positive sign if the input is positive or not (a negative sign will be included regardless of the value of this parameter)
             * \returns An std::string where each character represents a part of the input (and might also specifically include a sign); trailing zeros and decimals are omitted from floating-point conversions
             */
            template <class type> static std::string to_string(const type &input, const bool always_include_sign) {
                static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");
                return (always_include_sign && input >= 0 ? "+" : "") + bengine::string_helper::to_string<type>(input);
            }

            /** Convert an arithmetic data type to an std::string with extra leading/trailing zeros if specified
             * \tparam type An arithmetic data type to convert from
             * \param input The arithmetic value to convert into a string
             * \param left_digits The minimum amount of digits that will be to the left of the decimal (can be exceeded depending on input value); reached by adding leading zeros
             * \param right_digits The minimum amount of digits that will be to the right of the decimal (can be exceeded depending on input value); reached by adding trailing zeros
             * \returns An std::string where each character represents a part of the input; leading and trailing zeros are also included in order to meet certain requirements
             */
            template <class type> static std::string to_string_with_added_zeros(const type &input, const unsigned long int &left_digits, const unsigned long int &right_digits) {
                static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");
                std::string output = bengine::string_helper::to_string<type>(input);
                const unsigned long int decimal_index = output.find('.');
                const unsigned long int left_length = decimal_index == std::string::npos ? output.length() : decimal_index;
                const unsigned long int right_length = decimal_index == std::string::npos ? 0 : output.length() - left_length - 1;
                const bool insert_position = input < 0;

                // Add on the leading zeros
                for (unsigned long int i = left_length; i < left_digits; i++) {
                    output.insert(insert_position, "0");
                }
                // Return early if no trailing zeros are needed
                if (right_digits == 0) {
                    return output;
                }

                // Add a decimal place if it is missing
                if (decimal_index == std::string::npos) {
                    output += ".";
                }
                // Add on the trailing zeros
                for (unsigned long int i = right_length; i < right_digits; i++) {
                    output += '0';
                }
                return output;
            }

            /** Convert an arithmetic data type to an std::string with extra leading/trailing zeros if specified
             * \tparam type An arithmetic data type to convert from
             * \param input The arithmetic value to convert into a string
             * \param left_digits The minimum amount of digits that will be to the left of the decimal (can be exceeded depending on input value); reached by adding leading zeros
             * \param right_digits The minimum amount of digits that will be to the right of the decimal (can be exceeded depending on input value); reached by adding trailing zeros
             * \returns An std::string where each character represents a part of the input; leading and trailing zeros are also included in order to meet certain parameters
             */
            template <class type> static std::string to_string_with_added_zeros(const type &input, const unsigned long int &left_digits, const unsigned long int &right_digits, const bool &always_include_sign) {
                return (always_include_sign && input >= 0 ? "+" : "") + bengine::string_helper::to_string_with_added_zeros<type>(input, left_digits, right_digits);
            }

            /** Convert an arithmetic data type to an std::string with a specified minimum length
             * \tparam type An arithmetic data type to convert from
             * \param input The arithmetic value to convert into a string
             * \param length The minimum length of the output std::string (reached using leading/trailing zeros)
             * \param add_leading_zeros Whether to add leading zeros or not (trailing zeros) in order to reach the minimum length
             * \returns An std::string where each character represents a part of the input and also beginning with either a positive or negatie sign; leading or trailing zeros are included to reach a specified minimum string length
             */
            template <class type> static std::string to_string_with_target_length(const type &input, const unsigned long int &length, const bool &add_leading_zeros) {
                static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");
                std::string output = bengine::string_helper::to_string<type>(input);

                // Adding leading zeros until the desired length is reached
                if (add_leading_zeros) {
                    const bool insert_position = input < 0;
                    while (output.length() < length) {
                        output.insert(insert_position, "0");
                    }
                    return output;
                }
                
                if (output.find('.') == std::string::npos) {
                    // If the output would exceed the desired length by adding on a '.0', then a leading zero is added instead
                    if (output.length() >= length - 1) {
                        output.insert(input < 0, "0");
                        return output;
                    }
                    output += ".0";
                }

                // Add trailing zeros until the desired length is reached
                while (output.length() < length) {
                    output += "0";
                }
                return output;
            }

            /** Convert an arithmetic data type to an std::string with a specified minimum length
             * \tparam type An arithmetic data type to convert from
             * \param input The arithmetic value to convert into a string
             * \param length The minimum length of the output std::string (reached using leading/trailing zeros)
             * \param add_leading_zeros Whether to add leading zeros (true) or add trailing zeros (false) to reach the minimum length
             * \param always_include_sign Whether to include a positive sign if the input is positive or not (sign contributes to the desired length)
             * \returns An std::string where each character represents a part of the input; leading or trailing zeros are included to reach a specified minimum string length
             */
            template <class type> static std::string to_string_with_target_length(const type &input, const unsigned long int &length, const bool &add_leading_zeros, const bool &always_include_sign) {
                return (always_include_sign && input >= 0 ? "+" : "") + bengine::string_helper::to_string_with_target_length<type>(input, length, add_leading_zeros);
            }

            /** Convert an arithmetic data type to an std::u16string making use of the bengine::string_helper::to_string() function
             * \tparam type An arithmetic data type to convert from
             * \param input The arithmetic value to convert into an std::u16string
             * \returns An std::u16string where each character represents a part of the input; trailing zeros and decimals are omitted from floating-point conversions
             */
            template <class type> static std::u16string to_u16string(const type &input) {
                const std::string str = bengine::string_helper::to_string<type>(input);
                return {str.begin(), str.end()};
            }
            /** Convert an arithmetic data type to an std::u32string making use of the bengine::string_helper::to_string() function
             * \tparam type An arithmetic data type to convert from
             * \param input The arithmetic value to convert into an std::u32string
             * \returns An std::u32string where each character represents a part of the input; trailing zeros and decimals are omitted from floating-point conversions
             */
            template <class type> static std::u32string to_u32string(const type &input) {
                const std::string str = bengine::string_helper::to_string<type>(input);
                return {str.begin(), str.end()};
            }
            /** Convert an arithmetic data type to an std::wstring making use of the bengine::string_helper::to_string() function
             * \tparam type An arithmetic data type to convert from
             * \param input The arithmetic value to convert into an std::wstring
             * \returns An std::wstring where each character represents a part of the input; trailing zeros and decimals are omitted from floating-point conversions
             */
            template <class type> static std::wstring to_wstring(const type &input) {
                const std::string str = bengine::string_helper::to_string<type>(input);
                return {str.begin(), str.end()};
            }
    };
    /** Convert an std::string to an std::u16string
     * \param input The std::string to convert from
     * \returns An std::u16string derived from the inputted std::string
     */
    template <> std::u16string bengine::string_helper::to_u16string<std::string>(const std::string &input) {
        return {input.begin(), input.end()};
    }
    /** Convert an std::string to an std::u32string
     * \param input The std::string to convert from
     * \returns An std::u32string derived from the inputted std::string
     */
    template <> std::u32string bengine::string_helper::to_u32string<std::string>(const std::string &input) {
        return {input.begin(), input.end()};
    }
    /** Convert an std::string to an std::wstring
     * \param input The std::string to convert from
     * \returns An std::wstring derived from the inputted std::string
     */
    template <> std::wstring bengine::string_helper::to_wstring<std::string>(const std::string &input) {
        return {input.begin(), input.end()};
    }

    // \brief A class containing search algorithm functions
    class search_algorithms {
        public:
            // \brief bengine::search_algorithms constructor
            search_algorithms() {}
            // \brief bengine::search_algorithms deconstructor
            ~search_algorithms() {}

            /** Get the index of a target element within an unsorted/sorted list (note that it is recommended to use binary search for sorted lists)
             * \tparam Type Any datatype/class; must have equality operators available
             * \param list The list to search through
             * \param target The element to locate
             * \param lower_bounds The lowest index within the list to search from (can be used to search a particular section of a larger list)
             * \param upper_bounds the highest index within the list to search to (can be used to search a particular section of a larger list)
             * \returns An std::optional object that either contains the index of the target element if found, or will be an std::nullopt if the target is not present in the list between the inputted bounds
             */
            template <class type> static std::optional<std::size_t> linear_search(const std::vector<type> &list, const type &target, const std::size_t &lower_bounds = 0, const std::size_t &upper_bounds = __UINT64_MAX__) {
                if (list.size() == 0) {
                    return std::nullopt;
                }

                const std::size_t start = lower_bounds >= list.size() ? list.size() - 1 : lower_bounds;
                const std::size_t end = upper_bounds >= list.size() ? list.size() - 1 : upper_bounds;
                for (std::size_t index = start; index <= end; index++) {
                    if (list.at(index) == target) {
                        return index;
                    }
                }
                return std::nullopt;
            }

            /** Get the index of a target element within a sorted list
             * \tparam Type Any datatype/class; must have relational and equality operators available
             * \param list The sorted list to search through
             * \param target The element to locate
             * \param lower_bounds The lowest index within the list to search from (can be used to search a particular section of a larger list)
             * \param upper_bounds the highest index within the list to search to (can be used to search a particular section of a larger list)
             * \returns An std::optional object that either contains the index of the target element if found, or will be an std::nullopt if the target is not present in the list between the inputted bounds
             */
            template <class type> static std::optional<std::size_t> binary_search(const std::vector<type> &list, const type &target, const std::size_t &lower_bounds = 0, const std::size_t &upper_bounds = __UINT64_MAX__) {
                if (list.size() == 0) {
                    return std::nullopt;
                }

                std::size_t start = lower_bounds > list.size() ? list.size() - 1 : lower_bounds;
                std::size_t end = upper_bounds > list.size() ? list.size() - 1 : upper_bounds;
                while (start <= end) {
                    std::size_t index = start + (end - start) / 2;
                    if (list.at(index) == target) {
                        return index;
                    }
                    if (list.at(index) < target) {
                        start = index + 1;
                    } else {
                        end = index - 1;
                    }
                }
                return std::nullopt;
            }
    };

    // \brief A class containing functions that help with transforming matrices (like rotations and flips)
    class matrix_helper {
        public:
            // \brief bengine::matrix_helper constructor
            matrix_helper() {}
            // \brief bengine::matrix_helper deconstructor
            ~matrix_helper() {}

            /** Checks whether a 2D std::vector is rectangular or not
             * \tparam type Any datatype/class (not relevant for this function)
             * \param input The 2D std::vector to check
             * \returns Whether the input is rectangular (true) or jagged (false)
             */
            template <class type> static bool is_rectangular(const std::vector<std::vector<type>> &input) {
                // If each sub-vector of the input vector is the same size as the first sub-vector, then the vector is rectangular
                for (std::size_t i = 1; i < input.size(); i++) {
                    if (input.at(0).size() != input.at(i).size()) {
                        return false;
                    }
                }
                return true;
            }

            /** Rotate any rectangular 2D std::vector (matrix) 90 degrees
             * \tparam type Any datatype/class (not relevant for this function)
             * \param matrix The input vector to be rotated
             * \param rotate_ccw Whether to rotate counter-clockwise or not
             * \param check_input Whether to verify that the input is rectangular (not jagged) or not; this function will crash if the input vector isn't rectangular
             * \returns The rotated input vector (by 90 degrees)
             */
            template <class type> static std::vector<std::vector<type>> rotate_matrix(const std::vector<std::vector<type>> &matrix, const bool &rotate_ccw, const bool &check_input = false) {
                if (check_input && bengine::matrix_helper::is_rectangular(matrix)) {
                    return matrix;
                }
                std::vector<std::vector<type>> output;

                if (rotate_ccw) {
                    for (std::size_t col = matrix.at(0).size(); col--;) {
                        output.emplace_back();
                        for (std::size_t row = 0; row < matrix.size(); row++) {
                            output[matrix.at(0).size() - 1 - col].emplace_back(matrix.at(row).at(col));
                        }
                    }
                    return output;
                }
                for (std::size_t col = 0; col < matrix.at(0).size(); col++) {
                    output.emplace_back();
                    for (std::size_t row = matrix.size(); row--;) {
                        output[col].emplace_back(matrix.at(row).at(col));
                    }
                }
                return output;
            }

            /** Rotate any rectangular 2D std::vector (matrix) 90 degrees any amount of times
             * \tparam type Any datatype/class (not relevant for this function)
             * \param matrix The input vector to be rotated
             * \param rotations The amount of times to rotate the matrix; positive value for counter-clockwise and negative value for clockwise
             * \param check_input Whether to verify that the input is rectangular (not jagged) or not; this function will crash if the input vector isn't rectangular
             * \returns The rotated input vector (by 90 * rotations degrees)
             */
            template <class type> static std::vector<std::vector<type>> rotate_matrix(const std::vector<std::vector<type>> &matrix, const int &rotations, const bool &check_input = false) {
                switch (rotations % 4) {
                    default:
                    case 0:
                        return matrix;
                    case 3:
                    case -1:
                        return bengine::matrix_helper::rotate_matrix<type>(matrix, false, check_input);
                    case 1:
                    case -3:
                        return bengine::matrix_helper::rotate_matrix<type>(matrix, true, check_input);
                    case 2:
                    case -2:
                        return bengine::matrix_helper::rotate_matrix<type>(bengine::matrix_helper::rotate_matrix<type>(matrix, true, check_input), true, check_input);
                }
                return matrix;
            }

            /** Flip any rectangular 2D std::vector (matrix)
             * \tparam type Any datatype/class (not relevant for this function)
             * \param matrix The inputted vector to be flipped
             * \param flip_vertically Whether to flip vertically (true) or horizontally (false)
             * \param check_input Whether to verify that the input is rectangular (not jagged) or not; this function will crash if the input vector isn't rectangular
             * \returns The flipped input vector
             */
            template <class type> static std::vector<std::vector<type>> flip_matrix(const std::vector<std::vector<type>> &matrix, const bool &flip_vertically = true, const bool &check_input = false) {
                if (check_input && bengine::matrix_helper::is_rectangular(matrix)) {
                    return matrix;
                }
                std::vector<std::vector<type>> output;

                if (flip_vertically) {
                    for (std::size_t row = matrix.size(); row--;) {
                        output.emplace_back();
                        for (std::size_t col = 0; col < matrix.at(0).size(); col++) {
                            output[matrix.size() - 1 - row].emplace_back(matrix.at(row).at(col));
                        }
                    }
                    return output;
                }
                for (std::size_t row = 0; row < matrix.size(); row++) {
                    output.emplace_back();
                    for (std::size_t col = matrix.at(0).size(); col--;) {
                        output[row].emplace_back(matrix.at(row).at(col));
                    }
                }
                return output;
            }

            /** Flip any rectangular 2D std::vector (matrix) any amount of times
             * \tparam type Any datatype/class (not relevant for this function)
             * \param matrix The inputted vector to be flipped
             * \param flips The amount of times to flip the matrixl; positive values to flip vertically and negative values to flip horizontally
             * \param check_input Whether to verify that the input is rectangular (not jagged) or not; this function will crash if the input vector isn't rectangular
             * \returns The flipped input vector
             */
            template <class type> static std::vector<std::vector<type>> flip_matrix(const std::vector<std::vector<type>> &matrix, const int &flips, const bool &check_input = false) {
                switch (flips % 2) {
                    default:
                    case 0:
                        return matrix;
                    case 1:
                        return bengine::matrix_helper::flip_matrix<type>(matrix, true, check_input);
                    case -1:
                        return bengine::matrix_helper::flip_matrix<type>(matrix, false, check_input);
                }
                return matrix;
            }
    };

    // \brief A class containing useful general mathmatical functions (range mapping/clamping/etc, angle assistance, lcm, gcf, etc)
    class math_helper {
        public:
            // \brief bengine::math_helper constructor
            math_helper() {}
            // \brief bengine::math_helper deconstructor
            ~math_helper() {}

            /** Maps a value within a range onto a different range to maintain relative value (with static type conversion built-in)
             * \tparam input_type An arithmetic data type for the input value amd range
             * \tparam output_type An arithmetic data type for the output value and range
             * \param input_value Value to be mapped from within the input range
             * \param input_range_minimum Minimum value for the starting range
             * \param input_range_maximum Maximum value for the starting range
             * \param output_range_minimum Minimum value for the ending range
             * \param output_range_maximum Maximum value for the ending range
             * \returns A value within the ending range matching the input value's position within the starting range
             */
            template <class input_type = double, class output_type = double> static output_type map_value_to_range(const input_type &input_value, const input_type &input_range_minimum, const input_type &input_range_maximum, const output_type &output_range_minimum, const output_type &output_range_maximum) {
                static_assert(std::is_arithmetic<input_type>::value, "input_type must be an arithmetic type");
                static_assert(std::is_arithmetic<output_type>::value, "output_type must be an arithmetic type");
                return output_range_minimum + static_cast<output_type>((input_value - input_range_minimum) * (output_range_maximum - output_range_minimum) / (input_range_maximum - input_range_minimum));
            }
            /** Maps a value within a range onto a different range to maintain relative value
             * \tparam type An arithmetic data type for the input value amd range
             * \param input_value Value to be mapped from within the input range
             * \param input_range_minimum Minimum value for the starting range
             * \param input_range_maximum Maximum value for the starting range
             * \param output_range_minimum Minimum value for the ending range
             * \param output_range_maximum Maximum value for the ending range
             * \returns A value within the ending range matching the input value's position within the starting range
             */
            template <class type = double> static type map_value_to_range(const type &input_value, const type &input_range_minimum, const type &input_range_maximum, const type &output_range_minimum, const type &output_range_maximum) {
                return bengine::math_helper::map_value_to_range<type, type>(input_value, input_range_minimum, input_range_maximum, output_range_minimum, output_range_maximum);
            }

            /** Normalize a value to a range to keep retain its value, but represent it within a desired range
             * \tparam type An arithmetic data type for the input and range
             * \param input_value The value to normalize
             * \param range_minimum The smallest value making up the range (inclusive)
             * \param range_maximum The largest value making up the range (exclusive)
             * \returns A value normalized in the given range
             */
            template <class type = double> static type normalize_value_to_range(const type &input_value, const type &range_minimum, const type &range_maximum) {
                static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");
                const type width = range_maximum - range_minimum;
                const type value = input_value - range_minimum;
                return (value - (std::floor(value / width) * width)) + range_minimum;
            }
            /** Normalize a value to a range to keep retain its value, but represent it within a desired range with a minimum value of 0
             * \tparam type An arithmetic data type for the input and range
             * \param input_value The value to normalize
             * \param maximum_value The largest value making up the range (exclusive)
             * \returns A value normalized in the given range
             */
            template <class type = double> static type normalize_value_to_range(const type &input_value, const type &maximum_value) {
                return input_value - std::floor(input_value / maximum_value) * maximum_value;
            }

            /** Clamp a value to a range, either leaving it unchanged or as the minimum/maximum value of the range
             * \tparam type An arithmetic data type for the input and range
             * \param input_value The value to normalize
             * \param range_minimum The smallest value making up the range (inclusive)
             * \param range_maximum The largest value making up the range (inclusive)
             * \returns A value clamped to the given range
             */
            template <class type = double> static type clamp_value_to_range(const type &input_value, const type &range_minimum, const type &range_maximum) {
                static_assert(std::is_arithmetic<type>::value, "type must be an arithmetic type");
                return input_value < range_minimum ? range_minimum : (input_value > range_maximum ? range_maximum : input_value);
            }
            /** Clamp a value to a range with a minimum value of 0
             * \tparam type An arithmetic data type for the input and range
             * \param input_value The value to normalize
             * \param maximum_value The largest value making up the range (inclusive)
             * \returns A value clamped to the given range (0 as a minimum value)
             */
            template <class type = double> static type clamp_value_to_range(const type &input_value, const type &maximum_value) {
                return bengine::math_helper::clamp_value_to_range(input_value, 0, maximum_value);
            }

            /** Convert an angle from degrees to radians
             * \param angle An angle in degrees
             * \returns An angle in radians
             */
            static double degrees_to_radians(const double &angle) {
                return angle * U_PI_180;
            }
            /** Convert an angle from radians to degrees
             * \param angle An angle in radians
             * \returns An angle in degrees
             */
            static double radians_to_degrees(const double &angle) {
                return angle * U_180_PI;
            }
            /** Convert an angle from either degrees to radians or radians to degrees
             * \param angle An angle in either radians or degrees
             * \param convert_to_radians Whether to convert to radians or degrees
             * \returns An angle measured with the specified unit
             */
            static double convert_angle(const double &angle, const bool &convert_to_radians = true) {
                return convert_to_radians ? bengine::math_helper::degrees_to_radians(angle) : bengine::math_helper::radians_to_degrees(angle);
            }

            /** Normalize an angle in degrees to be along the interval [0, 360)
             * \param angle An angle in degrees
             * \returns The same angle, but along the interval [0, 360)
             */
            static double normalize_degree_angle(const double &angle) {
                return bengine::math_helper::normalize_value_to_range<double>(angle, 360.0);
            }
            /** Normalize an angle in radians to be along the interval [0, 2pi)
             * \param angle An angle in radians
             * \returns The same angle, but along the interval [0, 2pi)
             */
            static double normalize_radian_angle(const double &angle) {
                return bengine::math_helper::normalize_value_to_range<double>(angle, C_2PI);
            }
            /** Normalize an angle to be in the range of either [0, 2 * pi) or [0, 360) depending on the unit inputted
             * \param angle An angle in either radians or degrees
             * \param use_radians Whether to use radian measures (true) or degree measures (false)
             * \returns The same angle, but along either the interval [0, 2pi) or [0, 360)
             */
            static double normalize_angle(const double &angle, const bool &use_radians) {
                return use_radians ? bengine::math_helper::normalize_radian_angle(angle) : bengine::math_helper::normalize_degree_angle(angle);
            }

            /** Find the greatest common divisor/factor of two numbers
             * \tparam type An integral data type
             * \param num1 The first number to find the gcd of
             * \param num2 The second number to find the gcd of
             * \returns The greatest common divisor of the two inputted numbers (or 1 if either num1 or num2 are 0)
             */
            template <class type = int> static type greatest_common_divisor(const type &num1, const type &num2) {
                static_assert(std::is_integral<type>::value, "type must be an integral type");
                if (num1 == 0 || num2 == 0) {
                    return 1;
                }
                type a = num1, b = num2;

                while (a > 0 && b > 0) {
                    if (a > b) {
                        a %= b;
                    } else {
                        b %= a;
                    }
                }
                return a == 0 ? b : a;
            }
            /** Find the least common multiple of two numbers
             * \tparam type An integral data type
             * \param num1 The first number to find the lcm of
             * \param num2 The second number to find the lcm of
             * \returns The least common multiple of the two inputted numbers
             */
            template <class type = int> static type least_common_multiple(const type &num1, const type &num2) {
                static_assert(std::is_integral<type>::value, "type must be an integral type");
                return num1 / bengine::math_helper::greatest_common_divisor<type>(num1, num2) * num2;
            }
    };

    class kinematics_helper {
        private:
            // \brief The gravitational constant mainly used when looking at how the vertical motion of a projectile acts
            static double gravitational_constant;
        
        public:
            static double get_gravitational_constant() {
                return bengine::kinematics_helper::gravitational_constant;
            }
            static void set_gravitational_constant(const double &constant) {
                bengine::kinematics_helper::gravitational_constant = constant;
            }

            /** Calculate how long a projecticle will remain airborne
             * 
             */
            static double air_time(const double &magnitude, const double &angle, const double &y_difference) {
                const double y_velocity = magnitude * std::sin(angle);
                const double root = std::sqrt(y_velocity * y_velocity - 2 * bengine::kinematics_helper::gravitational_constant * y_difference);    // based off of a portion of the quadratic formula sqrt(b^2 - 4*a*c); b = vertical velocity, a = gravitational constant / 2, c = difference between y2 & y1 (dy)
                return std::fmax(root - y_velocity, -root - y_velocity) / -bengine::kinematics_helper::gravitational_constant;                     // return the larger solution to the quadratic formula b/c the smaller one will always be negative
            }

            static double peak_time(const double &magnitude, const double &angle) {
                return magnitude * std::sin(angle) / bengine::kinematics_helper::gravitational_constant;    // based off of x = -b/(2*a); x = time, b = vertical velocity, a = gravitational constant
            }

            static double peak_height(const double &magnitude, const double &angle, const double &starting_height) {
                const double peak = bengine::kinematics_helper::peak_time(magnitude, angle);
                return starting_height + magnitude * std::sin(angle) * peak - 0.5 * bengine::kinematics_helper::gravitational_constant * peak * peak;    // max height is achieved by substituting the time at which the projectile reaches its peak into the kinematic equation y = y0 + vy*t - 0.5*g*t^2
            }

            static double launch_angle(const double &magnitude, const double &x_difference, const double &y_difference, const bool &minimize_peak = true) {
                const double root = std::sqrt(magnitude * magnitude * magnitude * magnitude - bengine::kinematics_helper::gravitational_constant * (bengine::kinematics_helper::gravitational_constant * x_difference * x_difference + 2 * y_difference * magnitude * magnitude));
                const double angle_1 = std::atan2(magnitude * magnitude + root, bengine::kinematics_helper::gravitational_constant * x_difference);
                const double angle_2 = std::atan2(magnitude * magnitude - root, bengine::kinematics_helper::gravitational_constant * x_difference);
                return minimize_peak ? std::fmin(bengine::kinematics_helper::peak_height(magnitude, angle_1, 0), bengine::kinematics_helper::peak_height(magnitude, angle_2, 0)) : std::fmax(bengine::kinematics_helper::peak_height(magnitude, angle_1, 0), bengine::kinematics_helper::peak_height(magnitude, angle_2, 0));    // the peak heights of each angle are compared to determine which angle should be returned
            }

            static std::pair<double, double> landing_vector(const double &magnitude, const double &angle, const double &y_difference) {
                return {magnitude * std::cos(angle), magnitude * std::sin(angle) - bengine::kinematics_helper::gravitational_constant * bengine::kinematics_helper::air_time(magnitude, angle, y_difference)};    // the y-component of this vector is computed using the kinematic equation v = v0 + y*t; v = landing vertical velocity, v0 = initial vertical velocity, y = difference between y2 & y1 (dy), t = time for projectile to land
            }
    };
    double bengine::kinematics_helper::gravitational_constant = 9.0665;
}

#endif // BENGINE_HELPERS_hpp
