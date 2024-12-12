#include "LevelB.h"
#include "Utility.h"
#define LOG(argument) std::cout << argument << '\n'
#define LEVEL_WIDTH 25
#define LEVEL_HEIGHT 16



constexpr char SPRITESHEET_FILEPATH[] = "assets/Hero.png",
            VINE_FILEPATH[] = "assets/jungle_gate.png",
            FIRE_FILEPATH[] = "assets/fire_wall.png",
            ICE_FILEPATH[] = "assets/ice_wall.png",
           PLATFORM_FILEPATH[]    = "assets/greenPipe.png",
           ENEMY_FILEPATH[]       = "assets/goomba.png",
            MONKEY_BANANA_FILEPATH[] = "assets/monkeywin.png",
            FIRE_BULLET_FILEPATH[] = "assets/fire.png",
            WATER_BULLET_FILEPATH[] = "assets/water.png",
            ROCK_BULLET_FILEPATH[] = "assets/rocks.png",
            SKULL_BULLET_FILEPATH[] = "assets/fruit_banana.png", // Changed a skull into a banana
            FONT_FILEPATH[] = "assets/font1.png",
            STAR_FILEPATH[] = "assets/star.png",
            SPIKES_FILEPATH[] = "assets/red_spikes.png",
            BIG_DOOR_FILEPATH[] = "assets/door.png",
            BEAR_TRAP_FILEPATH[] = "assets/beartrap.png",
            GOAL_FILEPATH[] = "assets/peach.png";


unsigned int LEVEL_DATA_B[] =
{
    37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,
    37, 0,  0,  0,  37, 37, 0,  37, 0,  0,  0,  37, 0,  0,  0,  37, 37, 0,  0,  0,  37, 37, 37, 37, 37,
    37, 0,  37, 0,  0,  37, 0,  37, 0,  37, 0,  37, 0,  37, 0,  0,  0,  0,  37, 37, 0,  0,  0,  0,  37,
    37, 0,  37, 37, 0,  37, 0,  0,  0,  37, 0,  37, 0,  37, 37, 37, 37, 0,  37, 37, 0,  37, 37, 0,  37,
    37, 0,  0,  0,  0,  0,  0,  37, 0,  37, 0,  0,  0,  0,  0,  0,  37, 0,  0,  37, 0,  0,  0,  0,  37,
    37, 37, 37, 37, 37, 0,  37, 37, 0,  37, 37, 37, 37, 37, 37, 37, 37, 0,  37, 37, 0,  37, 37, 37, 37,
    37, 0,  0,  0,  37, 0,  0,  37, 0,  37, 0,  0,  0,  0,  0,  0,  37, 0,  0,  37, 0,  0,  0,  37, 37,
    37, 0,  37, 0,  37, 37, 0,  37, 0,  37, 0,  37, 37, 37, 37, 37, 37, 37, 0,  37, 0,  37, 0,  37, 37,
    37, 0,  37, 0,  0,  37, 0,  37, 0,  37, 0,  0,  0,  0,  0,  0,  0,  0,  0,  37, 0,  0,  0,  0,  37,
    37, 37, 37, 37, 0,  37, 0,  37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 0,  37, 37, 37, 37, 0,  37,
    37, 0,  0,  37, 0,  0,  0,  0,  0,  37, 0,  0,  0,  0,  0,  37, 0,  0,  0,  37, 0,  0,  37, 0,  37,
    37, 0,  37, 37, 37, 37, 37, 37, 0,  37, 37, 37, 37, 37, 0,  37, 37, 37, 37, 37, 37, 0,  37, 0,  37,
    37, 0,  37, 0,  0,  0,  0,  0,  0,  0,  0,  0,  37, 37, 0,  0,  0,  0,  0,  0,  37, 0, 0,  0,  37,
    37, 0,  37, 0,  37, 37, 37, 37, 37, 37, 37, 0,  37, 37, 37, 37, 37, 37, 37, 0,  37, 0,  37, 37, 37,
    37, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  37,
    37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37, 37,

};



void LevelB::refresh_bullet_textures(GLuint touched_texture_id, std::string new_bullet_filepath_string) {

    for (int i = 0; i < m_game_state.player->get_bullet_cap(); i++) {
        Entity bullet_obj = m_game_state.bullets[i];
        m_game_state.bullets[i] = Entity(touched_texture_id, bullet_obj.get_speed(), bullet_obj.get_width(), bullet_obj.get_height(), BULLET);
        m_game_state.bullets[i].set_sprite_size(glm::vec3(0.5f, 0.5f, 0.0f));
        m_game_state.bullets[i].set_bullet_filepath_string(new_bullet_filepath_string);
    }
}

