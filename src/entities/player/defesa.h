#ifndef DEFESA_H
#define DEFESA_H

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>

class defesa
{
public:
    defesa(WorldMap *worldMap);

    void setPlayer(Player *player);
    void pass(Player *recebedor);
    void wallPosition(float y);

private:
    Player *_player;
    WorldMap *_worldMap;
};

#endif // DEFESA_H
