#include "globals.h"
#include "player.h"
#include "player_manager.h"
#include  "enemies_manager.h"
#include "level.h"
#include "level_manager.h"

#include "player.h"
void Player::reset_player_stats() {
    player_lives = MAX_PLAYER_LIVES;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        player_level_scores[i] = 0;
    }
}

void Player::increment_player_score() {
    PlaySound(coin_sound);
    player_level_scores[level_index]++;
}

int Player::get_total_player_score() {
    int sum = 0;

    for (int i = 0; i < LEVEL_COUNT; i++) {
        sum += player_level_scores[i];
    }

    return sum;
}

void Player::spawn_player() {
    player_y_velocity = 0;

    for (size_t row = 0; row < LevelManager::getInstanceLevel().get_current_level().get_rows(); ++row) {
        for (size_t column = 0; column < LevelManager::getInstanceLevel().get_current_level().get_columns(); ++column) {
            char cell = Level::get_level_cell(row, column);;

            if (cell == PLAYER) {
                Player::getInstancePlayer().set_player_posX(column);
                Player::getInstancePlayer().set_player_posX(row);
                LevelManager::getInstanceLevel().set_level_cell(row, column, AIR);
                return;
            }
        }
    }
}

void Player::kill_player() {
    // Decrement a life and reset all collected coins in the current level
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    player_lives--;
    player_level_scores[level_index] = 0;
}

void Player::move_player_horizontally(float delta) {
    // See if the player can move further without touching a wall;
    // otherwise, prevent them from getting into a wall by rounding their position
    float next_x = Player::getInstancePlayer().get_player_posX() + delta;
    if (!LevelManager::getInstanceLevel().is_colliding({next_x, player_pos.y}, WALL)) {
        Player::getInstancePlayer().set_player_posX(next_x);
    }
    else {
        player_pos.x = roundf(player_pos.x);
        return;
    }

    // For drawing player animations
    looks_forward = delta > 0;
    if (delta != 0) moves = true;
}

void Player::update_player_gravity() {
    // Bounce downwards if approaching a ceiling with upwards velocity
    if (LevelManager::getInstanceLevel().is_colliding({player_pos.x, player_pos.x - 0.1f}, WALL) && player_y_velocity < 0) {
        player_y_velocity = CEILING_BOUNCE_OFF;
    }

    // Add gravity to player's y-position

    player_pos.y += player_y_velocity;
    player_y_velocity += GRAVITY_FORCE;


    // If the player is on ground, zero player's y-velocity
    // If the player is *in* ground, pull them out by rounding their position
   player_on_ground = LevelManager::getInstanceLevel().is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);
    if (player_on_ground) { // Use the getter to check the state
        player_y_velocity = 0;
        player_pos.y = roundf(player_pos.y);
    }
}

void Player::update_player() {
    getInstancePlayer().update_player_gravity();

    // Interacting with other level elements
    if (LevelManager::getInstanceLevel().is_colliding(player_pos, COIN)) {
        LevelManager::getInstanceLevel().get_collider(player_pos, COIN) = AIR; // Removes the coin
        increment_player_score();
    }

    if (LevelManager::getInstanceLevel().is_colliding(player_pos, EXIT)) {
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
    if (LevelManager::getInstanceLevel().is_colliding(player_pos, SPIKE) || player_pos.y > LevelManager::getInstanceLevel().get_current_level().get_rows()) {
        Player::kill_player();
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