#ifndef BENGINE_TEXTURE_hpp
#define BENGINE_TEXTURE_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace bengine {
    // \brief A wrapper class for the SDL_Texture that pretty much just contains the source texture and frame
    class basic_texture {
        protected:
            // \brief The source texture to use
            SDL_Texture *source = nullptr;
            // \brief The portion of the source texture to actually display
            SDL_Rect frame = {};

        public:
            /** bengine::basic_texture constructor
             * \param texture The SDL_Texture to use as a source
             * \param frame The portion of the source texture to actually display
             */
            basic_texture(SDL_Texture *texture = NULL, const SDL_Rect &frame = {}) {
                this->set_texture(texture);
                this->set_frame(frame);
            }
            // \brief bengine::basic_texture deconstructor; pretty much just handles some SDL cleanup
            ~basic_texture() {
                SDL_DestroyTexture(this->source);
                this->source = nullptr;
            }

            /** Assignment operator overload
             * \param rhs The bengine::basic_texture to inherit from
             */
            void operator=(const bengine::basic_texture &rhs) {
                this->set_texture(rhs.get_texture());
                this->set_frame(rhs.get_frame());
            }

            /** Get the source texture
             * \returns The source SDL_Texture
             */
            SDL_Texture *get_texture() const {
                return this->source;
            }
            /** Set the source texture to a new one
             * \param texture The new texture
             */
            void set_texture(SDL_Texture *texture) {
                this->source = texture;
            }
            /** Get the frame of the texture
             * \returns An SDL_Rect with the frame (NULL = entire texture)
             */
            SDL_Rect get_frame() const {
                return this->frame;
            }
            /** Set the frame of the texture
             * \param frame The new frame (NULL = entire texture)
             */
            void set_frame(const SDL_Rect &frame) {
                this->frame.x = frame.x;
                this->frame.y = frame.y;
                this->frame.w = frame.w;
                this->frame.h = frame.h;
            }
    };

    // \brief A wrapper class for the SDL_Texture that contains the source texture, frame, and color modifications
    class modded_texture : public bengine::basic_texture {
        protected:
            // \brief The SDL_BlendMode used for the texture
            SDL_BlendMode blend_mode = SDL_BLENDMODE_BLEND;
            // \brief The color modification used for the texture (includes opacity if certain blending modes are used)
            SDL_Color color_mod = {255, 255, 255, 255};

        public:
            /** bengine::modded_texture constructor
             * \param texture The SDL_Texture to use as a source
             * \param frame The portion of the source texture to actually display
             * \param color_mod The color modification for the texture
             */
            modded_texture(SDL_Texture *texture = nullptr, const SDL_Rect &frame = {}, const SDL_Color &color_mod = {255, 255, 255, 255}) {
                this->set_texture(texture);
                this->set_frame(frame);
                this->set_color_mod(color_mod);
            }
            // \brief bengine::modded_texture deconstructor
            ~modded_texture() {
                bengine::basic_texture::~basic_texture();
            }

            /** Assignment operator overload
             * \param rhs The bengine::modded_texture to inherit from
             */
            void operator=(const bengine::modded_texture &rhs) {
                this->set_texture(rhs.get_texture());
                this->set_frame(rhs.get_frame());
                this->set_color_mod(rhs.get_color_mod());
                this->set_blend_mode(rhs.get_blend_mode());
            }

            /** Get the blending mode used on this texture
             * \returns The SDL_BlendMode used on this texture
             */ 
            SDL_BlendMode get_blend_mode() const {
                return this->blend_mode;
            }
            /** Set the blending mode that will be used on this texture
             * \param blendMode The new SDL_BlendMode that will be used on this texture
             */
            void set_blend_mode(const SDL_BlendMode &blend_mode) {
                SDL_SetTextureBlendMode(this->source, blend_mode);
                this->blend_mode = blend_mode;
            }

            /** Get the color modification used on this texture
             * \returns The color modification used on this texture
             */
            SDL_Color get_color_mod() const {
                return this->color_mod;
            }
            /** Set the color modification that will be used on this texture
             * \param color_mod The new color mod that will be used on this texture
             */
            void set_color_mod(const SDL_Color &color_mod) {
                SDL_SetTextureColorMod(this->source, color_mod.r, color_mod.g, color_mod.b);
                SDL_SetTextureAlphaMod(this->source, color_mod.a);
                this->color_mod.r = color_mod.r;
                this->color_mod.g = color_mod.g;
                this->color_mod.b = color_mod.b;
                this->color_mod.a = color_mod.a;
            }

            /** Get the "amount of red" in the texture
             * \returns The "amount of red" in the texture
             */
            Uint8 get_red_mod() const {
                return this->color_mod.r;
            }
            /** Set the "amount of red" in the texture
             * \param red_mod The new "amount of red" that will be present in the texture
             */
            void set_red_mod(const Uint8 &red_mod) {
                SDL_SetTextureColorMod(this->source, red_mod, this->color_mod.g, this->color_mod.b);
                this->color_mod.r = red_mod;
            }

            /** Get the "amount of green" in the texture
             * \returns The "amount of green" in the texture
             */
            Uint8 get_green_mod() const {
                return this->color_mod.g;
            }
            /** Set the "amount of green" in the texture
             * \param green_mod The new "amount of green" that will be present in the texture
             */
            void set_green_mod(const Uint8 &green_mod) {
                SDL_SetTextureColorMod(this->source, this->color_mod.r, green_mod, this->color_mod.b);
                this->color_mod.g = green_mod;
            }

            /** Get the "amount of blue" in the texture
             * \returns The "amount of blue" in the texture
             */
            Uint8 get_blue_mod() const {
                return this->color_mod.b;
            }
            /** Set the "amount of blue" in the texture
             * \param blue_mod The new "amount of blue" that will be present in the texture
             */
            void set_blue_mod(const Uint8 &blue_mod) {
                SDL_SetTextureColorMod(this->source, this->color_mod.r, this->color_mod.g, blue_mod);
                this->color_mod.b = blue_mod;
            }

            /** Get the opacity of the texture (only visable with certain blendmodes)
             * \returns The opacity of the texture
             */
            Uint8 get_alpha_mod() const {
                return this->color_mod.a;
            }
            /** Set the opacity of the texture (only visable with certain blendmodes)
             * \param alpha_mod The new opacity of the texture
             */
            void set_alpha_mod(const Uint8 &alpha_mod) {
                SDL_SetTextureAlphaMod(this->source, alpha_mod);
                this->color_mod.a = alpha_mod;
            }
    };

    // \brief A wrapper class for the SDL_Texture that contains the source texture, frame, color modifications, and rotation/reflection data
    class shifting_texture : public bengine::modded_texture {
        protected:
            // \brief The point that the texture rotates about relative to the source frame's top-left corner
            SDL_Point pivot = {};
            // \brief The angle that the texture is rotated at (in degrees)
            double angle = 0;
            // \brief How the texture is flipped (SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, and SDL_FLIP_VERTICAL OR'd together)
            SDL_RendererFlip flip = SDL_FLIP_NONE;

        public:
            /** bengine::shifting_texture constructor
             * \param texture The SDL_Texture to use as a source
             * \param frame The portion of the source texture to actually display
             * \param pivot The point for the texture to be rotated about relative to the source frame's top-left corner
             * \param angle The angle for the texture to be rotated at
             * \param color_mod The color modification for the texture
             */
            shifting_texture(SDL_Texture *texture = NULL, const SDL_Rect &frame = {}, const SDL_Point &pivot = {}, const double &angle = 0, const SDL_Color &color_mod = {255, 255, 255, 255}) {
                this->set_texture(texture);
                this->set_frame(frame);
                this->set_color_mod(this->color_mod);
                this->set_pivot(pivot);
                this->set_angle(angle);
            }
            // \brief bengine::shifting_texture deconstructor
            ~shifting_texture() {
                bengine::modded_texture::~modded_texture();
            }

            /** Assignment operator overload
             * \param rhs The bengine::shifting_texture to inherit from
             */
            void operator=(const bengine::shifting_texture &rhs) {
                this->set_texture(rhs.get_texture());
                this->set_frame(rhs.get_frame());
                this->set_color_mod(rhs.get_color_mod());
                this->set_blend_mode(rhs.get_blend_mode());
                this->set_pivot(rhs.get_pivot());
                this->set_angle(rhs.get_angle());
                this->set_flip(rhs.get_flip());
            }

            /** Get the point for the texture to rotate about relative to the source frame's top-left corner
             * \returns The point for the texture to rotate about relative to the source frame's top-left corner
             */
            SDL_Point get_pivot() const {
                return this->pivot;
            }
            /** Set the point for the texture to rotate about relative to the source frame's top-left corner
             * \param pivot The new point for the texture to rotate about relative to the source frame's top-left corner
             */
            void set_pivot(const SDL_Point &pivot) {
                this->pivot.x = pivot.x;
                this->pivot.y = pivot.y;
            }

            /** Get the angle that the texture is rotated at (in degrees)
             * \returns The angle that the texture is rotated at (in degrees)
             */
            double get_angle() const {
                return this->angle;
            }
            /** Get the angle that the texture is rotated at (in degrees)
             * \param angle The new angle that the texture will be rotated at (in degrees)
             */
            void set_angle(const double &angle) {
                this->angle = angle;
            }

            /** Get the state of how the texture is flipped
             * \returns An SDL_RendererFlip mask representing how the texture is flipped
             */
            SDL_RendererFlip get_flip() const {
                return this->flip;
            }
            /** Set how the texture should be flipped
             * \param flip An SDL_RendererFlip mask representing how the texture should be flipped
             */
            void set_flip(const SDL_RendererFlip &flip) {
                this->flip = flip;
            }
    };
}

#endif // BENGINE_TEXTURE_hpp
