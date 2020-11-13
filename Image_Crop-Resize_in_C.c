#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
typedef struct
{
    char ImageType[4];
    uint8_t bands;
    int rows;
    int columns;
    uint8_t numberOfBits;
    unsigned char head[54];
    int *rgb;
} header;
int writeHeader(header *ptr, FILE *outFile)
{
    fwrite((const void *)&ptr->ImageType, sizeof(ptr->ImageType), 1, outFile);
    fwrite((const void *)&ptr->bands, sizeof(ptr->bands), 1, outFile);
    fwrite((const void *)&ptr->rows, sizeof(ptr->rows), 1, outFile);
    fwrite((const void *)&ptr->columns, sizeof(ptr->columns), 1, outFile);
    fwrite((const void *)&ptr->numberOfBits, sizeof(ptr->numberOfBits), 1, outFile);
}

int getHeader(header *ptr, FILE *imageFile)
{
    fread(&ptr->head,sizeof(unsigned char)*54,1,imageFile);
    
    // char fourBit[4];
    // fread(fourBit, sizeof(char) * 4, 1, imageFile);
    // strcpy(ptr->ImageType, fourBit);
    // // printf("%s\n", ptr->ImageType);
    // fseek(imageFile, 4, SEEK_SET);
    // fread(&ptr->bands, sizeof(uint8_t), 1, imageFile);
    // // printf("%d\n", ptr->bands);
    // fseek(imageFile, 5, SEEK_SET);
    // fread(&ptr->rows, sizeof(int), 1, imageFile);
    // // printf("%d\n", ptr->rows);
    // fseek(imageFile, 9, SEEK_SET);
    // fread(&ptr->columns, sizeof(int), 1, imageFile);
    // // printf("%d\n", ptr->columns);
    // fseek(imageFile, 13, SEEK_SET);
    // fread(&ptr->numberOfBits, sizeof(uint8_t), 1, imageFile);
    // printf("%d\n", ptr->numberOfBits);
    return 1;
}

int loadBinary(FILE *imageFile, int *arr)
{
    fseek(imageFile, 14, SEEK_SET);
    // writeHeader(ptr, outFile);
    long counter = 0;
    for (int i = 0; i < ptr->rows; i++)
    {
        for (int j = 0; j < ptr->columns; j++)
        {
            for (size_t z = 0; z < ptr->bands; z++)
            {
                // fread((arr + (i * ptr->columns * ptr->bands) + (j * ptr->bands) + z), sizeof(int), 1, imageFile);
                fread((arr+counter), 1, 1, imageFile);
                // counter++;
                // fwrite((arr+(i*ptr->columns*3)+(j*3)+z),1,4,outFile);
            }
        }
    }

    return 1;
}

int cropImage(int *arr, header *ptr, header *newPtr, int topleftX, int topleftY, int width, int height, int band, FILE *outFile)
{
    strcpy(newPtr->ImageType, ptr->ImageType);
    newPtr->bands = band;
    newPtr->rows = height;
    newPtr->columns = width;
    newPtr->numberOfBits = ptr->numberOfBits;
    int black = 0;

    writeHeader(newPtr, outFile);
    fseek(outFile, 14, SEEK_SET);
    int counter = 0;
    for (int i = topleftY; i < topleftY + height; i++)
    {

        for (int j = topleftX; j < topleftX + width; j++)

        {
            for (size_t d = 0; d < band; d++)
            {
                if (i > ptr->rows - 1 || j > ptr->columns - 1)
                {
                    fwrite(&black, 1, 1, outFile);
                    
                }
                else
                {
                    
                    fwrite((arr + (i * ptr->columns * band) + (j * band) + d), 1, 1, outFile);
                    
                }

                counter++;
                
            }
        }
    }
    
    return 1;
}

int gray(int *arr, header *ptr, header *newPtr, int topleftX, int topleftY, int width, int height, int band, FILE *outFile){

    
}

