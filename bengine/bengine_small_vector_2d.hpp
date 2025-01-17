#ifndef BENGINE_SMALL_VECTOR_2D_hpp
#define BENGINE_SMALL_VECTOR_2D_hpp

#include <vector>

#include "bengine_helpers.hpp"

namespace bengine {
    template <class type = double> class small_vector_2d {
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

            static bengine::small_vector_2d<type>::relation relation_metric;
            static bengine::small_vector_2d<type> reference_vector;

        public:
            small_vector_2d() {
                static_assert(std::is_arithmetic<type>::value, "Template type \"type\" must be an arithmetic type (int, long, float, double, etc)");
            }
            small_vector_2d(const type &x_comp, const type &y_comp) {
                static_assert(std::is_arithmetic<type>::value, "Template type \"type\" must be an arithmetic type (int, long, float, double, etc)");
                this->x_comp = x_comp;
                this->y_comp = y_comp;
            }
            small_vector_2d(const type &magnitude, const double &angle, const bool &use_radians) {
                static_assert(std::is_arithmetic<type>::value, "Template type \"type\" must be an arithmetic type (int, long, float, double, etc)");
                if (use_radians) {
                    this->x_comp = magnitude * std::cos(angle);
                    this->y_comp = magnitude * std::sin(angle);
                    return;
                }
                this->x_comp = magnitude * std::cos(angle * U_180_PI);
                this->y_comp = magnitude * std::sin(angle * U_180_PI);
            }
            small_vector_2d(const bengine::small_vector_2d<type> &vector) {
                *this = vector;
            }

            static bengine::small_vector_2d<type>::relation get_relation_metric() {
                return bengine::small_vector_2d<type>::relation_metric;
            }
            static void set_relation_metric(const bengine::small_vector_2d<type>::relation &metric) {
                bengine::small_vector_2d<type>::relation_metric = (static_cast<unsigned char>(metric) > 4 || static_cast<unsigned char>(metric) < 0) ? bengine::small_vector_2d<type>::relation::DEFAULT : metric;
            }
            static void reset_relation_metric() {
                bengine::small_vector_2d<type>::relation_metric = bengine::small_vector_2d<type>::relation::DEFAULT;
            }
            static bengine::small_vector_2d<type> get_reference_vector() {
                return bengine::small_vector_2d<type>::reference_vector;
            }
            static void set_reference_vector(const bengine::small_vector_2d<type> &vector) {
                bengine::small_vector_2d<type>::reference_vector = vector;
            }
            static void set_reference_vector(const type &x_comp, const type &y_comp) {
                bengine::small_vector_2d<type>::set_reference_vector(bengine::small_vector_2d<type>(x_comp, y_comp));
            }
            static void reset_reference_vector() {
                bengine::small_vector_2d<type>::reference_vector = bengine::small_vector_2d<type>(1, 0);
            }

            type get_x_comp() const {
                return this->x_comp;
            }
            type get_y_comp() const {
                return this->y_comp;
            }
            void set_x_comp(const type &x_comp) {
                this->x_comp = x_comp;
            }
            void set_y_comp(const type &y_comp) {
                this->y_comp = y_comp;
            }
            void adjust_x_comp(const type &x_comp) {
                this->set_x_comp(this->x_comp + x_comp);
            }
            void adjust_y_comp(const type &y_comp) {
                this->set_y_comp(this->y_comp + y_comp);
            }

            template <class arithmetic_type = double> arithmetic_type get_magnitude() const {
                static_assert(std::is_arithmetic<arithmetic_type>::value, "Template type \"arithmetic_type\" must be an arithmetic type (int, long, float, double, etc)");
                return std::is_integral<arithmetic_type>::value ? std::round(std::sqrt(this->x_comp * this->x_comp + this->y_comp * this->y_comp)) : std::sqrt(this->x_comp * this->x_comp + this->y_comp * this->y_comp);
            }
            double get_angle(const bool &use_radians) const {
                return use_radians ? std::atan2(this->y_comp, this->x_comp) : std::atan2(this->y_comp, this->x_comp) * U_180_PI;
            }
            template <class arithmetic_type = double> void set_magnitude(const arithmetic_type &magnitude) {
                const double angle = this->get_angle(true);
                if (std::is_integral<type>::value) {
                    this->x_comp = std::round(magnitude * std::cos(angle));
                    this->y_comp = std::round(magnitude * std::sin(angle));
                    return;
                }
                this->x_comp = magnitude * std::cos(angle);
                this->y_comp = magnitude * std::sin(angle);
            }
            void set_angle(const double &angle, const bool &use_radians) {
                const double magnitude = this->get_magnitude<double>();
                if (use_radians) {
                    if (std::is_integral<type>::value) {
                        this->x_comp = std::round(magnitude * std::cos(angle));
                        this->y_comp = std::round(magnitude * std::sin(angle));
                        return;
                    }
                    this->x_comp = magnitude * std::cos(angle);
                    this->y_comp = magnitude * std::sin(angle);
                    return;
                }
                if (std::is_integral<type>::value) {
                    this->x_comp = std::round(magnitude * std::cos(angle * U_PI_180));
                    this->y_comp = std::round(magnitude * std::sin(angle * U_PI_180));
                    return;
                }
                this->x_comp = magnitude * std::cos(angle * U_PI_180);
                this->y_comp = magnitude * std::sin(angle * U_PI_180);
            }
            template <class arithmetic_type> void adjust_magnitude(const arithmetic_type &amount) {
                this->set_magnitude<arithmetic_type>(this->get_magnitude<arithmetic_type>() + amount);
            }
            void adjust_angle(const double &amount, const bool &use_radians) {
                this->set_angle(this->get_angle(use_radians) + amount, use_radians);
            }

            void swap_x_and_y() {
                const type temp = this->x_comp;
                this->x_comp = this->y_comp;
                this->y_comp = temp;
            }
            void invert_x_comp() {
                this->x_comp = -this->x_comp;
            }
            void invert_y_comp() {
                this->y_comp = -this->y_comp;
            }
            void rotate_180_degrees() {
                this->x_comp = -this->x_comp;
                this->y_comp = -this->y_comp;
            }

            bengine::small_vector_2d<type>& operator=(const bengine::small_vector_2d<type> &rhs) {
                this->x_comp = rhs.get_x_comp();
                this->y_comp = rhs.get_y_comp();
                return *this;
            }
            bengine::small_vector_2d<type> operator!() const {
                return bengine::small_vector_2d<type>(-this->x_comp, -this->y_comp);
            }
            std::string to_string(const bool &as_components = true, const bool &use_radians = true) const {
                if (as_components) {
                    return "(" + std::to_string(this->x_comp) + ", " + std::to_string(this->y_comp) + ")";
                }
                return "(" + std::to_string(this->get_magnitude<type>()) + ", " + std::to_string(this->get_angle(use_radians)) + ")";
            }
            std::vector<type> to_vector() const {
                return {this->x_comp, this->y_comp};
            }
            bengine::small_vector_2d<type> to_unit_vector() const {
                return bengine::small_vector_2d<type>(1, this->get_angle(true), true);
            }

            bool is_equal_to(const bengine::small_vector_2d<type> &other, const bengine::small_vector_2d<type>::relation &metric = bengine::small_vector_2d<type>::relation_metric) const {
                switch (metric) {
                    default:
                    case bengine::small_vector_2d<type>::relation::DEFAULT:
                        return this->x_comp == other.get_x_comp() && this->y_comp == other.get_y_comp();
                    case bengine::small_vector_2d<type>::relation::MAGNITUDE:
                        return this->get_magnitude<double>() == other.get_magnitude<double>();
                    case bengine::small_vector_2d<type>::relation::XY_ANGLE:
                        return this->get_angle(true) == other.get_angle(true);
                    case bengine::small_vector_2d<type>::relation::X_COMPONENT:
                        return this->x_comp == other.get_x_comp();
                    case bengine::small_vector_2d<type>::relation::Y_COMPONENT:
                        return this->y_comp == other.get_y_comp();
                }
            }
            bool is_not_equal_to(const bengine::small_vector_2d<type> &other, const bengine::small_vector_2d<type>::relation &metric = bengine::small_vector_2d<type>::relation_metric) const {
                return !this->is_equal_to(other, metric);
            }
            bool is_less_than(const bengine::small_vector_2d<type> &other, const bengine::small_vector_2d<type>::relation &metric = bengine::small_vector_2d<type>::relation_metric) const {
                switch (metric) {
                    default:
                    case bengine::small_vector_2d<type>::relation::DEFAULT:
                    case bengine::small_vector_2d<type>::relation::MAGNITUDE:
                        return this->get_magnitude<double>() < other.get_magnitude<double>();
                    case bengine::small_vector_2d<type>::relation::XY_ANGLE:
                        return this->get_angle(true) < other.get_angle(true);
                    case bengine::small_vector_2d<type>::relation::X_COMPONENT:
                        return this->x_comp < other.get_x_comp();
                    case bengine::small_vector_2d<type>::relation::Y_COMPONENT:
                        return this->y_comp < other.get_y_comp();
                }
            }
            bool is_greater_than(const bengine::small_vector_2d<type> &other, const bengine::small_vector_2d<type>::relation &metric = bengine::small_vector_2d<type>::relation_metric) const {
                return other.is_less_than(*this, metric);
            }
            bool is_less_than_or_equal_to(const bengine::small_vector_2d<type> &other, const bengine::small_vector_2d<type>::relation &metric = bengine::small_vector_2d<type>::relation_metric) const {
                return !this->is_greater_than(other, metric);
            }
            bool is_greater_than_or_equal_to(const bengine::small_vector_2d<type> &other, const bengine::small_vector_2d<type>::relation &metric = bengine::small_vector_2d<type>::relation_metric) const {
                return !this->is_less_than(other, metric);
            }

            bool operator==(const bengine::small_vector_2d<type> &rhs) const {
                return this->is_equal_to(rhs, bengine::small_vector_2d<type>::relation_metric);
            }
            bool operator!=(const bengine::small_vector_2d<type> &rhs) const {
                return this->is_not_equal_to(rhs, bengine::small_vector_2d<type>::relation_metric);
            }
            bool operator<(const bengine::small_vector_2d<type> &rhs) const {
                return this->is_less_than(rhs, bengine::small_vector_2d<type>::relation_metric);
            }
            bool operator>(const bengine::small_vector_2d<type> &rhs) const {
                return this->is_greater_than(rhs, bengine::small_vector_2d<type>::relation_metric);
            }
            bool operator<=(const bengine::small_vector_2d<type> &rhs) const {
                return this->is_less_than_or_equal_to(rhs, bengine::small_vector_2d<type>::relation_metric);
            }
            bool operator>=(const bengine::small_vector_2d<type> &rhs) const {
                return this->is_greater_than_or_equal_to(rhs, bengine::small_vector_2d<type>::relation_metric);
            }

            bengine::small_vector_2d<type>& operator+=(const bengine::small_vector_2d<type> &rhs) {
                this->x_comp += rhs.get_x_comp();
                this->y_comp += rhs.get_y_comp();
                return *this;
            }
            bengine::small_vector_2d<type>& operator-=(const bengine::small_vector_2d<type> &rhs) {
                this->x_comp -= rhs.get_x_comp();
                this->y_comp -= rhs.get_y_comp();
                return *this;
            }
            bengine::small_vector_2d<type>& operator*=(const double &rhs) {
                this->x_comp *= rhs;
                this->y_comp *= rhs;
                return *this;
            }
            bengine::small_vector_2d<type>& operator/=(const double &rhs) {
                this->x_comp /= rhs;
                this->y_comp /= rhs;
                return *this;
            }
            bengine::small_vector_2d<type>& operator%=(const double &rhs) {
                this->x_comp = std::fmod(this->x_comp, rhs);
                this->y_comp = std::fmod(this->y_comp, rhs);
                return *this;
            }
            bengine::small_vector_2d<type> operator+(const bengine::small_vector_2d<type> &rhs) const {
                return bengine::small_vector_2d<type>(this->x_comp + rhs.get_x_comp(), this->y_comp + rhs.get_y_comp());
            }
            bengine::small_vector_2d<type> operator-(const bengine::small_vector_2d<type> &rhs) const {
                return bengine::small_vector_2d<type>(this->x_comp - rhs.get_x_comp(), this->y_comp - rhs.get_y_comp());
            }
            bengine::small_vector_2d<type> operator*(const double &rhs) const {
                return bengine::small_vector_2d<type>(this->x_comp * rhs, this->y_comp * rhs);
            }
            bengine::small_vector_2d<type> operator/(const double &rhs) const {
                return bengine::small_vector_2d<type>(this->x_comp / rhs, this->y_comp / rhs);
            }
            bengine::small_vector_2d<type> operator%(const double &rhs) const {
                return bengine::small_vector_2d<type>(std::fmod(this->x_comp, rhs), std::fmod(this->y_comp, rhs));
            }

            bengine::small_vector_2d<type> scalar_product(const double &scalar) const {
                return bengine::small_vector_2d<type>(this->x_comp * scalar, this->y_comp * scalar);
            }
            type dot_product(const bengine::small_vector_2d<type>& other) const {
                return this->x_comp * other.get_x_comp() + this->y_comp * other.get_y_comp();
            }
            type cross_product(const bengine::small_vector_2d<type>& other) const {
                return this->x_comp * other.get_y_comp() - this->y_comp * other.get_x_comp();
            }
    };
    template <class type> typename bengine::small_vector_2d<type>::relation bengine::small_vector_2d<type>::relation_metric = bengine::small_vector_2d<type>::relation::DEFAULT;
    template <class type> typename bengine::small_vector_2d<type> bengine::small_vector_2d<type>::reference_vector = bengine::small_vector_2d<type>(1, 0);
}

#endif // BENGINE_SMALL_VECTOR_2D_hpp
