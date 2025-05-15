#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "level.h"
#include "level_manager.h"
#include "player_manager.h"
class Player {
public:
    static Player &getInstancePlayer() {
        static Player instance;
        return instance;
    };
    Player(const Player&) = delete;
    Player operator=(const Player&) = delete;
    Player(Player&&) = delete;
    Player operator=(Player&&) = delete;
    [[nodiscard]] Vector2 get_player_pos() const {
        return player_pos;
    }
    [[nodiscard]] float get_player_posX() const {
        return player_pos.x;
    }
    [[nodiscard]] float get_player_posY() const {
        return player_pos.y;
    }
    float set_player_posX(const float x) {
        this->player_pos.x = x;
    }
    float set_player_posY(const float y) {
        this->player_pos.y = y;
    }
    void set_player_pos(const Vector2 player_pos) {
        this->player_pos = player_pos;
    }

    [[nodiscard]] bool is_player_on_ground() const {
        return player_on_ground;
    }

    void set_is_player_on_ground(const bool is_player_on_ground) {
        this->player_on_ground = is_player_on_ground;
    }

    [[nodiscard]] bool is_looking_forward() const {
        return looks_forward;
    }

    void set_is_looking_forward(const bool is_looking_forward) {
        this->looks_forward = is_looking_forward;
    }

    [[nodiscard]] bool is_moving() const {
        return moves;
    }

    void set_is_moving(const bool is_moving) {
        this->moves = is_moving;
    }

    void reset_player_stats();
    void increment_player_score();
    int get_total_player_score();
    void spawn_player();
    void kill_player();
    void move_player_horizontally(float delta);
    void update_player_gravity();
    void update_player();
private:
    Player() = default;
    ~Player() = default;
    Vector2 player_pos;
    bool player_on_ground;
    bool looks_forward;
    bool moves;
};


#endif //PLAYER_H
