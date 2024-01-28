#ifndef ATAQUE_H
#define ATAQUE_H

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>

class ataque
{
public:
    ataque(WorldMap *worldMap);

    void setPlayer(Player *player);

    void kickgoal(float y);
    void receiveBall();
    void pass(Player *recebedor);

private:
    Player *_player;
    WorldMap *_worldMap;
};

#endif // ATAQUE_H
