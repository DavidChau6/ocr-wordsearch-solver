#ifndef RESEAUX_H
#define RESEAUX_H

#define INPUT_SIZE 1600 
#define HIDDEN_SIZE 64
#define OUTPUT_SIZE 26
#define LEARNING_RATE 0.05
void init_network();
void train_sample(double* input, int label);
char predict_letter(double* input);

int save_model(const char* filename);
int load_model(const char* filename);

#endif

