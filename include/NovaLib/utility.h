#ifndef UTILITY_H
#define UTILITY_H

#include <array>
#include <utility> // for std::pair
#include <nlohmann/json.hpp>

namespace nova {
    // 픽셀 좌표를 OpenGL 좌표로 변환
    std::pair<float, float> pixelPosToGLPos(int x, int y, int screenWidth, int screenHeight);

    // 정점 데이터 생성
    std::array<float, 20> createVertices(int pixelX, int pixelY, int pixelWidth, int pixelHeight, int screenWidth, int screenHeight);

    nlohmann::json parseJsonFile(const std::string& filePath);
}

#endif // UTILITY_H
