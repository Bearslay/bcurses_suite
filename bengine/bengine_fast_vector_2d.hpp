#ifndef BENGINE_FAST_VECTOR_2D_hpp
#define BENGINE_FAST_VECTOR_2D_hpp

#include <vector>

#include "bengine_helpers.hpp"

namespace bengine {
    template <class type = double> class fast_vector_2d {
        public:
            enum class relation : unsigned char {
                DEFAULT = 0,
                MAGNITUDE = 1,
                XY_ANGLE = 2,
                X_COMPONENT = 3,
                Y_COMPONENT = 4
            };

        private:
            type x_comp = 1;
            type y_comp = 0;
            double angle = 0;
            type magnitude = 1;

            static bengine::fast_vector_2d<type>::relation relation_metric;
            static bengine::fast_vector_2d<type> reference_vector;
            // Allow for the suppression of updates that occur when setting/adjusting aspects of all vectors mainly in case of a massive wave of reassignments or something that would be hindered by extra mathmatical operations
            static bool suppress_updates;

        public:
            fast_vector_2d() {
                static_assert(std::is_arithmetic<type>::value, "Template type \"type\" must be an arithmetic type (int, long, float, double, etc)");
            }
            fast_vector_2d(const type &x_comp, const type &y_comp) {
                static_assert(std::is_arithmetic<type>::value, "Template type \"type\" must be an arithmetic type (int, long, float, double, etc)");
                this->set_x_comp(x_comp);
                this->set_y_comp(y_comp);
                this->update_polar_values();
            }
            fast_vector_2d(const type &magnitude, const double &angle, const bool &use_radians) {
                static_assert(std::is_arithmetic<type>::value, "Template type \"type\" must be an arithmetic type (int, long, float, double, etc)");
                this->set_magnitude(magnitude);
                this->set_angle(angle, use_radians);
                this->update_cartesian_values();
            }
            fast_vector_2d(const bengine::fast_vector_2d<type> &vector) {
                *this = vector;
            }

            void update_cartesian_values() {
                if (this->is_suppressing_updates()) {
                    return;
                }
                if (std::is_integral<type>::value) {
                    this->x_comp = std::round(this->magnitude * std::cos(this->angle));
                    this->y_comp = std::round(this->magnitude * std::sin(this->angle));
                    return;
                }
                this->x_comp = this->magnitude * std::cos(this->angle);
                this->y_comp = this->magnitude * std::sin(this->angle);
            }
            void update_polar_values() {
                if (this->is_suppressing_updates()) {
                    return;
                }
                this->magnitude = std::is_integral<type>::value ? std::round(std::sqrt(this->x_comp * this->x_comp + this->y_comp * this->y_comp)) : std::sqrt(this->x_comp * this->x_comp + this->y_comp * this->y_comp);
                this->angle = std::atan2(this->y_comp, this->x_comp);
            }

            static bengine::fast_vector_2d<type>::relation get_relation_metric() {
                return bengine::fast_vector_2d<type>::relation_metric;
            }
            static void set_relation_metric(const bengine::fast_vector_2d<type>::relation &metric) {
                bengine::fast_vector_2d<type>::relation_metric = (static_cast<unsigned char>(metric) > 4 || static_cast<unsigned char>(metric) < 0) ? bengine::fast_vector_2d<type>::relation::DEFAULT : metric;
            }
            static void reset_relation_metric() {
                bengine::fast_vector_2d<type>::relation_metric = bengine::fast_vector_2d<type>::relation::DEFAULT;
            }
            static bengine::fast_vector_2d<type> get_reference_vector() {
                return bengine::fast_vector_2d<type>::reference_vector;
            }
            static void set_reference_vector(const bengine::fast_vector_2d<type> &vector) {
                bengine::fast_vector_2d<type>::reference_vector = vector;
            }
            static void set_reference_vector(const type &x_comp, const type &y_comp) {
                bengine::fast_vector_2d<type>::set_reference_vector(bengine::fast_vector_2d<type>(x_comp, y_comp));
            }
            static void reset_reference_vector() {
                bengine::fast_vector_2d<type>::reference_vector = bengine::fast_vector_2d<type>(1, 0);
            }
            static bool is_suppressing_updates() {
                return bengine::fast_vector_2d<type>::suppress_updates;
            }
            static void set_update_suppression(const bool &suppress_updates) {
                bengine::fast_vector_2d<type>::suppress_updates = suppress_updates;
            }
            static void toggle_update_suppression() {
                bengine::fast_vector_2d<type>::suppress_updates = !bengine::fast_vector_2d<type>::suppress_updates;
            }

            type get_x_comp() const {
                return this->x_comp;
            }
            type get_y_comp() const {
                return this->y_comp;
            }
            void set_x_comp(const type &x_comp) {
                this->x_comp = x_comp;
                this->update_polar_values();
            }
            void set_y_comp(const type &y_comp) {
                this->y_comp = y_comp;
                this->update_polar_values();
            }
            void adjust_x_comp(const type &x_comp) {
                this->set_x_comp(this->x_comp + x_comp);
            }
            void adjust_y_comp(const type &y_comp) {
                this->set_y_comp(this->y_comp + y_comp);
            }

            type get_magnitude() const {
                return this->magnitude;
            }
            double get_angle(const bool &use_radians) const {
                return use_radians ? this->angle : this->angle * U_180_PI;
            }
            void set_magnitude(const type &magnitude) {
                this->magnitude = magnitude;
                this->update_cartesian_values();
            }
            void set_angle(const double &angle, const bool &use_radians) {
                this->angle = bengine::math_helper::normalize_angle(angle, use_radians);
                this->update_cartesian_values();
            }
            template <class arithmetic_type> void adjust_magnitude(const arithmetic_type &amount) {
                this->set_magnitude(this->magnitude + amount);
            }
            void adjust_angle(const double &amount, const bool &use_radians) {
                this->set_angle(this->get_angle(use_radians) + amount, use_radians);
            }

            void swap_x_and_y() {
                const type temp = this->x_comp;
                this->x_comp = this->y_comp;
                this->y_comp = temp;
                this->update_polar_values();
            }
            void invert_x_comp() {
                this->x_comp = -this->x_comp;
                this->update_polar_values();
            }
            void invert_y_comp() {
                this->y_comp = -this->y_comp;
                this->update_polar_values();
            }
            void rotate_180_degrees() {
                this->x_comp = -this->x_comp;
                this->y_comp = -this->y_comp;
                this->adjust_angle(M_PI, true);
            }

            bengine::fast_vector_2d<type>& operator=(const bengine::fast_vector_2d<type> &rhs) {
                this->x_comp = rhs.get_x_comp();
                this->y_comp = rhs.get_y_comp();
                this->magnitude = rhs.get_magnitude();
                this->angle = rhs.get_angle(true);
                return *this;
            }
            bengine::fast_vector_2d<type> operator!() const {
                return bengine::fast_vector_2d<type>(-this->x_comp, -this->y_comp);
            }
            std::string to_string(const bool &as_components = true, const bool &use_radians = true) const {
                if (as_components) {
                    return "(" + std::to_string(this->x_comp) + ", " + std::to_string(this->y_comp) + ")";
                }
                return "(" + std::to_string(this->magnitude) + ", " + std::to_string(this->get_angle(use_radians)) + ")";
            }
            std::vector<type> to_vector() const {
                return {this->x_comp, this->y_comp};
            }
            bengine::fast_vector_2d<type> to_unit_vector() const {
                return bengine::fast_vector_2d<type>(1, this->angle, true);
            }

            bool is_equal_to(const bengine::fast_vector_2d<type> &other, const bengine::fast_vector_2d<type>::relation &metric = bengine::fast_vector_2d<type>::relation_metric) const {
                switch (metric) {
                    default:
                    case bengine::fast_vector_2d<type>::relation::DEFAULT:
                        return this->x_comp == other.get_x_comp() && this->y_comp == other.get_y_comp();
                    case bengine::fast_vector_2d<type>::relation::MAGNITUDE:
                        return this->magnitude == other.get_magnitude();
                    case bengine::fast_vector_2d<type>::relation::XY_ANGLE:
                        return this->angle == other.get_angle(true);
                    case bengine::fast_vector_2d<type>::relation::X_COMPONENT:
                        return this->x_comp == other.get_x_comp();
                    case bengine::fast_vector_2d<type>::relation::Y_COMPONENT:
                        return this->y_comp == other.get_y_comp();
                }
            }
            bool is_not_equal_to(const bengine::fast_vector_2d<type> &other, const bengine::fast_vector_2d<type>::relation &metric = bengine::fast_vector_2d<type>::relation_metric) const {
                return !this->is_equal_to(other, metric);
            }
            bool is_less_than(const bengine::fast_vector_2d<type> &other, const bengine::fast_vector_2d<type>::relation &metric = bengine::fast_vector_2d<type>::relation_metric) const {
                switch (metric) {
                    default:
                    case bengine::fast_vector_2d<type>::relation::DEFAULT:
                    case bengine::fast_vector_2d<type>::relation::MAGNITUDE:
                        return this->magnitude < other.get_magnitude();
                    case bengine::fast_vector_2d<type>::relation::XY_ANGLE:
                        return this->angle < other.get_angle(true);
                    case bengine::fast_vector_2d<type>::relation::X_COMPONENT:
                        return this->x_comp < other.get_x_comp();
                    case bengine::fast_vector_2d<type>::relation::Y_COMPONENT:
                        return this->y_comp < other.get_y_comp();
                }
            }
            bool is_greater_than(const bengine::fast_vector_2d<type> &other, const bengine::fast_vector_2d<type>::relation &metric = bengine::fast_vector_2d<type>::relation_metric) const {
                return other.is_less_than(*this, metric);
            }
            bool is_less_than_or_equal_to(const bengine::fast_vector_2d<type> &other, const bengine::fast_vector_2d<type>::relation &metric = bengine::fast_vector_2d<type>::relation_metric) const {
                return !this->is_greater_than(other, metric);
            }
            bool is_greater_than_or_equal_to(const bengine::fast_vector_2d<type> &other, const bengine::fast_vector_2d<type>::relation &metric = bengine::fast_vector_2d<type>::relation_metric) const {
                return !this->is_less_than(other, metric);
            }

            bool operator==(const bengine::fast_vector_2d<type> &rhs) const {
                return this->is_equal_to(rhs, bengine::fast_vector_2d<type>::relation_metric);
            }
            bool operator!=(const bengine::fast_vector_2d<type> &rhs) const {
                return this->is_not_equal_to(rhs, bengine::fast_vector_2d<type>::relation_metric);
            }
            bool operator<(const bengine::fast_vector_2d<type> &rhs) const {
                return this->is_less_than(rhs, bengine::fast_vector_2d<type>::relation_metric);
            }
            bool operator>(const bengine::fast_vector_2d<type> &rhs) const {
                return this->is_greater_than(rhs, bengine::fast_vector_2d<type>::relation_metric);
            }
            bool operator<=(const bengine::fast_vector_2d<type> &rhs) const {
                return this->is_less_than_or_equal_to(rhs, bengine::fast_vector_2d<type>::relation_metric);
            }
            bool operator>=(const bengine::fast_vector_2d<type> &rhs) const {
                return this->is_greater_than_or_equal_to(rhs, bengine::fast_vector_2d<type>::relation_metric);
            }

            bengine::fast_vector_2d<type>& operator+=(const bengine::fast_vector_2d<type> &rhs) {
                this->x_comp += rhs.get_x_comp();
                this->y_comp += rhs.get_y_comp();
                this->update_polar_values();
                return *this;
            }
            bengine::fast_vector_2d<type>& operator-=(const bengine::fast_vector_2d<type> &rhs) {
                this->x_comp -= rhs.get_x_comp();
                this->y_comp -= rhs.get_y_comp();
                this->update_polar_values();
                return *this;
            }
            bengine::fast_vector_2d<type>& operator*=(const double &rhs) {
                this->x_comp *= rhs;
                this->y_comp *= rhs;
                this->update_polar_values();
                return *this;
            }
            bengine::fast_vector_2d<type>& operator/=(const double &rhs) {
                this->x_comp /= rhs;
                this->y_comp /= rhs;
                this->update_polar_values();
                return *this;
            }
            bengine::fast_vector_2d<type>& operator%=(const double &rhs) {
                this->x_comp = std::fmod(this->x_comp, rhs);
                this->y_comp = std::fmod(this->y_comp, rhs);
                this->update_polar_values();
                return *this;
            }
            bengine::fast_vector_2d<type> operator+(const bengine::fast_vector_2d<type> &rhs) const {
                return bengine::fast_vector_2d<type>(this->x_comp + rhs.get_x_comp(), this->y_comp + rhs.get_y_comp());
            }
            bengine::fast_vector_2d<type> operator-(const bengine::fast_vector_2d<type> &rhs) const {
                return bengine::fast_vector_2d<type>(this->x_comp - rhs.get_x_comp(), this->y_comp - rhs.get_y_comp());
            }
            bengine::fast_vector_2d<type> operator*(const double &rhs) const {
                return bengine::fast_vector_2d<type>(this->x_comp * rhs, this->y_comp * rhs);
            }
            bengine::fast_vector_2d<type> operator/(const double &rhs) const {
                return bengine::fast_vector_2d<type>(this->x_comp / rhs, this->y_comp / rhs);
            }
            bengine::fast_vector_2d<type> operator%(const double &rhs) const {
                return bengine::fast_vector_2d<type>(std::fmod(this->x_comp, rhs), std::fmod(this->y_comp, rhs));
            }

            bengine::fast_vector_2d<type> scalar_product(const double &scalar) const {
                return bengine::fast_vector_2d<type>(this->x_comp * scalar, this->y_comp * scalar);
            }
            type dot_product(const bengine::fast_vector_2d<type>& other) const {
                return this->x_comp * other.get_x_comp() + this->y_comp * other.get_y_comp();
            }
            type cross_product(const bengine::fast_vector_2d<type>& other) const {
                return this->x_comp * other.get_y_comp() - this->y_comp * other.get_x_comp();
            }
    };
    template <class type> typename bengine::fast_vector_2d<type>::relation bengine::fast_vector_2d<type>::relation_metric = bengine::fast_vector_2d<type>::relation::DEFAULT;
    template <class type> typename bengine::fast_vector_2d<type> bengine::fast_vector_2d<type>::reference_vector = bengine::fast_vector_2d<type>(1, 0);
    template <class type> bool bengine::fast_vector_2d<type>::suppress_updates = false;
}

#endif // BENGINE_FAST_VECTOR_2D_hpp
