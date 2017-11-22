#ifndef JointController_h
#define JointController_h

#ifdef PC_VERSION
#include <Arduino.h>
#include <cmath>
#define PI 3.1415
#endif

#ifdef PC_VERSION
#define MAESTRO_DEBUG
#endif
//#define MAESTRO_DEBUG
#ifndef MAESTRO_DEBUG
#include <PololuMaestro.h>
#else
class MiniMaestro {
public:
    void setAcceleration(int idx, int acceleration) {}
    void setSpeed(int idx, int speed) {}
    void setTarget(int idx, int position) {
        Serial.print(idx);
        Serial.print("->");
        Serial.println(position);
    }
};
#endif

class SinusoidController {
public:
    SinusoidController() : t0_(0), timeToDestiny_(0), initialPosition_(0), objectivePosition_(0), eleapsedTime_(0) {}

    SinusoidController(const SinusoidController& c)
            : t0_(c.t0_), timeToDestiny_(c.timeToDestiny_), initialPosition_(c.initialPosition_),
              objectivePosition_(c.objectivePosition_), eleapsedTime_(c.eleapsedTime_) {}

    SinusoidController(int objectivePosition, int initialPosition, float timeToDestiny)
            : t0_(millis()), timeToDestiny_(timeToDestiny), initialPosition_(initialPosition),
              objectivePosition_(objectivePosition), eleapsedTime_(0) {}

    void updateEleapsedTime() {
        eleapsedTime_ = millis() - t0_;
    }

    int getActualPosition() {
        if (isRunning()) {
            return static_cast<int>(
                    initialPosition_ + (1 + sin(PI * eleapsedTime_/timeToDestiny_ - PI/2)) *
                                               (objectivePosition_ - initialPosition_)/2);
        } else {
            return objectivePosition_;
        }
    }

    void init(int objectivePosition, int initialPosition, float timeToDestiny) {
        t0_ = millis();
        objectivePosition_ = objectivePosition;
        initialPosition_ = initialPosition;
        timeToDestiny_ = timeToDestiny;
        eleapsedTime_ = 0;
    }

    bool isRunning() const {
        if (0 < timeToDestiny_ && eleapsedTime_ <= timeToDestiny_) {
            return true;
        } else {
            return false;
        }
    }

protected:
private:
    unsigned long t0_;
    float timeToDestiny_;
    int initialPosition_;
    int objectivePosition_;
    unsigned long eleapsedTime_;

};

enum class MaestroJoints: int {
    // TODO: revisar indices
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
    HOMBRO_IZQD_ROTA = 15,
    MAX_JOINTS = 16,
};
    
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

class Joint {
public:
    Joint(MiniMaestro& maestro, MaestroJoints maestroId, RobonovaJoints  robonovaId, bool reversed = false)
    : maestro_(&maestro), maestroId_(maestroId), robonovaId_(robonovaId), reversed_(reversed) {}

    Joint(const Joint& joint)
            : maestro_(joint.maestro_), maestroId_(joint.maestroId_), robonovaId_(joint.robonovaId_),
              reversed_(joint.reversed_) {
    }

    Joint() : maestro_(nullptr), maestroId_(MaestroJoints::MAX_JOINTS), robonovaId_(RobonovaJoints::VOID_1) {}

    Joint& operator=(const Joint& other) {
        maestro_ = other.maestro_;
        maestroId_ = other.maestroId_;
        robonovaId_ = other.robonovaId_;
        reversed_ = other.reversed_;
        sentPosition_ = other.sentPosition_;
        speed_ = other.speed_;
        acceleration_ = other.acceleration_;
        actualOffset_ = other.actualOffset_;
        return *this;
    }

    void setTimedOffset(int offset, float time) {
        if (0 < time) {
            positionController_.init(offset, actualOffset_, time);
        } else {
            setOffset(offset);
        }
    }

    void refresh() {
        //Serial.print("update joint ");
        //Serial.print(static_cast<int>(maestroId_));
        positionController_.updateEleapsedTime();
        //Serial.print(" angle=");
        auto angle = positionController_.getActualPosition();
        //Serial.println(angle);
        setOffset(angle);
    }

    bool isRunning() const {
        return positionController_.isRunning();
    }

    MaestroJoints getMaestroId() const { return maestroId_; }

