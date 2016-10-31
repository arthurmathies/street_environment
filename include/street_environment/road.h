#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include "lms/inheritance.h"
#include "lms/math/vertex.h"
#include "lms/math/polyline.h"
#include "street_environment/street_environment.h"


#include "lms/serializable.h"
#include "cereal/cerealizable.h"
#include "cereal/cereal.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/base_class.hpp"

#include "lms/exception.h"
#include "lms/logger.h"

namespace street_environment {
enum class RoadStateType{
    UNKNOWN,STRAIGHT,STRAIGHT_CURVE,CURVE
};
struct RoadState{
    RoadState():type(RoadStateType::UNKNOWN),startDistance(0),endDistance(0),probability(0),curvature(1){}
    RoadStateType type;
    /**
     * @brief startDistance tangential distance to the start of the state
     */
    float startDistance;
    /**
     * @brief startDistance tangential distance to the end of the state
     */
    float endDistance;
    float probability;
    float curvature;
};
struct RoadStates{
    std::vector<RoadState> states;
    RoadState mostProbableState() const{
        RoadState r;
        r.type = RoadStateType::UNKNOWN;
        r.probability = 0;
        for(const RoadState &v:states){
            if(v.probability > r.probability)
                r = v;
        }
        return r;
    }
};

    enum class RoadLaneType {
        LEFT, MIDDLE, RIGHT
    };

    class RoadLane : public EnvironmentObject,public lms::math::polyLine2f
    {
        RoadLaneType m_type;
    public:
        static constexpr int TYPE = 0;
        bool isSubType(std::type_index tIndex) const override{
            return lms::Extends<lms::math::polyLine2f>::isSubType(this,tIndex);
        }

        RoadLane(){}

        virtual bool match(const RoadLane &obj) const{
            //doesn't handle subclasses
            if(!EnvironmentObject::match(obj)){
                return false;
            }
            //TODO

            return false;

        }
        virtual ~RoadLane() {}

        int getType()const override{
           return TYPE;
        }

        /**
         * @brief polarDarstellung
         * polarDarstellung[0] is the y-deviance
         * polarDarstellung[1] is the start-angle in rad
         * polarDarstellung[>1] Krümmungen relativ zum vorherigen stück
         */
        //std::vector<double> polarDarstellung; //TODO english name :)
        //float polarPartLength;
        RoadLaneType type() const{
            return m_type;
        }

        void type(RoadLaneType type){
            m_type = type;
        }

        CEREAL_SERIALIZATION()
        // member cereal serialize method
        template <class Archive>
        void serialize( Archive & archive) {
            archive (cereal::base_class<street_environment::EnvironmentObject>(this),
                        cereal::base_class<lms::math::polyLine2f>(this),
                      m_type);
        }
    };
    typedef std::shared_ptr<RoadLane> RoadLanePtr;

//from http://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
struct RoadMatrixCell{
private:
    float m_badness;
public:
    lms::math::vertex2f points[4];
    /**
     * @brief badness value between 0 and 1
     */
    RoadMatrixCell():m_badness(0){}
    bool contains(lms::math::vertex2f p) const{

        return lms::math::pointInTriangle(p,points[0],points[1],points[2]) || pointInTriangle(p,points[0],points[2],points[3]);
    }

    float badness() const{
        return m_badness;
    }

    void badness(float badness){
        if(badness < 0 || badness > 1){
            LMS_EXCEPTION("Invalid badness: " + std::to_string(badness));
        }
        m_badness = badness;
    }
};

/**
 * @brief The RoadMatrix struct 0,0 is on the left at the start of the road
 */
class RoadMatrix{
    int m_width;
    int m_length;

    /**
     * @brief m_points points row by row
     */
    std::vector<lms::math::vertex2f> m_points;
    std::vector<std::vector<RoadMatrixCell>> m_cells;

    RoadMatrixCell getCell(int x, int y) const{
        if(x < 0 || x >= m_length){
            LMS_EXCEPTION("x does not fit");
        }else if(y < 0 || y >= m_width){
            LMS_EXCEPTION("y does not fit");
        }
        RoadMatrixCell c;
        int arrayLength = m_length +1;
        c.points[0]=m_points[x+arrayLength*y];
        c.points[1]=m_points[x+1+arrayLength*y];
        c.points[2]=m_points[x+1+arrayLength*(y+1)];
        c.points[3]=m_points[x+arrayLength*(y+1)];
        return c;
    }
    void createCells(){
        m_cells.clear();
        std::vector<RoadMatrixCell> tmp;
        tmp.resize(width());
        m_cells.resize(length(),tmp);
        for(int x = 0; x < m_length; x++){
            for(int y = 0; y < m_width; y++){
                m_cells[x][y] = getCell(x,y);
            }
        }
    }

public:

    const RoadMatrixCell &cell(int x,int y) const{
        //if(x < 0 || x > m_cells.size())
        return m_cells[x][y];
    }
    RoadMatrixCell &cell(int x,int y){
        //if(x < 0 || x > m_cells.size())
        return m_cells[x][y];
    }

    int width() const{
        return m_width;
    }
    int length() const{
        return m_length;
    }

    void aroundRoad(const lms::math::polyLine2f &l, int widthSteps, float stepWidth, float stepLength){
        //get new line with points in the right distance
        lms::math::polyLine2f lp = l.getWithDistanceBetweenPoints(stepLength);
        m_width = widthSteps*2;
        m_length = lp.points().size()-1;
        //clear old points
        m_points.clear();
        //create new points
        for(int i = -widthSteps; i <= widthSteps; i++){
            lms::math::polyLine2f top = lp.moveOrthogonal(stepWidth*i);
            m_points.insert(std::end(m_points), std::begin(top.points()), std::end(top.points()));
        }
        /*
        lms::logging::Logger logger("MYLOGGER");
        logger.error("POINTS")<<m_points.size();
        for(lms::math::vertex2f v:m_points){
            logger.error("POINT")<<v.x << " " << v.y;
        }
        */
        createCells();
    }

    bool checkBadPosition(lms::math::vertex2f v, float deltaBadness){
        int x;
        int y;
        return checkBadPosition(v,x,y,deltaBadness);
    }
    bool checkBadPosition(lms::math::vertex2f v,int &ix, int &iy,float deltaBadness){
        for(int x = 0; x < m_length; x++){
            for(int y = 0; y < m_width; y++){
                RoadMatrixCell &rmc = m_cells[x][y];
                if(rmc.contains(v)){
                    float currentBadness = rmc.badness();
                    //TODO useful deltaBadness value
                    currentBadness += deltaBadness;
                    if(currentBadness > 1)
                        currentBadness = 1;
                    rmc.badness(currentBadness);
                    ix = x;
                    iy = y;
                    return true;
                }
            }
        }
        return false;
    }

};

}  // namespace street_environment

namespace cereal {

template <class Archive>
struct specialize<Archive, street_environment::RoadLane, cereal::specialization::member_serialize> {};
  // cereal no longer has any ambiguity when serializing street_environment::RoadLane

}  // namespace cereal

#endif /* ENVIRONMENT_H */
