#include "Scene.h"

class LevelC : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 10; // 5 dogs, 5 monkeys
    int PLATFORM_COUNT = 10;
    int DOG_COUNT = 5;
    int MONKEY_COUNT = 5;
    
    float MILLISECONDS_IN_SECOND = 1000.0;
    int enemies_killed = 0;
    bool goal_reached = false;
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float m_previous_ticks = 0.0f;
    float m_accumulator = 0.0f;
    // ————— DESTRUCTOR ————— //
    ~LevelC();
    
    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
    
    void decrement_dog_count() { DOG_COUNT--; }
    void decrement_monkey_count() { MONKEY_COUNT--; }
    void spawn_animal(std::string desired_filepath);
};