    RobonovaJoints getRobonovaId() const { return robonovaId_; }

protected:
    void setOffset(int offset, int speed=0, int acceleration=0) {
        if (!maestro_)
            return;
        if (acceleration != acceleration_) {
            acceleration_ = acceleration;
            maestro_->setAcceleration(static_cast<int>(maestroId_), acceleration_);
        }
        if (speed != speed_) {
            speed_ = speed;
            maestro_->setSpeed(static_cast<int>(maestroId_), speed_);
        }
        if (offset != sentPosition_) {
            sentPosition_ = offset;
            maestro_->setTarget(static_cast<int>(maestroId_),
                                getMaestroValueFromAngle(reversed_ ? 200 - offset : offset));
            Serial.print(static_cast<int>(maestroId_));
            Serial.print("->");
            Serial.println(getMaestroValueFromAngle(reversed_ ? 200 - offset : offset));
        }
        actualOffset_ = offset;
    }
    
    unsigned int getMaestroValueFromAngle(int x) const {
        // En el software del Robonova permiten poner los servos entre 10 y 190 con centro en 100
        // Los valores son grados, porque el rango de movimientos es de 180 grados
        // De la documentacion se extrae que el valor minimo de tiempo de pulso es de 550us y el maximo de 2450us para un rango total de 190 grados. Eso equivale a un rango en grados de 5 a 195 con centro en 100.
        // La funcion que convierte de grados a pulsos es entonces: f(x) = 550 + (x - 5) * 10
        // La librería de PololuMaestro recibe el tamaño de pulso de la siguiente manera:
        // 0 = 0us; 1000 = 250us; 4000 = 1000us -> para pasar de los us necesarios para el servo del motor al valor de la PololuMaestro es suficiente con multiplicar por 4
        return 4 * (550 + (x - 5) * 10);
    }
    
private:
    MiniMaestro* maestro_;
    MaestroJoints maestroId_;
    RobonovaJoints robonovaId_;
    bool reversed_;
    int sentPosition_ = 0;
    int speed_ = 0;
    int acceleration_ = 0;
    int actualOffset_ = 0;
    SinusoidController positionController_;

};

class JointController {
public:
    
    JointController(Stream& serial)
#ifndef MAESTRO_DEBUG
        : maestro_(Serial2),
#else
        : maestro_(),
#endif
          robonobaJoints_{nullptr}
    {
        maestroJoints_[static_cast<int>(MaestroJoints::TOBILLO_DCHO_ROT)] =
                Joint(maestro_, MaestroJoints::TOBILLO_DCHO_ROT, RobonovaJoints::TOBILLO_DCHO_ROT);
        maestroJoints_[static_cast<int>(MaestroJoints::TOBILLO_DCHO_FRONTAL)] =
                Joint(maestro_, MaestroJoints::TOBILLO_DCHO_FRONTAL, RobonovaJoints::TOBILLO_DCHO_FRONTAL, true);
        maestroJoints_[static_cast<int>(MaestroJoints::RODILLA_DCHA)] =
                Joint(maestro_, MaestroJoints::RODILLA_DCHA, RobonovaJoints::RODILLA_DCHA, true);
        maestroJoints_[static_cast<int>(MaestroJoints::CADERA_DCHA_FRONTAL)] =
                Joint(maestro_, MaestroJoints::CADERA_DCHA_FRONTAL, RobonovaJoints::CADERA_DCHA_FRONTAL);
        maestroJoints_[static_cast<int>(MaestroJoints::CADERA_DCHA_LATERAL)] =
                Joint(maestro_, MaestroJoints::CADERA_DCHA_LATERAL, RobonovaJoints::CADERA_DCHA_LATERAL, true);
        maestroJoints_[static_cast<int>(MaestroJoints::CODO_DCHO)] =
                Joint(maestro_, MaestroJoints::CODO_DCHO, RobonovaJoints::CODO_DCHO);
        maestroJoints_[static_cast<int>(MaestroJoints::CODO_IZQD)] =
                Joint(maestro_, MaestroJoints::CODO_IZQD, RobonovaJoints::CODO_IZQD, true);
        maestroJoints_[static_cast<int>(MaestroJoints::CADERA_IZQD_LATERAL)] =
                Joint(maestro_, MaestroJoints::CADERA_IZQD_LATERAL, RobonovaJoints::CADERA_IZQD_LATERAL);
        maestroJoints_[static_cast<int>(MaestroJoints::CADERA_IZQD_FRONTAL)] =
                Joint(maestro_, MaestroJoints::CADERA_IZQD_FRONTAL, RobonovaJoints::CADERA_IZQD_FRONTAL, true);
        maestroJoints_[static_cast<int>(MaestroJoints::RODILLA_IZQD)] =
                Joint(maestro_, MaestroJoints::RODILLA_IZQD, RobonovaJoints::RODILLA_IZQD);
        maestroJoints_[static_cast<int>(MaestroJoints::TOBILLO_IZQD_FRONTAL)] =
                Joint(maestro_, MaestroJoints::TOBILLO_IZQD_FRONTAL, RobonovaJoints::TOBILLO_IZQD_FRONTAL);
        maestroJoints_[static_cast<int>(MaestroJoints::TOBILLO_IZQD_ROT)] =
                Joint(maestro_, MaestroJoints::TOBILLO_IZQD_ROT, RobonovaJoints::TOBILLO_IZQD_ROT, true);
        maestroJoints_[static_cast<int>(MaestroJoints::HOMBRO_DCHO_EXTEN)] =
                Joint(maestro_, MaestroJoints::HOMBRO_DCHO_EXTEN, RobonovaJoints::HOMBRO_DCHO_EXTEN);
        maestroJoints_[static_cast<int>(MaestroJoints::HOMBRO_DCHO_ROTA)] =
                Joint(maestro_, MaestroJoints::HOMBRO_DCHO_ROTA, RobonovaJoints::HOMBRO_DCHO_ROTA);
        maestroJoints_[static_cast<int>(MaestroJoints::HOMBRO_IZQD_EXTEN)] =
                Joint(maestro_, MaestroJoints::HOMBRO_IZQD_EXTEN, RobonovaJoints::HOMBRO_IZQD_EXTEN, true);
        maestroJoints_[static_cast<int>(MaestroJoints::HOMBRO_IZQD_ROTA)] =
                Joint(maestro_, MaestroJoints::HOMBRO_IZQD_ROTA, RobonovaJoints::HOMBRO_IZQD_ROTA, true);

        for (auto& joint : maestroJoints_) {
            auto robonovaId = joint.getRobonovaId();
            robonobaJoints_[static_cast<int>(robonovaId)] = &joint;
        }
    }
    
