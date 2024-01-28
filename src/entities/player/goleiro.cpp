#include "goleiro.h"

goleiro::goleiro(WorldMap *worldMap)
{
    _worldMap = worldMap;
    _player = nullptr;

}

void goleiro::setPlayer(Player *player)
{
    _player = player;

}

void goleiro::defend()
{
    if(_player != nullptr){
        QVector2D ballPosition = _worldMap->ballPosition();
        QVector2D Goal = _worldMap->ourGoalCenter();
        if (std::abs(ballPosition.y()) < 0.5f) {
            _player->goTo(QVector2D(Goal.x(), ballPosition.y()));
        }
    }
}

void goleiro::pass(Player *recebedor)
{
    QVector2D GPosition = _player->getPosition(); //posição do goleiro
    QVector2D RPosition = recebedor->getPosition(); //posição do recebedor
    QVector2D distball = (GPosition - (_worldMap->ballPosition()));
    float orientacaoDesejada = atan2(RPosition.y() - GPosition.y(), RPosition.x() - GPosition.x());
    float orientacaoAtual = _player->getOrientation();
    if(_player != nullptr){
        if(std::abs((distball).length()) >= 0.12){ //caso n tenha chegado na bola, va pra ela kk
            _player->dribble(true);
            _player->goTo(_worldMap->ballPosition());
        }else if(std::abs((distball).length()) < 0.12) { //caso tenha chegado na bola olhe p o recebedor
            _player->goTo(GPosition);
            _player->dribble(true);
            _player->rotateTo(RPosition);
            if (std::abs(orientacaoAtual - orientacaoDesejada) < 0.01) { //caso estejam se olhando faça o passe
                _player->dribble(false);
                _player->kick(3.5f, false);
            }
        }
    }
}


