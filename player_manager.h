#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H
class PlayerController {
  public:
    static PlayerController &getInstancePlayerController() {
        static PlayerController instance;
        return instance;
    };

    PlayerController(const PlayerController&) = delete;
    PlayerController operator=(const PlayerController&) = delete;
    PlayerController(PlayerController&&) = delete;
    PlayerController operator=(PlayerController&&) = delete;

    void reset_player_stats();
    void increment_player_score();
    int get_total_player_score();
    void move_player_horizontally(float delta);
    void spawn_player();
    void draw_player();
    void update_player();
    void kill_player();

private:
    PlayerController() = default;
    ~PlayerController() = default;
};
#endif //PLAYER_MANAGER_H
