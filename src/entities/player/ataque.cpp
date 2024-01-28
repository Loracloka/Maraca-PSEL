#include "ataque.h"

ataque::ataque(WorldMap *worldMap)
{
    _worldMap = worldMap;
    _player = nullptr;
}

void ataque::setPlayer(Player *player)
{
    _player = player;
}

void ataque::kickgoal(float y)
{

    if(_player != nullptr){
        //la no coach -> se o atacante estiver com a bola
        QVector2D APosition = _player->getPosition(); //posição do Atacante
        QVector2D theirGoalCenter = _worldMap->theirGoalCenter();
        QVector2D Goal = (theirGoalCenter + QVector2D(0.0f, y)); //cantinho direito do gol
        //QVector2D distAGoal = (APosition - Goal);
        float orientacaoDesejada = atan2(Goal.y() - APosition.y(), Goal.x() - APosition.x());
        float orientacaoAtual = _player->getOrientation();
        if (std::abs(APosition.x() - theirGoalCenter.x()) > 2.0) { //se a distancia atacante p linha do gol > 2.0
            _player->dribble(true);
            _player->goTo(QVector2D(theirGoalCenter.x(), (theirGoalCenter.y()+APosition.y()))); //segue reto no y q o robô ta e vai pra linha do gol
        }else if (std::abs(APosition.x() - theirGoalCenter.x()) <= 2.0) { //se a distancia atacante p linha do gol <= 2.0
            _player->dribble(true);
            _player->goTo(APosition);
            _player->rotateTo(Goal);
            if (std::abs(orientacaoAtual - orientacaoDesejada) < 0.01) { //se olhou pro gol chuta
                _player->dribble(false);
                _player->kick(3.5f, false);
            }
        }
    }
}

void ataque::receiveBall()
{
    if(_player != nullptr){
        _player->dribble(true);
    }
}

void ataque::pass(Player *recebedor)
{   //no coach -> se a bola tiver em 0,0 start
    QVector2D APosition = _player->getPosition(); //posição do Atacante
    QVector2D RPosition = recebedor->getPosition(); //posição do recebedor
    QVector2D distball = (APosition - (_worldMap->ballPosition()));
    float orientacaoDesejada = atan2(RPosition.y() - APosition.y(), RPosition.x() - APosition.x());
    float orientacaoAtual = _player->getOrientation();
    if(_player != nullptr){
        if(std::abs((distball).length()) >= 0.12){ //caso n tenha chegado na bola, va pra ela kk
            _player->dribble(true);
            _player->goTo(_worldMap->ballPosition());
        }else if(std::abs((distball).length()) < 0.12) { //caso tenha chegado na bola olhe p o recebedor
            _player->goTo(APosition);
            _player->dribble(true);
            _player->rotateTo(RPosition);
            if (std::abs(orientacaoAtual - orientacaoDesejada) < 0.01) { //caso estejam se olhando faça o passe
                _player->dribble(false);
                _player->kick(3.0f, false);
            }
        }
    }
}
