#ifndef BENGINE_COLLIDERS_hpp
#define BENGINE_COLLIDERS_hpp

#include <cmath>
#include <optional>
#include <vector>

#include "bengine_helpers.hpp"
#include "bengine_coordinate_2d.hpp"

namespace bengine {
    class basic_collider_2d {
        public:
            enum class fix_mode : const unsigned char {
                MOVE_SELF = 0,
                MOVE_OTHER = 1,
                MOVE_BOTH = 2
            };

        protected:
            bengine::coordinate_2d<double> position = bengine::coordinate_2d<double>(0, 0);
            double width_2 = 0;
            double height_2 = 0;

        public:
            basic_collider_2d() {}
            basic_collider_2d(const double &x, const double &y, const double &width, const double &height) {
                this->set_x_pos(x);
                this->set_y_pos(y);
                this->set_width(width);
                this->set_height(height);
            }
            basic_collider_2d(const bengine::basic_collider_2d &rhs) {
                *this = rhs;
            }
            ~basic_collider_2d() {}

            bengine::basic_collider_2d& operator=(const bengine::basic_collider_2d &rhs) {
                this->position = bengine::coordinate_2d<double>(rhs.get_x_pos(), rhs.get_y_pos());
                this->width_2 = rhs.get_width_2();
                this->height_2 = rhs.get_height_2();
                return *this;
            }

            /** Export the collider as a string
             * @param verbose Whether to label each value (true) or not (false)
             * @returns An std::string representing the collider
             */
            std::string to_string(const bool &verbose = true) const {
                if (verbose) {
                    return "{Origin: " + this->position.to_string() + ", Width: " + bengine::string_helper::to_string<double>(this->width_2 + this->width_2) + ", Height: " + bengine::string_helper::to_string<double>(this->height_2 + this->height_2) + "}";
                }
                return "{" + bengine::string_helper::to_string<double>(this->position.get_x_pos()) + ", " + bengine::string_helper::to_string<double>(this->position.get_y_pos()) + ", " + bengine::string_helper::to_string<double>(this->height_2 + this->height_2) + ", " + bengine::string_helper::to_string<double>(this->height_2 + this->height_2) + "}";
            }

            double get_x_pos() const {
                return this->position.get_x_pos();
            }
            double get_y_pos() const {
                return this->position.get_y_pos();
            }
            double get_width() const {
                return this->width_2 + this->width_2;
            }
            double get_height() const {
                return this->height_2 + this->height_2;
            }
            double get_width_2() const {
                return this->width_2;
            }
            double get_height_2() const {
                return this->height_2;
            }

            double get_left_x() const {
                return this->position.get_x_pos() - this->width_2;
            }
            double get_right_x() const {
                return this->position.get_x_pos() + this->width_2;
            }
            double get_top_y() const {
                return this->position.get_y_pos() + this->height_2;
            }
            double get_bottom_y() const {
                return this->position.get_y_pos() - this->height_2;
            }

            void set_x_pos(const double &x_pos) {
                this->position.set_x_pos(x_pos);
            }
            void set_y_pos(const double &y_pos) {
                this->position.set_y_pos(y_pos);
            }
            void set_width(const double &width) {
                this->set_width_2(width / 2);
            }
            void set_height(const double &height) {
                this->set_height_2(height / 2);
            }
            void set_width_2(const double &width_2) {
                if (width_2 >= 0) {
                    this->width_2 = width_2;
                    return;
                }
                this->position.translate_horizontally(width_2);
                this->width_2 = -width_2;
            }
            void set_height_2(const double &height_2) {
                if (height_2 >= 0) {
                    this->height_2 = height_2;
                    return;
                }
                this->position.translate_vertically(height_2);
                this->height_2 = -height_2;
            }

            void translate_horizontally(const double &amount) {
                this->position.translate_horizontally(amount);
            }
            void translate_vertically(const double &amount) {
                this->position.translate_vertically(amount);
            }

            bool detect_collision(const bengine::basic_collider_2d &other) const {
                return !(this->get_right_x() < other.get_left_x() || this->get_left_x() > other.get_right_x() || this->get_top_y() < other.get_bottom_y() || this->get_bottom_y() > other.get_top_y());
            }

