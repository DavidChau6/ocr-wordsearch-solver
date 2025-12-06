#ifndef RESEAUX_H
#define RESEAUX_H

#define INPUT_SIZE 1600     // Exemple 40x40, change selon tes images
#define HIDDEN_SIZE 64
#define OUTPUT_SIZE 26
#define LEARNING_RATE 0.05
extern double W1[HIDDEN_SIZE][INPUT_SIZE];
extern double b1[HIDDEN_SIZE];

extern double W2[OUTPUT_SIZE][HIDDEN_SIZE];
extern double b2[OUTPUT_SIZE];
void init_network();
void train_sample(double* input, int label);
char predict_letter(double* input);

int save_model(const char* filename);
int load_model(const char* filename);

#endif

