# ocr-wordsearch-solver

## Extraction of all the letters
Use this command to delete all the images of letter 
```
rm -f grid_r*.bmp word_r*.bmp grid.bmp words.bmp
```

To compile:
```
gcc grid_extract.c -o grid_extract `sdl2-config --cflags --libs` -lSDL2_image -lm
```

When you run this file,
it will create .bmp images of all the letters in th grid and in the list of words.
```
./grid_extract images/'name_of_image'
```

## Solver

## Réseau de neurones (in english)
