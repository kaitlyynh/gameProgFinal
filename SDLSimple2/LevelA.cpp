#include "LevelA.h"
#include "Utility.h"
#define LOG(argument) std::cout << argument << '\n'
#define LEVEL_WIDTH 15
#define LEVEL_HEIGHT 15

constexpr char SPRITESHEET_FILEPATH[] = "assets/Hero.png",
            BULLET_FILEPATH[] = "assets/hotdog.png",
           ENEMY_FILEPATH[]       = "assets/weiner_dog.png",
            FONT_FILEPATH[] = "assets/font1.png",
        RELOAD_FILEPATH[] = "assets/hotdog_cart.png";
            
unsigned int LEVEL_DATA_A[] =
{
        24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
        24, 0,  0, 24, 0,  0,  0,  0,  0,  24, 0,  0,  0,  0,  24,
        24, 0, 24, 24, 0, 24, 24, 24, 0,  24, 24, 24, 0,  24, 24,
        24, 0, 0, 0,  0,  0,  0,  24, 0,  0,  0,  24, 0,  0,  24,
        24, 0, 24, 0, 24, 24, 24, 24, 24, 24, 0,  24, 0,  24, 24,
        24, 0,  0,  0,  0,  0, 24, 0,  0,  0,  0,  0,  0,  0,  24,
        24, 24, 24, 24, 24, 0,  24, 24, 24, 24, 24, 24, 24, 0,  24,
        24, 0,  0,  0,  24, 0,  0,  0,  0,  0,  0,  0,  24, 0,  24,
        24, 0, 24, 24, 24, 0, 24, 24, 24, 24, 24, 0,  24, 0,  24,
        24, 0,  0,  0,  24, 0,  0,  0,  0,  0,  24, 0,  0,  0,  24,
        24, 24, 24, 0,  24, 24, 24, 24, 24, 0,  24, 24, 24, 0,  24,
        24, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  24, 0,  24,
        24, 24, 24, 24, 24, 24, 0, 24, 24, 24, 24, 0,  24, 0,  24,
        24, 0,  0,  0,  0,  0,  0,  0,  0,  0,  24, 0,  0,  0,  24,
        24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24
};

