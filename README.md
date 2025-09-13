# 🖼️ LSB Image Steganography (C Project)

## Project Description and Overview

This project implements **Image Steganography using the Least Significant Bit (LSB) method** in the **C programming language**.
It allows users to **hide secret data inside BMP image files** (encoding) and later **extract the hidden data back** (decoding).

The project is useful for **secure communication**, **digital watermarking**, and **data hiding applications**.

-----

## Features

  - Encode and decode any file type (text, binary, etc.).
  - Works with `.bmp` format images.
  - Validates file format and capacity before encoding.
  - Uses a **magic string** for secure decoding.
  - Generates stego images that look identical to original.
  - Supports custom or default output filenames.

-----

## How It Works

### Encoding

1.  Validate input BMP image and secret file.
2.  Check if image has enough capacity.
3.  Copy BMP header (54 bytes).
4.  Encode in this order:
      - Magic string
      - Secret file extension size & extension
      - Secret file size
      - Secret file data
5.  Copy remaining image data unchanged.

### Decoding

1.  Validate BMP and magic string.
2.  Extract:
      - Secret file extension size & extension
      - Secret file size
      - Secret file data
3.  Save the recovered file with the correct extension.

-----

## Input and Output

### Encoding Command:

```bash
./a.out -e source.bmp secret.txt "#*" stego.bmp
```

```
[Input BMP] + [Secret File]
      |
      v
 [Check capacity] --> [Copy BMP Header]
      |
      v
[Embed: Magic String → Extension → File Size → File Data]
      |
      v
  [Stego BMP Image]
```

### Decoding Command:

```bash
./a.out -d stego.bmp "#*" output
```

```
[Stego BMP]
      |
      v
 [Check Magic String]
      |
      v
[Extract: Extension → File Size → File Data]
      |
      v
 [Recovered Secret File]
```

-----

## Project Structure

```
├── encode.c        # Encoding logic
├── decode.c        # Decoding logic
├── encode.h        # Encode header
├── decode.h        # Decode header
├── types.h         # Status and datatype definitions
├── test_encode.c   # Main driver file
├── Makefile        # (optional) Build instructions
└── README.md       # Project documentation
```

-----

## Feature Enhancements (Ideas)

  - Add support for PNG and JPEG formats (with compression handling).
  - Add password-based encryption before hiding data.
  - GUI tool for non-technical users.
  - Batch encode/decode multiple files at once.

-----

## ✍️ Author

Name: Atharv Hirapure
