#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "reseaux.h"

double W1[HIDDEN_SIZE][INPUT_SIZE];
double b1[HIDDEN_SIZE];

double W2[OUTPUT_SIZE][HIDDEN_SIZE];
double b2[OUTPUT_SIZE];

double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }
double sigmoid_derivative(double y) { return y * (1 - y); }

void softmax(double* x, int size) 
{
    	double max = x[0];
    	for (int i = 1; i < size; i++)
        	if (x[i] > max) max = x[i];

    	double sum = 0;
    	for (int i = 0; i < size; i++) 
	{
        	x[i] = exp(x[i] - max);
        	sum += x[i];
    	}
    	for (int i = 0; i < size; i++)
        	x[i] /= sum;
}

void forward(double* input, double* hidden, double* output) 	
{

    	for (int i = 0; i < HIDDEN_SIZE; i++) 
	{
        	double sum = b1[i];
        	for (int j = 0; j < INPUT_SIZE; j++)
            		sum += W1[i][j] * input[j];
        	hidden[i] = sigmoid(sum);
    	}

    	for (int i = 0; i < OUTPUT_SIZE; i++) 
	{
        	double sum = b2[i];
        	for (int j = 0; j < HIDDEN_SIZE; j++)
            		sum += W2[i][j] * hidden[j];
        	output[i] = sum;
    	}

    	softmax(output, OUTPUT_SIZE);
}

void train_sample(double* input, int label) 
{
    	double hidden[HIDDEN_SIZE];
    	double output[OUTPUT_SIZE];
    	forward(input, hidden, output);
    	double delta_out[OUTPUT_SIZE];
    	for (int i = 0; i < OUTPUT_SIZE; i++)
        	delta_out[i] = output[i] - (i == label ? 1.0 : 0.0);

    	double delta_hidden[HIDDEN_SIZE];
    	for (int i = 0; i < HIDDEN_SIZE; i++) 
	{
        	double s = 0;
        	for (int j = 0; j < OUTPUT_SIZE; j++)
            		s += W2[j][i] * delta_out[j];
        	delta_hidden[i] = s * sigmoid_derivative(hidden[i]);
    	}

    	for (int i = 0; i < OUTPUT_SIZE; i++) 
	{
        	for (int j = 0; j < HIDDEN_SIZE; j++)
            		W2[i][j] -= LEARNING_RATE * delta_out[i] * hidden[j];
        	b2[i] -= LEARNING_RATE * delta_out[i];
    	}

    	for (int i = 0; i < HIDDEN_SIZE; i++) 
	{
        	for (int j = 0; j < INPUT_SIZE; j++)
            		W1[i][j] -= LEARNING_RATE * delta_hidden[i] * input[j];
        	b1[i] -= LEARNING_RATE * delta_hidden[i];
    	}
}

char predict_letter(double* input) 
{
    	double hidden[HIDDEN_SIZE], output[OUTPUT_SIZE];
    	forward(input, hidden, output);
    	int max = 0;
    	for (int i = 1; i < OUTPUT_SIZE; i++)
        	if (output[i] > output[max])
            		max = i;

    	return 'A' + max;
}

void init_network() 
{
    	for (int i = 0; i < HIDDEN_SIZE; i++) 
	{
        	for (int j = 0; j < INPUT_SIZE; j++)
            		W1[i][j] = ((double)rand() / RAND_MAX - 0.5);
        	b1[i] = 0.0;
    	}

    	for (int i = 0; i < OUTPUT_SIZE; i++) 
	{
        	for (int j = 0; j < HIDDEN_SIZE; j++)
            		W2[i][j] = ((double)rand() / RAND_MAX - 0.5);
        	b2[i] = 0.0;
    	}	
}

int save_model(const char* file) 
{
    	FILE* f = fopen(file, "wb");
    	if (!f) return 0;

    	fwrite(W1, sizeof(W1), 1, f);
    	fwrite(b1, sizeof(b1), 1, f);
    	fwrite(W2, sizeof(W2), 1, f);
    	fwrite(b2, sizeof(b2), 1, f);

    	fclose(f);
    	return 1;
}

int load_model(const char* file) 
{
    	FILE* f = fopen(file, "rb");
    	if (!f) return 0;

    	fread(W1, sizeof(W1), 1, f);
    	fread(b1, sizeof(b1), 1, f);
    	fread(W2, sizeof(W2), 1, f);
    	fread(b2, sizeof(b2), 1, f);

    	fclose(f);
    	return 1;
}