            /** Fix the collision between two colliders so that they are no longer colliding
             * \param other The other bengine::basic_collider_2d that is being collided with
             * \param fix_mode The way that the collision should be fixed (mainly with the movement of either colliders)
             * \param do_initial_collision_check Whether to do a check for collision or not; this is recommended if no other check is performed prior to calling this function, but is optional otherwise
             * \returns False in the case that do_initial_collision_check is true but no collision was detected, true in all other cases
             */
            bool fix_collision(bengine::basic_collider_2d &other, const bengine::basic_collider_2d::fix_mode &fix_mode, const bool &do_initial_collision_check = true) {
                if (do_initial_collision_check) {
                    if (!this->detect_collision(other)) {
                        return false;
                    }
                }

                const double top_overlap = this->get_top_y() - other.get_bottom_y();
                const double left_overlap = other.get_right_x() - this->get_left_x();
                const double bottom_overlap = other.get_top_y() - this->get_bottom_y();

                double smallest_overlap = this->get_right_x() - other.get_left_x();
                double x_vector = -smallest_overlap, y_vector = 0;

                if (top_overlap < smallest_overlap) {
                    smallest_overlap = top_overlap;
                    x_vector = 0;
                    y_vector = -smallest_overlap;
                }
                if (left_overlap < smallest_overlap) {
                    smallest_overlap = left_overlap;
                    x_vector = smallest_overlap;
                    y_vector = 0;
                }
                if (bottom_overlap < smallest_overlap) {
                    smallest_overlap = bottom_overlap;
                    x_vector = 0;
                    y_vector = smallest_overlap;
                }

                switch (fix_mode) {
                    case bengine::basic_collider_2d::fix_mode::MOVE_SELF:
                        this->position.translate_horizontally(x_vector);
                        this->position.translate_vertically(y_vector);
                        break;
                    case bengine::basic_collider_2d::fix_mode::MOVE_OTHER:
                        other.set_x_pos(other.get_x_pos() - x_vector);
                        other.set_y_pos(other.get_y_pos() - y_vector);
                        break;
                    case bengine::basic_collider_2d::fix_mode::MOVE_BOTH:
                        this->position.translate_horizontally(x_vector / 2);
                        this->position.translate_vertically(y_vector / 2);
                        other.set_x_pos(other.get_x_pos() - x_vector / 2);
                        other.set_y_pos(other.get_y_pos() - y_vector / 2);
                        break;
                }
                return true;
            }
    };

    class hitscanner_2d {
        private:
            bengine::coordinate_2d<double> position = bengine::coordinate_2d<double>(0, 0);
            bengine::fast_vector_2d<double> vector = bengine::fast_vector_2d<double>(1, 0, true);
            bool infinite_range = false;

            std::optional<bengine::coordinate_2d<double>> do_range_check(const bengine::coordinate_2d<double> &position) const {
                if (this->has_infinite_range() || this->position.get_euclidean_distance_to(position) <= std::fabs(this->vector.get_magnitude())) {
                    return position;
                }
                return std::nullopt;
            }

        public:
            hitscanner_2d() {}
            hitscanner_2d(const double &x_pos, const double &y_pos, const double &angle, const double &range, const bool &have_infinite_range = false) {
                this->set_x_pos(x_pos);
                this->set_y_pos(y_pos);
                this->set_angle(angle);
                this->set_range(range);
                this->make_range_infinite(have_infinite_range);
            }
            hitscanner_2d(const bengine::hitscanner_2d &rhs) {
                *this = rhs;
            }
            ~hitscanner_2d() {}

            bengine::hitscanner_2d& operator=(const bengine::hitscanner_2d &rhs) {
                this->position = rhs.get_position();
                this->vector = rhs.get_vector();
                this->infinite_range = rhs.has_infinite_range();
                return *this;
            }

            double get_x_pos() const {
                return this->position.get_x_pos();
            }
            double get_y_pos() const {
                return this->position.get_y_pos();
            }
            bengine::coordinate_2d<double> get_position() const {
                return this->position;
            }
            double get_angle() const {
                return this->vector.get_angle(true);
            }
            double get_range() const {
                return this->vector.get_magnitude();
            }
            bengine::fast_vector_2d<double> get_vector() const {
                return this->vector;
            }

            void set_x_pos(const double &x_pos) {
                this->position.set_x_pos(x_pos);
            }
            void set_y_pos(const double &y_pos) {
                this->position.set_y_pos(y_pos);
            }
            void set_position(const bengine::coordinate_2d<double> &position) {
                this->position = position;
            }
            void set_angle(const double &angle) {
                this->vector.set_angle(angle, true);
            }
            void set_range(const double &range) {
                this->vector.set_magnitude(std::fabs(range));
            }
            void set_vector(const bengine::fast_vector_2d<double> &vector) {
                this->set_angle(vector.get_angle(true));
                this->set_range(vector.get_magnitude());
            }

