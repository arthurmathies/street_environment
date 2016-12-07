#include "street_environment/roadmatrix.h"

#include <algorithm>

namespace street_environment {

bool RoadMatrix::initialize(float laneWidth, int cellsPerLane, float cellLength,
                            int maxTranslation) {
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
    if (size() > 0) {
        if (deltaPosition.x >= m_cellLength) {
            translate(deltaPosition);
        }
        rotate(deltaRotation);
    }
    lms::math::polyLine2f scaledLine =
        line.getWithDistanceBetweenPoints(m_cellLength);
    if (scaledLine.points().size() >= 2) {
        aroundLine(scaledLine);
    }
    initCells();
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

void RoadMatrix::rotate(float deltaRotation) {
    for (int x = 0; x < m_translation; x++) {
        for (int y = 0; y < m_pWidth; y++) {
            point(x, y) = point(x, y).rotate(deltaRotation);
        }
    }
}

void RoadMatrix::translate(const lms::math::vertex2f& deltaPosition) {
    RoadMatrixCell deltaCell;
    if (findCell(deltaPosition, &deltaCell)) {
        int prevTranslation = m_translation;
        int deltaX = deltaCell.x - prevTranslation;
        m_translation = std::min(deltaX + prevTranslation, m_maxTranslation);
        int firstColumn = deltaX - m_translation + prevTranslation;
        for (int x = 0; x < m_translation; x++) {
            for (int y = 0; y < m_pWidth; y++) {
                point(x, y) = point(x + firstColumn, y) - deltaPosition;
            }
        }
        m_points.resize(m_translation * m_pWidth);
        m_length = m_translation - 1;
        m_pLength = m_translation;
    }
}

void RoadMatrix::aroundLine(const lms::math::polyLine2f& line) {
    m_length = line.points().size() - 1 + m_translation;
    m_pLength = m_length + 1;
    m_points.resize(m_pLength * m_pWidth);
    for (int dy = m_cellsPerLane; dy >= -m_cellsPerLane; dy--) {
        lms::math::polyLine2f movedLine = line.moveOrthogonal(m_cellWidth * dy);
        for (int x = m_translation, i = 0; x < m_pLength; x++, i++) {
            int y = -(dy - m_cellsPerLane);
            point(x, y) = movedLine.points().at(i);
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
