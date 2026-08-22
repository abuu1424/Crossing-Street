# 🎮 CROSSING STREET: TIME ODYSSEY ⏳

> **Đồ Án Môn Học: Lập Trình Hướng Đối Tượng (Object-Oriented Programming - OOP)**  
> **Khoa Công Nghệ Thông Tin — Trường Đại Học Khoa Học Tự Nhiên, ĐHQG-HCM (HCMUS)**

<div align="center">

![C++17](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=c%2B%2B)
![SFML](https://img.shields.io/badge/Library-SFML_2.6.2-8CC651?style=for-the-badge&logo=sfml)
![CMake](https://img.shields.io/badge/Build-CMake_3.16%2B-064F8C?style=for-the-badge&logo=cmake)
![Platform](https://img.shields.io/badge/Platform-macOS_%7C_Windows-blue?style=for-the-badge)
![Status](https://img.shields.io/badge/Release-v1.0_Final-success?style=for-the-badge)

</div>

---

## 📖 1. Giới Thiệu Dự Án (Overview)

**Crossing Street: Time Odyssey** là tựa game arcade 2D vượt đường kết hợp du hành thời gian được phát triển bằng **C++17** và thư viện đồ họa **SFML 2.6.2 / 2.5**. 

Người chơi sẽ điều khiển nhân vật băng qua các làn đường giao thông nguy hiểm, xuyên không qua **5 kỷ nguyên lịch sử** từ thời Tiền sử hoang dã đến Tương lai Cyberpunk. Dự án được thiết kế hoàn chỉnh với kiến trúc hướng đối tượng (OOP) mẫu mực, tích hợp nhiều chế độ chơi đột phá: chiến dịch cốt truyện, đối kháng 2 người cục bộ, đua tốc độ với Bot AI thông minh (thuật toán Spatio-Temporal A*), kỹ năng chủ động, cửa hàng bổ trợ, thảm họa môi trường động và hệ thống cutscene điện ảnh mượt mà.

---

## 👥 2. Đội Ngũ Phát Triển (Development Team)

| Thành Viên | Vai Trò Chuyên Trách | Phụ Trách Module & Đóng Góp Kỹ Thuật |
| :--- | :--- | :--- |
| **Phan Gia Huy** | **Lead Architect & AI Engineer** | • **Core Modules:** `CGAME`, `BotAI` (Spatio-Temporal A*), `SaveData`, `LevelConfig`, `ElevatorCutscene`, `DeathCutscene`<br>• Thiết kế kiến trúc tổng thể, vòng lặp game đa trạng thái (State Machine), thuật toán tìm đường AI né tránh 16.6Hz, chuyển cảnh thang máy điện ảnh, hệ thống lưu/tải dữ liệu đa slot và tối ưu hóa bộ nhớ. |
| **Nguyễn Văn Hải** | **Audio & Entity Simulation Engineer** | • **Core Modules:** `CANIMAL` (Base & Derived), `SoundManager`, Dynamic Audio Ducking<br>• Động cơ âm thanh thích ứng đa kênh với hiệu ứng Dynamic Ducking, hành vi di chuyển động vật và vật thể bay trên không (20+ entities), tái hiện không gian âm thanh cho từng thời kỳ. |
| **Phan Đình Quốc** | **UI/UX Director & Systems Designer** | • **Core Modules:** `Menu` Systems, `HUD` Telemetry, `ShopData`, Item Shop, High Scores, Movie Roll Credits<br>• Giao diện Menu tương tác đa màn hình, thanh đo chỉ số HUD song song thời gian thực, kinh tế cửa hàng vật phẩm, bảng xếp hạng kỷ lục và hệ thống cuộn Credits phong cách điện ảnh. |
| **Nguyễn Hoàng Nhật** | **World Artist & Gameplay Programmer** | • **Core Modules:** `CPEOPLE`, Obstacles & Traffic (`CVEHICLE`, `CTRAFFICLIGHT`), `HazardManager`, `CollisionEffect`<br>• Vật lý nhân vật và bộ kỹ năng chủ động (Tốc biến, Nam châm, Đóng băng thời gian), cơ chế thảm họa thiên tai động, hiệu ứng hạt va chạm VFX và 5 chuỗi hoạt ảnh chết đặc trưng cho từng màn chơi. |

---

## 🕹️ 3. Các Chế Độ Chơi (Game Modes)

### 🏛️ 1. Story Campaign (Chế Độ Cốt Truyện 5 Kỷ Nguyên)
- Du hành xuyên qua **5 màn chơi** với bối cảnh, âm thanh, chướng ngại vật và giới hạn thời gian riêng biệt.
- Chuyển tiếp giữa các thời kỳ bằng **Elevator Cutscene** (Thang máy thời gian chuyển động cơ học).
- Hoạt ảnh tử vong (Death Cutscene) riêng biệt cho từng kỷ nguyên khi người chơi thất bại.

### ⚔️ 2. Local 2-Player Versus (Đối Kháng 1v1 Cùng Bàn Phím)
- Chia đôi quyền điều khiển trên cùng một bàn phím (`WASD` vs `Phím Mũi Tên`).
- Thanh máu (HP), thể lực (Stamina), kỹ năng và bộ đếm hiệp thắng độc lập (`P1` vs `P2`).
- Tranh giành các vật phẩm Power-Up xuất hiện ngẫu nhiên trên đường và hỗ trợ chơi lại tức thì (`R`).

### 🤖 3. VS Bot AI (Đua Tốc Độ Với Trí Tuệ Nhân Tạo)
- Thi đấu với đối thủ Bot tự động vận hành bởi thuật toán **Spatio-Temporal A\*** (tìm đường không-thời gian dự đoán va chạm):
  - **Easy:** Tốc độ di chuyển vừa phải, thong thả qua đường, thích hợp cho người mới làm quen.
  - **Normal:** Tốc độ tương đương người chơi, biết tính toán nhịp đèn giao thông và chủ động nhặt buff.
  - **Hard:** Cập nhật quỹ đạo liên tục ở tần số 16.6Hz, né tránh siêu chuẩn xác (micro-dodging), không phạm sai lầm.
- Bảng điều khiển HUD hiển thị song song trạng thái Bot (`RACING`, `FROZEN`, `ELIMINATED`), lượng máu và tiến độ về đích.

### ♾️ 4. Endless Chrono Mode (Chế Độ Sinh Tồn Vô Tận)
- Vượt ải vô hạn qua các dòng thời gian với cấp số nhân điểm thưởng.
- Tốc độ di chuyển của vật cản và tần suất xuất hiện thảm họa gia tăng lũy tiến theo từng Wave.
- Tự động lưu trữ điểm kỷ lục cao nhất (High Score Persistence) cùng huy hiệu Wave đã đạt.

---

## ⚡ 4. Kỹ Năng Chủ Động & Vật Phẩm (Skills & Items)

### 🔮 Bộ Kỹ Năng Chủ Động (Active Skills)
| Kỹ Năng | Phím P1 | Phím P2 | Thời Gian | Hồi Chiêu | Hiệu Ứng Chi Tiết |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Sprint** | `LShift` | `RShift` / `RCtrl` | Khi giữ | Hồi tự động | Tăng +50% tốc độ di chuyển, tiêu hao thanh Thể lực (Stamina). |
| **Speed Surge** | `E` / `Space` | `I` / `[` | 5.0 giây | 10.0 giây | Tăng +50% tốc độ chạy kèm hào quang sấm sét xung quanh nhân vật. |
| **Coin Magnet Radar** | `Q` | `K` / `]` / `O` | 6.0 giây | 10.0 giây | Phát sóng radar hút toàn bộ xu trong bán kính 700px và làm chậm vật cản xung quanh. |
| **Time Freeze Clock** | `T` | `L` / `\` | 5.0 giây | 12.0 giây | Ngưng đọng thời gian, làm tê liệt toàn bộ xe cộ, sinh vật và thảm họa trên bản đồ. |

### 🎁 Vật Phẩm Nhặt Trên Làn Đường (In-Lane Power-Ups)
- 🧲 **Magnet Orb (Xanh lam):** Hút toàn bộ đồng xu trên màn hình về phía người chơi trong 8s.
- ⏱️ **Time Stop (Đồng hồ cát):** Làm chậm 85% tốc độ của toàn bộ xe cộ và chướng ngại vật trong 4s.
- 🔥 **Speed Boost (Ngọn lửa):** Gia tăng 60% tốc độ di chuyển kèm vệt lửa lướt gió trong 6s.
- 🛡️ **Bubble Shield (Bong bóng năng lượng):** Miễn nhiễm hoàn toàn 1 lần va chạm chí mạng và chống hiệu ứng giật lùi.
- ⭐ **2X Multiplier (Huy chương vàng):** Nhân đôi toàn bộ số vàng và điểm số ghi được trong 10s.

### 🛒 Cửa Hàng Trang Bị (Item Shop)
- **Energy Shield (250 Coins):** Khiên năng lượng sử dụng 1 lần, cứu mạng khi xảy ra tai nạn.
- **Speed Boots (400 Coins):** Tăng vĩnh viễn +15% tốc độ cơ bản và mở khóa kỹ năng Speed Surge (`E`).
- **Time Extender (350 Coins):** Cộng thêm +8 giây giới hạn thời gian mỗi màn và mở khóa kỹ năng Time Freeze (`T`).
- **Coin Magnet Radar (500 Coins):** Giảm 15% tốc độ nguy hiểm xung quanh và mở khóa kỹ năng Radar (`Q`).
- **Extra Heart (1500 Coins):** Mua thêm 1 tim tối đa và hồi đầy toàn bộ thanh máu.

---

## 🌪️ 5. Thảm Họa Thiên Tai Động (Dynamic Level Hazards)

| Cấp Độ | Kỷ Nguyên | Vật Cản Mặt Đất | Vật Thể Trên Không | Thảm Họa Thiên Tai Đặc Trưng | Hoạt Ảnh Tử Vong (Death VFX) |
| :---: | :--- | :--- | :--- | :--- | :--- |
| **Level 1** | **Thời Tiền Sử** *(Prehistoric)* | Khủng long bạo chúa, Voi ma mút | Chim cổ đại Pterodactyl | **Dino Stampede Surge**<br>Đàn khủng long tràn qua các làn đường kèm rung chấn dữ dội. | Quái vật đầm lầy trồi lên cắn xé *(Swamp Monster Chomp)* |
| **Level 2** | **Ai Cập Cổ Đại** *(Ancient Egypt)* | Lạc đà sa mạc, Tượng Nhân sư | Chim ưng Horus, Kền kền Ai Cập | **Sandstorm Wind Drift**<br>Bão cát cuồng phong làm mờ màn hình và tạo lực gió đẩy trôi người chơi. | Lún chìm vào cát lún sa mạc *(Quicksand Sink)* |
| **Level 3** | **Thời Trung Cổ** *(Medieval Kingdom)* | Xe ngựa chiến, Xe kéo, Khúc gỗ lăn | Mũi tên xạ thủ, Đàn quạ đen | **Flaming Arrow Rain**<br>Mưa tên lửa rực cháy trút xuống ngẫu nhiên từ bầu trời theo tâm cảnh báo. | Cuốn bay trong lốc xoáy lá phong *(Leaf Whirlwind)* |
| **Level 4** | **Hiện Đại** *(Modern Metropolis)* | Xe hơi thể thao, Xe buýt, Xe máy | Máy bay phản lực, Tên lửa tầm nhiệt | **Rush Hour & Thunderstorm**<br>Giờ cao điểm xe tăng tốc đột biến kết hợp sấm sét giáng thẳng xuống làn. | Tan biến thành làn khói đen *(Smoke Dissolve)* |
| **Level 5** | **Tương Lai** *(Cyber Future)* | Xe điện thông minh, Tàu Hyperloop | Đĩa bay UFO, Drone chiến đấu, Xe bay | **Singularity Black Hole & Lasers**<br>Lỗ đen vũ trụ hút mọi vật thể cùng các chùm tia laser tử thần quét ngang. | Bị phân rã thành hạt Plasma *(Plasma Disintegration)* |

---

## ⌨️ 6. Bảng Phím Điều Khiển (Controls Guide)

### Điều Khiển Người Chơi (Gameplay Controls)
| Phím Nhấn | Thao Tác / Chức Năng |
| :--- | :--- |
| **`W` `A` `S` `D`** | Player 1 / Chế độ Đơn: Di chuyển Lên / Trái / Xuống / Phải |
| **`LShift`** | Player 1: Tăng tốc chạy nhanh (Sprint - tiêu hao Thể lực) |
| **`E` / `Space`** | Player 1: Kích hoạt kỹ năng Tăng Tốc (Speed Surge) |
| **`Q`** | Player 1: Kích hoạt kỹ năng Sóng Radar Hút Xu (Magnet Radar) |
| **`T`** | Player 1: Kích hoạt kỹ năng Đóng Băng Thời Gian (Time Freeze Clock) |
| **`1` `2` `3` `4`** | Phím tắt sử dụng nhanh trang bị trong túi đồ (Khiên, Giày, Đồng hồ, Radar) |
| **`↑` `←` `↓` `→`** | Player 2 (Chế độ 2 Người): Di chuyển Lên / Trái / Xuống / Phải |
| **`RShift` / `RCtrl` / `Num 0`** | Player 2: Tăng tốc chạy nhanh (Sprint) |
| **`I` / `[`** | Player 2: Kỹ năng Tăng Tốc (Speed Surge) |
| **`K` / `]` / `O`** | Player 2: Kỹ năng Sóng Radar Hút Xu |
| **`L` / `\`** | Player 2: Kỹ năng Đóng Băng Thời Gian |

### Phím Chức Năng Hệ Thống (System & Utility Hotkeys)
| Phím Nhấn | Thao Tác / Chức Năng |
| :--- | :--- |
| **`P`** | Tạm dừng trò chơi (Pause) / Bảng điều chỉnh âm lượng BGM & SFX |
| **`R`** | Chơi lại màn chơi hiện tại (Quick Restart / Rematch) |
| **`S`** | Mở Cửa Hàng Vật Phẩm tại màn hình chiến thắng hoặc xóa màn |
| **`M`** | Mở bảng xác nhận quay về Menu chính |
| **`ESC`** | Trở về trang trước / Mở bảng xác nhận thoát game |
| **`Enter` / `Space`** | Bỏ qua nhanh đoạn phim chuyển cảnh (Skip Cutscene) |
| **`F1` — `F3`** | Lưu game nhanh vào Ô Lưu Trữ (Slot 1 — Slot 3) |
| **`F4` — `F6`** | Tải game nhanh từ Ô Lưu Trữ (Slot 1 — Slot 3) |
| **`F11` / `Alt + Enter`** | Bật / Tắt chế độ Toàn màn hình (Fullscreen Toggle) |
| **`V`** | Bật / Tắt hiển thị khung va chạm (Debug Hitboxes) |

---

## 🛠️ 7. Bảng Lệnh Nhà Phát Triển (Developer Console & Cheats)

> Nhấn phím **`~`** (hoặc `` ` ``, `F8`, `F9`, `F12`, `Tab`, `]`, `/`) trong khi chơi để mở thanh nhập lệnh Developer Console.

| Cú Pháp Lệnh | Chức Năng & Kết Quả Thực Thi |
| :--- | :--- |
| `god` / `invuln` | Bật / Tắt chế độ Bất Tử *(God Mode - Infinite Invulnerability)* |
| `coins <Số_Lượng>` | Thêm ngay số tiền vàng mong muốn (Ví dụ: `coins 5000`) |
| `hp <Số_Tim>` | Đặt lượng máu tối đa của người chơi từ 1 đến 10 tim (Ví dụ: `hp 5`) |
| `heal` | Hồi phục 100% HP và đầy đủ thanh năng lượng Thể lực |
| `level <1-5>` / `lvl <1-5>` | Dịch chuyển tức thời đến Màn chơi chỉ định (Ví dụ: `level 5`) |
| `skill all` / `buy all` | Mở khóa toàn bộ vật phẩm trong Shop và mở full kỹ năng |
| `nrg` / `energy` | Hồi phục tức thì 100% thanh Thể lực (Stamina) |
| `score <Số_Điểm>` | Cộng trực tiếp điểm số vào điểm hiện tại (Ví dụ: `score 1000`) |
| `help` | Hiển thị danh sách tóm tắt toàn bộ câu lệnh console |

---

## 🎬 8. Hệ Thống Credits Phong Cách Điện Ảnh (Movie Roll)

Truy cập mục **CREDITS** từ Menu chính để trải nghiệm hệ thống cuộn thông tin tri ân phong cách phim chiếu rạp:
- **Tự động cuộn mượt mà:** Trình bày thông tin Trường ĐH KHTN (HCMUS), Bộ môn OOP, Chân dung thành viên, Chi tiết module phụ trách và Lời cảm ơn.
- **Điều khiển tiện lợi:**
  - `Space` hoặc nút trên màn hình: Chuyển đổi tốc độ cuộn **1X / 3X**.
  - `P`: Tạm dừng / Tiếp tục cuộn (Pause / Resume).
  - `R`: Khởi động cuộn lại từ đầu (Restart).
  - `Cuộn chuột` hoặc `W`/`S` (`↑`/`↓`): Kéo xem vị trí bất kỳ tùy ý.
  - `ESC`: Trở về Main Menu.

---

## 🏗️ 9. Kiến Trúc Hướng Đối Tượng (OOP & Project Architecture)

### Cấu Trúc Mã Nguồn Chuẩn Hóa
```text
Crossing Street/
├── CMakeLists.txt              # Cấu hình biên dịch CMake đa nền tảng
├── build.sh                    # Kịch bản tự động build & run trên macOS
├── build.bat                   # Kịch bản tự động build & run trên Windows
├── assets/                     # Tài nguyên hình ảnh, âm thanh, font chữ
│   ├── background/             # Hình nền 5 màn chơi
│   ├── credits/                # Ảnh chân dung thành viên nhóm
│   ├── font/                   # Pixel Operator font
│   ├── hazards/                # Sprite sheet hoạt ảnh thảm họa 12 frames
│   ├── shop/                   # Icon và animation hiệu ứng shop & kỹ năng
│   ├── sounds/                 # Âm nhạc BGM và âm thanh SFX cho từng thời kỳ
│   ├── sprites/                # Nhân vật, xe cộ, thú và vật thể bay
│   └── ui/                     # Nút bấm, khung thoại, HUD, pop-up
├── saves/                      # Thư mục chứa file lưu game & kỷ lục High Score
└── src/                        # Toàn bộ mã nguồn C++
    ├── main.cpp                # Điểm khởi chạy chương trình (Entry point)
    ├── ai/                     # Trí tuệ nhân tạo
    │   ├── BotAI.h/.cpp        # Thuật toán Spatio-Temporal A* & né tránh va chạm
    ├── core/                   # Hạt nhân động cơ game
    │   ├── CGAME.h/.cpp        # Vòng lặp chính, quản lý State, render, phím bấm, dev console
    │   ├── SaveData.h/.cpp     # Đọc/ghi tuần tự hóa file lưu trữ (Slot 1-3)
    │   ├── SoundManager.h/.cpp # Động cơ âm thanh, BGM ducking, SFX mixer
    │   └── TextureManager.h/.cpp # Bộ nhớ đệm tài nguyên hình ảnh (Texture Cache)
    ├── cutscenes/              # Hệ thống hoạt họa & chuyển cảnh
    │   ├── Animation.h/.cpp    # Bộ điều khiển cắt frame Sprite Sheet
    │   ├── ElevatorCutscene.h/.cpp # Chuyển cảnh thang máy thời gian
    │   ├── DeathCutscene.h/.cpp    # 5 hoạt ảnh tử vong đặc thù từng kỷ nguyên
    │   └── CollisionEffect.h/.cpp  # Hiệu ứng nổ hạt khi xảy ra va chạm
    ├── entities/               # Thực thể trò chơi
    │   ├── base/               # Lớp cơ sở (CPEOPLE, CANIMAL, CVEHICLE, CTRAFFICLIGHT, PlayerStats)
    │   ├── EntityManager.h/.cpp# Quản lý vòng đời làn đường, spawn & kiểm tra va chạm
    │   ├── CoinManager.h/.cpp  # Phân phối tiền vàng & cơ chế hút nam châm
    │   ├── PowerUpManager.h/.cpp # Quản lý buff nhặt trong làn đường & đếm giờ hiệu ứng
    │   └── obstacles/          # Lớp kế thừa chướng ngại vật theo từng kỷ nguyên (lv1 -> lv5)
    ├── hazards/                # Thảm họa môi trường động
    │   └── HazardManager.h/.cpp# Stampede, Sandstorm, Arrow Rain, Thunderstorm, Black Hole
    ├── levels/                 # Cấu hình dữ liệu màn chơi
    │   └── LevelConfig.h/.cpp  # Tham số tốc độ, số làn, texture, audio từng thời kỳ
    ├── ui/                     # Giao diện người dùng
    │   ├── HUD.h/.cpp          # Thanh máu, thể lực, điểm số, thời gian, bot telemetry
    │   ├── Menu.h/.cpp         # Main menu, Pause, Challenges, Shop, Settings, Credits
    │   ├── MenuButton.h        # Nút bấm UI với hiệu ứng phóng to hover động
    │   └── UIText.h            # Tiện ích định dạng và căn chỉnh văn bản
    └── utils/                  # Tiện ích bổ trợ
        ├── HighScore.h/.cpp    # Quản lý điểm kỷ lục vĩnh viễn
        ├── ShopData.h/.cpp     # Quản lý tài chính, giao dịch và trạng thái túi đồ
        └── Utils.h             # Hằng số toàn cục, kích thước cửa sổ & cấu hình thời gian
```

### Các Nguyên Lý & Mẫu Thiết Kế OOP Được Áp Dụng:
1. **Encapsulation (Đóng Gói):** Toàn bộ trạng thái nội bộ của nhân vật (`CPEOPLE`), động cơ âm thanh (`SoundManager`), các thực thể (`CANIMAL`, `CVEHICLE`) và cửa hàng (`ShopData`) được bảo vệ bằng phạm vi `private`, chỉ tương tác qua các phương thức giao tiếp rõ ràng.
2. **Inheritance (Kế Thừa):** Hệ thống chướng ngại vật phong phú kế thừa chặt chẽ từ hai lớp cơ sở trừu tượng `CANIMAL` và `CVEHICLE`.
3. **Polymorphism (Đa Hình):** Cơ chế gọi hàm ảo (`virtual void Draw()`, `virtual void Move()`, `virtual void update()`) cho phép `EntityManager` xử lý đồng nhất hàng chục loại vật thể khác nhau trên cùng một danh sách làn xe.
4. **Abstraction (Trừu Tượng Hóa):** `CGAME` được giải phóng hoàn toàn khỏi logic chi tiết nhờ các bộ quản lý độc lập (`HazardManager`, `PowerUpManager`, `CoinManager`, `EntityManager`).
5. **Design Patterns:**
   - **Singleton Pattern:** Quản lý chia sẻ bộ nhớ đệm hình ảnh tập trung (`TextureManager`).
   - **Factory Pattern:** Tự động khởi tạo cấu hình màn chơi, vật cản và môi trường tương ứng theo từng kỷ nguyên từ `LevelConfig`.
   - **State Machine Pattern:** Chuyển đổi mượt mà giữa các trạng thái game: Menu chính, Đang chơi, Tạm dừng, Xem Cửa hàng, Chuyển cảnh Thang máy, Tử vong, Chiến thắng, Credits.

---

## 💻 10. Hướng Dẫn Biên Dịch & Chạy Game (Build & Run)

### 🍎 Dành cho macOS

**Yêu cầu môi trường:** Đã cài đặt `cmake`, `clang` và thư viện `sfml@2` qua Homebrew.

```bash
# 1. Cài đặt thư viện SFML qua Homebrew (nếu chưa có)
brew install sfml@2 cmake

# 2. Cấp quyền thực thi cho kịch bản biên dịch
chmod +x build.sh

# 3. Biên dịch và tự động khởi chạy game
./build.sh run
```

*Tùy chọn lệnh hỗ trợ:*
- `./build.sh` : Chỉ biên dịch dự án.
- `./build.sh clean` : Dọn dẹp thư mục build cũ.
- `./build.sh clean run` : Dọn dẹp sạch sẽ, biên dịch lại từ đầu và khởi chạy.

---

### 🪟 Dành cho Windows

**Yêu cầu môi trường:** Visual Studio 2022 (với gói *Desktop development with C++*), CMake 3.16+ và bộ thư viện SFML 2.6.2 (đặt tại `C:\SFML-2.6.2` hoặc thiết lập biến môi trường `SFML_DIR`).

```cmd
:: 1. Chạy kịch bản tự động cấu hình CMake, biên dịch Release và copy DLLs
build.bat run
```

*Tùy chọn lệnh hỗ trợ:*
- `build.bat` : Biên dịch dự án ra file `build_win/Release/CrossingGame.exe`.
- `build.bat clean run` : Xóa bản build cũ, cấu hình và chạy lại.

---

## 🏫 11. Thông Tin Môn Học (Course Information)

- **Môn học:** Lập Trình Hướng Đối Tượng (Object-Oriented Programming - OOP)
- **Đơn vị đào tạo:** Khoa Công Nghệ Thông Tin (Faculty of Information Technology)
- **Trường:** Trường Đại học Khoa học Tự nhiên — Đại học Quốc gia TP. Hồ Chí Minh (HCMUS)
- **Năm thực hiện:** 2026

---

<div align="center">
  <sub>Được thiết kế và phát triển với trọn vẹn tâm huyết bởi <b>Nhóm Đồ Án Crossing Street — HCMUS</b>. Chúc bạn có những giờ phút trải nghiệm game tuyệt vời!</sub>
</div>
