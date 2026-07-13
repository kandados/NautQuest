#!/usr/bin/env python3

from pathlib import Path
import struct

from PIL import Image


PROJECT_ROOT = Path(__file__).resolve().parent.parent

SOURCE_IMAGE = (
    PROJECT_ROOT
    / "src"
    / "assets"
    / "home"
    / "nauty_home_466.png"
)

OUTPUT_BIN = (
    PROJECT_ROOT
    / "sdcard"
    / "ui"
    / "home"
    / "nauty_home.bin"
)

EXPECTED_SIZE = (466, 466)

# LVGL 8: LV_IMG_CF_TRUE_COLOR
LV_IMG_CF_TRUE_COLOR = 4


def rgb888_to_rgb565(red: int, green: int, blue: int) -> int:
    """
    Convierte RGB888 a RGB565.
    """
    return (
        ((red & 0xF8) << 8)
        | ((green & 0xFC) << 3)
        | (blue >> 3)
    )


def create_lvgl_v8_header(
    width: int,
    height: int,
    color_format: int,
) -> bytes:
    """
    Genera la cabecera binaria de 4 bytes usada por LVGL 8.

    Distribución del descriptor:

    bits  0-4  : formato de color
    bits  5-7  : always_zero
    bits  8-9  : reserved
    bits 10-20 : ancho
    bits 21-31 : alto
    """

    if width <= 0 or width > 0x7FF:
        raise ValueError(
            f"Ancho no válido para LVGL 8: {width}"
        )

    if height <= 0 or height > 0x7FF:
        raise ValueError(
            f"Alto no válido para LVGL 8: {height}"
        )

    if color_format < 0 or color_format > 0x1F:
        raise ValueError(
            f"Formato de color no válido: {color_format}"
        )

    header_value = (
        (color_format & 0x1F)
        | ((width & 0x7FF) << 10)
        | ((height & 0x7FF) << 21)
    )

    return struct.pack("<I", header_value)


def image_to_rgb565_bytes(image: Image.Image) -> bytearray:
    """
    Convierte todos los píxeles a RGB565 little-endian.

    Conserva el mismo orden de bytes utilizado por los assets
    .c que ya se muestran correctamente en NautQuest.
    """

    output = bytearray()

    for red, green, blue in image.getdata():
        pixel = rgb888_to_rgb565(
            red,
            green,
            blue,
        )

        output.append(pixel & 0xFF)
        output.append((pixel >> 8) & 0xFF)

    return output


def convert_image() -> None:
    if not SOURCE_IMAGE.exists():
        raise FileNotFoundError(
            "No se encuentra la imagen de origen:\n"
            f"{SOURCE_IMAGE}"
        )

    image = Image.open(SOURCE_IMAGE).convert("RGB")

    if image.size != EXPECTED_SIZE:
        raise ValueError(
            f"La imagen mide "
            f"{image.width}x{image.height} px.\n"
            f"Debe medir exactamente "
            f"{EXPECTED_SIZE[0]}x{EXPECTED_SIZE[1]} px."
        )

    OUTPUT_BIN.parent.mkdir(
        parents=True,
        exist_ok=True,
    )

    header = create_lvgl_v8_header(
        width=image.width,
        height=image.height,
        color_format=LV_IMG_CF_TRUE_COLOR,
    )

    pixel_data = image_to_rgb565_bytes(image)

    with OUTPUT_BIN.open("wb") as output_file:
        output_file.write(header)
        output_file.write(pixel_data)

    expected_pixel_bytes = (
        image.width
        * image.height
        * 2
    )

    expected_total_bytes = (
        4
        + expected_pixel_bytes
    )

    actual_total_bytes = OUTPUT_BIN.stat().st_size

    if len(pixel_data) != expected_pixel_bytes:
        raise RuntimeError(
            "El tamaño de los píxeles convertidos "
            "no es correcto.\n"
            f"Esperado: {expected_pixel_bytes}\n"
            f"Obtenido: {len(pixel_data)}"
        )

    if actual_total_bytes != expected_total_bytes:
        raise RuntimeError(
            "El tamaño final del archivo BIN "
            "no es correcto.\n"
            f"Esperado: {expected_total_bytes}\n"
            f"Obtenido: {actual_total_bytes}"
        )

    print()
    print("========================================")
    print("     NautQuest Asset Converter")
    print("========================================")
    print("Conversión completada correctamente.")
    print()
    print(f"Origen       : {SOURCE_IMAGE}")
    print(f"Destino      : {OUTPUT_BIN}")
    print(f"Resolución   : {image.width}x{image.height}")
    print("Formato      : LVGL 8 RGB565")
    print(f"Cabecera     : {len(header)} bytes")
    print(f"Datos imagen : {len(pixel_data)} bytes")
    print(f"Tamaño total : {actual_total_bytes} bytes")
    print("========================================")


if __name__ == "__main__":
    try:
        convert_image()

    except Exception as error:
        print()
        print("========================================")
        print(" ERROR AL CONVERTIR EL ASSET")
        print("========================================")
        print(error)
        print("========================================")

        raise SystemExit(1)