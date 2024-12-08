#include "Scene.h"

class LevelB : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 5;
    int PLATFORM_COUNT = 25; // 11
    float MILLISECONDS_IN_SECOND = 1000.0;
    int enemies_killed = 0;
    bool goal_reached = false;
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float m_previous_ticks = 0.0f;
    float m_accumulator = 0.0f;
    void refresh_bullet_textures(GLuint touched_texture_id, std::string new_bullet_filepath_string);
    float damage_factor = 2.0f;
    Entity* collidables;
    // ————— DESTRUCTOR ————— //
    ~LevelB();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
