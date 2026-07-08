#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "LevelConfig.h"
#include "CVEHICLE.h"
#include "CANIMAL.h"
#include "CTRAFFIC_LV1.h"

// EntityManager: tách phần quản lý vòng đời của obstacle/animal/traffic light
// ra khỏi CGAME. CGAME chỉ cần gọi spawnFromLevel/update/draw/clear và
// đọc lại danh sách entity khi cần (vd. để check collision).
//
// Lý do tách: trước đây CGAME tự new/delete mObstacles, mAnimals, mTraffic
// và có 1 hàm createObstacle/createAnimal rải rác giữa file 1140 dòng.
// Gom vào đây giúp:
//  - Chỗ duy nhất chịu trách nhiệm vòng đời entity (dễ tìm leak nếu có).
//  - Chỗ duy nhất cần sửa khi thêm loại obstacle/animal mới (thêm level 3-5).
class EntityManager {
public:
    ~EntityManager();

    // Xoá toàn bộ entity hiện có rồi spawn theo LevelConfig mới.
    void spawnFromLevel(const LevelConfig& cfg);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    // Giải phóng entity hiện có (gọi tự động khi spawnFromLevel/destructor).
    void clear();

    const std::vector<CVEHICLE*>& obstacles() const { return mObstacles; }
    const std::vector<CANIMAL*>&  animals()   const { return mAnimals; }
    CTRAFFIC_LV1* traffic() const { return mTraffic; }

private:
    std::vector<CVEHICLE*> mObstacles;
    std::vector<CANIMAL*>  mAnimals;
    CTRAFFIC_LV1*          mTraffic = nullptr;

    static CVEHICLE* createObstacle(ObstacleType type, float speed, float direction);
    static CANIMAL*  createAnimal(AnimalType type, float speed, float direction);
};