int doubleSize(int *arr, header *ptr, header *newPtr, int topleftX, int topleftY, int width, int height, int band, FILE *outFile)
{

    strcpy(newPtr->ImageType, ptr->ImageType);
    newPtr->bands = band;
    newPtr->rows = height * 2;
    newPtr->columns = width * 2;
    newPtr->numberOfBits = ptr->numberOfBits;
    writeHeader(newPtr, outFile);
    
    int black = 0;
    for (int i =0 ; i < (height * 2) - 1; i += 2)
    {
        for (int j = 0; j < (width * 2) - 1; j += 2)
        {
            for (int d = 0; d < 3; d++)
            {
                if (i+(topleftY*2) >= ptr->rows * 2 || j+(topleftX*2) >= ptr->columns * 2)
                {

                    fseek(outFile, 14 + (i *  width * 6) + (j * 3) + d, SEEK_SET);
                    fwrite(&black, 1, 1, outFile);

                    fseek(outFile, 14 + (i *  width * 6) + ((j + 1) * 3) + d, SEEK_SET);
                    fwrite(&black, 1, 1, outFile);

                    fseek(outFile, 14 + ((i + 1) *  width* 6) + (j * 3) + d, SEEK_SET);
                    fwrite(&black, 1, 1, outFile);

                    fseek(outFile, 14 + ((i + 1) * width  * 6) + ((j + 1) * 3) + d, SEEK_SET);
                    fwrite(&black, 1, 1, outFile);
                }
                else
                {

                    
                    fseek(outFile, 14 + (i * width * 6) + (j * 3) + d, SEEK_SET);
                    fwrite(arr + ((topleftY+(i / 2)) * ptr->columns * 3) + ((topleftX+(j / 2)) * 3) + d, 1, 1, outFile);
                    
                    fseek(outFile, 14 + (i * width * 6) + ((j + 1) * 3) + d, SEEK_SET);
                    fwrite(arr + ((topleftY+(i / 2)) * ptr->columns * 3) + ((topleftX+(j / 2)) * 3) + d, 1, 1, outFile);
                   
                    fseek(outFile, 14 + ((i + 1) * width * 6) + (j * 3) + d, SEEK_SET);
                    fwrite(arr + ((topleftY+(i / 2)) * ptr->columns * 3) + ((topleftX+(j / 2)) * 3) + d, 1, 1, outFile);
                   
                    fseek(outFile, 14 + ((i + 1) * width * 6) + ((j + 1) * 3) + d, SEEK_SET);
                    fwrite(arr + ((topleftY+(i / 2)) * ptr->columns * 3) + ((topleftX+(j / 2)) * 3) + d, 1, 1, outFile);
                    
                    
                }
            }
           
        }
    }

    
}

int main(int argc,char *argv[])
{

     if (argc != 7)
    {
         printf("Wrong inputs");
         return 0;
    }
    
    header *ptr = malloc(sizeof(header));
    
    header *newPtr = (header *)malloc(sizeof(header));
    
    FILE *imageFile = fopen("SAT115cm.IMG", "rb");
    if (imageFile ==NULL)
    {
        return 0;
    }
    
    FILE *normalFile = fopen("Testout1.IMG", "wb");
    if (normalFile ==NULL)
    {
        return 0;
    }
    char d[50] = "D";
    char filename[50];
    // printf("%s\n",argv[2]);
    strcpy(filename,"Testout1.IMG");
    strcat(d,filename);
    // printf("%s\n",d);

    FILE *doubleFile =fopen(d,"wb");
    if (doubleFile ==NULL)
    {
        return 0;
    }
    // getHeader(ptr, imageFile);
    
    ptr->rgb =malloc(ptr->rows * ptr->columns * ptr->bands * sizeof(int));
    loadBinary(imageFile,ptr->rgb, ptr);
    
    // cropImage(ptr->rgb,ptr,newPtr,16000,32000,2000,3000,ptr->bands,normalFile);
    // cropImage(ptr->rgb,ptr,newPtr,16000,32000,2000,3000,ptr->bands,doubleFile);
    // doubleSize(ptr->rgb, ptr, newPtr,atoi(argv[3]),atoi(argv[4]),atoi(argv[5]),atoi(argv[6]), ptr->bands, doubleFile);
    
    

    // printf("%s", "sds2");
    fclose(normalFile);
    fclose(doubleFile);
    fclose(imageFile);
    return 0;
}
