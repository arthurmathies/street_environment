#ifndef LMS_STREET_ENVIRONMENT_POSE_H
#define LMS_STREET_ENVIRONMENT_POSE_H
struct Pose2D{
    Eigen::Vector3f m_position;
    /**
     * @brief update
     * @param dx
     * @param dy
     * @param dphi drehung des Koordinatensystems
     */
    void update(float dx, float dy, float dphi){
        Eigen::Matrix<float,3,3> transRot;
        transRot(0,0)=cos(dphi);
        transRot(0,1)=-sin(dphi);
        transRot(0,2)=dx;
        transRot(1,0)=sin(dphi);
        transRot(1,1)=cos(dphi);
        transRot(1,2)=dy;
        transRot(2,0)=0;
        transRot(2,1)=0;
        transRot(2,2)=1;
        m_position=transRot*m_position;
    }
};
struct PoseEntity2D{

    int posesMaxSize;
    Pose2D currentPose;
    std::vector<std::pair<int,Pose2D>> poses;
    /**
     * @brief update
     * @param dx
     * @param dy
     * @param dphi drehung des Koordinatensystems
     * @param time (can be abused as id) in ms
     */
    void update(float dx, float dy, float dphi, int time){
        currentPose.update(dx,dy,dphi);
        poses.push_back({time,currentPose});
        if(posesMaxSize != -1 && (int)poses.size() > posesMaxSize){
            //TODO don't use vectorposes.
        }
    }
};

#endif //LMS_STREET_ENVIRONMENT_POSE_H
