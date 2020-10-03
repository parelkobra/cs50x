#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int rgbAvg = 0;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            rgbAvg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = rgbAvg;
            image[i][j].rgbtGreen = rgbAvg;
            image[i][j].rgbtRed = rgbAvg;
        }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int j, mid = width / 2;
    RGBTRIPLE temp;
    
    for (int i = 0; i < height; i++)
    {   
        j = 0;
        while (j < mid)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp;
            j++;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    float rgbtBlueSum = 0, rgbtGreenSum = 0, rgbtRedSum = 0;
    int neighbors;

    // Loop through each pixel
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            neighbors = 0, rgbtBlueSum = 0, rgbtGreenSum = 0, rgbtRedSum = 0;

            // Loop through the surrounding pixels of image[i][j]
            for (int k = -1; k < 2; k++)
                for (int l = -1; l < 2; l++)
                {
                    // Skip adjacent pixels that are out of the edges of the image
                    if ((i + k >= 0 && i + k < height) && (j + l >= 0 && j + l < width))
                    {
                        rgbtBlueSum += image[i + k][j + l].rgbtBlue;
                        rgbtGreenSum += image[i + k][j + l].rgbtGreen;
                        rgbtRedSum += image[i + k][j + l].rgbtRed;

                        neighbors++;
                    }
                }

            // Set pixel with it's average color
            temp[i][j].rgbtBlue = round(rgbtBlueSum / neighbors);
            temp[i][j].rgbtGreen = round(rgbtGreenSum / neighbors);
            temp[i][j].rgbtRed = round(rgbtRedSum / neighbors);
        }

    // Replace each pixel of the original image with the ones of the temporary image
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            image[i][j] = temp[i][j];

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    const int Gx[3][3] = {
        { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 }
    };

    const int Gy[3][3] = {
        { -1, -2, -1 },
        { 0, 0, 0 },
        { 1, 2, 1 }
    };

    RGBTRIPLE temp[height][width];
    int GxRed, GxGreen, GxBlue;
    int GyRed, GyGreen, GyBlue;
    float redVal, greenVal, blueVal;

    // Loop through each pixel
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            GxRed = 0, GxGreen = 0, GxBlue = 0;
            GyRed = 0, GyGreen = 0, GyBlue = 0;

            // Loop through the surrounding pixels of image[i][j]
            for (int k = -1; k < 2; k++)
                for (int l = -1; l < 2; l++)
                {
                    // Skip adjacent pixels that are out of the edges of the image
                    if ((i + k >= 0 && i + k < height) && (j + l >= 0 && j + l < width))
                    {
                        GxRed += Gx[k + 1][l + 1] * image[i + k][j + l].rgbtRed;
                        GxGreen += Gx[k + 1][l + 1] * image[i + k][j + l].rgbtGreen;
                        GxBlue += Gx[k + 1][l + 1] * image[i + k][j + l].rgbtBlue;

                        GyRed += Gy[k + 1][l + 1] * image[i + k][j + l].rgbtRed;
                        GyGreen += Gy[k + 1][l + 1] * image[i + k][j + l].rgbtGreen;
                        GyBlue += Gy[k + 1][l + 1] * image[i + k][j + l].rgbtBlue;
                    }
                }

            redVal = round(sqrt(pow(GxRed, 2) + pow(GyRed, 2)));
            greenVal = round(sqrt(pow(GxGreen, 2) + pow(GyGreen, 2)));
            blueVal = round(sqrt(pow(GxBlue, 2) + pow(GyBlue, 2)));

            temp[i][j].rgbtRed = (redVal > 255) ? 255 : redVal;
            temp[i][j].rgbtGreen = (greenVal > 255) ? 255 : greenVal;
            temp[i][j].rgbtBlue = (blueVal > 255) ? 255 : blueVal;
        }

    // Replace each pixel of the original image with the ones of the temporary image
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            image[i][j] = temp[i][j];

    return;
}
