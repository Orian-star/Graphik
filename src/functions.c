#include "functions.h"
#include <math.h>

void init_structs(Pars_Functions *pars_functions)
{
    if (!pars_functions) return;
    
    pars_functions->add = pars_add;
    pars_functions->div = pars_divi;
    pars_functions->mult = pars_mult;
    pars_functions->sub = pars_sub;
    pars_functions->power = pars_power;
}

double add(double a, double b){
    return a + b;
}
double sub(double a, double b){
    return a - b;
}
double mult(double a, double b){
    return a * b;
}
double divi(double a, double b){
    return a / b;
}