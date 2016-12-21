#include "street_environment/roadmatrix.h"

#include <algorithm>

namespace street_environment {

bool RoadMatrix::initialize(float laneWidth, int cellsPerLane, float cellLength,
                            float maxTranslation) {
    if (!m_initalized) {
        m_cellsPerLane = cellsPerLane;
        m_cellLength = cellLength;
        m_cellWidth = static_cast<float>(laneWidth / m_cellsPerLane);
        m_width = cellsPerLane * 2;
        m_pWidth = m_width + 1;
        m_maxTranslation = maxTranslation;
        return true;
    }
    return false;
}

void RoadMatrix::aroundLine(const lms::math::polyLine2f& line,
                            const lms::math::vertex2f& deltaPosition,
                            float deltaRotation) {
    lms::math::polyLine2f centerLine =
        prepCenterLine(line, deltaPosition, deltaRotation);
    aroundLine(centerLine);
    initCells();
    m_prevCenterLine = line;
}

bool RoadMatrix::findCell(const lms::math::vertex2f& v,
                          street_environment::RoadMatrixCell* foundCell) {
    for (int x = 0; x < length(); x++) {
        for (int y = 0; y < width(); y++) {
            const street_environment::RoadMatrixCell& rmc = cell(x, y);
            if (rmc.contains(v)) {
                *foundCell = rmc;
                return true;
            }
        }
    }
    return false;
}

lms::math::polyLine2f RoadMatrix::prepCenterLine(
    const lms::math::polyLine2f& line, const lms::math::vertex2f& deltaPosition,
    float deltaRotation) {
    std::vector<lms::math::vertex2f> points(
        negativeCenterLinePoints(deltaPosition, deltaRotation));

    lms::math::polyLine2f centerLine = negativeCenterLine(points);

    m_translation = centerLine.points().size();

    if (line.points().size() > 0) {
        lms::math::polyLine2f scaledLine =
            line.getWithDistanceBetweenPoints(m_cellLength);
        for (const auto& point : scaledLine.points()) {
            centerLine.points().push_back(point);
        }
    }

    return centerLine;
}

std::vector<lms::math::vertex2f> RoadMatrix::negativeCenterLinePoints(
    const lms::math::vertex2f& deltaPosition, float deltaRotation) {
    std::vector<lms::math::vertex2f> points;
    for (const auto& point : m_negativeCenterLine.points()) {
        points.push_back((point - deltaPosition).rotate(-deltaRotation));
    }
    for (const auto& point : m_prevCenterLine.points()) {
        lms::math::vertex2f newPoint =
            (point - deltaPosition).rotate(-deltaRotation);
        if (newPoint.x < 0) {
            points.push_back(newPoint);
        } else {
            break;
        }
    }
    return points;
}

lms::math::polyLine2f RoadMatrix::negativeCenterLine(
    const std::vector<lms::math::vertex2f>& points) {
    lms::math::polyLine2f negativeCenterLineBackwards;
    for (auto it = points.crbegin(); it != points.crend(); it++) {
        if (negativeCenterLineBackwards.length() < m_maxTranslation) {
            negativeCenterLineBackwards.points().push_back(*it);
        } else {
            break;
        }
    }
    const std::vector<lms::math::vertex2f>& centerLinePointsBackwards =
        negativeCenterLineBackwards.points();
    lms::math::polyLine2f centerLine;
    for (auto it = centerLinePointsBackwards.crbegin();
         it != centerLinePointsBackwards.crend(); it++) {
        centerLine.points().push_back(*it);
    }
    if (centerLine.points().size() > 0) {
        centerLine = centerLine.getWithDistanceBetweenPoints(m_cellLength);
        centerLine.points().pop_back();
        m_negativeCenterLine = centerLine;
        centerLine.points().pop_back();
    }

    return centerLine;
}

void RoadMatrix::aroundLine(const lms::math::polyLine2f& line) {
    m_length = line.points().size() - 1;
    m_pLength = m_length + 1;
    m_points.resize(m_pLength * m_pWidth);
    for (int dy = m_cellsPerLane; dy >= -m_cellsPerLane; dy--) {
        lms::math::polyLine2f movedLine = line.moveOrthogonal(m_cellWidth * dy);
        for (int x = 0; x < m_pLength; x++) {
            int y = -(dy - m_cellsPerLane);
            point(x, y) = movedLine.points().at(x);
        }
    }
}

RoadMatrixCell RoadMatrix::initCell(int x, int y) const {
    RoadMatrixCell c;
    c.x = x;
    c.y = y;
    c.points.at(0) = point(x, y);
    c.points.at(1) = point(x + 1, y);
    c.points.at(2) = point(x + 1, y + 1);
    c.points.at(3) = point(x, y + 1);
    return c;
}

void RoadMatrix::initCells() {
    m_cells.resize(length() * width());
    for (int x = 0; x < length(); x++) {
        for (int y = 0; y < width(); y++) {
            cell(x, y) = initCell(x, y);
        }
    }
}

const RoadMatrixCell& RoadMatrix::cell(int x, int y) const {
    return m_cells.at(x * m_width + y);
}
RoadMatrixCell& RoadMatrix::cell(int x, int y) {
    return m_cells.at(x * m_width + y);
}

const lms::math::vertex2f& RoadMatrix::point(int x, int y) const {
    return m_points.at(x * m_pWidth + y);
}
lms::math::vertex2f& RoadMatrix::point(int x, int y) {
    return m_points.at(x * m_pWidth + y);
}

std::ostream& operator<<(std::ostream& stream, const RoadMatrix& matrix) {
    stream << "[\n";
    for (int y = matrix.width() - 1; y >= 0; y--) {
        stream << "[";
        for (int x = 0; x < matrix.length(); x++) {
            stream << "[";
            for (int i = 0; i < 4; i++) {
                stream << "(" << matrix.cell(x, y).points[i].x << ","
                       << matrix.cell(x, y).points[i].y << ")";
            }
            stream << "]";
        }
        stream << "]\n";
    }
    stream << "]\n";
    return stream;
}

}  // namespace street_environment
