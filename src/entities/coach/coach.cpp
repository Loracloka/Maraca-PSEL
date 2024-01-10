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

int estado = 0;

void Coach::runCoach() {
    QVector2D BallPosition = getWorldMap()->ballPosition();
    QVector2D Goal = getWorldMap()->theirGoalCenter();

    if (estado == 0){ //Robô2 vai p posição de recebimento de bola
        QVector2D Position2(-0.926f, -1.418f);
        getPlayer(YELLOW, 2).value()->goTo(Position2);
        QVector2D Robot2Position = getPlayer(YELLOW, 2).value()->getPosition();
        if (std::abs((Robot2Position-Position2).length()) < 0.2){
            estado = 1;
        }
    }
    if (estado == 1){ //Robô2 liga o drible, Robô3 vai pegar bola
        getPlayer(YELLOW, 2).value()->dribble(true);
        getPlayer(YELLOW, 3).value()->dribble(true);
        QVector2D Position3(0.0f,0.0f);
        getPlayer(YELLOW, 3).value()->goTo(Position3);
        QVector2D Robot3Position = getPlayer(YELLOW, 3).value()->getPosition();
        QVector2D dist3center = (Robot3Position - Position3);
        if (std::abs((dist3center).length()) < 0.1) {
            estado = 2;
        }
    }
    if (estado == 2){ //robô2 olha pra bola, Robô3 olha pra Robô2
        getPlayer(YELLOW, 2).value()->dribble(true);
        getPlayer(YELLOW, 3).value()->dribble(true);
        getPlayer(YELLOW, 2).value()->rotateTo(BallPosition);
        QVector2D Robot2Position = getPlayer(YELLOW, 2).value()->getPosition();
        QVector2D Robot3Position = getPlayer(YELLOW, 3).value()->getPosition();
        float orientacaoDesejada = atan2(Robot2Position.y() - Robot3Position.y(), Robot2Position.x() - Robot3Position.x());
        float orientacaoAtual = getPlayer(YELLOW, 3).value()->getOrientation();
        getPlayer(YELLOW, 3).value()->rotateTo(getPlayer(YELLOW, 2).value()->getPosition());
        if (std::abs(orientacaoAtual - orientacaoDesejada) < 0.01) {
            estado = 3;
        }
    }
    if (estado == 3){ //Robô 3 passa a bola
        getPlayer(YELLOW, 2).value()->dribble(true);
        getPlayer(YELLOW, 3).value()->dribble(false);
        getPlayer(YELLOW, 2).value()->rotateTo(BallPosition);
        getPlayer(YELLOW, 3).value()->kick(3.0f, false);
        QVector2D Robot2Position = getPlayer(YELLOW, 2).value()->getPosition();
        QVector2D dist2ball = (Robot2Position - BallPosition);
        if (std::abs((dist2ball).length()) < 0.2) {
            estado = 4;
        }
    }
    if (estado == 4){ //Manda Robô 0 p posição de recebimento de bola
        QVector2D Position0(-3.145, 1.533f);
        getPlayer(YELLOW, 0).value()->goTo(Position0);
        QVector2D Robot0Position = getPlayer(YELLOW, 0).value()->getPosition();
        if (std::abs((Robot0Position-Position0).length()) < 0.2){
            estado = 5;
        }
    }
    if (estado == 5){ //Robô0 olha pra bola, Robô2 olha pra Robô0
        getPlayer(YELLOW, 0).value()->dribble(true);
        getPlayer(YELLOW, 2).value()->dribble(true);
        getPlayer(YELLOW, 0).value()->rotateTo(BallPosition);
        QVector2D Robot0Position = getPlayer(YELLOW, 0).value()->getPosition();
        QVector2D Robot2Position = getPlayer(YELLOW, 2).value()->getPosition();
        float orientacaoDesejada = atan2(Robot0Position.y() - Robot2Position.y(), Robot0Position.x() - Robot2Position.x());
        float orientacaoAtual = getPlayer(YELLOW, 2).value()->getOrientation();
        getPlayer(YELLOW, 2).value()->rotateTo(getPlayer(YELLOW, 0).value()->getPosition());
        if (std::abs(orientacaoAtual - orientacaoDesejada) < 0.01) {
            estado = 6;
        }
    }
    if (estado == 6){ //Robô2 passa a bola
        getPlayer(YELLOW, 0).value()->dribble(true);
        getPlayer(YELLOW, 2).value()->dribble(false);
        getPlayer(YELLOW, 0).value()->rotateTo(BallPosition);
        getPlayer(YELLOW, 2).value()->kick(3.5f, false);
        QVector2D Robot0Position = getPlayer(YELLOW, 0).value()->getPosition();
        QVector2D dist0ball = (Robot0Position - BallPosition);
        if (std::abs((dist0ball).length()) < 0.2) {
            estado = 7;
        }
    }
    if (estado == 7){
        getPlayer(YELLOW, 0).value()->dribble(true);
        QVector2D Robot0Position = getPlayer(YELLOW, 0).value()->getPosition();
        float orientacaoDesejada = atan2(Goal.y() - Robot0Position.y(), Goal.x() - Robot0Position.x());
        float orientacaoAtual = getPlayer(YELLOW, 0).value()->getOrientation();
        getPlayer(YELLOW, 0).value()->rotateTo(Goal);
        if (std::abs(orientacaoAtual - orientacaoDesejada) < 0.01) {
            estado = 8;
        }
    }
    if (estado == 8){
        getPlayer(YELLOW, 0).value()->dribble(false);
        getPlayer(YELLOW, 0).value()->kick(3.0f, false);
    }
}

    // Here you can control the robots freely.
    // Remember that the getPlayer(color, id) function can return a std::nullopt object, so
    // be careful when you use it (remember to only use ids from 0-2 and the BLUE and YELLOW
    // defines).

    // Example 1: here we get the ball position and set the BLUE and YELLOW player 0 to follow it
    //QVector2D ballPosition = getWorldMap()->ballPosition();
    //getPlayer(BLUE, 0).value()->goTo(ballPosition);
    //getPlayer(YELLOW, 0).value()->goTo(ballPosition);

    // Example 2: here we set the BLUE and YELLOW players 1 and 2 to rotate to the ball
    //getPlayer(BLUE, 1).value()->rotateTo(ballPosition);
    //getPlayer(BLUE, 2).value()->rotateTo(ballPosition);
    //getPlayer(YELLOW, 1).value()->rotateTo(ballPosition);
    //getPlayer(YELLOW, 2).value()->rotateTo(ballPosition);

    //getPlayer(BLUE, 3).value()->dribble(true);
    //getPlayer(YELLOW, 3).value()->dribble(true);
    //getPlayer(BLUE, 2).value()->dribble(true);
    //getPlayer(YELLOW, 2).value()->dribble(true);

    //Coordenadas do ponto de rotação, raio e velocidade angular
    //QVector2D RotationCenter(2.766f, -1.539f);
    //float Radius = 1.0f;
    //Calcula a distância que o robô está do ponto
    //QVector2D RobotPosition = getPlayer(YELLOW, 3).value()->getPosition();
    //QVector2D DistanceCenter = (RobotPosition - RotationCenter);
    //if (std::abs((DistanceCenter).length() - Radius) > 0.2){
        //Robô vai para a borda do círculo
    //    QVector2D VetorR = ((RobotPosition - RotationCenter)/(RobotPosition - RotationCenter).length()) * Radius;
    //    QVector2D PositionOnCircle = RotationCenter + VetorR;
    //    getPlayer(YELLOW, 3).value()->goTo(PositionOnCircle);
    //}else{
        // Faz o robô continuar  rotação
        // Calcula a nova posição no círculo
    //    float CosNewAngle = ((DistanceCenter.x() / Radius)*cos(0.1)) - ((DistanceCenter.y() / Radius)*sin(0.1));
    //    float SinNewAngle = ((DistanceCenter.y() / Radius)*cos(0.1)) + ((DistanceCenter.x() / Radius)*sin(0.1));
    //    QVector2D NextPosition = RotationCenter + Radius*QVector2D(CosNewAngle,SinNewAngle);
        //Robô vai para nova posição
    //    getPlayer(YELLOW, 3).value()->goTo(NextPosition);
    //}

