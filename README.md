# blur - GLSL によるモーションブラーレンダリング

## 1. 概要

本プログラムは、GLSL を用いて動くオブジェクトの残像（モーションブラー）や後処理フィルタ（パス分割レンダリング）を行うサンプルプログラムです。

- 移行元ブログ記事:
  - [GLSL による画像のフィルタ処理 - 床井研究室](https://tokoik.github.io/blog/2012/09/04/)

## 2. 対応環境

- **Windows**: Visual Studio 2019 / 2022 / 2026 (CMake 経由で freeglut および OpenGL 拡張ヘッダを自動ダウンロード)
- **macOS**: Xcode (macOS 標準の GLUT および OpenGL Framework を使用)
- **Ubuntu Linux**: GCC / Make (システム標準の freeglut3-dev, libgl1-mesa-dev を使用)

## 3. ビルド手順

### Windows (Visual Studio)

```pwsh
cmake -B build -S .
cmake --build build --config Release
```

### macOS (Xcode)

```bash
cmake -B build -G Xcode
cmake --build build --config Release
```

### Ubuntu Linux (Makefile)

```bash
sudo apt-get update
sudo apt-get install -y freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev
cmake -B build -S .
cmake --build build
```

## 4. 起動方法

ビルド完了後、生成された実行ファイルを実行します。

- **Windows**: `build/Release/blur.exe`
- **macOS**: `build/Release/blur.app`
- **Linux**: `build/blur`

## 5. 操作方法

- **マウスドラッグ**: シーンの視点回転
- **マウス右クリック**: ポップアップメニューによる各種設定変更
- **[q] / [ESC]**: プログラムの終了

## 6. プログラムの解説

フレームバッファオブジェクト (FBO) を用いて複数パスによるオフスクリーンレンダリングを行い、頂点シェーダ・ジオメトリシェーダ・フラグメントシェーダを協調動作させてモーションブラーやエフェクトを合成しています。
