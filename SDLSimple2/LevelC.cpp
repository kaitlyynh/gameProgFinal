#include "LevelC.h"
#include "Utility.h"
#define LOG(argument) std::cout << argument << '\n'
#define LEVEL_WIDTH 9
#define LEVEL_HEIGHT 9



constexpr char SPRITESHEET_FILEPATH[] = "assets/Hero.png",
            BED_FILEPATH[] = "assets/bed.png",
            MONKEY_BANANA_FILEPATH[] = "assets/monkeywin.png",
            WEINER_DOG_FILEPATH[] = "assets/weiner_dog.png",
            LAMP_FILEPATH[] = "assets/lamp_on.png",
            FONT_FILEPATH[] = "assets/font1.png";

unsigned int LEVEL_DATA_C[] =
{
    25, 25, 25, 25, 25, 25, 25, 25, 25,
    25,  0,  0,  0,  0,  0,  0,  0, 25,
    25,  0,  0,  0,  0,  0,  0,  0, 25,
    25,  0,  0,  0,  0,  0,  0,  0, 25,
    25,  0,  0,  0,  0,  0,  0,  0, 25,
    25,  22,  0,  22,  22,  22,  0,  0, 25,
    25,  0,  0,  0,  0,  22,  0,  0, 25,
    25,  0,  0,  0,  0,  22,  0,  0, 25,
    25, 25, 25, 25, 25, 25, 25, 25, 25,
};


