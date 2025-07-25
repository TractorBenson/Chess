"""
shrink_opaque.py
在不改变图像尺寸的前提下，把 PNG 中“不透明区域”向内收缩 N 像素。
"""

import argparse
from pathlib import Path
from PIL import Image, ImageFilter

def shrink_opaque(img: Image.Image, shrink: int) -> Image.Image:
    if img.mode != "RGBA":
        img = img.convert("RGBA")
    r, g, b, a = img.split()

    # 对 alpha 做“腐蚀”（最小滤波），每次 MinFilter(3) 约等于向内收缩 1 像素
    eroded = a
    for _ in range(shrink):
        eroded = eroded.filter(ImageFilter.MinFilter(3))

    # 用腐蚀后的 alpha 替换原 alpha，RGB 保持不变
    out = Image.merge("RGBA", (r, g, b, eroded))
    return out

def main():
    ap = argparse.ArgumentParser(description="将 PNG 的不透明区域在不改变尺寸的情况下收缩（变瘦）")
    ap.add_argument("input", help="输入文件或目录")
    ap.add_argument("output", help="输出文件或目录")
    ap.add_argument("--shrink", type=int, default=2, help="收缩像素，默认 2")
    ap.add_argument("-r", "--recursive", action="store_true", help="目录模式下递归处理")
    args = ap.parse_args()

    in_path = Path(args.input)
    out_path = Path(args.output)

    def process_one(src: Path, dst: Path):
        dst.parent.mkdir(parents=True, exist_ok=True)
        with Image.open(src) as im:
            out = shrink_opaque(im, args.shrink)
            out.save(dst, "PNG", optimize=True)

    if in_path.is_file():
        dst = out_path if out_path.suffix.lower() == ".png" else out_path / in_path.name
        process_one(in_path, dst)
    else:
        pattern = "**/*.png" if args.recursive else "*.png"
        for src in in_path.glob(pattern):
            rel = src.relative_to(in_path)
            dst = out_path / rel
            process_one(src, dst)

if __name__ == "__main__":
    main()