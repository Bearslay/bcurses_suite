#ifndef BENGINE_PHYSICS_hpp
#define BENGINE_PHYSICS_hpp

namespace bengine {
    class physics_object_2d {
        protected:
            double x_pos = 0;
            double y_pos = 0;
            double x_vel = 0;
            double y_vel = 0;
            double x_acl = 0;
            double y_acl = 0;

            double x_frc = 0;
            double y_frc = 0;

            double mass = 1;
        
        public:
            physics_object_2d() {}
    };
}

#endif // BENGINE_PHYSICS_hpp
