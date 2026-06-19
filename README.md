# 🦕 Crossing Street

> Đồ án môn Lập Trình Hướng Đối Tượng — FIT HCMUS  
> Game băng qua đường qua 5 thời đại: Tiền Sử → Cổ Đại → Phong Kiến → Hiện Đại → Tương Lai

---

## 👥 Thành viên nhóm

| MSSV     | Họ tên            | Phụ trách                                                      |
|----------|-------------------|----------------------------------------------------------------|
| 23127065 | Phan Gia Huy      | CGAME, main, LevelConfig, Save,Utils                           |
| 23127xxx | Nguyễn Hoàng Nhật | CPEOPLE, CVEHICLE, vật cản, sprite(Con người, map)             |
| 23127xxx | Nguyễn Văn Hải    | CANIMAL, động vật, âm thanh, sprite(động vật bay, vật thể bay) |
| 23127xxx | Phan Đình Quốc    | Menu, HUD, sprite(menu, hud)                                   |

> **Giáo viên hướng dẫn:** Thầy Trương Toàn Thịnh — Khoa CNTT, ĐHKHTN TP.HCM

---

## 🎮 Giới thiệu

**Crossing Street** là game 2D theo phong cách Frogger cổ điển, nơi người chơi điều khiển nhân vật băng qua các làn đường đầy nguy hiểm qua 5 thời đại lịch sử. Tránh khủng long thời tiền sử, xe ngựa cổ đại, kỵ binh phong kiến, xe hơi hiện đại và robot tương lai để đến đích an toàn!

---

## ⚙️ Yêu cầu hệ thống

| | Windows | macOS |
|---|---|---|
| OS | Windows 10/11 | macOS 12+ |
| Compiler | Visual Studio 2022 | Xcode / clang++ |
| Build tool | CMake 3.16+ | CMake 3.16+ |
| Thư viện | SFML 2.6.2 | SFML 2.6.2 |

---

## 🚀 Cài đặt và chạy

### Windows

**Bước 1** — Cài đặt SFML 2.6.2:
- Tải tại: https://www.sfml-dev.org/download/sfml/2.6.2/
- Chọn: `Visual C++ 17 (2022) - 64-bit`
- Giải nén vào `C:\SFML-2.6.2\`

**Bước 2** — Cài đặt CMake:
- Tải tại: https://cmake.org/download/
- Tick **"Add to PATH"** khi cài đặt

**Bước 3** — Build và chạy:
```
Double-click: build.bat
```
Hoặc mở CMD trong thư mục project:
```cmd
build.bat run
```

> Nếu báo lỗi thiếu DLL, copy thủ công từ `C:\SFML-2.6.2\bin\` vào `build_win\Release\`

---

### macOS

**Bước 1** — Cài Homebrew (nếu chưa có):
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

**Bước 2** — Cài SFML và CMake:
```bash
brew install sfml@2 cmake
brew link sfml@2 --force
```

**Bước 3** — Build và chạy:
```bash
chmod +x build.sh
./build.sh run
```

---

## 🕹️ Điều khiển

| Phím                                      | Tác dụng |
|-------------------------------------------|---|
| `W / A / S / D`  hoặc nút di chuyển hướng | Di chuyển nhân vật |
| `P`                                       | Pause / Resume |
| `R`                                       | Restart level |
| `ESC`                                     | Mở hộp thoại thoát game |
| `F1 / F2 / F3`                            | Lưu game vào slot 1 / 2 / 3 |
| `F4 / F5 / F6`                            | Tải game từ slot 1 / 2 / 3 |

---

## 🗺️ Gameplay

| Level | Thời đại | Vật cản | Động vật bay |
|---|---|---|---|
| 1 | 🦕 Tiền Sử | Khủng long, Voi Mamut | Pterodactyl |
| 2 | 🏛️ Cổ Đại | Xe ngựa, Chiến binh | Đại bàng |
| 3 | ⚔️ Phong Kiến | Ngựa chiến, Xe bò | Quạ |
| 4 | 🚗 Hiện Đại | Xe hơi, Xe tải | Chim bồ câu |
| 5 | 🚀 Tương Lai | Xe bay, Robot | Drone |

### Luật chơi
- Điều khiển nhân vật từ vùng cỏ dưới lên đến vùng cỏ trên để qua màn
- Chạm vào vật cản → chết, phải chơi lại từ đầu level
- Hết thời gian giới hạn → chết
- Đèn giao thông xanh → vật cản chạy, đèn đỏ → vật cản dừng
- Qua đủ 5 level → VICTORY!

### Tính điểm
```
Score = Base Score + Time Bonus
Base Score  = 100 × Level
Time Bonus  = Thời gian còn lại (giây) × 10
```

---

## 💾 Save / Load

Game hỗ trợ **3 slot lưu**:
- `F1/F2/F3` để lưu vào slot tương ứng (lần đầu sẽ hỏi tên)
- `F4/F5/F6` để tải lại từ slot tương ứng
- Load Game từ Menu chính để chọn slot có sẵn

File save được lưu tại `saves/save_slot{N}.txt`

---

## 📁 Cấu trúc project

```
CrossingGame/
├── src/
│   ├── main.cpp
│   ├── core/              ← CGAME, CPEOPLE, Animation, CTRAFFICLIGHT, SaveData
│   ├── obstacles/
│   │   └── lv1/
│   │       ├── animal_lv1/   ← CDINOSOUR, CMAMMOTH, CBIRD
│   │       └── traffic/      ← CTRAFFIC_LV1
│   ├── animals/           ← CANIMAL (base)
│   ├── vehicle/           ← CVEHICLE (base)
│   ├── levels/            ← LevelConfig
│   └── ui/                ← Menu, HUD
├── assets/
│   ├── sprites/           ← spritesheet nhân vật, xe, thú
│   ├── background/        ← ảnh nền 5 thời đại
│   ├── sounds/            ← nhạc nền, hiệu ứng âm thanh
│   ├── font/              ← font chữ pixel
│   └── ui/                ← ảnh menu, HUD
├── saves/                 ← file lưu game (tự tạo khi chạy)
├── build.bat              ← build Windows
├── build.sh               ← build macOS
└── CMakeLists.txt
```

---

## 🛠️ Công nghệ sử dụng

- **Ngôn ngữ:** C++17
- **Thư viện đồ họa:** SFML 2.6.2
- **Build system:** CMake 3.16+
- **IDE:** CLion (macOS), Visual Studio 2022 (Windows)
- **Đồ họa:** pixel art tự thiết kế + AI-generated (DALL-E, ChatGPT)
- **Âm thanh:** Suno AI

---

## 📝 Ghi chú

- Nếu game chạy chậm trên máy yếu, kiểm tra build đang ở chế độ **Release** (không phải Debug)
- File save lưu dạng text tại `saves/` — không xóa thư mục này nếu muốn giữ tiến trình
- Trên macOS với màn hình Retina, cửa sổ game được cố định ở `1280×720` để tránh lỗi scaling
