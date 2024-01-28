#ifndef GOLEIRO_H
#define GOLEIRO_H

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>

class goleiro
{
public:
    goleiro(WorldMap *worldMap);

    void setPlayer(Player *player);

    void defend();
    void pass(Player *recebedor);

private:
    Player *_player;
    WorldMap *_worldMap;
};

#endif // GOLEIRO_H
