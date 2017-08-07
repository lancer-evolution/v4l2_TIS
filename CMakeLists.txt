# CMakeのバージョン
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

#---------------------------------------------------
# コンパイル時のオプションの設定
#---------------------------------------------------
if ( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(main PRIVATE "-w")
endif ( CMAKE_COMPILER_IS_GNUCC )
# ビルドのタイプはリリース
#SET(CMAKE_BUILD_TYPE Release)
# 実行ファイルの出力先の設定
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../) 
#---------------------------------------------------


# プロジェクト名
PROJECT(v4l2_TIS)
# バージョン番号
#SET(serial "1.0.0")
# 共有ライブラリのバージョン番号
#SET(soserial "1")
# ビルドするためにインクルードするヘッダーファイルの場所
#INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}")
#ライブラリの追加
find_package( OpenCV REQUIRED )

#メインビルド
ADD_EXECUTABLE(v4ldevice v4ldevice.cpp main.cpp)
#ターゲットリンクライブラリ
target_link_libraries( v4ldevice ${OpenCV_LIBS} )
ADD_EXECUTABLE(listup listup.c)
ADD_EXECUTABLE(capture_one capture_one.c)

