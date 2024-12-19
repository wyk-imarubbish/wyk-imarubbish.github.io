#!/bin/bash

# 定义目标图片目录
TARGET_DIR="../assets/images/"

# 确保目标目录存在
mkdir -p "$TARGET_DIR"

# 遍历指定目录下的所有 Markdown 文件
for file in *.md; do
    echo "Processing $file..."

    # 使用grep找到所有图片路径并处理
    grep -oP '!\[.*?\]\(\K[^)]+' "$file" | while read -r img_path; do
        # 检查是否是网址（以 http:// 或 https:// 开头）
        if [[ "$img_path" =~ ^https?:// ]]; then
            echo "Skipping URL: $img_path"
            continue
        fi

        # 提取图片文件名
        img_name=$(basename "$img_path")

        # 如果图片文件存在，移动到目标目录
        if [ -f "$img_path" ]; then
            cp "$img_path" "$TARGET_DIR$img_name"
            echo "Moved $img_path to $TARGET_DIR$img_name"
        else
            echo "Image $img_path not found!"
        fi

        # 将路径中的反斜杠替换为正斜杠
        img_path=$(echo "$img_path" | sed 's|\\|\\\\|g')

        echo "replace path to $img_path"
    
        # 更新Markdown文件中的图片路径为相对路径
        sed -i "s|$img_path|/assets/images/$img_name|g" "$file"

    done
done

echo "All Markdown files processed."
