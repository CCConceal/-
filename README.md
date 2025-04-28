# Hotel Booking System（酒店预订系统）

## 项目简介
这是一个基于 C++ 的控制台酒店预订系统，模拟了从用户注册、查看酒店、下单预订、支付结算到评价管理的完整流程。  
主要面向 C++ 课程的期末项目，演示了面向对象设计、文件读写、数据结构与算法在小型系统中的综合应用。

## 核心功能
1. **用户管理**  
   - 注册／登录  
   - 普通用户、会员用户身份区分  
2. **酒店与房间浏览**  
   - 从 `hotels.csv` 加载酒店及房间基础信息  
   - 支持按城市、价格区间筛选  
3. **订单管理**  
   - 创建、查看、取消预订（`bookings.csv`）  
   - 支持入住日期／离店日期判断  
4. **支付模块**  
   - 现金支付 (`CashPayment`)  
   - 信用卡支付 (`CreditCardPayment`)  
   - 电子钱包支付 (`EWalletPayment`)  
5. **折扣与优惠**  
   - 固定折扣、会员忠诚度折扣、优惠码折扣等多种策略  
   - 在 `discounts.csv` 中管理可用折扣  
6. **评价系统**  
   - 星级评价 (`StarReview`)  
   - 文字评价 (`TextReview`)  
   - 存储于 `reviews.csv`，支持后续统计分析  
7. **工具类支持**  
   - 通用工具函数 (`Utils`)  
   - 菜单与交互封装 (`Menu`)  

## 目录结构
Hotel_booking/ ├── data/
│ ├── bookings.csv # 订单数据
│ ├── discounts.csv # 折扣规则
│ ├── hotels.csv # 酒店和房间信息
│ ├── reviews.csv # 评价记录
│ └── users.csv # 用户信息
├── data/xlsx/ # 可选的 Excel 导出／导入版本
│ ├── users.xlsx
│ └── reviews.xlsx
├── src/ # 源代码目录
│ ├── Booking.h/.cpp
│ ├── BookingManager.h/.cpp
│ ├── CashPayment.h/.cpp
│ ├── CommonUser.h/.cpp
│ ├── CreditCardPayment.h/.cpp
│ ├── Discount.h/.cpp
│ ├── DiscountManager.h/.cpp
│ ├── EWalletPayment.h/.cpp
│ ├── Hotel.h/.cpp
│ ├── HotelManager.h/.cpp
│ ├── LoyaltyDiscount.h/.cpp
│ ├── MemberUser.h/.cpp
│ ├── Menu.h/.cpp
│ ├── Payment.h/.cpp
│ ├── PromoCodeDiscount.h/.cpp
│ ├── Review.h/.cpp
│ ├── ReviewManager.h/.cpp
│ ├── Room.h/.cpp
│ ├── StarReview.h/.cpp
│ ├── TextReview.h/.cpp
│ ├── User.h/.cpp
│ ├── UserManager.h/.cpp
│ ├── Utils.h/.cpp
│ └── main.cpp # 程序入口
├── .gitignore # 忽略 IDE 中间文件和编译产物
├── README.md # 本文档
└── Hotel_booking.sln # Visual Studio 解决方案（Windows）



## 构建与运行

1. **Windows + Visual Studio**  
   - 直接打开 `Hotel_booking.sln`  
   - 选择 Release 或 Debug 配置  
   - 构建并运行即可  

2. **跨平台（g++）**  
   ```bash
   cd Hotel_booking/src
   g++ -std=c++17 *.cpp -o HotelBooking
   ./HotelBooking
   
数据准备
所有 CSV 文件均放在项目根目录下的 data/ 文件夹中。

如果想使用 Excel 文件，请放在 data/xlsx/ 并确保 Utils 中有相应的解析逻辑。

使用说明
启动程序后，选择「注册」或「登录」。

登录后可浏览酒店列表，输入酒店编号查看房间详情。

选择房型、输入入住／离店日期并提交订单。

选择支付方式完成支付。

支付成功后可以对入住过的酒店进行星级或文字评价。

会员用户可使用积分或优惠码获得折扣。

常见问题
找不到数据文件：请确保运行目录下存在 data/ 文件夹及对应 CSV。

编码问题：CSV 文件使用 UTF-8，无 BOM；如出现乱码，可在编辑器中转换编码。

许可证
此项目仅作为课程作业演示使用，未经授权不得商用。
