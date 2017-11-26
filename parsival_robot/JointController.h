#ifndef JointController_h
#define JointController_h

#include <PololuMaestro.h>
#include "JointControllerInterface.h"
#include "SinusoidMovementJoint.h"

/*
enum class RobonovaJoints: int {
    TOBILLO_IZQD_ROT = 0,
    TOBILLO_IZQD_FRONTAL = 1,
    RODILLA_IZQD = 2,
    CADERA_IZQD_FRONTAL = 3,
    CADERA_IZQD_LATERAL = 4,
    VOID_1 = 5,
    HOMBRO_IZQD_ROTA = 6,
    HOMBRO_IZQD_EXTEN = 7,
    CODO_IZQD = 8,
    VOID_2 = 9,
    VOID_3 = 10,
    VOID_4 = 11,
    HOMBRO_DCHO_ROTA = 12,
    HOMBRO_DCHO_EXTEN = 13,
    CODO_DCHO = 14,
    VOID_5 = 15,
    VOID_6 = 16,
    VOID_7 = 17,
    TOBILLO_DCHO_ROT = 18,
    TOBILLO_DCHO_FRONTAL = 19,
    RODILLA_DCHA = 20,
    CADERA_DCHA_FRONTAL = 21,
    CADERA_DCHA_LATERAL = 22,
    VOID_8 = 23,
    MAX_JOINTS = 24,
};
 */

class JointController : public JointControllerInterface {
public:
    JointController(Stream& serial)
        : maestro_(serial),
          maestroJointIndexes_{TOBILLO_IZQD_ROT, TOBILLO_IZQD_FRONTAL, RODILLA_IZQD, CADERA_IZQD_FRONTAL,
                                CADERA_IZQD_LATERAL, -1,
                                HOMBRO_IZQD_ROTA, HOMBRO_IZQD_EXTEN, CODO_IZQD, -1, -1, -1,
                                HOMBRO_DCHO_ROTA, HOMBRO_DCHO_EXTEN, CODO_DCHO, -1, -1, -1,
                                TOBILLO_DCHO_ROT, TOBILLO_DCHO_FRONTAL, RODILLA_DCHA, CADERA_DCHA_FRONTAL,
                                CADERA_DCHA_LATERAL, -1},
          reversedJoint_{true, false, false, true, false, false, false, false, true, false, false, false,
                         true, true, false, false, false, false, false, true, true, false, true, false},
          actualJointsPosition_{0},
          validIndexes_{0, 1, 2, 3, 4, 6, 7, 8, 12, 13, 14, 18, 19, 20, 21, 22}
    {
    }

    void setJointAngles(const int *jointsAngles, int time = -1) {
        for (const auto& i : validIndexes_) {
            Serial.print(jointsAngles[i]);
            Serial.print(" ");
            sinusoidMovementJoints_[i].setObjectivePosition(jointsAngles[i], time);
        }
    }
    
    bool isRunning() const {
        for (const auto& i : validIndexes_) {
            if (sinusoidMovementJoints_[i].isRunning()) {
                return true;
            }
        }
        return false;
    }
    
    void refresh() {
        for (const auto& i : validIndexes_) {
            sinusoidMovementJoints_[i].refresh();
            unsigned int desiredPosition = getMaestroValueFromAngle(sinusoidMovementJoints_[i].getActualPosition(),
                                                                    reversedJoint_[i]);
            if (actualJointsPosition_[i] != desiredPosition) {
                actualJointsPosition_[i] = desiredPosition;
                maestro_.setTarget(maestroJointIndexes_[i], actualJointsPosition_[i]);
            }
        }
    }
    
protected:
    unsigned int getMaestroValueFromAngle(long x, bool reversed) const {
        // En el software del Robonova permiten poner los servos entre 10 y 190 con centro en 100
        // Los valores son grados, porque el rango de movimientos es de 180 grados
        // De la documentacion se extrae que el valor minimo de tiempo de pulso es de 550us y el maximo de 2450us para un rango total de 190 grados. Eso equivale a un rango en grados de 5 a 195 con centro en 100.
        // La funcion que convierte de grados a pulsos es entonces: f(x) = 550 + (x - 5) * 10
        // La librería de PololuMaestro recibe el tamaño de pulso de la siguiente manera:
        // 0 = 0us; 1000 = 250us; 4000 = 1000us -> para pasar de los us necesarios para el servo del motor al valor de la PololuMaestro es suficiente con multiplicar por 4
        if (reversed) {
            x = 200 - x;
        }
        return static_cast<unsigned int>(4 * (550 + (x - 5) * 10));
    }
    
private:
    static const unsigned int jointsNumber = 24;
    MiniMaestro maestro_;
    int maestroJointIndexes_[JointController::jointsNumber];
    bool reversedJoint_[JointController::jointsNumber];
    SinusoidMovementJoint sinusoidMovementJoints_[JointController::jointsNumber];
    unsigned int actualJointsPosition_[JointController::jointsNumber];
    int validIndexes_[16];

    enum MaestroJoints {
        TOBILLO_DCHO_ROT = 0,
        TOBILLO_DCHO_FRONTAL = 1,
        RODILLA_DCHA = 2,
        CADERA_DCHA_FRONTAL = 3,
        CADERA_DCHA_LATERAL = 4,
        CODO_DCHO = 5,
        CODO_IZQD = 6,
        CADERA_IZQD_LATERAL = 7,
        CADERA_IZQD_FRONTAL = 8,
        RODILLA_IZQD = 9,
        TOBILLO_IZQD_FRONTAL = 10,
        TOBILLO_IZQD_ROT = 11,
        HOMBRO_DCHO_EXTEN = 13,
        HOMBRO_DCHO_ROTA = 14,
        HOMBRO_IZQD_EXTEN = 12,
        HOMBRO_IZQD_ROTA = 15
    };
};

#endif /* JointController_h */
