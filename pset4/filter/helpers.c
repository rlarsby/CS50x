#include "helpers.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void swap(RGBTRIPLE *a, RGBTRIPLE *b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //get RGB values of each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //get values
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            //calculate average, make sure to round
            int avg = round(((float)red + (float)green + (float)blue) / 3);

            //assign average to each pixel
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //get RGB values of each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //get values
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            //get new colours
            int sepiaRed = round((float).393 * red + (float).769 * green + (float).189 * blue);
            int sepiaGreen = round((float).349 * red + (float).686 * green + (float).168 * blue);
            int sepiaBlue = round((float).272 * red + (float).534 * green + (float).131 * blue);

            //check if new colours are larger than 255 and assign new colours
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //find each pixel and swap places
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            //get values and swap

            RGBTRIPLE extra = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = extra;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    //create a copy to avoid overwriting of values
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = 0;
            int green = 0;
            int blue = 0;
            int track = 0;

            //check if i-1, j-1 (upper left) OK
            if (i > 0 && j > 0)
            {
                red = red + copy[i - 1][j - 1].rgbtRed;
                green = green + copy[i - 1][j - 1].rgbtGreen;
                blue = blue + copy[i - 1][j - 1].rgbtBlue;
                track++;
            }

            //check if i-1,j (above) OK
            if (i > 0)
            {
                red = red + copy[i - 1][j].rgbtRed;
                green = green + copy[i - 1][j].rgbtGreen;
                blue = blue + copy[i - 1][j].rgbtBlue;
                track++;
            }

            //check if i-1,j+1 (upper right) OK
            if (i > 0 && j < (width - 1))
            {
                red = red + copy[i - 1][j + 1].rgbtRed;
                green = green + copy[i - 1][j + 1].rgbtGreen;
                blue = blue + copy[i - 1][j + 1].rgbtBlue;
                track++;
            }

            //check if i,j-1 (left) OK
            if (j > 0)
            {
                red = red + copy[i][j - 1].rgbtRed;
                green = green + copy[i][j - 1].rgbtGreen;
                blue = blue + copy[i][j - 1].rgbtBlue;
                track++;
            }

            // i,j (center) always OK
            red = red + copy[i][j].rgbtRed;
            green = green + copy[i][j].rgbtGreen;
            blue = blue + copy[i][j].rgbtBlue;
            track++;

            //check if i,j+1 (right) OK
            if (j < (width - 1))
            {
                red = red + copy[i][j + 1].rgbtRed;
                green = green + copy[i][j + 1].rgbtGreen;
                blue = blue + copy[i][j + 1].rgbtBlue;
                track++;
            }

            //check if i+1,j-1 (bottom left) OK
            if (i < (height - 1) && j > 0)
            {
                red = red + copy[i + 1][j - 1].rgbtRed;
                green = green + copy[i + 1][j - 1].rgbtGreen;
                blue = blue + copy[i + 1][j - 1].rgbtBlue;
                track++;
            }

            //check if i+1,j (under) OK
            if (i < (height - 1))
            {
                red = red + copy[i + 1][j].rgbtRed;
                green = green + copy[i + 1][j].rgbtGreen;
                blue = blue + copy[i + 1][j].rgbtBlue;
                track++;
            }

            //check if i+1,j+1 (bottom right) OK
            if (i < (height - 1) && j < (width - 1))
            {
                red = red + copy[i + 1][j + 1].rgbtRed;
                green = green + copy[i + 1][j + 1].rgbtGreen;
                blue = blue + copy[i + 1][j + 1].rgbtBlue;
                track++;
            }

            //assign new colours to pixel
            image[i][j].rgbtRed = round((float)red / (float)track);
            image[i][j].rgbtGreen = round((float)green / (float)track);
            image[i][j].rgbtBlue = round((float)blue / (float)track);
        }
    }
    return;
}
