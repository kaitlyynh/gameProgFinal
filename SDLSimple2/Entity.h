#ifndef ENTITY_H
#define ENTITY_H

#include "Map.h"
#include "glm/glm.hpp"
#include "ShaderProgram.h"
enum EntityType { PLATFORM, PLAYER, ENEMY, BULLET};
enum AIType     { WALKER, GUARD, WALKER_V            }; // WALKER_V is a vertical roamer
enum AIState    { WALKING, IDLE, ATTACKING };


enum AnimationDirection { LEFT, RIGHT, UP, DOWN };

class Scene;

class Entity
{
private:
    
    // Modify size of player sprite
    glm::vec3 m_sprite_size;
    
    // Initialize a bullet attributes
    Entity* m_bullet = nullptr;
    glm::vec3 m_bullet_position;
    int m_bullet_cap = 0;
    int m_bullet_curr; // Indicate which bullet you've shot so far
    bool m_render_bullet = false; // If Entity is a bullet, do we render it?
    bool m_render = true;
    bool m_is_active = true;
    bool m_collided_with_map = false;
    bool m_collided_with_enemy = false;
    float m_max_health = 10000;
    float m_curr_health = 10000;
    std::string m_bullet_filepath_string = "";
    bool m_walk_left = false;
    bool m_walk_up = false;
    int m_walking[4][4]; // 4x4 array for walking animations

    
    EntityType m_entity_type;
    AIType     m_ai_type;
    AIState    m_ai_state;
    // ————— TRANSFORMATIONS ————— //
    glm::vec3 m_movement;
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;

    glm::mat4 m_model_matrix;

    float     m_speed,
              m_jumping_power;
    
    bool m_is_jumping;

    // ————— TEXTURES ————— //
    GLuint    m_texture_id;

    // ————— ANIMATION ————— //
    int m_animation_cols;
    int m_animation_frames,
        m_animation_index,
        m_animation_rows;

    int* m_animation_indices = nullptr;
    float m_animation_time = 0.0f;

    float m_width = 1.0f,
          m_height = 1.0f;
    // ————— COLLISIONS ————— //
    bool m_collided_top    = false;
    bool m_collided_bottom = false;
    bool m_collided_left   = false;
    bool m_collided_right  = false;
    Entity* m_collided_with = nullptr;

public:
    // Bullet getter & setters
    Entity* get_bullet() const { return m_bullet;}
    void set_bullet_position(glm::vec3 new_position) { m_bullet_position = new_position; }
    void create_bullet(Entity* bullet_ptr, int bullet_idx, int num_of_bullets);
    void set_bullet_cap(int new_bullet_cap) { m_bullet_cap = new_bullet_cap; }
    int get_bullet_cap() const { return m_bullet_cap; }
    void set_bullet_curr(int new_bullet_curr) { m_bullet_curr = new_bullet_curr; }
    int get_bullet_curr() const { return m_bullet_curr; }
    bool get_render_bullet() const { return m_render_bullet; }
    void shoot_bullet(Scene* curr_scene, glm::vec3 shoot_direction);
    void set_render_bullet(bool new_render_bullet) { m_render_bullet = new_render_bullet; }
    bool get_render() const { return m_render; }
    void set_render(bool new_render) { m_render = new_render; }
    float get_max_health() const { return m_max_health; }
    float get_curr_health() const { return m_curr_health; }
    void set_max_health(float new_max_health) { m_max_health = new_max_health; }
    void set_curr_health(float new_curr_health) { m_curr_health = new_curr_health; }
    std::string get_bullet_filepath_string() const { return m_bullet_filepath_string; }
    void set_bullet_filepath_string(std::string new_bullet_filepath_string) { m_bullet_filepath_string = new_bullet_filepath_string; }
    void set_walk_left(bool new_walk_left) { m_walk_left = new_walk_left; }
    bool get_walk_left() const { return m_walk_left; }
    void set_walk_up(bool new_walk_up) { m_walk_up = new_walk_up; }
    bool get_walk_up() const { return m_walk_up; }
    
    // ————— STATIC VARIABLES ————— //
    static constexpr int SECONDS_PER_FRAME = 4;

    // ————— METHODS ————— //
    Entity();
    Entity(GLuint texture_id, float speed, glm::vec3 acceleration, float jump_power, int walking[4][4], float animation_time,
        int animation_frames, int animation_index, int animation_cols,
           int animation_rows, float width, float height, EntityType EntityType);
    Entity(GLuint texture_id, float speed, float width, float height, EntityType EntityType); // Simpler constructor
    Entity(GLuint texture_id, float speed, float width, float height, EntityType EntityType, AIType AIType, AIState AIState); // AI constructor
    ~Entity();

