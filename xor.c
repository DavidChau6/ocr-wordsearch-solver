#include "xor.h"
double sigmoid(double x) 
{
	return 1.0 / (1.0 + exp(-x));
}

double dsigmoid(double y)
{
    	return y * (1.0 - y);
}

double rand_weight()
{	
    	return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}
int main() 
{
    	srand(time(NULL));

    	double input[4][2] = {{0,0}, {0,1}, {1,0}, {1,1}};
    	double target[4] = {0, 1, 1, 0};

    	double w_input_hidden[2][2];
    	double w_hidden_output[2];
    	double b_hidden[2];
    	double b_output;

    	for (int i = 0; i < 2; i++) 
	{
        	b_hidden[i] = rand_weight();
        	w_hidden_output[i] = rand_weight();
        	for (int j = 0; j < 2; j++) 
            		w_input_hidden[i][j] = rand_weight();
    	}
    	b_output = rand_weight();

    	double lr = 0.1; 
    	int epochs = 40000;

    	for (int epoch = 0; epoch < epochs; epoch++) 
	{
        	double total_error = 0.0;

        	for (int s = 0; s < 4; s++) 
		{
            		double h[2];
            		for (int i = 0; i < 2; i++) 
                		h[i] = sigmoid(input[s][0] * w_input_hidden[0][i] +input[s][1] * w_input_hidden[1][i] + b_hidden[i]);
            	

            		double output = sigmoid(h[0] * w_hidden_output[0] + h[1] * w_hidden_output[1] + b_output);

            		double error = target[s] - output;
            		total_error += error * error;

            		double d_output = error * dsigmoid(output);

            		double d_hidden[2];
            		for (int i = 0; i < 2; i++) 
                		d_hidden[i] = d_output * w_hidden_output[i] * dsigmoid(h[i]);

           		for (int i = 0; i < 2; i++) 
	    		{
                		w_hidden_output[i] += lr * d_output * h[i];
                		b_hidden[i] += lr * d_hidden[i];
                		for (int j = 0; j < 2; j++) 
                    			w_input_hidden[j][i] += lr * d_hidden[i] * input[s][j];
            		}
            		b_output += lr * d_output;
		}
        	if (epoch % 1000 == 0)
            		printf("Epoch %d, Erreur totale = %.4f\n", epoch, total_error);
	}

    	printf("\nRésultats après entraînement:\n");
    	for (int s = 0; s < 4; s++) 
	{
        	double h[2];
        	for (int i = 0; i < 2; i++) 
            		h[i] = sigmoid(input[s][0] * w_input_hidden[0][i] + input[s][1] * w_input_hidden[1][i] + b_hidden[i]);
       		double output = sigmoid(h[0] * w_hidden_output[0] + h[1] * w_hidden_output[1] + b_output);
        	printf("[%g, %g] => %.4f\n", input[s][0], input[s][1], output);
	}
    	return 0;
}