    template <typename AngleType>
    void setPose(const int * jointsAngles, int time = -1) {
        Serial.print("New pose ");
        for (int i = 0; i < static_cast<int>(AngleType::MAX_JOINTS); ++i) {
            Serial.print(jointsAngles[i]);
            Serial.print(" ");
        }
        Serial.println("");
        for (int i = 0; i < static_cast<int>(AngleType::MAX_JOINTS); ++i) {
            moveJoint(static_cast<AngleType>(i), jointsAngles[i], time);
        }
    }
    
    bool isRunning() {
        for (auto& joint : maestroJoints_) {
            if (joint.isRunning()) {
                return true;
            }
        }
        return false;
    }
    
    void refresh() {
        for (auto& joint : maestroJoints_) {
            joint.refresh();
        }
    }
    
protected:
    void moveJoint(MaestroJoints index, int position, int time = -1) {
        Serial.print("maestroIdx:");
        Serial.print(static_cast<int>(index));
        Serial.print(" to angle:");
        Serial.print(position);
        Serial.print(" in time:");
        Serial.println(time);
        Joint * joint = getJoint(index);
        if (joint) {
            joint->setTimedOffset(position, time);
        }
    }
    
    void moveJoint(RobonovaJoints index, int position, int time = -1) {
        Serial.print("robonovaIdx:");
        Serial.print(static_cast<int>(index));
        Serial.print(" to angle:");
        Serial.print(position);
        Serial.print(" in time:");
        Serial.println(time);
        Joint * joint = getJoint(index);
        if (joint) {
            joint->setTimedOffset(position, time);
        }
    }

    Joint* getJoint(RobonovaJoints index) {
        if (index != RobonovaJoints ::MAX_JOINTS) {
            return robonobaJoints_[static_cast<int>(index)];
        } else {
            return nullptr;
        }
    }

    Joint* getJoint(MaestroJoints index) {
        if (index != MaestroJoints::MAX_JOINTS) {
            return &maestroJoints_[static_cast<int>(index)];
        } else {
            return nullptr;
        }
    }
    
private:
    MiniMaestro maestro_;
    Joint maestroJoints_[static_cast<int>(MaestroJoints ::MAX_JOINTS)];
    Joint* robonobaJoints_[static_cast<int>(RobonovaJoints::MAX_JOINTS)];
};

#endif /* JointController_h */