LevelB::~LevelB()
{
    delete [] m_game_state.enemies;
    delete [] m_game_state.bullets;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelB::initialise()
{
    m_game_state.next_scene_id = 3;
    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA_B, map_texture_id, 1.0f, 8, 8);
    
    // Player initialization
    
    int player_walking_animation[4][4] =
    {
        { 8, 9, 10, 11 },    //left
        { 12, 13, 14, 15 },  // right
        { 0, 1, 2, 3 },      //up
        { 4, 5, 6, 7 }       //down
    };
    
    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f); // - 4.81 y
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    
    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        4.5f,                      // speed
        acceleration,              // acceleration
        0.0,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        8,                         // animation column amount
        3,                         // animation row amount
        0.20f,                      // width
        0.20f,                       // height
        PLAYER
        );
    m_game_state.player->set_position(glm::vec3(2.0f, -4.0f, 0.0f)); // Initialize player position
    m_game_state.player->set_sprite_size(glm::vec3(0.5f, 0.5f, 0.0f)); // Change size of sprite
    m_game_state.player->set_max_health(100.0f);
    m_game_state.player->set_curr_health(100.0f);
    
    // Give player "infinite" ammo
    constexpr float BULLET_SPEED = 4.0f,
                BULLET_WIDTH = 0.5f,
                BULLET_HEIGHT = 0.5f;
    GLuint bullet_texture_id = Utility::load_texture(SKULL_BULLET_FILEPATH);
    m_game_state.player->set_bullet_cap(10);
    m_game_state.player->set_bullet_curr(m_game_state.player->get_bullet_cap());
    m_game_state.bullets = new Entity[m_game_state.player->get_bullet_cap()];
    for (int i = 0; i < m_game_state.player->get_bullet_cap(); i++) {
        m_game_state.bullets[i] =  Entity(bullet_texture_id, BULLET_SPEED, BULLET_WIDTH, BULLET_HEIGHT, BULLET);
        m_game_state.bullets[i].set_sprite_size(glm::vec3(0.5f, 0.5f, 0.0f));
        m_game_state.bullets[i].set_bullet_filepath_string(SKULL_BULLET_FILEPATH); // a string
        m_game_state.bullets[i].set_entity_type(BULLET);
    }

    // Platform initializations
    // Wall textures
    GLuint vine_wall_texture_id = Utility::load_texture(VINE_FILEPATH);
    GLuint fire_wall_texture_id = Utility::load_texture(FIRE_FILEPATH);
    GLuint ice_wall_texture_id = Utility::load_texture(ICE_FILEPATH);
    // Red spike texture id
    GLuint red_spike_texture_id = Utility::load_texture(SPIKES_FILEPATH);
    // Bear trap texture id
    GLuint bear_trap_texture_id = Utility::load_texture(BEAR_TRAP_FILEPATH);
    // Bullet selection textures
    GLuint skull_bullet_texture_id = Utility::load_texture(SKULL_BULLET_FILEPATH);
    GLuint fire_bullet_texture_id = Utility::load_texture(FIRE_BULLET_FILEPATH);
    GLuint water_bullet_texture_id = Utility::load_texture(WATER_BULLET_FILEPATH);
    GLuint rock_bullet_texture_id = Utility::load_texture(ROCK_BULLET_FILEPATH);

    
    // First 3 spike platforms ignore (?) They do not render ?
    m_game_state.platforms = new Entity[PLATFORM_COUNT];
    for (int i = 0; i < 3; i++) {
        m_game_state.platforms[i] =  Entity(red_spike_texture_id, 0.5f, 0.0f, 0.0f, PLATFORM); // Spike
        m_game_state.platforms[i].set_sprite_size(glm::vec3(2.0f, 2.0f, 0.0f));
        m_game_state.platforms[i].set_render(false);
    }
    
    // Position the spike platforms (Ignored)
    m_game_state.platforms[0].set_position(glm::vec3(-50.0f, -50.0f, 0.0f));
    m_game_state.platforms[1].set_position(glm::vec3(-50.0f, -50.0f, 0.0f));
    m_game_state.platforms[2].set_position(glm::vec3(-50.0f, -50.0f, 0.0f));
    
    // Create the bullet selector platforms
    m_game_state.platforms[3] =  Entity(skull_bullet_texture_id, 0.5f, 1.0f, 1.0f, PLATFORM); // Skull Bullet Selector
    m_game_state.platforms[4] =  Entity(fire_bullet_texture_id, 0.5f, 1.0f, 1.0f, PLATFORM); // Fire Bullet Selector
    m_game_state.platforms[5] =  Entity(rock_bullet_texture_id, 0.5f, 1.0f, 1.0f, PLATFORM); // Rock Bullet Selector
    
    // Position the bullet selector platforms
    m_game_state.platforms[3].set_position(glm::vec3(22.0f, -2.0f, 0.0f)); // Skull
    m_game_state.platforms[4].set_position(glm::vec3(16.0f, -10.0f, 0.0f)); // Fire
    m_game_state.platforms[5].set_position(glm::vec3(8.0f, -8.0f, 0.0f)); // Rock
    
    // Modify the sizing of the bullet selectors
    for (int i = 3; i < 6; i++) // Position the bullet selectors
    {
        m_game_state.platforms[i].set_sprite_size(glm::vec3(1.0f, 1.0f, 0.0f));
        m_game_state.platforms[i].update(0.0f, m_game_state.player, m_game_state.player, 1, m_game_state.map);
    }
    
    // Create spike (beartrap) platforms [6 - 15]
    
    for (int i = 6; i < 16; i++) {
        m_game_state.platforms[i] =  Entity(bear_trap_texture_id, 0.5f, 1.0f, 1.0f, PLATFORM); // Spike
        m_game_state.platforms[i].set_sprite_size(glm::vec3(1.0f, 1.0f, 0.0f));
    }
    // Position the spike (beartrap) platforms
    m_game_state.platforms[6].set_position(glm::vec3(17.0f, -4.0f, 0.0f));
    m_game_state.platforms[7].set_position(glm::vec3(10.0f, -3.0f, 0.0f));
    m_game_state.platforms[8].set_position(glm::vec3(17.0f, -8.0f, 0.0f));
    m_game_state.platforms[9].set_position(glm::vec3(1.0f, -11.0f, 0.0f));
    m_game_state.platforms[10].set_position(glm::vec3(19.0f, -14.0f, 0.0f));
    m_game_state.platforms[11].set_position(glm::vec3(6.0f, -8.0f, 0.0f));
    m_game_state.platforms[12].set_position(glm::vec3(8.0f, -14.0f, 0.0f));
    m_game_state.platforms[13].set_position(glm::vec3(14.0f, -14.0f, 0.0f));
    m_game_state.platforms[14].set_position(glm::vec3(21.0f, -14.0f, 0.0f));
    m_game_state.platforms[15].set_position(glm::vec3(20.0f, -5.0f, 0.0f));


    // Create door
    GLuint big_door_texture_id = Utility::load_texture(BIG_DOOR_FILEPATH);
    m_game_state.platforms[16] = Entity(big_door_texture_id, 0.5f, 2.0f, 2.0f, PLATFORM); // Door
    m_game_state.platforms[16].set_sprite_size(glm::vec3(2.0f, 2.0f, 0.0f));
    m_game_state.platforms[16].set_position(glm::vec3(2.0f, -13.5f, 0.0f));
    
    // Create goal star trophy
    GLuint star_texture_id = Utility::load_texture(STAR_FILEPATH);
    m_game_state.platforms[17] = Entity(star_texture_id, 0.5f, 1.0f, 1.0f, PLATFORM); // Star trophy
    m_game_state.platforms[17].set_sprite_size(glm::vec3(1.0f, 1.0f, 0.0f));
    m_game_state.platforms[17].set_position(glm::vec3(2.0f, -10.0f, 0.0f));
    
    // Create walker AI enemies
    /* Enemies' stuff */
    // Monkey banana shooters
    
    GLuint monkey_banana_texture_id = Utility::load_texture(MONKEY_BANANA_FILEPATH);
    constexpr float ENEMY_SPEED = 0.250f,
                ENEMY_WIDTH = 0.5f,
                ENEMY_HEIGHT = 0.5f;
    m_game_state.enemies = new Entity[ENEMY_COUNT];
    
    int monkey_walking_animation[4][4] =
    {
        { 0, 1, 2, 3 },    //left
        { 0, 1, 2, 3 },  // right
        { 0, 1, 2, 3 },      //up
        { 0, 1, 2, 3 }       //down
    };
    
    for (int i = 0; i < ENEMY_COUNT; i++) {
        m_game_state.enemies[i] = Entity(
            monkey_banana_texture_id,         // texture id
            ENEMY_SPEED,                      // speed
            ENEMY_WIDTH,                      // width
            ENEMY_HEIGHT,                       // height
            ENEMY
            );
        m_game_state.enemies[i].set_ai_type(WALKER);
        m_game_state.enemies[i].set_ai_state(WALKING);
        m_game_state.enemies[i].set_position(glm::vec3(3.0f + i, -4.0f, 0.0f));
        m_game_state.enemies[i].set_sprite_size(glm::vec3(0.75f, 0.75f, 0.0f));
    }
    // Set enemy positions
    m_game_state.enemies[0].set_position(glm::vec3(5.0f, -4.0f, 0.0f));
    m_game_state.enemies[1].set_position(glm::vec3(6.0f, -10.0f, 0.0f));
    m_game_state.enemies[2].set_position(glm::vec3(7.0f, -12.0f, 0.0f));
    m_game_state.enemies[3].set_position(glm::vec3(22.0f, -8.0f, 0.0f));
    m_game_state.enemies[4].set_position(glm::vec3(2.0f, -6.0f, 0.0f));
    
    // Create collidables array (Enemies & Certain platforms)
    collidables = new Entity[ENEMY_COUNT + 1]; // Enemy count + 1 for the door
    for (int i = 0; i < ENEMY_COUNT; i++) {
        collidables[i] = m_game_state.enemies[i];
    }
    collidables[ENEMY_COUNT + 1 - 1] = m_game_state.platforms[16]; // The door
    
    /* BGM and SFX */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_game_state.bgm = Mix_LoadMUS("assets/background_music.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4.0f);

    m_game_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
}


