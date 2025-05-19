#include "globals.h"
#include "player.h"
#include  "player_manager.h"
#include  "enemies_manager.h"
#include "level.h"
#include "level_manager.h"

#include "player.h"
void PlayerController::reset_player_stats() {
    player_lives = MAX_PLAYER_LIVES;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        player_level_scores[i] = 0;
    }
}

void PlayerController::increment_player_score() {
    PlaySound(coin_sound);
    player_level_scores[level_index]++;
}

int PlayerController::get_total_player_score() {
    int sum = 0;

    for (int i = 0; i < LEVEL_COUNT; i++) {
        sum += player_level_scores[i];
    }

    return sum;
}

void PlayerController::spawn_player() {
    player_y_velocity = 0;

    for (size_t row = 0; row < LevelManager::getInstanceLevel().get_current_level().get_rows(); ++row) {
        for (size_t column = 0; column < LevelManager::getInstanceLevel().get_current_level().get_columns(); ++column) {
            char cell = Level::get_level_cell(row, column);;

            if (cell == PLAYER) {
                Player::getInstancePlayer().set_player_posX(column);
                Player::getInstancePlayer().set_player_posY(row);
                LevelManager::getInstanceLevel().set_level_cell(Player::getInstancePlayer().get_player_posX(), Player::getInstancePlayer().get_player_posY(), AIR);
                return;
            }
        }
    }
}

void PlayerController::move_player_horizontally(float delta) {
    // See if the player can move further without touching a wall;
    // otherwise, prevent them from getting into a wall by rounding their position
    float next_x = Player::getInstancePlayer().get_player_posX() + delta;
    if (!LevelManager::getInstanceLevel().is_colliding({next_x, Player::getInstancePlayer().get_player_posY()}, WALL)) {
        Player::getInstancePlayer().set_player_posX(next_x);
    }
    else {
        Player::getInstancePlayer().set_player_posX(roundf(Player::getInstancePlayer().get_player_posX()));
        return;
    }

    // For drawing player animations
    Player::getInstancePlayer().set_is_looking_forward(delta > 0);
    if (delta != 0) Player::getInstancePlayer().set_is_moving(true);
}


void PlayerController::update_player() {
    Player::getInstancePlayer().update_player_gravity();

    // Interacting with other level elements
    if (LevelManager::getInstanceLevel().is_colliding(Player::getInstancePlayer().get_player_pos(), COIN)) {
        LevelManager::getInstanceLevel().get_collider(Player::getInstancePlayer().get_player_pos(), COIN) = AIR; // Removes the coin
        PlayerController::getInstancePlayerController().increment_player_score();
    }

    if (LevelManager::getInstanceLevel().is_colliding(Player::getInstancePlayer().get_player_pos(), EXIT)) {
        // Reward player for being swift
        if (timer > 0) {
            // For every 9 seconds remaining, award the player 1 coin
            timer -= 25;
            time_to_coin_counter += 5;

            if (time_to_coin_counter / 60 > 1) {
                increment_player_score();
                time_to_coin_counter = 0;
            }
        }
        else {
            // Allow the player to exit after the level timer goes to zero
            LevelManager::getInstanceLevel().load_level(1);
            PlaySound(exit_sound);
        }
    }
    else {
        // Decrement the level timer if not at an exit
        if (timer >= 0) timer--;
    }

    // Kill the player if they touch a spike or fall below the level
    if (LevelManager::getInstanceLevel().is_colliding(Player::getInstancePlayer().get_player_pos(), SPIKE) || Player::getInstancePlayer().get_player_posY() > LevelManager::getInstanceLevel().get_current_level().get_rows()) {
        kill_player();
    }

    // Upon colliding with an enemy...
    if (EnemiesManager::getInstance().is_colliding_with_enemies(Player::getInstancePlayer().get_player_pos())) {
        // ...check if their velocity is downwards...
        if (player_y_velocity > 0) {
            // ...if yes, award the player and kill the enemy
            EnemiesManager::getInstance().remove_colliding_enemy(Player::getInstancePlayer().get_player_pos());
            PlaySound(kill_enemy_sound);

            increment_player_score();
            player_y_velocity = -BOUNCE_OFF_ENEMY;
        }
        else {
            // ...if not, kill the player
           kill_player();
        }
    }
}

void PlayerController::draw_player() {
    horizontal_shift = (screen_size.x - cell_size) / 2;

    // Shift the camera to the center of the screen to allow to see what is in front of the player
    Vector2 pos = {
        horizontal_shift,
        Player::getInstancePlayer().get_player_posY() * cell_size
};

    // Pick an appropriate sprite for the player
    if (game_state == GAME_STATE) {
        if (!(Player::getInstancePlayer().is_player_on_ground())) {
            draw_image((Player::getInstancePlayer().is_looking_forward() ? player_jump_forward_image : player_jump_backwards_image), pos, cell_size);
        }
        else if (Player::getInstancePlayer().is_moving()) {
            draw_sprite((Player::getInstancePlayer().is_looking_forward() ? player_walk_forward_sprite : player_walk_backwards_sprite), pos, cell_size);
            Player::getInstancePlayer().set_is_moving(false);
        }
        else {
            draw_image((Player::getInstancePlayer().is_looking_forward() ? player_stand_forward_image : player_stand_backwards_image), pos, cell_size);
        }
    }
    else {
        draw_image(player_dead_image, pos, cell_size);
    }
}
void kill_player() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    player_lives--;
    player_level_scores[level_index] = 0;
}
