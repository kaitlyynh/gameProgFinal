#include "Menu.h"
#include "Utility.h"
#define LOG(argument) std::cout << argument << '\n'


constexpr char SPRITESHEET_FILEPATH[] = "assets/mario4.png",
           PLATFORM_FILEPATH[]    = "assets/greenPipe.png",
           ENEMY_FILEPATH[]       = "assets/thwomp.png",
            FONT_FILEPATH[] = "assets/font1.png";


Menu::~Menu()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void Menu::initialise()
{
    m_game_state.next_scene_id = 1;
    m_game_state.map = nullptr;
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);

    glm::vec3 acceleration = glm::vec3(0.0f, -4.81f, 0.0f);
    
    m_game_state.player = new Entity(player_texture_id, 5.0f, 0.2f, 1.3f, PLAYER); // sprite hitbox (center of pos)
//    m_game_state.player->set_sprite_size(glm::vec3(1.0f, 2.0f, 0.0f)); // change size of sprite
    m_game_state.player->set_position(glm::vec3(0.0f, 2.0f, 0.0f));
    m_game_state.player->set_acceleration(acceleration);
    m_game_state.player->set_jumping_power(3.0f);
//    m_game_state.player->set_animation_indices(nullptr);
    
//    m_game_state.player = new Entity(
//        player_texture_id,         // texture id
//        5.0f,                      // speed
//        acceleration,              // acceleration
//        5.0f,                      // jumping power
//        player_walking_animation,  // animation index sets
//        0.0f,                      // animation time
//        4,                         // animation frame amount
//        0,                         // current animation index
//        4,                         // animation column amount
//        4,                         // animation row amount
//        1.0f,                      // width
//        1.0f,                       // height
//        PLAYER
//    );
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

//    m_game_state.enemies = new Entity[ENEMY_COUNT];

//    for (int i = 0; i < ENEMY_COUNT; i++)
//    {
//    m_game_state.enemies[i] =  Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, GUARD, IDLE);
//    }
//
//
//    m_game_state.enemies[0].set_position(glm::vec3(0.0f, 0.0f, 0.0f));
//    m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
//    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/zenmusic.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(0.0f);

    m_game_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
}

void Menu::update(float delta_time)
{
//    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
//    
//    for (int i = 0; i < ENEMY_COUNT; i++)
//    {
//        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
//    }
}


void Menu::render(ShaderProgram *g_shader_program)
{
//    m_game_state.map->render(g_shader_program);
//    m_game_state.player->render(g_shader_program);
//    for (int i = 0; i < m_number_of_enemies; i++)
//            m_game_state.enemies[i].render(g_shader_program);
    glClearColor(1.0f, 1.0f, 0.6f, 1.0f); // Pastel yellow
    GLuint file_texture_id = Utility::load_texture(FONT_FILEPATH);
    Utility::draw_text(g_shader_program, file_texture_id, "Animal", 0.600f, 0.03f, glm::vec3(-1.5f, 5.0f, 0.0f));
    Utility::draw_text(g_shader_program, file_texture_id, "Collector", 0.600f, 0.03f, glm::vec3(-2.5f, 4.0f, 0.0f));
    Utility::draw_text(g_shader_program, file_texture_id, "Enter to start", 0.600f, 0.03f, glm::vec3(-4.0f, 2.0f, 0.0f));

}
