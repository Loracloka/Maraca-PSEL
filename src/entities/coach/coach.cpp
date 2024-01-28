/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "coach.h"

Coach::Coach(const QMap<bool, QList<Player*>>& players, WorldMap* worldMap)
    : _players(players), _worldMap(worldMap)
{
    // Create QTimer and connects to the runCoach() slot
    _actuatorTimer = new QTimer(this);
    QObject::connect(_actuatorTimer, &QTimer::timeout, this, &Coach::runCoach);
    _actuatorTimer->start(COACH_ITERATION_INTERVAL_MS);
    _goleiro = new goleiro(worldMap);
    _defesa = new defesa(worldMap);
    _ataque = new ataque(worldMap);
}

std::optional<Player*> Coach::getPlayer(const bool& isTeamBlue, const quint8& playerId) {
    // Get list of players
    QList<Player*> playersForColor = _players.value(isTeamBlue);

    // Iterate searching for playerId
    for(const auto& player : playersForColor) {
        if(player->getPlayerId() == playerId) {
            return player;
        }
    }

    // If could not found, just return std::nullopt (should trait later)
    return std::nullopt;
}

WorldMap* Coach::getWorldMap() {
    return _worldMap;
}

int alerta = 0;

void Coach::runCoach() {
    QVector2D ballPosition = getWorldMap()->ballPosition();

    QMap<quint8, std::optional<Player *>>players;
    for (quint8 playerId = 0; playerId < 6; playerId++){
        players.insert(playerId, getPlayer(YELLOW, playerId));
    }
    for (quint8 playerId : players.keys()) {
        if(players.value(playerId).has_value()){
            if(playerId == 0){ //atacante
                //player 0 faz algo
                players.value(playerId).value()->rotateTo(ballPosition);
                _ataque->setPlayer(players.value(playerId).value());
                QVector2D theirGoalCenter = getWorldMap()->theirGoalCenter();
                QVector2D dist0ball = (ballPosition - players.value(playerId).value()->getPosition());
                if ((std::abs((dist0ball).length()) < 0.7) && (std::abs(theirGoalCenter.x() - ballPosition.x()) > 0.5)){ //se a bola tiver perto, ele prepara p receber
                    _ataque->receiveBall();
                    players.value(playerId).value()->goTo(ballPosition);
                }else {
                    players.value(playerId).value()->goTo(QVector2D(0.0f, getWorldMap()->theirGoalCenter().y() + 1.0f));
                }
                if (std::abs((dist0ball).length()) < 0.12) {//se ele tiver com a bola, chuta pro gol
                        _ataque->kickgoal(-0.3f);
                }
            }else if(playerId == 1){ //defesa
                //player 1 faz algo
                players.value(playerId).value()->rotateTo(ballPosition);
                _defesa->setPlayer(players.value(playerId).value());
                QVector2D dist1ball = (ballPosition - players.value(playerId).value()->getPosition());
                if (std::abs((dist1ball).length()) < 0.2){
                    _defesa->pass(getPlayer(YELLOW, 0).value());
                }else if (std::abs((dist1ball).length()) < 0.6) {
                    players.value(playerId).value()->goTo(ballPosition);
                }else{
                    _defesa->wallPosition(0.5f);
                }


            }else if(playerId == 2){ //atacante
                //player 2 faz algo
                players.value(playerId).value()->rotateTo(ballPosition);
                _ataque->setPlayer(players.value(playerId).value());
                QVector2D theirGoalCenter = getWorldMap()->theirGoalCenter();
                QVector2D dist2ball = (ballPosition - players.value(playerId).value()->getPosition());
                if ((std::abs((dist2ball).length()) < 0.7) && (std::abs(theirGoalCenter.x() - ballPosition.x()) > 0.5)){ //se a bola tiver perto, ele prepara p receber
                    _ataque->receiveBall();
                    players.value(playerId).value()->goTo(ballPosition);
                }else {
                    players.value(playerId).value()->goTo(QVector2D(0.0f, getWorldMap()->theirGoalCenter().y() - 1.0f));
                }
                if (std::abs((dist2ball).length()) < 0.12) {//se ele tiver com a bola, chuta pro gol
                        _ataque->kickgoal(0.3f);
                }
            }else if(playerId == 3){ //central
                //player 3 faz algo
                players.value(playerId).value()->rotateTo(ballPosition);
                QVector2D distballcenter = (QVector2D(0.0f,0.0f) - ballPosition);
                QVector2D dist3ball = (ballPosition - players.value(playerId).value()->getPosition());
                //se tiver no inicio do jogo vai pra bola
                if ((std::abs((distballcenter).length()) < 0.01)) {
                    players.value(playerId).value()->goTo(ballPosition);
                //se tiver perto da bola, passa pro atacante
                }else if (std::abs((dist3ball).length()) < 0.12){
                        _ataque->setPlayer(players.value(playerId).value());
                        _ataque->pass(getPlayer(YELLOW, 2).value());
                }
            }else if(playerId == 4){ //defesa
                //player 4 faz algo
                players.value(playerId).value()->rotateTo(ballPosition);
                _defesa->setPlayer(players.value(playerId).value());
                QVector2D dist4ball = (ballPosition - players.value(playerId).value()->getPosition());
                if (std::abs((dist4ball).length()) < 0.12){
                    _defesa->pass(getPlayer(YELLOW, 2).value());
                }else if (std::abs((dist4ball).length()) < 0.6) {
                    players.value(playerId).value()->goTo(ballPosition);
                }else{
                    _defesa->wallPosition(-0.5f);
                }
            }else if(playerId == 5){ //goleiro
                //player 5 faz algo
                players.value(playerId).value()->rotateTo(ballPosition);
                _goleiro->setPlayer(players.value(playerId).value());
                if (getWorldMap()->isInsideOurPenaltyArea(ballPosition)) {
                    //pass
                    _goleiro->pass(getPlayer(YELLOW, 4).value()); //passando para o jogador amigo mais próximo que por enquanto é o 4
                }else{
                    //defend
                    _goleiro->defend();
                }
            }
        }
    }
}