void LevelB::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    
    // Door platform [16]
    m_game_state.platforms[16].update(delta_time, m_game_state.player, m_game_state.bullets, m_game_state.player->get_bullet_cap(), m_game_state.map);
    if (m_game_state.player->check_collision(&m_game_state.platforms[16])) { // Insta kill if contact with door
//        std::cout << "Collided with door, insta kill" << std::endl;
        m_game_state.player->set_curr_health(0);
    }
    // Star trophy
    m_game_state.platforms[17].update(delta_time, m_game_state.player, m_game_state.player, 1, m_game_state.map);
    if (m_game_state.player->check_collision(&m_game_state.platforms[17])) { // Star is touched
        bool all_enemies_dead = true;
        for (int i = 0; i < ENEMY_COUNT; i++) { // Enemies are dead
            if (m_game_state.enemies[i].get_render()) {
                all_enemies_dead = false;
            }
        }
        if (all_enemies_dead) { // Star is touched + Enemies are killed
            complete = true;
//            std::cout << "Level B complete" << std::endl;
        }
    }
    float new_health_calculation;
    
    // Update enemy movements
    for (int i = 0; i < ENEMY_COUNT; i++) {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, m_game_state.player, 1, m_game_state.map);
        // Decrement health if the player hits an enemy
        if (m_game_state.player->check_collision(&m_game_state.enemies[i])) {
            new_health_calculation = (m_game_state.player->get_curr_health() - damage_factor <= 0.0f ? 0.0f : m_game_state.player->get_curr_health() - damage_factor);
            if (m_game_state.player->get_curr_health() - damage_factor <= 0.0f) { // Level B lost
                new_health_calculation = 0.0f;
                lose = true;
            } else {
                new_health_calculation = m_game_state.player->get_curr_health() - damage_factor;
            }
            m_game_state.player->set_curr_health(new_health_calculation);
            m_game_state.player->set_curr_health(new_health_calculation);
        }
        // Have enemies change direction
        if (m_game_state.enemies[i].get_collided_right()) {
            m_game_state.enemies[i].set_walk_left(true);
        } else if (m_game_state.enemies[i].get_collided_left()) {
            m_game_state.enemies[i].set_walk_left(false);
        }
        // Check if enemy has been hit by a bullet
        for (int j = 0; j < m_game_state.player->get_bullet_cap(); j++) {
            if (m_game_state.enemies[i].check_collision(&m_game_state.bullets[j])) {
//                std::cout << "Enemy " << i << " collided with bullet " << j << std::endl;
                m_game_state.bullets[i].set_render_bullet(false);
                m_game_state.bullets[i].uncollide_with_entity();
                m_game_state.bullets[i].uncollide_with_map();
                m_game_state.enemies[i].set_render(false);
                m_game_state.enemies[i].deactivate();
            }
        }

    }
    
    // Update bullet movements, set render status if they collided with map or platform
    for (int i = 0; i < m_game_state.player->get_bullet_cap(); i++) {
        m_game_state.bullets[i].update(delta_time,
                                       m_game_state.player,
                                       m_game_state.platforms,
                                       PLATFORM_COUNT,
                                       m_game_state.map
                                       );
        // Fire bullet collided with the door & it hasn't been deleted already!
        if (m_game_state.bullets[i].get_collided_with() == (&m_game_state.platforms[16]) && m_game_state.bullets[i].get_bullet_filepath_string() == FIRE_BULLET_FILEPATH &&
            m_game_state.platforms[16].get_render()) {
//            std::cout << "Bullet " << i << " collided with the door " << std::endl;
            m_game_state.platforms[16].set_render(false); // Delete the door
            m_game_state.platforms[16].uncollide_with_map();
            m_game_state.platforms[16].uncollide_with_entity();
            m_game_state.bullets[i].set_render_bullet(false); // Delete the bullet
            m_game_state.bullets[i].uncollide_with_map();
            m_game_state.bullets[i].uncollide_with_entity();
        }
        // Bullet collided with the map!
        if (m_game_state.bullets[i].get_collided_with_map()) {
            m_game_state.bullets[i].set_render_bullet(false);
            m_game_state.bullets[i].uncollide_with_map();
            m_game_state.bullets[i].uncollide_with_entity();
//            std::cout << "Bullet " << i << " set to false & collided with map " << std::endl;
        }
        // If not map, then collided w/ platform & it is a rock bullet
        else if (m_game_state.bullets[i].get_collided_with() != nullptr && m_game_state.bullets[i].get_bullet_filepath_string() == ROCK_BULLET_FILEPATH && m_game_state.bullets[i].get_collided_with() != &m_game_state.platforms[16]) { // If not map, then collided w/ bear trap platform & it is a rock bullet & the platform is not the door
            m_game_state.bullets[i].set_render_bullet(false);
            m_game_state.bullets[i].get_collided_with()->set_render(false);
            m_game_state.bullets[i].set_position(glm::vec3(-50.0f, -50.0f, 0.0f));
            m_game_state.bullets[i].uncollide_with_map();
            m_game_state.bullets[i].uncollide_with_entity();
            for (int j = 6; j < 16; j++) {
                if (m_game_state.platforms[j].check_collision(&m_game_state.bullets[i])) {
//                    std::cout << i << " Platform render is false" << std::endl;
                    m_game_state.platforms[j].set_render(false);
                    m_game_state.platforms[j].set_position(glm::vec3(-50.0f, -50.0f, 0.0f));
                }
            }
            
//            std::cout << "Rock bullet collided with bear trap platform, deleted bullet & beartrap platform" << std::endl;
        }
        // It collided with *something*
        else if (m_game_state.bullets[i].get_collided_with() != nullptr) {
            m_game_state.bullets[i].set_render_bullet(false);
            m_game_state.bullets[i].uncollide_with_map();
            m_game_state.bullets[i].uncollide_with_entity();
        }
    }
        
    // Spike (beartrap) platforms (0, 3 or 6, 15)
    for (int i = 6; i < 15; i++) {
        m_game_state.platforms[i].update(delta_time, m_game_state.player, m_game_state.bullets, m_game_state.player->get_bullet_cap(), m_game_state.map);
        if (m_game_state.player->check_collision(&m_game_state.platforms[i])) {
            m_game_state.player->set_curr_health(m_game_state.player->get_curr_health() - damage_factor);
//            std::cout << "Bear trap " << i << " collision detected, health at: " << m_game_state.player->get_curr_health() << std::endl;
        }
    }
    
    
    // Update bullet texture based on which platform was selected
    for (int i = 3; i < 6; i++) {
        m_game_state.bullets[i].update(
                                       delta_time,
                                       m_game_state.player,
                                       m_game_state.player,
                                       1,
                                       m_game_state.map
                                   );
        if (m_game_state.player->check_collision(&m_game_state.platforms[i]) && i == 3) { // Vine bullet selector platform
            refresh_bullet_textures(Utility::load_texture(SKULL_BULLET_FILEPATH), SKULL_BULLET_FILEPATH);
        }
        else if (m_game_state.player->check_collision(&m_game_state.platforms[i]) && i == 4) { // Fire bullet selector platform
            refresh_bullet_textures(Utility::load_texture(FIRE_BULLET_FILEPATH), FIRE_BULLET_FILEPATH);
        }
        else if (m_game_state.player->check_collision(&m_game_state.platforms[i]) && i == 5) { // Rock bullet selector platform
            refresh_bullet_textures(Utility::load_texture(ROCK_BULLET_FILEPATH), ROCK_BULLET_FILEPATH);
        }
    }

    // Reload ammo if player runs out
    if (m_game_state.player->get_bullet_curr() <= 0) {
        m_game_state.player->set_bullet_curr(m_game_state.player->get_bullet_cap()); // Reload
    }