LevelA::~LevelA()
{
    delete [] m_game_state.bullets;
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::initialise()
{
    m_game_state.next_scene_id = 2;
    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA_A, map_texture_id, 1.0f, 8, 8);
    
    // Player details

    int player_walking_animation[4][4] =
    {
        { 8, 9, 10, 11 },    //left
        { 12, 13, 14, 15 },  // right
        { 0, 1, 2, 3 },      //up
        { 4, 5, 6, 7 }       //down
    };
    
    
    glm::vec3 acceleration = glm::vec3(0.0f, 0, 0.0f); // - 4.81 y
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    
    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        2.0f,                      // speed
        acceleration,              // acceleration
        0.0,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        8,                         // animation column amount
        3,                         // animation row amount
        0.55f,                      // width
        0.55f,                       // height
        PLAYER
        );
    m_game_state.player->set_position(glm::vec3(13.0f, -1.0f, 0.0f)); // Initialize player position
    m_game_state.player->set_sprite_size(glm::vec3(0.5f, 0.5f, 0.0f)); // Change size of sprite
    m_game_state.player->set_max_health(100.0f);
    m_game_state.player->set_curr_health(100.0f);
    
    // Bullet variables
    GLuint bullet_texture_id = Utility::load_texture(BULLET_FILEPATH);
    constexpr float BULLET_SPEED = 4.0f,
                BULLET_WIDTH = 0.5f,
                BULLET_HEIGHT = 0.5f;
    m_game_state.player->set_bullet_cap(25);
    m_game_state.player->set_bullet_curr(m_game_state.player->get_bullet_cap());
    m_game_state.bullets = new Entity[m_game_state.player->get_bullet_cap()];
    
    for (int i = 0; i < m_game_state.player->get_bullet_cap(); i++) {
        m_game_state.bullets[i] =  Entity(bullet_texture_id, BULLET_SPEED, BULLET_WIDTH, BULLET_HEIGHT, BULLET);
        m_game_state.bullets[i].set_sprite_size(glm::vec3(0.5f, 0.5f, 0.0f));
        
        // Set m_game_state.bullets[i].m_render_bullet is false;
    }
    
    /*
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);
    
    int enemy_walking_animation[4][4] = {
        {27, 28, 29}, // left
        {9, 10, 11},  // right
        {0, 1, 2},     // up
        {18, 19, 20}  // down
    };
    
    m_game_state.enemies = new Entity[ENEMY_COUNT];
    
    constexpr float ENEMY_SPEED = 0.25f,
                ENEMY_WIDTH = 0.35f,
                ENEMY_HEIGHT = 0.35f;
    for (int i = 0; i < ENEMY_COUNT; i++) {
        m_game_state.enemies[i] = Entity(
                                         enemy_texture_id,
                                         ENEMY_SPEED + (i * 0.25f),
                                         ENEMY_WIDTH,
                                         ENEMY_HEIGHT,
                                         ENEMY,
                                         GUARD,
                                         IDLE
                                         );
        m_game_state.enemies[i].set_sprite_size(glm::vec3(0.75f, 0.75f, 0.0f));
    }
    // There are 7 enemies
    m_game_state.enemies[0].set_position(glm::vec3(11.0f, -5.0f, 0.0f));
    m_game_state.enemies[1].set_position(glm::vec3(7.0f, -9.0f, 0.0f));
    m_game_state.enemies[2].set_position(glm::vec3(9.0f, -11.0f, 0.0f));
    m_game_state.enemies[3].set_position(glm::vec3(2.0f, -13.0f, 0.0f));
    m_game_state.enemies[4].set_position(glm::vec3(6.0f, -9.0f, 0.0f));
    m_game_state.enemies[5].set_position(glm::vec3(3.0f, -5.0f, 0.0f));
    m_game_state.enemies[6].set_position(glm::vec3(12.0f, -13.0f, 0.0f));
    
    // Draw RELOADING platforms
    GLuint reload_texture_id = Utility::load_texture(RELOAD_FILEPATH);
    
    m_game_state.platforms = new Entity[PLATFORM_COUNT];
    
    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        m_game_state.platforms[i] = Entity(reload_texture_id, 0.0f, 1.0f, 1.0f, PLATFORM); // Speed, hitbox width, height
        m_game_state.platforms[i].set_sprite_size(glm::vec3(1.0f, 1.0f, 0.0f)); // Change size of sprite
    }
    m_game_state.platforms[0].set_position(glm::vec3(2.0f, -3.0f, 0.0f));
    m_game_state.platforms[1].set_position(glm::vec3(6.0f, -12.0f, 0.0f));
    
    // BGM and SFX //
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_game_state.bgm = Mix_LoadMUS("assets/background_music.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4.0f);
    m_game_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
}

void LevelA::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    // Test
    // Update bullet movements, set render status if they collided with map or platform
    for (int i = 0; i < m_game_state.player->get_bullet_cap(); i++) {
        m_game_state.bullets[i].update(delta_time,
                                       m_game_state.player,
                                       m_game_state.platforms,
                                       PLATFORM_COUNT,
                                       m_game_state.map
                                       );
        // Bullet collided with the map!
        if (m_game_state.bullets[i].get_collided_with_map()) {
            m_game_state.bullets[i].set_render_bullet(false);
            m_game_state.bullets[i].uncollide_with_map();
            m_game_state.bullets[i].uncollide_with_entity();
            std::cout << "Bullet " << i << " set to false & collided with map " << std::endl;
        }
        // It collided with *something*
        else if (m_game_state.bullets[i].get_collided_with() != nullptr) {
            m_game_state.bullets[i].set_render_bullet(false);
            m_game_state.bullets[i].uncollide_with_map();
            m_game_state.bullets[i].uncollide_with_entity();
        }
    }
    // Update enemy movements
    float new_health_calculation;
    for (int i = 0; i < ENEMY_COUNT; i++) {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, m_game_state.player, 1, m_game_state.map);
        // Have enemies change direction
        if (m_game_state.enemies[i].check_collision(m_game_state.player)) {
            std::cout << "Enemy " << i << " collided with player " << std::endl;
            if (m_game_state.player->get_curr_health() - damage_factor <= 0.0f) {
                new_health_calculation = 0.0f;
                lose = true;
            } else {
                new_health_calculation = m_game_state.player->get_curr_health() - damage_factor;
            }
            m_game_state.player->set_curr_health(new_health_calculation);
        }
        // Check if enemy has been hit by a bullet
        for (int j = 0; j < m_game_state.player->get_bullet_cap(); j++) {
            if (m_game_state.enemies[i].check_collision(&m_game_state.bullets[j])) {
                std::cout << "Enemy " << i << " collided with bullet " << j << std::endl;
                m_game_state.bullets[i].set_render_bullet(false);
                m_game_state.bullets[i].uncollide_with_entity();
                m_game_state.bullets[i].uncollide_with_map();
                m_game_state.enemies[i].set_render(false);
                m_game_state.enemies[i].deactivate();
            }
        }
        
    }
    
    // Test
    //    // Update enemy movements
    //    for (int i = 0; i < ENEMY_COUNT; i++) {
    //        m_game_state.enemies[i].update(
    //                                       delta_time,
    //                                       m_game_state.player,
    //                                       m_game_state.bullets,
    //                                       m_game_state.player->get_bullet_cap(),
    //                                       m_game_state.map
    //                                       );
    //
    //        // Collided with a non-map object (e.g. Player or Bullet)
    //        if (m_game_state.enemies[i].get_collided_with() != nullptr && !m_game_state.enemies[i].get_collided_with_map()) {
    //            std::cout << "Bullet " << i << " set to false & collided with enemy " << i << std::endl;
    //            m_game_state.bullets[i].uncollide_with_entity();
    //            m_game_state.bullets[i].uncollide_with_map();
    //            m_game_state.enemies[i].deactivate();
    //            m_game_state.bullets[i].set_render_bullet(false);
    //        }
    //    }
    
    // Update bullet movements, set render status if they collided with map or enemy
    //    for (int i = 0; i < m_game_state.player->get_bullet_cap(); i++) {
    //        m_game_state.bullets[i].update(delta_time,
    //                                       m_game_state.player,
    //                                       m_game_state.enemies,
    //                                       ENEMY_COUNT,
    //                                       m_game_state.map
    //                                       );
    //        // Bullet collided with the map or enemy!
    //        if (m_game_state.bullets[i].get_collided_with_map()) {
    //            m_game_state.bullets[i].set_render_bullet(false);
    //            m_game_state.bullets[i].uncollide_with_map();
    //            m_game_state.bullets[i].uncollide_with_entity();
    //            std::cout << "Bullet " << i << " set to false & collided with map " << std::endl;
    //        }
    //        // Collided with enemy
    //        else if (m_game_state.bullets[i].get_collided_with_enemy()) {
    //            m_game_state.bullets[i].set_render_bullet(false);
    //            m_game_state.bullets[i].uncollide_with_map();
    //            m_game_state.bullets[i].uncollide_with_entity();
    //            enemies_killed++;
    //            std::cout << "Bullet " << i << " set to false & collided with enemy " << std::endl;
    //        }
    
    // Check if Level is complete
    // Count # of enemies not rendered (aka # of enemies killed)
    int inactive = 0;
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (!m_game_state.enemies[i].get_is_active()) {
            enemies_killed = inactive;
            inactive++;
        }
    }
//    std::cout << "# of inactive " << inactive << std::endl;
    if (inactive == ENEMY_COUNT) {
        complete = true;
    }
    
    
    // Check for player collision with platform
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        m_game_state.platforms[i].update(delta_time, m_game_state.player, m_game_state.player, 1, m_game_state.map);
        if (m_game_state.player->check_collision(&m_game_state.platforms[i])) {
            m_game_state.player->set_bullet_curr(m_game_state.player->get_bullet_cap()); // Reload
        }
    }
//    std::cout << " A lose is: " << lose << std::endl;
}




void LevelA::render(ShaderProgram *g_shader_program)
{
    glClearColor(0.8f, 0.7f, 1.0f, 1.0f); // Background color
    
    m_game_state.map->render(g_shader_program);
    
    m_game_state.player->render(g_shader_program);
    
    // Render bullets
    for (int i = 0; i < m_game_state.player->get_bullet_cap(); i++) {
        if (m_game_state.bullets[i].get_render_bullet()) {
            m_game_state.bullets[i].set_animation_indices(nullptr);
            m_game_state.bullets[i].render(g_shader_program);
        }
    }
    
    // Render enemies only if active
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (m_game_state.enemies[i].get_is_active()) {
            m_game_state.enemies[i].render(g_shader_program);
        } else {
            m_game_state.enemies[i].set_position(glm::vec3(50.0f, 50.0f, 0.0f)); // Move them off the screen
        }
    }
    
    // Render reloading platforms
    for (int i = 0; i < PLATFORM_COUNT; i++) {
//        std::cout << "Rendering reloading platform " << i << " " << m_game_state.platforms[i].get_position().x << " " << m_game_state.platforms[i].get_position().y << std::endl;
        m_game_state.platforms[i].render(g_shader_program);
    }
    
    // Display ammo count
    constexpr float CHAR_SIZE = 0.25f,
                    CHAR_SPACING = 0.05f;
    glm::vec3 AMMO_POSITION = glm::vec3(
                                        m_game_state.player->get_position().x + 3.60f,
                                        m_game_state.player->get_position().y + 3.0f,
                                        0.0f
                                        );
    glm::vec3 CHAR_POSITION = glm::vec3(
                                        m_game_state.player->get_position().x + 1.60f,
                                        m_game_state.player->get_position().y + 3.0f,
                                        0.0f
                                        );
    GLuint font_texture_id = Utility::load_texture(FONT_FILEPATH);
    std::string DISPLAY_TEXT =
                            std::to_string(m_game_state.player->get_bullet_curr()) + '/'
                            + std::to_string(m_game_state.player->get_bullet_cap());
    
    
    
    // Draw position (for testing)
    std::string DISPLAY_TEXT_2 =
                            std::to_string(m_game_state.player->get_position().x) + ','
                            + std::to_string(m_game_state.player->get_position().y);
    glm::vec3 CHAR_POSITION_2 = glm::vec3(
                                        m_game_state.player->get_position().x - 1.0f,
                                        m_game_state.player->get_position().y,
                                        0.0f
                                        );
    
    
    glm::vec3 SCORE_POS = glm::vec3(
                                        m_game_state.player->get_position().x - 4.60f,
                                        m_game_state.player->get_position().y + 3.0f,
                                        0.0f
                                        );
    int score_count = 0;
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (!m_game_state.enemies[i].get_is_active()) {
            score_count++;
        }
    }
    std::string SCORE_TEXT = "Score: " + std::to_string(score_count) + "/" + std::to_string(ENEMY_COUNT);
    
    
    glm::vec3 HEALTH_POSITION = glm::vec3(
                                        m_game_state.player->get_position().x - 4.85f,
                                        m_game_state.player->get_position().y - 3.30f,
                                        0.0f
                                        );
    std::string HEALTH_TEXT =
                            "Health: " +
                            std::to_string((int) m_game_state.player->get_curr_health()) + '/'
                            + std::to_string((int) m_game_state.player->get_max_health());
    // Check if player lost
    if (m_game_state.player->get_curr_health() <= 0) {
        Utility::draw_text(g_shader_program, font_texture_id, "You lose!", CHAR_SIZE, CHAR_SPACING, CHAR_POSITION); // Lose message
        if (complete) complete = false;
    } else {
        Utility::draw_text(g_shader_program, font_texture_id, DISPLAY_TEXT, CHAR_SIZE, CHAR_SPACING, AMMO_POSITION); // Ammo
        Utility::draw_text(g_shader_program, font_texture_id, HEALTH_TEXT, CHAR_SIZE, CHAR_SPACING, HEALTH_POSITION); // Health
        // User pos for testing
//        Utility::draw_text(g_shader_program, font_texture_id, DISPLAY_TEXT_2, CHAR_SIZE, CHAR_SPACING, CHAR_POSITION_2);
        Utility::draw_text(g_shader_program, font_texture_id, SCORE_TEXT, CHAR_SIZE, CHAR_SPACING, SCORE_POS); // Score
    }
    
}
            
    

