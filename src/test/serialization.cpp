#include "gtest/gtest.h"

#include "street_environment/road.h"

#include <cereal/archives/portable_binary.hpp>

using namespace street_environment;

template<typename T>
using output_serializers = cereal::traits::detail::count_output_serializers<T,
    cereal::PortableBinaryOutputArchive>;

template<typename T>
using input_serializers = cereal::traits::detail::count_input_serializers<T,
    cereal::PortableBinaryInputArchive>;

TEST(Serialization, RoadLane) {
    // Static tests for valid serializers
    ASSERT_GT(output_serializers<RoadLane>::value, 0);

    std::stringstream ss;
    {
        cereal::PortableBinaryOutputArchive oarchive(ss); // Create an output archive

        RoadLane lane;
        lane.polarDarstellung.push_back(1);
        lane.polarDarstellung.push_back(2);
        lane.polarDarstellung.push_back(3);
        lane.polarPartLength = 20;
        lane.points().push_back(lms::math::vertex2f(4,5));
        lane.points().push_back(lms::math::vertex2f(6,7));
        lane.type(RoadLaneType::MIDDLE);
        lane.name("my lane");
        oarchive(lane); // Write the data to the archive
    }

    {
        cereal::PortableBinaryInputArchive iarchive(ss); // Create an input archive

        RoadLane lane;
        iarchive(lane); // Read the data from the archive

        ASSERT_EQ(3, lane.polarDarstellung.size());
        EXPECT_DOUBLE_EQ(1, lane.polarDarstellung[0]);
        ASSERT_EQ(20, lane.polarPartLength);

        ASSERT_EQ(2, lane.points().size());
        EXPECT_EQ(lms::math::vertex2f(4,5), lane.points()[0]);

        ASSERT_EQ(RoadLaneType::MIDDLE, lane.type());
        ASSERT_EQ(std::string("my lane"), lane.name());
    }
}