//    std::cout << " B lose is: " << lose << std::endl;
}
    

    
    
void LevelB::render(ShaderProgram *g_shader_program)
{
    m_number_of_enemies = ENEMY_COUNT;
    m_game_state.map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    
    glClearColor(0.6f, 0.8f, 0.6f, 1.0f); // pastel green
    
    // Render each enemy entity
    for (int i = 0; i < m_number_of_enemies; i++)
        if (m_game_state.enemies[i].get_is_active()) {
            m_game_state.enemies[i].render(g_shader_program);
        } else {
            m_game_state.enemies[i].set_position(glm::vec3(-50.0f, -50.0f, 0.0f));
        }
    
    // Render the platforms (spikes & door)
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        if (m_game_state.platforms[i].get_render()) {
            m_game_state.platforms[i].render(g_shader_program);
        } else if (i == 16 && !m_game_state.platforms[i].get_render()) {
//            std::cout << "Moving platform " << i << " to new location" << std::endl;
            m_game_state.platforms[16].set_position(glm::vec3(-50.0f, -50.0f, 0.0f)); // Move platform off the map
        } else if (i >= 6 && i <= 15 && !m_game_state.platforms[i].get_render()) {
//            std::cout << "Moving " << i << " platform off the map " << std::endl;
            m_game_state.platforms[i].set_position(glm::vec3(-50.0f, -50.0f, 0.0f)); // Move platform off the map
        }
    }
    
    // Render bullets
    for (int i = 0; i < m_game_state.player->get_bullet_cap(); i++) {
        if (m_game_state.bullets[i].get_render_bullet()) {
            m_game_state.bullets[i].set_animation_indices(nullptr);
            m_game_state.bullets[i].render(g_shader_program);
        }
    }
    // Render enemies
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (m_game_state.enemies[i].get_render()) {
            m_game_state.enemies[i].render(g_shader_program);
        } else {
            m_game_state.enemies[i].set_position(glm::vec3(-50.0f, -50.0f, 0.0f)); // Move off the map
        }
    }
    
    // Render star trophy
    m_game_state.platforms[17].render(g_shader_program);
    
    // Draw position (for testing)
    constexpr float CHAR_SIZE = 0.25f,
    CHAR_SPACING = 0.05f;
    GLuint font_texture_id = Utility::load_texture(FONT_FILEPATH);
    std::string DISPLAY_TEXT_2 =
    std::to_string(m_game_state.player->get_position().x) + ','
    + std::to_string(m_game_state.player->get_position().y);
    glm::vec3 CHAR_POSITION_2 = glm::vec3(
                                          m_game_state.player->get_position().x - 1.0f,
                                          m_game_state.player->get_position().y,
                                          0.0f
                                          );
    
    
    // Draw health percentage
    
    glm::vec3 CHAR_POSITION = glm::vec3(
                                        m_game_state.player->get_position().x - 4.60f,
                                        m_game_state.player->get_position().y + 3.0f,
                                        0.0f
                                        );
    std::string DISPLAY_TEXT =
                            "Health: " +
                            std::to_string((int) m_game_state.player->get_curr_health()) + '/'
                            + std::to_string((int) m_game_state.player->get_max_health());
    
    
    
    // Check if player lost
    if (m_game_state.player->get_curr_health() <= 0.0f) {
        Utility::draw_text(g_shader_program, font_texture_id, "You lose!", CHAR_SIZE, CHAR_SPACING, CHAR_POSITION);
        if (complete) complete = false;
    } else { // Game ongoing
//        Utility::draw_text(g_shader_program, font_texture_id, DISPLAY_TEXT_2, CHAR_SIZE, CHAR_SPACING, CHAR_POSITION_2); // Pos
        Utility::draw_text(g_shader_program, font_texture_id, DISPLAY_TEXT, CHAR_SIZE, CHAR_SPACING, CHAR_POSITION); // HP
    }
}