void LevelC::spawn_animal(std::string desired_filepath) {
    GLuint weiner_dog_texture_id = Utility::load_texture(WEINER_DOG_FILEPATH);
    GLuint monkey_banana_texture_id = Utility::load_texture(MONKEY_BANANA_FILEPATH);
    
    

    Entity* player = m_game_state.player;
    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    // Spawned a dog
    if (desired_filepath == "WEINER_DOG_FILEPATH") {
//        std::cout << "Dog count: " << DOG_COUNT << std::endl;
        if (DOG_COUNT <= 0) { return; }
        for (int i = 0; i < 5; i++) {
            if (!m_game_state.enemies[i].get_render()) {
                AIType rand_ai_type = static_cast<AIType>(rand() % 3);
                float random_value = 0.25f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));
                m_game_state.enemies[i] = Entity(
                                                 weiner_dog_texture_id,         // texture id
                                                 random_value,                      // speed
                                                 0.5f,                      // width
                                                 0.5f,                       // height
                                                 ENEMY
                                                 );
                m_game_state.enemies[i].set_ai_type(rand_ai_type);
                m_game_state.enemies[i].set_ai_state(WALKING);
                m_game_state.enemies[i].set_position(glm::vec3(player->get_position().x, player->get_position().y, 0.0f));
                m_game_state.enemies[i].set_sprite_size(glm::vec3(1.0f, 1.0f, 0.0f));
                m_game_state.enemies[i].set_render(true);
//                std::cout << "Spawning dog " << i << std::endl;
                decrement_dog_count();
                return;
            }
        }
    }
    // Spawned a monkey
    else if (desired_filepath == "MONKEY_BANANA_FILEPATH") {
        if (MONKEY_COUNT <= 0) { return; }
        for (int i = 5; i < PLATFORM_COUNT; i++) {
            AIType rand_ai_type = static_cast<AIType>(rand() % 2);
            float random_value = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2.0f));
            if (!m_game_state.enemies[i].get_render()) {
                m_game_state.enemies[i] = Entity(
                                                 monkey_banana_texture_id,         // texture id
                                                 random_value,                      // speed
                                                 0.5f,                      // width
                                                 0.5f,                       // height
                                                 ENEMY
                                                 );
                m_game_state.enemies[i].set_position(glm::vec3(player->get_position().x, player->get_position().y, 0.0f));
                m_game_state.enemies[i].set_ai_type(rand_ai_type);
                m_game_state.enemies[i].set_ai_state(WALKING);
                m_game_state.enemies[i].set_sprite_size(glm::vec3(0.5f, 0.5f, 0.0f));
                m_game_state.enemies[i].set_render(true);
//                std::cout << "Spawning monkey " << i << std::endl;
                decrement_monkey_count();
                return;
            }
        }
    }
}
LevelC::~LevelC()
{
    delete [] m_game_state.enemies;
    delete [] m_game_state.bullets;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelC::initialise()
{
    m_game_state.next_scene_id = 4; // 4 - > Stop progression
    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA_C, map_texture_id, 1.0f, 8, 8);
    
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
                                     3.0f,                      // speed
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
    // Initialise platforms
    m_game_state.platforms = new Entity[PLATFORM_COUNT];
    // Initialise bed platform
    GLuint bed_texture_id = Utility::load_texture(BED_FILEPATH);
    m_game_state.platforms[0] = Entity(bed_texture_id, 0.0f, 2.0f, 3.0f, PLATFORM); // Width, height, Entity type
    m_game_state.platforms[0].set_sprite_size(glm::vec3(2.0f, 3.0f, 0.0f));
    m_game_state.platforms[0].set_position(glm::vec3(6.5f, -2.0f, 0.0f));
    // Initialise carpet platform
    GLuint lamp_texture_id = Utility::load_texture(LAMP_FILEPATH);
    m_game_state.platforms[1] = Entity(lamp_texture_id, 0.0f, 1.0f, 2.0f, PLATFORM);
    m_game_state.platforms[1].set_sprite_size(glm::vec3(1.0f, 2.0f, 0.0f));
    m_game_state.platforms[1].set_position(glm::vec3(5.0f, -1.5f, 0.0f));
    // Initialise Dog & Monkey icons

    GLuint weiner_dog_texture_id = Utility::load_texture(WEINER_DOG_FILEPATH);
    GLuint monkey_banana_texture_id = Utility::load_texture(MONKEY_BANANA_FILEPATH);
    m_game_state.platforms[2] = Entity(weiner_dog_texture_id, 0.0f, 1.0f, 2.0f, PLATFORM);
    m_game_state.platforms[2].set_sprite_size(glm::vec3(1.0f, 1.0f, 0.0f));
    m_game_state.platforms[3] = Entity(
                                       monkey_banana_texture_id,         // texture id
                                       0.0f,                      // speed
                                       2.0f,                      // width
                                       2.0f,                       // height
                                       PLATFORM
                                       );
    m_game_state.platforms[3].set_sprite_size(glm::vec3(0.5f, 0.5f, 0.0f));
    
    // Initialise enemies to be un-renderable
    m_game_state.enemies = new Entity[ENEMY_COUNT];
    for (int i = 0; i < ENEMY_COUNT; i++) {
        m_game_state.enemies[i] = Entity();
        m_game_state.enemies[i].set_render(false);
    }
    
    /* BGM and SFX */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_game_state.bgm = Mix_LoadMUS("assets/background_music.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4.0f);

    m_game_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
    
    
}


    
void LevelC::update(float delta_time) {
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.platforms, PLATFORM_COUNT, m_game_state.map);
    // Update platforms
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        m_game_state.platforms[i].update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
        // Inventory icons, update position relative to the player
        if (i == 2) { // Dog Icon
            m_game_state.platforms[i].set_position(glm::vec3(m_game_state.player->get_position().x - 3.0f, m_game_state.player->get_position().y + 3.15f, 0.0f));
        } else if (i == 3) { // Monkey Icon
            m_game_state.platforms[i].set_position(glm::vec3(m_game_state.player->get_position().x - 0.5f, m_game_state.player->get_position().y + 2.95f, 0.0f));
        }
    }
    // Update enemies if they are render-able
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (m_game_state.enemies[i].get_render()) {
            m_game_state.enemies[i].update(delta_time, m_game_state.player, nullptr, 0, m_game_state.map);
            if (m_game_state.enemies[i].get_ai_type() == WALKER) {
                if (m_game_state.enemies[i].get_collided_right()) {
                    m_game_state.enemies[i].set_walk_left(true);
                } else if (m_game_state.enemies[i].get_collided_left()) {
                    m_game_state.enemies[i].set_walk_left(false);
                }
            } else if (m_game_state.enemies[i].get_ai_type() == WALKER_V) {
                if (m_game_state.enemies[i].get_collided_top()) {
                    m_game_state.enemies[i].set_walk_up(false);
                } else if (m_game_state.enemies[i].get_collided_bottom()) {
                    m_game_state.enemies[i].set_walk_up(true);
                }
                
            }
        }
        // Check if all items in bag are placed
        if (DOG_COUNT == 0 && MONKEY_COUNT == 0) {
            complete = true;
        }
    }
    
}
    void LevelC::render(ShaderProgram *g_shader_program)
    {
        m_number_of_enemies = ENEMY_COUNT;
        m_game_state.map->render(g_shader_program);
        m_game_state.player->render(g_shader_program);
        
        glClearColor(0.76f, 0.48f, 0.33f, 1.0f); // Lighter reddish-brown (Soft Russet)
        
        // Render platforms

        // Bed platform
        for (int i = 0; i < PLATFORM_COUNT; i++) {
            m_game_state.platforms[i].render(g_shader_program);
        }
        // Render enemy monkey and dogs
        for (int i = 0; i < ENEMY_COUNT; i++) {
            if (m_game_state.enemies[i].get_render()) {
                m_game_state.enemies[i].render(g_shader_program);
            }
        }


        // Draw position (for testing)
        constexpr float CHAR_SIZE = 0.25f,
        CHAR_SPACING = 0.05f;
        GLuint font_texture_id = Utility::load_texture(FONT_FILEPATH);

        // Is game complete?
        std::string DISPLAY_TEXT_2 =
        std::to_string(m_game_state.player->get_position().x) + ','
        + std::to_string(m_game_state.player->get_position().y);
        glm::vec3 CHAR_POSITION_2 = glm::vec3(
                                              m_game_state.player->get_position().x - 1.0f,
                                              m_game_state.player->get_position().y,
                                              0.0f
                                              );
//        Utility::draw_text(g_shader_program, font_texture_id, DISPLAY_TEXT_2, CHAR_SIZE, CHAR_SPACING, CHAR_POSITION_2);

        // Draw inventory
        glm::vec3 INVENTORY_POS = glm::vec3(
                                            m_game_state.player->get_position().x - 4.60f,
                                            m_game_state.player->get_position().y + 3.0f,
                                            0.0f
                                            );

        glm::vec3 DOG_COUNT_POS = glm::vec3(
                                            m_game_state.player->get_position().x - 2.30f,
                                            m_game_state.player->get_position().y + 3.0f,
                                            0.0f
                                            );
        glm::vec3 MONKEY_COUNT_POS = glm::vec3(
                                               m_game_state.player->get_position().x,
                                               m_game_state.player->get_position().y + 3.0f,
                                               0.0f
                                               );
        std::string DISPLAY_TEXT = "Bag: ";

        std::string DOG_COUNT_TEXT = std::to_string(DOG_COUNT) + 'x';
        std::string MONKEY_COUNT_TEXT = std::to_string(MONKEY_COUNT) + 'x';

        // Is game complete?
        glm::vec3 GAME_COMPLETE_POS = glm::vec3(
                                                m_game_state.player->get_position().x - 1.0f,
                                                m_game_state.player->get_position().y + 1.0f,
                                                0.0f
                                                );
        if (complete) {
            Utility::draw_text(g_shader_program, font_texture_id, "You win!", CHAR_SIZE, CHAR_SPACING, MONKEY_COUNT_POS);
        }
        // Is game complete?
        else {
            Utility::draw_text(g_shader_program, font_texture_id, DISPLAY_TEXT, CHAR_SIZE, CHAR_SPACING, INVENTORY_POS);
            Utility::draw_text(g_shader_program, font_texture_id, DOG_COUNT_TEXT, CHAR_SIZE, CHAR_SPACING, DOG_COUNT_POS);
            Utility::draw_text(g_shader_program, font_texture_id, MONKEY_COUNT_TEXT, CHAR_SIZE, CHAR_SPACING, MONKEY_COUNT_POS);

        }
        
    }
