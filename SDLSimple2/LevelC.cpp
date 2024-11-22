#include "LevelC.h"
#include "Utility.h"
#define LOG(argument) std::cout << argument << '\n'
#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 5

constexpr char SPRITESHEET_FILEPATH[] = "assets/luigi.png",
           PLATFORM_FILEPATH[]    = "assets/greenPipe.png",
           ENEMY_FILEPATH[]       = "assets/thwomp.png",
            GOAL_FILEPATH[] = "assets/peach.png";

unsigned int LEVEL_DATA_C[] =
{
    24, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0,
    27, 0, 0, 0, 0, 0, 25, 26, 0, 0, 0, 0, 0, 0,
    26, 0, 24, 0, 0, 24, 0, 0, 0, 0, 24, 0, 0, 0,
    25, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 25, 0,
    24, 25, 26, 27, 24, 25, 26, 27, 24, 25, 26, 27, 24, 25
};

LevelC::~LevelC()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelC::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA_C, map_texture_id, 1.0f, 8, 8);
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);

    glm::vec3 acceleration = glm::vec3(0.0f, -4.81f, 0.0f);
    
    m_game_state.player = new Entity(player_texture_id, 7.0f, 0.75f, 1.0f, PLAYER); // sprite hitbox (center of pos)
//    m_game_state.player->set_sprite_size(glm::vec3(1.0f, 2.0f, 0.0f)); // change size of sprite
    m_game_state.player->set_position(glm::vec3(3.0f, 2.0f, 0.0f));
    m_game_state.player->set_acceleration(acceleration);
    m_game_state.player->set_jumping_power(5.0f);
    m_game_state.player->set_animation_indices(nullptr);
    
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        LOG("Spawn enemy");
        LOG(i);
        m_game_state.enemies[i] =  Entity(enemy_texture_id, 0.45f, 0.45f, 1.1f, ENEMY, GUARD, IDLE);
        m_game_state.enemies[i].set_position(glm::vec3((i * 0.5f) + 0.5f, 2.0f, 0.0f));
        m_game_state.enemies[i].set_movement(glm::vec3(0.0f));
        m_game_state.enemies[i].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
        
    }


    

    GLuint goal_texture_id = Utility::load_texture(GOAL_FILEPATH);
    
    m_game_state.platforms = new Entity[PLATFORM_COUNT];
    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        m_game_state.platforms[i] =  Entity(goal_texture_id, 0.5f, 0.5f, 1.0f, PLATFORM);
        m_game_state.platforms[i].set_position(glm::vec3(13.0f, 0.0f, 0.0f));
        m_game_state.platforms[i].update(0.0f, m_game_state.player, m_game_state.player, 1, m_game_state.map);
    }
//    m_game_state.platforms[0].set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/crypto.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4.0f);

    m_game_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
}

void LevelC::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    
    
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, m_game_state.player, 1, m_game_state.map);
        m_game_state.player->check_collision_x(m_game_state.enemies, ENEMY_COUNT);
        m_game_state.player->check_collision_y(m_game_state.enemies, ENEMY_COUNT);
        if ((m_game_state.player->get_collided_left() || m_game_state.player->get_collided_right()) && m_game_state.player->get_collided_with() == &m_game_state.enemies[i]) {
                LOG("Enemy side hit C");
            m_game_state.player->uncollide();
            damage_taken += 1;
        } else if (m_game_state.player->get_collided_bottom() && m_game_state.player->get_collided_with() == &m_game_state.enemies[i] &&
                   !m_game_state.enemies[i].get_collided_left() && !m_game_state.enemies[i].get_collided_right()) {
            LOG("Enemy head hit C");
            if (m_game_state.enemies[i].get_is_active()) {
                m_game_state.enemies[i].deactivate();
                enemies_killed++;
                m_game_state.player->uncollide();
            }

        }
        
        
    }
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        m_game_state.player->check_collision_x(m_game_state.platforms, PLATFORM_COUNT);
        m_game_state.player->check_collision_y(m_game_state.platforms, PLATFORM_COUNT);
        if (m_game_state.player->get_collided_with() == &m_game_state.platforms[i]) {
            LOG("Peach Level 3 Collision");
            LOG(enemies_killed);
            goal_reached = true;
        }
    }
    if (goal_reached && enemies_killed == ENEMY_COUNT) {
        complete = true;
        LOG("Complete is true level C");
    }
}


void LevelC::render(ShaderProgram *g_shader_program)
{
    m_number_of_enemies = ENEMY_COUNT;
    m_game_state.map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f); // pastel blue


    for (int i = 0; i < m_number_of_enemies; i++)
        if (m_game_state.enemies[i].get_is_active()) {
            m_game_state.enemies[i].render(g_shader_program);
        } else {
            m_game_state.enemies[i].set_position(glm::vec3(-10.0f,-10.0f,0.0f));
        }
            
    for (int i = 0; i < PLATFORM_COUNT; i++)
            m_game_state.platforms[i].render(g_shader_program);
}
