#include <ov7670.h>
#include <stdio.h>
#include <swap.h>
#include <switches.h>
#include <vga.h>

#define PIXEL_AMOUNT 640 * 480
volatile uint16_t rgb565[PIXEL_AMOUNT];
volatile uint8_t grayscale[PIXEL_AMOUNT];
volatile uint8_t sobel[PIXEL_AMOUNT];

static void grayscaleFilter(uint32_t width, uint32_t height) {
    for (uint32_t line = 0; line < height; line++) {
        for (uint32_t pixel = 0; pixel < width; pixel++) {
            uint16_t rgb = swap_u16(rgb565[line * width + pixel]);
            uint32_t red = ((rgb >> 11) & 0x1F) << 3;
            uint32_t green = ((rgb >> 5) & 0x3F) << 2;
            uint32_t blue = (rgb & 0x1F) << 3;
            uint8_t gray = (uint8_t)((red * 54 + green * 183 + blue * 19) >> 8);
            grayscale[line * width + pixel] = gray;
        }
    }
}

static uint32_t sobelKernel(uint32_t line, uint32_t width, uint32_t pixel) {
    // clang-format off
    const int32_t gx_array[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    const int32_t gy_array[3][3] = {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}
    };
    // clang-format on

    int32_t valueX = 0;
    int32_t valueY = 0;

    for (int32_t dx = -1; dx < 2; dx++) {
        for (int32_t dy = -1; dy < 2; dy++) {
            uint32_t index = ((line + ((uint32_t)dy)) * width) + ((uint32_t)dx) + pixel;
            int32_t gray = (int32_t)(grayscale[index]);
            valueX += gray * gx_array[dy + 1][dx + 1];
            valueY += gray * gy_array[dy + 1][dx + 1];
        }
    }

    int32_t result = (valueX < 0) ? -valueX : valueX;
    result += (valueY < 0) ? -valueY : valueY;

    return (uint32_t)result;
}

static void sobelFilter(uint32_t width, uint32_t height, uint32_t threshold) {
    for (uint32_t line = 1; line < height - 1; line++) {
        for (uint32_t pixel = 1; pixel < width - 1; pixel++) {
            uint32_t result = sobelKernel(line, width, pixel);
            sobel[line * width + pixel] = (result > threshold) ? 0xFF : 0;
        }
    }
}

int main() {
    volatile uint32_t* switches = (uint32_t*)SWITCHES_BASE_ADDRESS;
    volatile uint32_t* vga = (uint32_t*)VGA_BASE_ADDRESS;

    vga_clear();

    printf("Initialising camera (this takes up to 3 seconds)!\n");

    camParameters camParams = initOv7670(VGA);

    printf("Done!\n");
    printf("NrOfPixels : %d\n", camParams.nrOfPixelsPerLine);
    printf("NrOfLines  : %d\n", camParams.nrOfLinesPerImage);
    printf("PCLK (kHz) : %d\n", camParams.pixelClockInkHz);
    printf("FPS        : %d\n", camParams.framesPerSecond);

    // Display configuration
    vga[0] = swap_u32(camParams.nrOfPixelsPerLine);

    while (1) {
        takeSingleImageBlocking((uint32_t)&rgb565[0]);

        switch ((*switches >> 24) & 0xf) {
            case 1: {
                vga[1] = swap_u32(camParams.nrOfLinesPerImage);
                vga[2] = swap_u32(2);
                vga[3] = swap_u32((uint32_t)&grayscale[0]);

                grayscaleFilter(camParams.nrOfPixelsPerLine, camParams.nrOfLinesPerImage);

                break;
            }
            case 3: {
                vga[1] = swap_u32(camParams.nrOfLinesPerImage);
                vga[2] = swap_u32(2);
                vga[3] = swap_u32((uint32_t)&sobel[0]);

                grayscaleFilter(camParams.nrOfPixelsPerLine, camParams.nrOfLinesPerImage);
                sobelFilter(camParams.nrOfPixelsPerLine, camParams.nrOfLinesPerImage, 128);

                break;
            }
            default: {
                vga[1] = swap_u32((camParams.nrOfLinesPerImage >> 1) | 0x80000000);
                vga[2] = swap_u32(1);
                vga[3] = swap_u32((uint32_t)&rgb565[0]);

                break;
            }
        }
    }
}

