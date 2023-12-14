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

void Coach::runCoach() {
    //testando se função isDribbleOn funciona
    getPlayer(YELLOW, 3).value()->dribble(true);
    if (getPlayer(YELLOW, 3).value()->isDribbleOn() == true){
        QVector2D ballPosition = getWorldMap()->ballPosition();
        getPlayer(YELLOW, 3).value()->goTo(ballPosition);
    }


//    QVector2D RobotPosition = getPlayer(YELLOW, 3).value()->getPosition();
//    QVector2D BallPosition = getWorldMap()->ballPosition();
//        if (std::abs((RobotPosition - BallPosition).length()) > 0.12){
//            //Robô 1 olha pra bola e vai até ela
//            getPlayer(YELLOW, 3).value()->rotateTo(BallPosition);
//            getPlayer(YELLOW, 3).value()->dribble(true);
//            QVector2D sideBall = BallPosition - 0.1116 * ((BallPosition - RobotPosition)/std::abs((BallPosition - RobotPosition).length()));
//            getPlayer(YELLOW, 3).value()->goTo(sideBall);
//        }

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