            bool has_infinite_range() const {
                return this->infinite_range;
            }
            void make_range_infinite(const bool &have_infinite_range) {
                this->infinite_range = have_infinite_range;
            }
            void toggle_infinite_range() {
                this->infinite_range = !this->infinite_range;
            }

            std::optional<bengine::coordinate_2d<double>> get_hit(const bengine::basic_collider_2d &collider) const {
                // Colliders are treated as solid, so a hitscanner physically placed inside of one will always hit
                if (this->get_x_pos() >= collider.get_left_x() && this->get_x_pos() <= collider.get_right_x() && this->get_y_pos() >= collider.get_top_y() && this->get_y_pos() <= collider.get_bottom_y()) {
                    return this->position;
                }
                if (this->vector.get_magnitude() == 0 && !this->has_infinite_range()) {
                    return std::nullopt;
                }

                // Some basic culling can be done for hitscanners that obviously (to a computer at least) look away from the collider
                // Annoyingly, having an angle of zero and being below the collider will not be properly culled by the obvious conditions (so another one was added at the end), but this can probably be fixed somehow
                if ((this->get_angle() <= C_PI && this->get_y_pos() > collider.get_top_y()) || (this->get_angle() >= C_PI && this->get_y_pos() < collider.get_bottom_y()) || ((this->get_angle() <= C_PI_2 || this->get_angle() >= C_3PI_2) && this->get_x_pos() > collider.get_right_x()) || ((this->get_angle() >= C_PI_2 && this->get_angle() <= C_3PI_2) && this->get_x_pos() < collider.get_left_x()) || (this->get_angle() == 0 && this->get_y_pos() < collider.get_bottom_y())) {
                    return std::nullopt;
                }

                // Angles that would produce either an undefined slope or a slope of zero are handled seperately for clarity's sake as well as being a bit faster for these specific cases
                if (this->vector.get_x_comp() == 0) {
                    return this->do_range_check(bengine::coordinate_2d<double>(this->get_x_pos(), this->get_angle() < C_PI ? collider.get_bottom_y() : collider.get_top_y()));
                } else if (this->vector.get_y_comp() == 0) {
                    return this->do_range_check(bengine::coordinate_2d<double>(this->get_angle() < C_PI_2 || this->get_angle() > C_3PI_2 ? collider.get_left_x() : collider.get_right_x(), this->get_y_pos()));
                }

                const double slope = this->vector.get_y_comp() / this->vector.get_x_comp();
                const double x_difference = this->get_angle() < C_PI_2 || this->get_angle() > C_3PI_2 ? collider.get_left_x() - this->get_x_pos() : collider.get_right_x() - this->get_x_pos();
                const double y_difference = this->get_angle() < C_PI ? collider.get_bottom_y() - this->get_y_pos() : collider.get_top_y() - this->get_y_pos();

                // works both as a guess for a y-position and then as a guess for an x-position if the first guess fails
                double guess_pos = this->get_y_pos() + slope * x_difference;
                if (guess_pos >= collider.get_bottom_y() && guess_pos <= collider.get_top_y()) {
                    return this->do_range_check(bengine::coordinate_2d<double>(this->get_x_pos() + x_difference, guess_pos));
                }
                guess_pos = this->get_x_pos() + y_difference / slope;
                if (guess_pos >= collider.get_left_x() && guess_pos <= collider.get_right_x()) {
                    return this->do_range_check(bengine::coordinate_2d<double>(guess_pos, this->get_y_pos() + y_difference));
                }
                return std::nullopt;
            }
            std::optional<bengine::coordinate_2d<double>> get_hit(const std::vector<bengine::basic_collider_2d> &colliders) const {
                std::optional<bengine::coordinate_2d<double>> output = std::nullopt;
                double output_distance = __DBL_MAX__;
                for (std::size_t current_index = 0; current_index < colliders.size(); current_index++) {
                    const std::optional<bengine::coordinate_2d<double>> scan = this->get_hit(colliders.at(current_index));
                    if (!scan.has_value()) {
                        continue;
                    }
                    const double current_distance = scan.value().get_euclidean_distance_to(this->position);
                    if (current_distance < output_distance) {
                        output_distance = current_distance;
                        output = scan;
                    }
                }
                return output;
            }
    };
}

#endif // BENGINE_COLLIDERS_hpp
