#ifndef Pose_h
#define Pose_h

class Pose {
public:
    Pose() : nextPose_(nullptr) {}
    virtual void update() = 0;
    
    virtual Pose * getNextPose() = 0;
    
    virtual void init() = 0;
    
    virtual ~Pose() {}
    
    void setNextPose(Pose * nextPose) {
        nextPose_ = nextPose;
    }
    
protected:
    Pose * getInitilizedNextPose() {
        if (nextPose_) {
            nextPose_->init();
        }
        return nextPose_;
    }
    Pose * nextPose_;
    
private:
    
};

#endif /* Pose_h */
