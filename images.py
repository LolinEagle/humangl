from PIL import Image

# Input file order matches the UV layout used earlier
files = [
    "./panorama/Panorama_4_JE1.png",
    "./panorama/Panorama_1_JE1.png",
    "./panorama/Panorama_2_JE1.png",
    "./panorama/Panorama_3_JE1.png",
    "./panorama/Panorama_0_JE1.png",
    "./panorama/Panorama_5_JE1.png",
]

# Load images
images = [Image.open(f).convert("RGBA") for f in files]

# Assume all images are the same size
w, h = images[0].size

# Create atlas: 3 wide, 2 tall
atlas_width = w * 3
atlas_height = h * 2
atlas = Image.new("RGBA", (atlas_width, atlas_height))

# Paste images into atlas
for i, img in enumerate(images):
    x = (i % 3) * w
    y = (i // 3) * h
    if (i == 5):
        img = img.transpose(Image.FLIP_LEFT_RIGHT)
    atlas.paste(img, (x, y))

# Save result
atlas.save("./model/texture/panorama_atlas.png")
print("Saved panorama_atlas.png")