    void draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index);
    bool const check_collision(Entity* other);
    
    void const check_collision_y(Entity* collidable_entities, int collidable_entity_count);
    void const check_collision_x(Entity* collidable_entities, int collidable_entity_count);
    
    // Overloading our methods to check for only the map
    void const check_collision_y(Map *map);
    void const check_collision_x(Map *map);
    
    void update(float delta_time, Entity *player, Entity *collidable_entities, int collidable_entity_count, Map *map);
    void render(ShaderProgram* program);

    void ai_activate(Entity *player);
    void ai_walk();
    void ai_guard(Entity *player);
    void ai_shoot();
    void ai_walk_v(); 
    
    
    void normalise_movement() { m_movement = glm::normalize(m_movement); }
    
    void face_left() { m_animation_indices = m_walking[LEFT]; }
    void face_right() {  m_animation_indices = m_walking[RIGHT]; }
    void face_up() { m_animation_indices = m_walking[UP]; }
    void face_down() { m_animation_indices = m_walking[DOWN]; }

    void move_left() { m_movement.x = -1.0f; face_left();}
    void move_right() { m_movement.x = 1.0f;  face_right(); }
    void move_up() { m_movement.y = 1.0f;  ; face_up(); }
    void move_down() { m_movement.y = -1.0f; face_down(); }
    
    void const jump() { m_is_jumping = true; }

    // ————— GETTERS ————— //
    EntityType const get_entity_type()    const { return m_entity_type;   };
    AIType     const get_ai_type()        const { return m_ai_type;       };
    AIState    const get_ai_state()       const { return m_ai_state;      };
    float const get_jumping_power() const { return m_jumping_power; }
    glm::vec3 const get_position()     const { return m_position; }
    glm::vec3 const get_velocity()     const { return m_velocity; }
    glm::vec3 const get_acceleration() const { return m_acceleration; }
    glm::vec3 const get_movement()     const { return m_movement; }
    glm::vec3 const get_scale()        const { return m_scale; }
    GLuint    const get_texture_id()   const { return m_texture_id; }
    float     const get_speed()        const { return m_speed; }
    bool      const get_collided_top() const { return m_collided_top; }
    bool      const get_collided_bottom() const { return m_collided_bottom; }
    bool      const get_collided_right() const { return m_collided_right; }
    bool      const get_collided_left() const { return m_collided_left; }
    bool      const get_is_active() const { return m_is_active; }
    Entity*     get_collided_with() const { return m_collided_with; }
    bool const get_collided_with_map() const { return m_collided_with_map; }
    bool const get_collided_with_enemy() const { return m_collided_with_enemy; }
    
    float get_width() const { return m_width; }
    float get_height() const { return m_height; }
    void activate()   { m_is_active = true;  };
    void deactivate() { m_is_active = false; };
    // ————— SETTERS ————— //
    void const set_entity_type(EntityType new_entity_type)  { m_entity_type = new_entity_type;};
    void const set_ai_type(AIType new_ai_type){ m_ai_type = new_ai_type;};
    void const set_ai_state(AIState new_state){ m_ai_state = new_state;};
    void const set_position(glm::vec3 new_position) { m_position = new_position; }
    void const set_velocity(glm::vec3 new_velocity) { m_velocity = new_velocity; }
    void const set_acceleration(glm::vec3 new_acceleration) { m_acceleration = new_acceleration; }
    void const set_movement(glm::vec3 new_movement) { m_movement = new_movement; }
    void const set_scale(glm::vec3 new_scale) { m_scale = new_scale; }
    void const set_texture_id(GLuint new_texture_id) { m_texture_id = new_texture_id; }
    void const set_speed(float new_speed) { m_speed = new_speed; }
    void const set_animation_cols(int new_cols) { m_animation_cols = new_cols; }
    void const set_animation_rows(int new_rows) { m_animation_rows = new_rows; }
    void const set_animation_frames(int new_frames) { m_animation_frames = new_frames; }
    void const set_animation_index(int new_index) { m_animation_index = new_index; }
    void const set_animation_time(float new_time) { m_animation_time = new_time; }
    void const set_animation_indices(int* ptr) { m_animation_indices = ptr; }
    void const set_jumping_power(float new_jumping_power) { m_jumping_power = new_jumping_power;}
    void const set_width(float new_width) {m_width = new_width; }
    void const set_height(float new_height) {m_height = new_height; }
    void const set_sprite_size(glm::vec3 new_sprite_size) { m_sprite_size = new_sprite_size; }
    void uncollide_with_entity() { m_collided_with = nullptr; }
    void uncollide_with_map() { m_collided_with_map = false; }
    void uncollide() { m_collided_with = nullptr; m_collided_with_map = false; }
    // Setter for m_walking
    void set_walking(int walking[4][4])
    {
        if (walking == nullptr) return;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                m_walking[i][j] = walking[i][j];
            }
        }
    }
};

#endif // ENTITY_H
