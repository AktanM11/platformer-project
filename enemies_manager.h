#ifndef ENEMIES_MANAGER_H
#define ENEMIES_MANAGER_H
#include <vector>
#include <raylib.h>
#include "enemy.h"

class EnemiesManager {
public:
    [[nodiscard]] std::vector<Enemy> get_enemies() const {
        return enemies;
    }

    static EnemiesManager &getInstance() {
        static EnemiesManager instance;
        return instance;
    };
    EnemiesManager(const EnemiesManager&) = delete;
    EnemiesManager operator=(const EnemiesManager&) = delete;
    EnemiesManager(EnemiesManager&&) = delete;
    EnemiesManager operator=(EnemiesManager&&) = delete;
    void spawn_enemies();
    void update_enemies();
    bool is_colliding_with_enemies(Vector2 pos) const;
    void remove_colliding_enemy(Vector2 pos);
    private:
    EnemiesManager() = default;
    ~EnemiesManager() = default;
    std::vector<Enemy> enemies{};
};
#endif //ENEMIES_MANAGER_H
