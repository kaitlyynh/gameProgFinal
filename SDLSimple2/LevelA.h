#include "Scene.h"

class LevelA : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 7;
    int PLATFORM_COUNT = 2; // Reload stations
    float damage_factor = 2.0f;
    float MILLISECONDS_IN_SECOND = 1000.0;
    int enemies_killed = 0;
    bool goal_reached = false;
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float m_previous_ticks = 0.0f;
    float m_accumulator = 0.0f;
    bool lose = false;
    
    // ————— DESTRUCTOR ————— //
    ~LevelA();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
