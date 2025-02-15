#include <array>
#include <utility> // for std::pair
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace nova {

    // 픽셀 좌표를 OpenGL 좌표로 변환
    std::pair<float, float> pixelPosToGLPos(int x, int y, int screenWidth, int screenHeight) {
        float glX = 2.0f * (x + 0.5f) / screenWidth - 1.0f;
        float glY = 2.0f * (y + 0.5f) / screenHeight - 1.0f;
        return {glX, -glY}; // OpenGL에서 Y축은 반대로 사용됨
    }




    std::array<float, 20> createVertices(
        int pixelX, int pixelY, int pixelWidth, int pixelHeight, 
        int screenWidth, int screenHeight) {
        float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

        auto [startX, startY] = pixelPosToGLPos(pixelX, pixelY, screenWidth, screenHeight);
        auto [endX, endY] = pixelPosToGLPos(pixelX + pixelWidth, pixelY + pixelHeight, screenWidth, screenHeight);

        // OpenGL 좌표에서 화면 비율을 적용
        startX *= aspectRatio;
        endX *= aspectRatio;

        return std::array<float, 20>{{
            startX, startY, 0.0f, 0.0f, 1.0f,  // 왼쪽 상단 정점
            endX, startY, 0.0f, 1.0f, 1.0f,   // 오른쪽 상단 정점
            endX, endY, 0.0f, 1.0f, 0.0f,    // 오른쪽 하단 정점
            startX, endY, 0.0f, 0.0f, 0.0f   // 왼쪽 하단 정점
        }};
    }

    // JSON 파일을 읽고 파싱하는 함수
    json parseJsonFile(const std::string& filePath) {
        std::ifstream inputFile(filePath);

        // 파일 열기 확인
        if (!inputFile.is_open()) {
            throw std::runtime_error("Could not open the file: " + filePath);
        }

        // JSON 파일 읽기
        json jsonData;
        try {
            inputFile >> jsonData;
        } catch (const json::parse_error& e) {
            throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
        }

        return jsonData;
    }
}
