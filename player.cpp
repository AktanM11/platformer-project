#include "player.h"
#include "globals.h"

void Player::update_player_gravity() {
    // Bounce downwards if approaching a ceiling with upwards velocity
    if (LevelManager::getInstanceLevel().is_colliding({Player::getInstancePlayer().get_player_posX(), Player::getInstancePlayer().get_player_posY() - 0.1f}, WALL) && player_y_velocity < 0) {
        player_y_velocity = CEILING_BOUNCE_OFF;
    }

    // Add gravity to player's y-position

    player_pos.y += player_y_velocity;
    player_y_velocity += GRAVITY_FORCE;


    // If the player is on ground, zero player's y-velocity
    // If the player is *in* ground, pull them out by rounding their position
    player_on_ground = LevelManager::getInstanceLevel().is_colliding({Player::getInstancePlayer().get_player_posX(), Player::getInstancePlayer().get_player_posY() + 0.1f}, WALL);
    if (player_on_ground) { // Use the getter to check the state
        player_y_velocity = 0;
        player_pos.y = roundf(player_pos.y);
    }
}