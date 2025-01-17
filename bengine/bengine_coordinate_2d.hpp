#ifndef BENGINE_COORDINATE_2D_hpp
#define BENGINE_COORDINATE_2D_hpp

#include <cmath>
#include <vector>

#include "bengine_helpers.hpp"

namespace bengine {
    template <class type = double> class coordinate_2d {
        public:
            enum class relation : unsigned char {
                DEFAULT = 0,
                EUCLIDEAN = 1,
                TAXICAB = 2,
                X_COMPONENT = 3,
                Y_COMPONENT = 4
            };

        private:
            type x_pos = 0;
            type y_pos = 0;

            static bengine::coordinate_2d<type>::relation relation_metric;
            static bengine::coordinate_2d<type> reference_point;

        public:
            coordinate_2d() {
                static_assert(std::is_arithmetic<type>::value, "Template type \"type\" must be an arithmetic type (int, long, float, double, etc)");
            }
            coordinate_2d(const type &x_pos, const type &y_pos) {
                static_assert(std::is_arithmetic<type>::value, "Template type \"type\" must be an arithmetic type (int, long, float, double, etc)");
                this->x_pos = x_pos;
                this->y_pos = y_pos;
            }
            coordinate_2d(const bengine::coordinate_2d<type> &coord) {
                *this = coord;
            }

            static bengine::coordinate_2d<type>::relation get_relation_metric() {
                return bengine::coordinate_2d<type>::relation_metric;
            }
            static void set_relation_metric(const bengine::coordinate_2d<type>::relation &metric) {
                bengine::coordinate_2d<type>::relation_metric = (static_cast<unsigned char>(metric) > 4 || static_cast<unsigned char>(metric) < 0) ? bengine::coordinate_2d<type>::relation::DEFAULT : metric;
            }
            static void reset_relation_metric() {
                bengine::coordinate_2d<type>::relation_metric = bengine::coordinate_2d<type>::relation::DEFAULT;
            }
            static bengine::coordinate_2d<type> get_reference_point() {
                return bengine::coordinate_2d<type>::reference_point;
            }
            static void set_reference_point(const bengine::coordinate_2d<type> &coord) {
                bengine::coordinate_2d<type>::reference_point = coord;
            }
            static void set_reference_point(const type &x_pos, const type &y_pos) {
                bengine::coordinate_2d<type>::set_reference_point(bengine::coordinate_2d<type>(x_pos, y_pos));
            }
            static void reset_reference_point() {
                bengine::coordinate_2d<type>::reference_point = bengine::coordinate_2d<type>(0, 0);
            }

            type get_x_pos() const {
                return this->x_pos;
            }
            type get_y_pos() const {
                return this->y_pos;
            }
            void set_x_pos(const type &x_pos) {
                this->x_pos = x_pos;
            }
            void set_y_pos(const type &y_pos) {
                this->y_pos = y_pos;
            }
            void translate_horizontally(const type &x_comp) {
                this->x_pos += x_comp;
            }
            void translate_vertically(const type &y_comp) {
                this->y_pos += y_comp;
            }

            void swap_x_and_y() {
                const type temp = this->x_pos;
                this->x_pos = this->y_pos;
                this->y_pos = temp;
            }
            void reflect_over_x_axis() {
                this->y_pos = -this->y_pos;
            }
            void reflect_over_y_axis() {
                this->x_pos = -this->x_pos;
            }
            template <class arithmetic_type = double> void reflect_over_horizontal_line(const arithmetic_type &y_intercept) {
                static_assert(std::is_arithmetic<arithmetic_type>::value, "Template type \"arithmetic_type\" must be an arithmetic type (int, long, float, double, etc)");
                this->y_pos = std::is_integral<type>::value ? std::round(2 * y_intercept - this->y_pos) : 2 * y_intercept - this->y_pos;
            }
            template <class arithmetic_type = double> void reflect_over_vertical_line(const arithmetic_type &x_intercept) {
                static_assert(std::is_arithmetic<arithmetic_type>::value, "Template type \"arithmetic_type\" must be an arithmetic type (int, long, float, double, etc)");
                this->x_pos = std::is_integral<type>::value ? std::round(2 * x_intercept - this->x_pos) : 2 * x_intercept - this->x_pos;
            }
            /** Reflect the coordinate over a line defined by y = mx + b (slope-intercept form)
             * 
             * Should the slope be zero, the coordinate will be reflected over the horizontal line where y = y-intercept
             * 
             * Should the slope be undefined (NAN), nothing will happen as there is no x-intercept to form a vertical line from
             * 
             * \param slope The slope (m) of the line to reflect over
             * \param y_intercept The y-intercept (b) of the line to reflect over
             */
            template <class arithmetic_type = double> void reflect_over_line(const arithmetic_type &slope, const arithmetic_type &y_intercept) {
                static_assert(std::is_arithmetic<arithmetic_type>::value, "Template type \"arithmetic_type\" must be an arithmetic type (int, long, float, double, etc)");
                if (slope == 0) {
                    return this->reflect_over_horizontal_line<arithmetic_type>(y_intercept);
                }
                if (std::isnan(slope)) {
                    return;
                }
                const double x_line_interception = (this->y_pos + this->x_pos / slope - y_intercept) / (slope + 1 / slope);
                if (std::is_integral<type>::value) {
                    this->x_pos = std::round(2 * x_line_interception - this->x_pos);
                    this->y_pos = std::round(2 * (slope * x_line_interception + y_intercept) - this->y_pos);
                    return;
                }
                this->x_pos = 2 * x_line_interception - this->x_pos;
                this->y_pos = 2 * (slope * x_line_interception + y_intercept) - this->y_pos;
            }
            /** Reflect the coordinate over a line defined by y - y1 = m(x - x1) (point-slope form)
             * 
             * Should the slope be zero, the coordinate will be reflected over the horizontal line where y = y1
             * 
             * Should the slope be undefined (NAN), the coordinate will be reflected over the vertical line where x = x1
             * 
             * \param x_pos The x-position (x1) of the point defining the line to reflect over
             * \param y_pos The y-position (y1) of the point defining the line to reflect over
             * \param slope The slope (m) of the line to reflect over
             */
            template <class arithmetic_type = double> void reflect_over_line(const arithmetic_type &x_pos, const arithmetic_type &y_pos, const arithmetic_type &slope) {
                static_assert(std::is_arithmetic<arithmetic_type>::value, "Template type \"arithmetic_type\" must be an arithmetic type (int, long, float, double, etc)");
                if (std::isnan(slope)) {
                    return this->reflect_over_vertical_line<arithmetic_type>(x_pos);
                }
                this->reflect_over_line<arithmetic_type>(slope, y_pos - slope * x_pos);
            }
            /** Reflect the coordinate over a line defined by two inputted points (x1, y1) and (x2, y2)
             * 
             * Should the slope be zero, the coordinate will be reflected over the horizontal line where y = y1 = y2
             * 
             * Should the slope be undefined (NAN), the coordinate will be reflected over the vertical line where x = x1 = x2
             * 
             * \param x_pos_1 The x-position (x1) of the first point defining the line to reflect over
             * \param y_pos_1 The y-position (y1) of the first point defining the line to reflect over
             * \param x_pos_2 The x-position (x2) of the second point defining the line to reflect over
             * \param y_pos_2 The y-position (y2) of the second point defining the line to reflect over
             */
            template <class arithmetic_type = double> void reflect_over_line(const arithmetic_type &x_pos_1, const arithmetic_type &y_pos_1, const arithmetic_type &x_pos_2, const arithmetic_type &y_pos_2) {
                static_assert(std::is_arithmetic<arithmetic_type>::value, "Template type \"arithmetic_type\" must be an arithmetic type (int, long, float, double, etc)");
                if (x_pos_2 - x_pos_1 == 0) {
                    return this->reflect_over_vertical_line<arithmetic_type>(x_pos_1);
                }
                this->reflect_over_line<arithmetic_type>(x_pos_1, y_pos_1, (y_pos_2 - y_pos_1) / (x_pos_2 - x_pos_1));
            }

            template <class arithmetic_type = double> void stretch_horizontally(const arithmetic_type &scalar) {
                static_assert(std::is_arithmetic<arithmetic_type>::value, "Template type \"arithmetic_type\" must be an arithmetic type (int, long, float, double, etc)");
                this->x_pos = std::is_integral<type>::value ? std::round(this->x_pos * scalar) : this->x_pos * scalar;
            }
            template <class arithmetic_type = double> void stretch_vertically(const arithmetic_type &scalar) {
                static_assert(std::is_arithmetic<arithmetic_type>::value, "Template type \"arithmetic_type\" must be an arithmetic type (int, long, float, double, etc)");
                this->y_pos = std::is_integral<type>::value ? std::round(this->y_pos * scalar) : this->y_pos * scalar;
            }
            template <class arithmetic_type = double> void stretch_along_slope(const arithmetic_type &slope, const arithmetic_type &scalar) {
                static_assert(std::is_arithmetic<arithmetic_type>::value, "Template type \"arithmetic_type\" must be an arithmetic type (int, long, float, double, etc)");
                if (std::isnan(slope)) {
                    this->x_pos = 0;
                    return this->stretch_vertically<arithmetic_type>(scalar);
                }
                const double angle = std::atan(slope);
                this->x_pos = std::is_integral<type>::value ? std::round(this->x_pos * scalar * std::cos(angle)) : this->x_pos * scalar * std::cos(angle);
                this->y_pos = std::is_integral<type>::value ? std::round(this->y_pos * scalar * std::sin(angle)) : this->y_pos * scalar * std::sin(angle);
            }

            void rotate_about_pivot(const bengine::coordinate_2d<type> &pivot, const double &angle) {
                const double sin = std::sin(angle);
                const double cos = std::cos(angle);
                this->x_pos = (this->x_pos - pivot.get_x_pos()) * cos - (this->y_pos - pivot.get_y_pos()) * sin + pivot.get_x_pos();
                this->y_pos = (this->x_pos - pivot.get_x_pos()) * sin + (this->y_pos - pivot.get_y_pos()) * cos + pivot.get_y_pos();
            }
            void rotate_about_origin(const double &angle) {
                const double sin = std::sin(angle);
                const double cos = std::cos(angle);
                this->x_pos = this->x_pos * cos - this->y_pos * sin;
                this->y_pos = this->x_pos * sin + this->y_pos * cos;
            }
            void rotate_about_reference_point(const double &angle) {
                this->rotate_about_pivot(bengine::coordinate_2d<type>::reference_point, angle);
            }

            static void rotate_points_about_pivot(std::vector<bengine::coordinate_2d<type>> &points, const bengine::coordinate_2d<type> &pivot, const double &angle) {
                const double sin = std::sin(angle);
                const double cos = std::cos(angle);
                for (std::size_t i = 0; i < points.size(); i++) {
                    points[i].set_x_pos((points[i].get_x_pos() - pivot.get_x_pos()) * cos - (points[i].get_y_pos() - pivot.get_y_pos()) * sin + pivot.get_x_pos());
                    points[i].set_x_pos((points[i].get_x_pos() - pivot.get_x_pos()) * sin + (points[i].get_y_pos() - pivot.get_y_pos()) * cos + pivot.get_y_pos());
                }
            }
            static void rotate_points_about_reference_point(std::vector<bengine::coordinate_2d<type>> &points, const double &angle) {
                return bengine::coordinate_2d<type>::rotate_points_about_pivot(points, bengine::coordinate_2d<type>::reference_point, angle);
            }

            bengine::coordinate_2d<type>& operator=(const bengine::coordinate_2d<type> &rhs) {
                this->x_pos = rhs.get_x_pos();
                this->y_pos = rhs.get_y_pos();
                return *this;
            }
            bengine::coordinate_2d<type> operator!() const {
                return bengine::coordinate_2d<type>(-this->x_pos, -this->y_pos);
            }
            std::string to_string() const {
                return "(" + bengine::string_helper::to_string(this->x_pos) + ", " + bengine::string_helper::to_string(this->y_pos) + ")";
            }
            std::vector<type> to_vector() const {
                return {this->x_pos, this->y_pos};
            }

            bool is_equal_to(const bengine::coordinate_2d<type> &other, const bengine::coordinate_2d<type>::relation &metric = bengine::coordinate_2d<type>::relation_metric) const {
                switch (metric) {
                    default:
                    case bengine::coordinate_2d<type>::relation::DEFAULT:
                        return this->x_pos == other.get_x_pos() && this->y_pos == other.get_y_pos();
                    case bengine::coordinate_2d<type>::relation::EUCLIDEAN:
                        return get_euclidean_distance_to(bengine::coordinate_2d<type>::reference_point) == other.get_euclidean_distance_to(bengine::coordinate_2d<type>::reference_point);
                    case bengine::coordinate_2d<type>::relation::TAXICAB:
                        return get_taxicab_distance_to(bengine::coordinate_2d<type>::reference_point) == other.get_taxicab_distance_to(bengine::coordinate_2d<type>::reference_point);
                    case bengine::coordinate_2d<type>::relation::X_COMPONENT:
                        return this->x_pos == other.get_x_pos();
                    case bengine::coordinate_2d<type>::relation::Y_COMPONENT:
                        return this->y_pos == other.get_y_pos();
                }
            }
            bool is_not_equal_to(const bengine::coordinate_2d<type> &other, const bengine::coordinate_2d<type>::relation &metric = bengine::coordinate_2d<type>::relation_metric) const {
                return !this->is_equal_to(other, metric);
            }
            bool is_less_than(const bengine::coordinate_2d<type> &other, const bengine::coordinate_2d<type>::relation &metric = bengine::coordinate_2d<type>::relation_metric) const {
                switch (metric) {
                    default:
                    case bengine::coordinate_2d<type>::relation::DEFAULT:
                    case bengine::coordinate_2d<type>::relation::EUCLIDEAN:
                        return get_euclidean_distance_to(bengine::coordinate_2d<type>::reference_point) < other.get_euclidean_distance_to(bengine::coordinate_2d<type>::reference_point);
                    case bengine::coordinate_2d<type>::relation::TAXICAB:
                        return get_taxicab_distance_to(bengine::coordinate_2d<type>::reference_point) < other.get_taxicab_distance_to(bengine::coordinate_2d<type>::reference_point);
                    case bengine::coordinate_2d<type>::relation::X_COMPONENT:
                        return this->x_pos < other.get_x_pos();
                    case bengine::coordinate_2d<type>::relation::Y_COMPONENT:
                        return this->y_pos < other.get_y_pos();
                }
            }
            bool is_greater_than(const bengine::coordinate_2d<type> &other, const bengine::coordinate_2d<type>::relation &metric = bengine::coordinate_2d<type>::relation_metric) const {
                return other.is_less_than(*this, metric);
            }
            bool is_less_than_or_equal_to(const bengine::coordinate_2d<type> &other, const bengine::coordinate_2d<type>::relation &metric = bengine::coordinate_2d<type>::relation_metric) const {
                return !this->is_greater_than(other, metric);
            }
            bool is_greater_than_or_equal_to(const bengine::coordinate_2d<type> &other, const bengine::coordinate_2d<type>::relation &metric = bengine::coordinate_2d<type>::relation_metric) const {
                return !this->is_less_than(other, metric);
            }

            bool operator==(const bengine::coordinate_2d<type> &rhs) const {
                return this->is_equal_to(rhs, bengine::coordinate_2d<type>::relation_metric);
            }
            bool operator!=(const bengine::coordinate_2d<type> &rhs) const {
                return this->is_not_equal_to(rhs, bengine::coordinate_2d<type>::relation_metric);
            }
            bool operator<(const bengine::coordinate_2d<type> &rhs) const {
                return this->is_less_than(rhs, bengine::coordinate_2d<type>::relation_metric);
            }
            bool operator>(const bengine::coordinate_2d<type> &rhs) const {
                return this->is_greater_than(rhs, bengine::coordinate_2d<type>::relation_metric);
            }
            bool operator<=(const bengine::coordinate_2d<type> &rhs) const {
                return this->is_less_than_or_equal_to(rhs, bengine::coordinate_2d<type>::relation_metric);
            }
            bool operator>=(const bengine::coordinate_2d<type> &rhs) const {
                return this->is_greater_than_or_equal_to(rhs, bengine::coordinate_2d<type>::relation_metric);
            }

            bengine::coordinate_2d<type>& operator+=(const bengine::coordinate_2d<type> &rhs) {
                this->x_pos += rhs.get_x_pos();
                this->y_pos += rhs.get_y_pos();
                return *this;
            }
            bengine::coordinate_2d<type>& operator-=(const bengine::coordinate_2d<type> &rhs) {
                this->x_pos -= rhs.get_x_pos();
                this->y_pos -= rhs.get_y_pos();
                return *this;
            }
            bengine::coordinate_2d<type>& operator*=(const double &rhs) {
                this->x_pos *= rhs;
                this->y_pos *= rhs;
                return *this;
            }
            bengine::coordinate_2d<type>& operator/=(const double &rhs) {
                this->x_pos /= rhs;
                this->y_pos /= rhs;
                return *this;
            }
            bengine::coordinate_2d<type>& operator%=(const double &rhs) {
                this->x_pos = std::fmod(this->x_pos, rhs);
                this->y_pos = std::fmod(this->y_pos, rhs);
                return *this;
            }
            bengine::coordinate_2d<type> operator+(const bengine::coordinate_2d<type> &rhs) const {
                return bengine::coordinate_2d<type>(this->x_pos + rhs.get_x_pos(), this->y_pos + rhs.get_y_pos());
            }
            bengine::coordinate_2d<type> operator-(const bengine::coordinate_2d<type> &rhs) const {
                return bengine::coordinate_2d<type>(this->x_pos - rhs.get_x_pos(), this->y_pos - rhs.get_y_pos());
            }
            bengine::coordinate_2d<type> operator*(const double &rhs) const {
                return bengine::coordinate_2d<type>(this->x_pos * rhs, this->y_pos * rhs);
            }
            bengine::coordinate_2d<type> operator/(const double &rhs) const {
                return bengine::coordinate_2d<type>(this->x_pos / rhs, this->y_pos / rhs);
            }
            bengine::coordinate_2d<type> operator%(const double &rhs) const {
                return bengine::coordinate_2d<type>(std::fmod(this->x_pos, rhs), std::fmod(this->y_pos, rhs));
            }

            type get_euclidean_distance_to(const bengine::coordinate_2d<type> &other) const {
                return std::is_integral<type>::value ? std::round(std::sqrt((this->x_pos - other.get_x_pos()) * (this->x_pos - other.get_x_pos()) + (this->y_pos - other.get_y_pos()) * (this->y_pos - other.get_y_pos()))) : std::sqrt((this->x_pos - other.get_x_pos()) * (this->x_pos - other.get_x_pos()) + (this->y_pos - other.get_y_pos()) * (this->y_pos - other.get_y_pos()));
            }
            type get_euclidean_distance() const {
                return this->get_euclidean_distance_to(bengine::coordinate_2d<type>::reference_point);
            }
            type get_taxicab_distance_to(const bengine::coordinate_2d<type> &other) const {
                return std::is_integral<type>::value ? std::abs(this->x_pos - other.get_x_pos()) + std::abs(this->y_pos - other.get_y_pos()) : std::fabs(this->x_pos - other.get_x_pos()) + std::fabs(this->y_pos - other.get_y_pos());
            }
            type get_taxicab_distance() const {
                return this->get_taxicab_distance_to(bengine::coordinate_2d<type>::reference_point);
            }
    };
    template <class type> typename bengine::coordinate_2d<type>::relation bengine::coordinate_2d<type>::relation_metric = bengine::coordinate_2d<type>::relation::DEFAULT;
    template <class type> typename bengine::coordinate_2d<type> bengine::coordinate_2d<type>::reference_point = bengine::coordinate_2d<type>(0, 0);
}

#endif // BENGINE_COORDINATE_2D_hpp
