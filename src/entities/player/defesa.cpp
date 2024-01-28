#include "defesa.h"

defesa::defesa(WorldMap *worldMap)
{
    _worldMap = worldMap;
    _player = nullptr;
}

void defesa::setPlayer(Player *player)
{
    _player = player;
}

void defesa::pass(Player *recebedor)
{
    QVector2D DPosition = _player->getPosition(); //posição do Defesa
    QVector2D RPosition = recebedor->getPosition(); //posição do recebedor
    QVector2D distball = (DPosition - (_worldMap->ballPosition()));
    float orientacaoDesejada = atan2(RPosition.y() - DPosition.y(), RPosition.x() - DPosition.x());
    float orientacaoAtual = _player->getOrientation();
    if(_player != nullptr){
        if(std::abs((distball).length()) >= 0.12){ //caso n tenha chegado na bola, va pra ela kk
            _player->dribble(true);
            _player->goTo(_worldMap->ballPosition());
        }else if(std::abs((distball).length()) < 0.12) { //caso tenha chegado na bola olhe p o recebedor
            _player->goTo(DPosition);
            _player->dribble(true);
            _player->rotateTo(RPosition);
            if (std::abs(orientacaoAtual - orientacaoDesejada) < 0.01) { //caso estejam se olhando faça o passe
                _player->dribble(false);
                _player->kick(3.5f, false);
            }
        }
    }
}

void defesa::wallPosition(float y)
{
    if(_player != nullptr){
        QVector2D DPosition = _player->getPosition(); //posição do Defesa
        QVector2D ourGoalCenter = _worldMap->ourGoalCenter();
        QVector2D theirGoalCenter = _worldMap->theirGoalCenter();
        QVector2D ballPosition = _worldMap->ballPosition();
        if (std::abs(DPosition.x() - ourGoalCenter.x()) > 3.2){
            _player->goTo(QVector2D(ourGoalCenter.x(), ballPosition.y() + y));
        }else if (std::abs(DPosition.x() - ourGoalCenter.x()) < 2.8){
            _player->goTo(QVector2D(theirGoalCenter.x(), ballPosition.y() + y));
        }else {
            _player->goTo(QVector2D(DPosition.x(), ballPosition.y() + y));
        }

        //QVector2D theirPenaltyMark = _worldMap->theirPenaltyMark();
        //QVector2D ballPosition = _worldMap->ballPosition();
        //_player->goTo(QVector2D(theirPenaltyMark.x(),ballPosition.y() + y));
    }

}

